#include "videotargetreporting.h"
#include "ui_videotargetreporting.h"
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QPainter>
#include "HoleOverlay.h"
#include <QDateTime>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <HIDCommandParser.h>
#include <HIDResponseGenerator.h>
#include <QFile>
#include <QLatin1String>
#include <QMessageBox>
#include <QSoundEffect>
VideoTargetReporting::VideoTargetReporting(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VideoTargetReporting), m_current_target(HID_TARGET::TARGET_NONE), hid_dev(nullptr), m_hid_thread(nullptr)
{
    ui->setupUi(this);

    // 初始化历史模式靶型状态映射（根据实际靶型名称调整文本）
    m_historyTargetStateMap.insert(HID_TARGET::TARGET_CHEST, "胸环靶");
    m_historyTargetStateMap.insert(HID_TARGET::TARGET_IPSC, "IPSC靶");
    m_historyTargetStateMap.insert(HID_TARGET::TARGET_IDPA, "IDPA靶");
    m_historyTargetStateMap.insert(HID_TARGET::TARGET_NONE, "未选择靶型");

    // 初始化历史记录管理器（核心修改：获取单例）
    m_historyManager = BulletHistoryManager::getInstance(this);
    // 连接历史记录变化信号，更新UI
    connect(m_historyManager, &BulletHistoryManager::historyRecordsChanged,
            this, &VideoTargetReporting::onHistoryRecordsChanged);

    // 初始化UI状态（默认待连接）
    ui->label_HID->setText("待连接");
    ui->label_HID->setStyleSheet("color: black;");
    ui->pushButton_HID->setText("HID连接");

    // 程序启动自动连接HID设备
    ui->textEdit_HID->append("程序启动，尝试连接HID设备...");
    connectHIDDevice(); // 调用封装的连接函数

    // 1. 初始化三个靶面的弹孔绘制层（之前的代码，确保已正确创建）
    ui->pushButton_back->hide();
    m_chestHoleOverlay = ui->widget_xhbHole;
    m_chestHoleOverlay->setTargetSize(900, 900);
    m_chestHoleOverlay->setHoleRadius(6);
    m_chestHoleOverlay->setRawCoordRange(640,658);

    m_ipscHoleOverlay = ui->widget_ipscHole;
    m_ipscHoleOverlay->setTargetSize(714, 900);
    m_ipscHoleOverlay->setHoleRadius(6);
    m_ipscHoleOverlay->setRawCoordRange(460,580);

    m_idpaHoleOverlay = ui->widget_idpaHole;
    m_idpaHoleOverlay->setTargetSize(531, 900);
    m_idpaHoleOverlay->setHoleRadius(6);
    m_idpaHoleOverlay->setRawCoordRange(460,780);

    setWindowFlags(Qt::FramelessWindowHint);
    showFullScreen();

    // 初始显示空白页（索引0）
    // ui->stackedWidget_target->setCurrentIndex(0);
    ui->stackedWidget_right->setCurrentIndex(0);
    // 1. 初始化业务处理器（传入UI指针）
    m_business_handler = new HIDBusinessHandler(ui,
                                                m_chestHoleOverlay,
                                                m_ipscHoleOverlay,
                                                m_idpaHoleOverlay,
                                                this);

    // 2. 连接业务处理器的“发送响应”信号到HID发送函数
    connect(m_business_handler, &HIDBusinessHandler::sendResponse, this, [=](const QByteArray& data) {
        if (hid_dev == nullptr) {
            ui->textEdit_HID->append("[HID发送] 失败：设备未连接");
            return;
        }

        // 关键：判断当前发送的是否是握手指令（CMD=0x01）
        bool is_handshake_req = false;
        if (data.size() >= 4) { // 至少包含：报告ID(1) + 55(1) + AA(1) + CMD(1)
            uint8_t cmd = static_cast<uint8_t>(data.at(3)); // CMD位于索引3（第4字节）
            if (cmd == static_cast<uint8_t>(HID_CMD::CMD_HANDSHAKE_REQ)) {
                is_handshake_req = true;
            }
        }

        // 拦截逻辑：未握手成功时，只允许握手指令发送
        if (m_business_handler->getConnectionState() != HID_STATE::STATE_CONNECTED
            && !is_handshake_req) {
            ui->textEdit_HID->append("[HID发送] 失败：未握手成功，禁止发送非握手指令");
            return;
        }

        // 正常发送逻辑（握手指令或已连接状态下的其他指令）
        int bytes_sent = hid_write(hid_dev, reinterpret_cast<const unsigned char*>(data.data()), data.size());
        if (bytes_sent > 0) {
            ui->textEdit_HID->append(QString("[HID发送] 成功，数据：%1").arg(data.toHex(' ').toUpper()));
        } else {
            const wchar_t* error = hid_error(hid_dev);
            QString err_str = QString::fromWCharArray(error);
            ui->textEdit_HID->append(QString("[HID发送] 失败！返回值：%1，原因：%2").arg(bytes_sent).arg(err_str));
        }
    });

    // 3. 启动握手流程（程序启动即自动发握手）
    m_business_handler->startHandshake();

    // 4. 读取靶型配置（只读取，不发送）
    QSettings settings("SpecialPie", "TargetReportingSystem");
    uint8_t targetType = settings.value("LastSelectedTarget", static_cast<uint8_t>(HID_TARGET::TARGET_NONE)).toUInt();
    m_lastSelectedTarget = static_cast<HID_TARGET>(targetType);
    m_current_target = m_lastSelectedTarget; // 同步到当前靶型变量

    // 5. 处理首次运行无配置的情况（只更新UI，不发送指令）
    QUrl url;
    if (m_lastSelectedTarget == HID_TARGET::TARGET_NONE) {
        ui->label_connectionState->setText("待选择");
        ui->stackedWidget_target->setCurrentIndex(0);
        url = "qrc:/new/prefix1/sound/CT_NONE.wav";
        QSoundEffect *sound = new QSoundEffect(this);
        sound->setSource(url);
        sound->play();
    } else {
        // 非首次运行，先更新UI显示上次选中的靶型名称
        ui->label_connectionState->setText(getTargetName(m_lastSelectedTarget));
        ui->stackedWidget_target->setCurrentIndex(static_cast<uint8_t>(m_current_target));
        // 页面暂不切换，等发送指令后再同步（或根据指令响应切换）
    }

    // 6. 关键：监听握手成功信号，收到后再发送靶型选择指令
    connect(m_business_handler, &HIDBusinessHandler::handshakeSuccessed, this, [=]() {
        ui->textEdit_HID->append("[握手成功] 开始发送上次保存的靶型选择指令");

        // 根据读取的靶型生成指令并发送
        HID_Response resp;
        switch (m_lastSelectedTarget) {
        case HID_TARGET::TARGET_CHEST:
            resp = HIDResponseGenerator::generateTargetSelectReq(HID_TARGET::TARGET_CHEST);
            break;
        case HID_TARGET::TARGET_IPSC:
            resp = HIDResponseGenerator::generateTargetSelectReq(HID_TARGET::TARGET_IPSC);
            break;
        case HID_TARGET::TARGET_IDPA:
            resp = HIDResponseGenerator::generateTargetSelectReq(HID_TARGET::TARGET_IDPA);
            break;
        case HID_TARGET::TARGET_NONE:
            ui->textEdit_HID->append("[靶型选择] 无历史记录，不发送指令");
            return; // 无靶型，不发送
        }

        // 发送靶型指令（此时已握手成功，拦截逻辑会放行）
        if (resp.need_send) {
            emit m_business_handler->sendResponse(resp.raw_data);
            ui->textEdit_HID->append(
                QString("[靶型选择] 发送 0x%1 指令（%2）")
                    .arg(
                        static_cast<int>(HID_CMD::CMD_TARGET_SELECT),
                        2,
                        16,
                        QLatin1Char('0')
                        )
                    .arg(getTargetName(m_lastSelectedTarget))
                );
        } else {
            ui->textEdit_HID->append("[靶型选择] 指令生成失败，发送取消");
        }
    });

    // 关键：连接弹孔数据信号，接收来自 HIDBusinessHandler 的弹孔信息
    connect(m_business_handler, &HIDBusinessHandler::bulletDataReceived,
            this, &VideoTargetReporting::onBulletDataReceived);

    // 给三个靶型的 Label 安装事件过滤器（监听点击）
    ui->label_xhbSelected->installEventFilter(this);
    ui->label_ipscSelected->installEventFilter(this);
    ui->label_idpaSelected->installEventFilter(this);

}

bool VideoTargetReporting:: eventFilter(QObject *watched, QEvent *event) {
    // 只处理鼠标左键点击事件
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            // 判断点击的是哪个靶型的 Label
            if (watched == ui->label_xhbSelected) {
                // 触发胸环靶选中逻辑（你已写好的逻辑，示例调用）
                setTargetSelected(ui->label_xhbSelected, ui->label_ipscSelected, ui->label_idpaSelected);
                m_current_target = HID_TARGET::TARGET_CHEST;
            } else if (watched == ui->label_ipscSelected) {
                setTargetSelected(ui->label_ipscSelected, ui->label_xhbSelected, ui->label_idpaSelected);
                m_current_target = HID_TARGET::TARGET_IPSC;
            } else if (watched == ui->label_idpaSelected) {
                setTargetSelected(ui->label_idpaSelected, ui->label_xhbSelected, ui->label_ipscSelected);
                m_current_target = HID_TARGET::TARGET_IDPA;
            }
            return true; // 事件已处理，不再传递
        }
    }
    // 其他事件交给父类处理
    return QMainWindow::eventFilter(watched, event);
}

void VideoTargetReporting::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    }
    QWidget::keyPressEvent(event);
}

VideoTargetReporting::~VideoTargetReporting() {
    delete m_business_handler;
    delete m_hid_thread;
    delete ui;
}

void VideoTargetReporting::on_pushButton_exit_clicked() {
    // close();
    QCoreApplication::quit();
}

// VideoTargetReporting.cpp 中实现
bool VideoTargetReporting::connectHIDDevice() {
    // 若已连接，直接返回
    if (hid_dev != nullptr) return true;

    // 初始化hidapi（只需初始化一次）
    if (hid_init() != 0) {
        ui->textEdit_HID->append("HIDAPI初始化失败！");
        return false;
    }

    // 打开指定VID/PID的HID设备
    hid_dev = hid_open(VID, PID, NULL);
    if (hid_dev != nullptr) {
        // 更新UI为连接成功状态
        ui->label_HID->setText("已连接");
        ui->label_HID->setStyleSheet("color: green;");
        ui->pushButton_HID->setText("断开连接");
        ui->textEdit_HID->append("HID设备连接成功！");

        // 启动数据接收线程
        startHIDReadThread();


        return true;
    } else {
        // 更新UI为连接失败状态
        ui->label_HID->setText("连接失败");
        ui->label_HID->setStyleSheet("color: red;");
        ui->textEdit_HID->append("未找到指定HID设备（VID:0x4476, PID:0x5101）！");
        return false;
    }
}

void VideoTargetReporting::disconnectHIDDevice() {
    // 若未连接，直接返回
    if (hid_dev == nullptr) return;

    // 停止接收线程
    if (m_hid_thread != nullptr) {
        m_hid_thread->stop();
        m_hid_thread->wait(1000);
        m_hid_thread = nullptr;
    }

    // 断开设备
    hid_close(hid_dev);
    hid_exit();
    hid_dev = nullptr;

    // 更新UI为断开状态
    ui->label_HID->setText("待连接");
    ui->label_HID->setStyleSheet("color: black;");
    ui->pushButton_HID->setText("HID连接");
    ui->textEdit_HID->append("HID设备已断开！");
}

void VideoTargetReporting::on_pushButton_HID_clicked()
{
    if (hid_dev == nullptr) {
        // 未连接时，调用连接函数
        connectHIDDevice();
    } else {
        // 已连接时，调用断开函数
        disconnectHIDDevice();
    }
}

// 启动HID数据接收线程
void VideoTargetReporting::startHIDReadThread()
{
    // 1. 先停止并释放旧线程（若存在），避免线程冲突
    if (m_hid_thread != nullptr)
    {
        m_hid_thread->stop();       // 调用线程的停止方法（设置is_running=false）
        m_hid_thread->wait(1000);   // 等待线程退出（最多等1秒，避免无限阻塞）
        delete m_hid_thread;        // 释放线程对象
        m_hid_thread = nullptr;     // 重置指针，避免野指针
    }

    // 2. 创建新线程，传入当前HID设备句柄（确保hid_dev已连接）
    if (hid_dev == nullptr)
    {
        ui->textEdit_HID->append("[线程启动失败] HID设备未连接，无法创建接收线程");
        return;
    }
    m_hid_thread = new HIDReadThread(hid_dev, this);

    // 3. 连接线程的信号：接收数据后打印到文本框，并执行指令解析
    connect(m_hid_thread, &HIDReadThread::dataReceived, this, [=](const QString& raw_data_str)
            {
                // 先将原始数据打印到文本框（保留原功能）
                ui->textEdit_HID->append(raw_data_str);

                // 步骤1：删除前缀“收到数据：”
                QString cleaned_str = raw_data_str;
                int start_idx = cleaned_str.indexOf(QString::fromUtf8("收到数据："));
                if (start_idx != -1) {
                    cleaned_str.remove(start_idx, 5); // 删除前缀（5个字符）
                }

                // 步骤2：删除所有空格（关键！确保fromHex能正确转换）
                cleaned_str.remove(" ");

                // 步骤3：先转换为QByteArray（未截取前的完整数据），校验转换结果
                QByteArray raw_data = QByteArray::fromHex(cleaned_str.toUtf8());
                if (raw_data.isEmpty()) {
                    ui->textEdit_HID->append("[转换错误] 数据转换失败（可能包含无效字符）");
                    return;
                }

                // 步骤4：校验最小长度（至少6字节：报告ID(1) + 55(1) + AA(1) + CMD(1) + DATALENGTH(1) + CHKSUM(1)）
                if (raw_data.size() < 6) {
                    ui->textEdit_HID->append(QString("[数据错误] 数据长度不足6字节（当前：%1字节）").arg(raw_data.size()));
                    return;
                }

                // 步骤5：解析 DATALENGTH 字段（关键！确定有效数据总长度）
                uint8_t data_length = static_cast<uint8_t>(raw_data.at(4)); // 第5字节=DATALENGTH（索引4）
                int total_valid_length = 6 + data_length; // 总有效长度=6（固定部分）+ DATALENGTH（数据部分）

                // 步骤6：校验总有效长度是否在合理范围（避免异常值导致越界）
                if (total_valid_length > raw_data.size()) {
                    ui->textEdit_HID->append(QString("[数据错误] 有效长度计算异常（需%1字节，实际%2字节），可能数据不完整").arg(total_valid_length).arg(raw_data.size()));
                    return;
                }
                if (data_length > 100) { // 限制最大DATA长度（根据协议调整，避免恶意数据）
                    ui->textEdit_HID->append(QString("[数据错误] DATALENGTH超出最大限制（当前：%1字节，最大允许100字节）").arg(data_length));
                    return;
                }

                // 步骤7：精准截取有效数据（从0到total_valid_length-1，含报告ID）
                QByteArray valid_data = raw_data.left(total_valid_length);

                // 打印截取后的日志，方便调试
                ui->textEdit_HID->append(QString("[截取后] 有效长度：%1字节（DATALENGTH：%2字节），内容：%3")
                                             .arg(valid_data.size())
                                             .arg(data_length)
                                             .arg(valid_data.toHex(' ').toUpper()));

                // 步骤8：校验截取后的数据最小长度（确保包含完整指令）
                if (valid_data.size() < 6) {
                    ui->textEdit_HID->append("[数据错误] 截取后有效数据长度不足6字节");
                    return;
                }

                // 步骤9：传入解析器处理（使用精准截取后的 valid_data）
                HID_Command cmd = HIDCommandParser::parseRawData(raw_data);
                HID_Response resp = m_business_handler->handleCommand(cmd);
                if (resp.need_send)
                {
                    emit m_business_handler->sendResponse(resp.raw_data);
                }
            });

    // 4. 线程结束后自动删除，避免内存泄漏
    connect(m_hid_thread, &HIDReadThread::finished, m_hid_thread, &QObject::deleteLater);

    // 5. 启动线程
    m_hid_thread->start();
    ui->textEdit_HID->append("[线程启动成功] HID数据接收线程已启动");
}

void VideoTargetReporting::closeEvent(QCloseEvent *event)
{
    // 停止线程
    if (m_hid_thread != nullptr)
    {
        m_hid_thread->stop();
        if (!m_hid_thread->wait(2000))
        {
            m_hid_thread->terminate();
            m_hid_thread->wait(500);
        }
    }
    // 断开HID设备
    if (hid_dev != nullptr)
    {
        hid_close(hid_dev);
        hid_exit();
    }
    event->accept();
}

// void VideoTargetReporting::on_pushButton_HIDsend_clicked()
// {
//     // 1. 检查HID设备是否已连接
//     if (hid_dev == nullptr)
//     {
//         ui->textEdit_HID->append("[错误] HID设备未连接，请先点击\"HID连接\"");
//         return;
//     }

//     // 2. 获取lineEdit中的输入文本（例如："55 AA 11 00 11"）
//     QString hex_str = ui->lineEdit->text().trimmed(); // lineEdit是你的输入控件名
//     if (hex_str.isEmpty())
//     {
//         ui->textEdit_HID->append("[错误] 输入为空，请输入十六进制指令");
//         return;
//     }

//     // 3. 校验并处理输入格式（去除空格，检查是否为合法十六进制）
//     hex_str.remove(" "); // 去掉所有空格，变为"55AA110011"
//     if (!isHexStringValid(hex_str))
//     {
//         ui->textEdit_HID->append("[错误] 输入包含非十六进制字符（仅允许0-9、A-F、a-f）");
//         return;
//     }
//     if (hex_str.length() % 2 != 0)
//     {
//         ui->textEdit_HID->append("[错误] 十六进制长度必须为偶数（例如\"55 AA\"）");
//         return;
//     }

//     // 4. 将十六进制字符串转换为字节数组
//     QByteArray send_data = QByteArray::fromHex(hex_str.toUtf8());

//     // 5. 发送数据到HID设备（注意：HID通常需要报告ID，这里默认第0位为报告ID=0x23）
//     const unsigned char REPORT_ID = 0x23; // 固定报告ID
//     unsigned char send_buf[64] = {0};     // 假设设备支持最大64字节报告
//     send_buf[0] = REPORT_ID;              // 第1个字节填入报告ID
//     int send_len = qMin(send_data.size(), 63); // 留1字节给报告ID（第0位）
//     memcpy(send_buf + 1, send_data.data(), send_len); // 从第1位开始存数据

//     // 6. 调用hidapi发送函数
//     int bytes_sent = hid_write(hid_dev, send_buf, send_len + 1); // +1包含报告ID

//     // 7. 在textEdit中显示发送状态
//     if (bytes_sent > 0)
//     {
//         // 格式化显示发送的十六进制（带空格分隔，如"55 AA 11 00 11"）
//         QString formatted_hex;
//         for (int i = 0; i < send_data.size(); ++i)
//         {
//             formatted_hex += QString("%1 ").arg((unsigned char)send_data[i], 2, 16, QChar('0')).toUpper();
//         }
//         ui->textEdit_HID->append("[发送成功] " + formatted_hex.trimmed());
//     }
//     else
//     {
//         ui->textEdit_HID->append("[发送失败] 设备可能已断开或数据格式错误");
//         // 可选：尝试重新连接设备
//         // hid_close(hid_dev);
//         // hid_dev = hid_open(VID, PID, NULL);
//     }
// }

// 辅助函数：校验字符串是否为合法十六进制
bool VideoTargetReporting::isHexStringValid(const QString& str)
{
    QRegularExpression hex_regex("^[0-9A-Fa-f]+$");
    return hex_regex.match(str).hasMatch();
}

void VideoTargetReporting::on_pushButton_reset_clicked()
{
    // 1. 清空三个靶面的弹孔
    if (m_chestHoleOverlay) {
        m_chestHoleOverlay->clearHoles(); // 调用HoleOverlay的清空函数
    }
    if (m_ipscHoleOverlay) {
        m_ipscHoleOverlay->clearHoles();
    }
    if (m_idpaHoleOverlay) {
        m_idpaHoleOverlay->clearHoles();
    }

    // 2. 清空成绩表格（tableWidget）
    if (ui->tableWidget) {
        ui->tableWidget->clearContents(); // 清空表格内容（保留表头）
        ui->tableWidget->setRowCount(0);  // 重置行数为0
        m_currentBulletList.clear();
    }

    if(ui->textEdit_HID){
        ui->textEdit_HID->clear();
    }
}


void VideoTargetReporting::on_pushButton_settings_clicked()
{
    QWidget* currentPage = ui->stackedWidget_right->currentWidget();
    if(currentPage->objectName() == "page_history"){
        m_fromHistoryToSettings = true;
    }
    ui->stackedWidget_right->setCurrentWidget(ui->pageSettings);
    ui->pushButton_back->setVisible(true);
    // ui->pushButton_settings->setEnabled(false);
}


void VideoTargetReporting::on_pushButton_back_clicked()
{
    QWidget* currentPage = ui->stackedWidget_right->currentWidget();
    if (currentPage->objectName() == "page_history" || m_fromHistoryToSettings) {
        // 👉 历史记录页返回：还原靶型状态 + 切换回主射击页
        // 还原主界面原始的连接状态（文本 + 颜色）
        ui->label_connectionState->setText(m_mainConnStateText);
        QPalette palette = ui->label_connectionState->palette();
        palette.setColor(QPalette::WindowText, m_mainConnStateColor);
        ui->label_connectionState->setPalette(palette);
        // 1. 退出历史模式
        m_isHistoryMode = false;

        // 2. 清空历史弹孔和高亮
        clearHistoryHoles();
        // 还原主界面靶面和弹孔（原有逻辑）
        switchTargetSurface(m_current_target);
        redrawCurrentShootingHoles();
        m_fromHistoryToSettings = false;
    }

    ui->stackedWidget_right->setCurrentWidget(ui->pageScoreDisplay);
    ui->pushButton_back->setVisible(false);
    // ui->pushButton_settings->setEnabled(true);
}

// 通用函数：设置选中状态（显示当前√，隐藏其他√）
void VideoTargetReporting::setTargetSelected(QLabel* currentLabel, QLabel* other1, QLabel* other2) {
    QPixmap pixmap(":/new/prefix1/icon/tick.png");
    // 显示当前选中的√
    currentLabel->setPixmap(pixmap);
    currentLabel->setScaledContents(true);
    // 隐藏其他两个
    other1->clear();
    other2->clear();
}

void VideoTargetReporting::on_pushButton_reconnection_clicked()
{
    m_business_handler->startHandshake();
    ui->frame_connectionState->setStyleSheet("background-color: rgb(255, 0, 0);");
    m_business_handler->m_shootingState = ShootingState::STATE_TO_START;
    ui->label_currentState->setText("待开始");
    ui->label_currentState->setStyleSheet("color: rgb(255, 0, 0)");
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_end->setEnabled(false);
}


void VideoTargetReporting::on_pushButton_connect_clicked() {
    // 1. 检查是否已选择靶型（使用HID_TARGET枚举判断）
    if (m_current_target == HID_TARGET::TARGET_NONE) { // 直接用当前选中的靶型变量判断
        QMessageBox::warning(this, "选择提示", "请先选择靶型！");
        ui->textEdit_HID->append("[靶型选择] 未选择靶型，发送失败");
        return;
    }

    // 2. 检查 HID 连接状态（保持原逻辑）
    if (hid_dev == nullptr || m_business_handler->getConnectionState() != HID_STATE::STATE_CONNECTED) {
        QMessageBox::warning(this, "连接提示", "请先完成 HID 设备连接和握手！");
        ui->textEdit_HID->append("[靶型选择] HID 未连接或未握手，发送失败");
        return;
    }

    // 3. 发送靶型选择指令前，先保存当前靶型到配置文件（核心：记忆功能）
    QSettings settings("SpecialPie", "TargetReportingSystem");
    settings.setValue("LastSelectedTarget", static_cast<uint8_t>(m_current_target));
    ui->textEdit_HID->append(QString("[靶型记忆] 已保存靶型：%1").arg(getTargetName(m_current_target)));

    // 4. 生成并发送靶型选择指令（保持原逻辑，补充日志）
    HID_Response resp = HIDResponseGenerator::generateTargetSelectReq(m_current_target);
    if (resp.need_send) {
        emit m_business_handler->sendResponse(resp.raw_data);
        ui->textEdit_HID->append(
            QString("[靶型选择] 发送 0x%1 指令（%2）")
                .arg(
                    static_cast<int>(HID_CMD::CMD_TARGET_SELECT),
                    2,
                    16,
                    QLatin1Char('0')
                    )
                .arg(getTargetName(m_current_target)) // 第二个参数用链式arg()传递
            );
    } else {
        ui->textEdit_HID->append("[靶型选择] 指令生成失败，发送取消");
    }

    // 5. 页面切换（保持原逻辑）
    ui->stackedWidget_right->setCurrentIndex(0);
    ui->pushButton_settings->setEnabled(true);
    ui->pushButton_back->hide();
}
QString VideoTargetReporting::getTargetName(HID_TARGET target) {
    switch (target) {
    case HID_TARGET::TARGET_CHEST: return "胸环靶";
    case HID_TARGET::TARGET_IPSC: return "IPSC靶";
    case HID_TARGET::TARGET_IDPA: return "IDPA靶";
    default: return "未选择";
    }
}
void VideoTargetReporting::on_pushButton_start_clicked() {
    // 1. 检查是否已选择靶型
    if (m_business_handler->getCurrentTarget() == HID_TARGET::TARGET_NONE) { // 假设默认值为 TARGET_NONE
        QMessageBox::warning(this, "选择提示", "请先选择靶型！");
        ui->textEdit_HID->append("[开始状态] 未选择靶型，发送失败");
        return;
    }

    // 2. 检查 HID 连接状态（必须握手成功才能发送）
    if (hid_dev == nullptr) {
        QMessageBox::warning(this, "连接提示", "请先完成 HID 设备连接！");
        ui->textEdit_HID->append("[开始状态] HID 未连接，发送失败");
        return;
    }

    // 3. 检查设备连接状态
    if(ui->label_connect->text() != "已连接"){
        QMessageBox::warning(this, "连接提示", "请先完成设备连接和握手！");
        ui->textEdit_HID->append("[开始状态]设备未连接或未握手，发送失败");
        return;
    }

    // 4. 发送开始指令
    HID_Response resp = HIDResponseGenerator::generateStartReq();
    if (resp.need_send) {
        emit m_business_handler->sendResponse(resp.raw_data); // 通过已有发送链路发送
    } else {
        ui->textEdit_HID->append("[开始状态] 指令生成失败，发送取消");
    }
}

void VideoTargetReporting::on_pushButton_pause_clicked()
{
    HID_Response resp = HIDResponseGenerator::generatePauseReq();
    if (resp.need_send) {
        emit m_business_handler->sendResponse(resp.raw_data); // 通过已有发送链路发送
    } else {
        ui->textEdit_HID->append("[暂停状态] 指令生成失败，发送取消");
    }
}

void VideoTargetReporting::on_pushButton_end_clicked()
{
    HID_Response resp = HIDResponseGenerator::generateEndReq();
    if (resp.need_send) {
        emit m_business_handler->sendResponse(resp.raw_data); // 通过已有发送链路发送
    } else {
        ui->textEdit_HID->append("[结束状态] 指令生成失败，发送取消");
    }
}

void VideoTargetReporting::addBulletToTable(int x, int y, QString score, HID_TARGET targetType) {
    // 表格带坐标的成绩，不带坐标的是score
    QString score_str = QString("(%1, %2) - %3").arg(x).arg(y).arg(score);
    // 关键步骤1：获取当前靶面的 HoleOverlay，校验弹孔是否有效（去重）
    HoleOverlay* overlay = getCurrentOverlay();
    if (!overlay) {
        ui->textEdit_HID->append("[错误] 未找到当前靶面，无法添加弹孔");
        return;
    }

    // 关键步骤2：复用 HoleOverlay 的去重逻辑，判断弹孔是否重复
    if (!overlay->isHoleValid(x, y)) {
        ui->textEdit_HID->append(QString("[去重] 重复弹孔（X:%1, Y:%2），不添加到表格和绘制").arg(x).arg(y));
        return; // 重复弹孔直接返回，不执行后续添加逻辑
    }

    QUrl soundUrl;
    if (score.contains("A")) {
        soundUrl = QUrl("qrc:/new/prefix1/sound/score_A.wav");
    } else if (score.contains("B")) {
        soundUrl = QUrl("qrc:/new/prefix1/sound/score_B.wav");
    } else if (score.contains("C")) {
        soundUrl = QUrl("qrc:/new/prefix1/sound/score_C.wav");
    } else if (score.contains("D")) {
        soundUrl = QUrl("qrc:/new/prefix1/sound/score_D.wav");
    }else if (score == "0环") {
        soundUrl = QUrl("qrc:/new/prefix1/sound/score_0.wav");
    }else if (score.contains("5环")) {
        soundUrl = QUrl("qrc:/new/prefix1/sound/score_5.wav");
    }else if (score.contains("6环")) {
        soundUrl = QUrl("qrc:/new/prefix1/sound/score_6.wav");
    }else if (score.contains("7环")) {
        soundUrl = QUrl("qrc:/new/prefix1/sound/score_7.wav");
    }else if (score.contains("8环")) {
        soundUrl = QUrl("qrc:/new/prefix1/sound/score_8.wav");
    }else if (score.contains("9环")) {
        soundUrl = QUrl("qrc:/new/prefix1/sound/score_9.wav");
    }else if (score == "10环") {
        soundUrl = QUrl("qrc:/new/prefix1/sound/score_10.wav");
    }

    // 播放音效（确保路径有效）
    if (soundUrl.isValid()) {
        QSoundEffect *sound = new QSoundEffect(this);
        sound->setSource(soundUrl);
        sound->play();
        // 可选：设置音效音量（0.0~1.0）
        // sound->setVolume(0.8);
    }

    // 关键步骤3：弹孔有效，才添加表格（只加一次）
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    // 序号（行号+1，和弹孔index一一对应）
    QTableWidgetItem* item_idx = new QTableWidgetItem(QString::number(row + 1));
    item_idx->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 0, item_idx);

    // 成绩
    QTableWidgetItem* item_score = new QTableWidgetItem(score_str);
    item_score->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 1, item_score);

    // 时间
    QTableWidgetItem* item_time = new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    item_time->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 2, item_time);

    // 同步到弹孔记录列表
    BulletRecord record;
    record.index = row + 1; // 序号和表格行号一致，和弹孔index（overlay->holeCount() 后续会+1）对应
    record.x = x;
    record.y = y;
    record.score = score;
    record.targetType = targetType;
    record.time = QDateTime::currentDateTime().toString("HH:mm:ss");
    m_currentBulletList.append(record);

    // 非历史模式下，绘制弹孔（和表格添加同步）
    if (!m_isHistoryMode) {
        // 恢复上一个最新弹孔颜色
        if (m_latestHoleIndex != -1 && m_latestHoleIndex < overlay->holeCount()) {
            overlay->updateHoleColor(m_latestHoleIndex, overlay->getDefaultColor());
        }
        // 绘制新弹孔（最新颜色）
        overlay->addHole(x, y, overlay->getLatestColor());
        m_latestHoleIndex = overlay->holeCount() - 1; // 弹孔index = 表格行号（一一对应）

        // 恢复选中弹孔高亮
        if (m_selectedHoleIndex != -1 && m_selectedHoleIndex < overlay->holeCount()) {
            overlay->updateHoleColor(m_selectedHoleIndex, overlay->getSelectedColor());
        }
    } else {
        ui->textEdit_HID->append(QString("[历史模式] 有效弹孔：X:%1, Y:%2, 成绩:%3（返回后显示）").arg(x).arg(y).arg(score));
    }

    m_current_target = targetType;
}

// 保存记录按钮（修改：调用接口）
void VideoTargetReporting::on_pushButton_saveRecord_clicked() {
    if (m_currentBulletList.isEmpty()) {
        QMessageBox::warning(this, "保存提示", "当前无射击数据，无法保存！");
        ui->textEdit_HID->append("[历史记录] 保存失败：无射击数据");
        return;
    }

    if (m_current_target == HID_TARGET::TARGET_NONE) {
        QMessageBox::warning(this, "保存提示", "请先选择靶型再保存！");
        ui->textEdit_HID->append("[历史记录] 保存失败：未选择靶型");
        return;
    }

    // 构造记录（逻辑不变）
    QString targetName = getTargetName(m_current_target);
    QString title = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + " - " + targetName;
    HistoryRecord history;
    history.title = title;
    history.createTime = QDateTime::currentDateTime();
    history.bulletList = m_currentBulletList;

    // 核心修改：调用管理器接口保存
    m_historyManager->saveHistoryRecord(history);

    QMessageBox::information(this, "保存成功", "当前射击记录已保存！");
    redrawCurrentShootingHoles();
    ui->textEdit_HID->append(QString("[历史记录] 保存成功：%1（共%2发）").arg(title).arg(m_currentBulletList.size()));
}


// 保存单条历史记录到本地（QSettings 序列化）
void VideoTargetReporting::saveHistoryRecord(const HistoryRecord& history) {
    QSettings settings("SpecialPie", "TargetReportingSystem"); // 与靶型记忆共用配置文件

    // 1. 先读取已有的历史记录
    QList<HistoryRecord> existingRecords = loadAllHistoryRecords();

    // 2. 添加新记录（追加到列表末尾）
    existingRecords.append(history);

    // 3. 序列化存储到配置文件
    settings.beginWriteArray("HistoryRecords"); // 数组名：HistoryRecords（唯一标识）
    for (int i = 0; i < existingRecords.size(); ++i) {
        settings.setArrayIndex(i); // 设置当前数组索引
        const HistoryRecord& rec = existingRecords[i];
        // 存储记录基本信息
        settings.setValue("Title", rec.title);
        settings.setValue("CreateTime", rec.createTime);

        // 序列化弹孔列表（嵌套数组）
        settings.beginWriteArray("BulletList");
        for (int j = 0; j < rec.bulletList.size(); ++j) {
            settings.setArrayIndex(j);
            const BulletRecord& bullet = rec.bulletList[j];
            settings.setValue("Index", bullet.index);
            settings.setValue("X", bullet.x);
            settings.setValue("Y", bullet.y);
            settings.setValue("Score", bullet.score);
            settings.setValue("TargetType", static_cast<uint8_t>(bullet.targetType));
            settings.setValue("Time", bullet.time);
        }
        settings.endArray(); // 结束弹孔列表序列化
    }
    settings.endArray(); // 结束历史记录序列化
}

// 读取所有历史记录（反序列化）
QList<HistoryRecord> VideoTargetReporting::loadAllHistoryRecords() {
    QList<HistoryRecord> records;
    QSettings settings("SpecialPie", "TargetReportingSystem");

    // 1. 读取历史记录总数
    int historyCount = settings.beginReadArray("HistoryRecords");
    for (int i = 0; i < historyCount; ++i) {
        settings.setArrayIndex(i);
        HistoryRecord rec;
        // 读取记录基本信息
        rec.title = settings.value("Title").toString();
        rec.createTime = settings.value("CreateTime").toDateTime();

        // 读取弹孔列表
        int bulletCount = settings.beginReadArray("BulletList");
        for (int j = 0; j < bulletCount; ++j) {
            settings.setArrayIndex(j);
            BulletRecord bullet;
            bullet.index = settings.value("Index").toInt();
            bullet.x = settings.value("X").toInt();
            bullet.y = settings.value("Y").toInt();
            bullet.score = settings.value("Score").toString();
            bullet.targetType = static_cast<HID_TARGET>(settings.value("TargetType").toUInt());
            bullet.time = settings.value("Time").toString();
            rec.bulletList.append(bullet);
        }
        settings.endArray(); // 结束弹孔列表读取

        records.append(rec);
    }
    settings.endArray(); // 结束历史记录读取

    return records;
}

void VideoTargetReporting::on_pushButton_history_clicked() {
    QWidget* currentPage = ui->stackedWidget_right->currentWidget();
    if(currentPage->objectName() == "page_score"){
        m_mainConnStateText = ui->label_connectionState->text();
        m_mainConnStateColor = ui->label_connectionState->palette().color(QPalette::WindowText);
    }

    ui->pushButton_back->setVisible(true);
    ui->stackedWidget_right->setCurrentWidget(ui->pageScoreHistory);

    // 关键修改1：先清空 UI 残留（包括状态标签），再保存主界面原始状态
    ui->listWidget_historyTitles->clear();
    ui->tableWidget_historyDetail->setRowCount(0);
    ui->label_historyDetailTitle->setText("");
    clearHistoryHoles();
    m_historyHighlightedIndex = -1;
    m_isHistoryMode = true;

    // 加载最新历史记录
    m_displayHistoryRecords = m_historyManager->loadAllHistoryRecords();

    if (m_displayHistoryRecords.isEmpty()) {
        ui->listWidget_historyTitles->addItem("暂无历史记录");
        switchTargetSurface(HID_TARGET::TARGET_NONE);
        // 关键修改2：强制更新状态为「未选择靶型」（覆盖主界面原始状态）
        updateConnectionState(HID_TARGET::TARGET_NONE);
    } else {
        // 非空场景：填充列表并选中第一条
        for (const auto& rec : m_displayHistoryRecords) {
            new QListWidgetItem(rec.title, ui->listWidget_historyTitles);
        }
        ui->listWidget_historyTitles->setCurrentRow(0);
        const HistoryRecord& firstRec = m_displayHistoryRecords[0];
        showHistoryDetail(0);
        HID_TARGET firstTarget = firstRec.bulletList.isEmpty() ? HID_TARGET::TARGET_NONE : firstRec.bulletList.first().targetType;
        drawHistoryHoles(firstRec.bulletList, firstTarget);
        updateConnectionState(firstTarget);
    }

    // 关键修改3：最后保存主界面原始状态（避免覆盖历史模式的状态设置）

}

void VideoTargetReporting::on_listWidget_historyTitles_itemClicked(QListWidgetItem *item) {
    // 1. 先判断是否是“暂无历史记录”项，直接返回
    if (item->text() == "暂无历史记录") {
        ui->tableWidget_historyDetail->setRowCount(0);
        clearHistoryHoles(); // 清空靶面，避免残留
        switchTargetSurface(HID_TARGET::TARGET_NONE);
        m_historyHighlightedIndex = -1;
        return;
    }

    // 2. 获取点击的索引，先校验索引有效性
    int index = ui->listWidget_historyTitles->row(item);
    if (index < 0 || index >= m_displayHistoryRecords.size()) {
        qWarning() << "[历史记录] 索引越界：index=" << index << "，列表长度=" << m_displayHistoryRecords.size();
        ui->tableWidget_historyDetail->setRowCount(0);
        clearHistoryHoles();
        return;
    }

    // 3. 安全访问历史记录（此时索引一定有效）
    const HistoryRecord& rec = m_displayHistoryRecords[index];
    showHistoryDetail(index); // 显示详情表格

    HID_TARGET currentTarget = rec.bulletList.isEmpty() ? HID_TARGET::TARGET_NONE : rec.bulletList.first().targetType;
    updateConnectionState(currentTarget);

    // 4. 绘制弹孔（额外校验弹孔列表是否为空）
    if (rec.bulletList.isEmpty()) {
        clearHistoryHoles();
        switchTargetSurface(HID_TARGET::TARGET_NONE);
        m_historyHighlightedIndex = -1;
        return;
    }

    HID_TARGET recordTarget = rec.bulletList.first().targetType;
    drawHistoryHoles(rec.bulletList, recordTarget);
    m_isHistoryMode = true;
}

void VideoTargetReporting::showHistoryDetail(int index) {
    // 1. 先校验索引有效性
    if (index < 0 || index >= m_displayHistoryRecords.size()) {
        qWarning() << "[历史详情] 索引越界：index=" << index;
        ui->tableWidget_historyDetail->setRowCount(0);
        ui->label_historyDetailTitle->setText("");
        return;
    }

    const HistoryRecord& rec = m_displayHistoryRecords[index];
    QTableWidget* detailTable = ui->tableWidget_historyDetail;
    detailTable->setRowCount(0);

    // 2. 填充详情表格（原逻辑不变）
    for (const auto& bullet : rec.bulletList) {
        int row = detailTable->rowCount();
        detailTable->insertRow(row);
        // 序号列
        QTableWidgetItem* indexItem = new QTableWidgetItem(QString::number(bullet.index));
        indexItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        detailTable->setItem(row, 0, indexItem);
        // 成绩列
        QTableWidgetItem* scoreItem = new QTableWidgetItem(bullet.score);
        scoreItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        detailTable->setItem(row, 1, scoreItem);
        // 时间列
        QTableWidgetItem* timeItem = new QTableWidgetItem(bullet.time);
        timeItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        detailTable->setItem(row, 2, timeItem);
    }

    // 更新详情标题
    QString detailTitle = QString("成绩详情：%1（共%2发）").arg(rec.title).arg(rec.bulletList.size());
    ui->label_historyDetailTitle->setText(detailTitle);
    ui->label_historyDetailTitle->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

void VideoTargetReporting::onBulletDataReceived(int x, int y, QString score, HID_TARGET targetType) {
    // 1. 将弹孔数据添加到表格（复用之前的 addBulletToTable 函数）
    addBulletToTable(x, y, score, targetType);

    // 2. （可选）同步更新当前靶型（如果需要）
    m_current_target = targetType;
}

void VideoTargetReporting::on_pushButton_deleteRecord_clicked() {
    int currentRow = ui->listWidget_historyTitles->currentRow();
    if (currentRow < 0 || m_displayHistoryRecords.isEmpty()) {
        QMessageBox::warning(this, "删除提示", "请先选中一条记录！");
        return;
    }

    const HistoryRecord& rec = m_displayHistoryRecords[currentRow];
    int ret = QMessageBox::question(
        this,
        "确认删除",
        QString("是否删除记录：\n%1？").arg(rec.title),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
        );
    if (ret != QMessageBox::Yes) {
        return;
    }

    // 核心修改1：调用管理器接口删除
    bool success = m_historyManager->deleteHistoryRecord(currentRow);
    if (success) {
        ui->textEdit_HID->append(QString("[历史记录] 已删除：%1").arg(rec.title));
        QMessageBox::information(this, "删除成功", "选中的记录已删除");

        // 核心修改2：删除成功后，刷新历史记录 UI（清空残留 + 重新加载数据）
        refreshHistoryUI(); // 调用通用刷新函数（关键！解决残留问题）
    } else {
        QMessageBox::warning(this, "删除失败", "删除记录时发生错误！");
    }
}
// 辅助函数：删除指定索引的历史记录（并同步到本地配置文件）
void VideoTargetReporting::deleteHistoryRecord(int index) {
    // 1. 从内存列表中删除
    if (index >= 0 && index < m_historyRecords.size()) {
        m_historyRecords.removeAt(index);
    }

    // 2. 同步删除本地配置文件中的记录（重新写入整个列表）
    QSettings settings("SpecialPie", "TargetReportingSystem");
    settings.beginWriteArray("HistoryRecords"); // 覆盖原有数组
    for (int i = 0; i < m_historyRecords.size(); ++i) {
        settings.setArrayIndex(i);
        const HistoryRecord& rec = m_historyRecords[i];
        settings.setValue("Title", rec.title);
        settings.setValue("CreateTime", rec.createTime);

        // 重新写入弹孔列表
        settings.beginWriteArray("BulletList");
        for (int j = 0; j < rec.bulletList.size(); ++j) {
            settings.setArrayIndex(j);
            const BulletRecord& bullet = rec.bulletList[j];
            settings.setValue("Index", bullet.index);
            settings.setValue("X", bullet.x);
            settings.setValue("Y", bullet.y);
            settings.setValue("Score", bullet.score);
            settings.setValue("TargetType", static_cast<uint8_t>(bullet.targetType));
            settings.setValue("Time", bullet.time);
        }
        settings.endArray();
    }
    settings.endArray();

    // 3. 更新UI列表（重新加载并显示）
    ui->listWidget_historyTitles->clear();
    if (m_historyRecords.isEmpty()) {
        ui->listWidget_historyTitles->addItem("暂无历史记录");
        ui->tableWidget_historyDetail->setRowCount(0); // 清空详情表格
        ui->label_historyDetailTitle->setText(""); // 清空标题
        return;
    }

    // 重新填充列表并选中第一条
    for (const auto& rec : m_historyRecords) {
        new QListWidgetItem(rec.title, ui->listWidget_historyTitles);
    }
    ui->listWidget_historyTitles->setCurrentRow(0);
    showHistoryDetail(0); // 显示新的第一条记录详情
}

void VideoTargetReporting::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    if (!item) return;

    HoleOverlay* overlay = getCurrentOverlay();
    if (!overlay) return;

    // 1. 获取点击的表格行索引（表格行 = 弹孔索引，一一对应）
    int newSelectedIndex = item->row();

    // 2. 校验索引有效性
    if (newSelectedIndex < 0 || newSelectedIndex >= overlay->holeCount()) {
        return;
    }

    // 3. 恢复上一个选中弹孔的颜色（区分是普通弹孔还是最新弹孔）
    if (m_selectedHoleIndex != -1 && m_selectedHoleIndex < overlay->holeCount()) {
        QColor restoreColor = (m_selectedHoleIndex == m_latestHoleIndex)
        ? overlay->getLatestColor()  // 上一个选中的是最新弹孔，恢复金色
        : overlay->getDefaultColor(); // 普通弹孔，恢复默认黑色
        overlay->updateHoleColor(m_selectedHoleIndex, restoreColor);
    }

    // 4. 高亮当前选中的弹孔（绿色）
    overlay->updateHoleColor(newSelectedIndex, overlay->getSelectedColor());

    // 5. 更新选中索引状态
    m_selectedHoleIndex = newSelectedIndex;
}


void VideoTargetReporting::mousePressEvent(QMouseEvent *event) {
    // 若点击区域不是 tableWidget，取消选中
    if (!ui->tableWidget->geometry().contains(event->pos())) {
        HoleOverlay* overlay = getCurrentOverlay();
        if (!overlay || m_selectedHoleIndex == -1) {
            QMainWindow::mousePressEvent(event);
            return;
        }

        // 恢复选中弹孔的原颜色
        QColor restoreColor = (m_selectedHoleIndex == m_latestHoleIndex)
                                  ? overlay->getLatestColor()
                                  : overlay->getDefaultColor();
        overlay->updateHoleColor(m_selectedHoleIndex, restoreColor);

        // 重置选中索引
        m_selectedHoleIndex = -1;
    }

    QMainWindow::mousePressEvent(event);
}

// 切换靶面显示（根据靶型切换左侧靶面的UI）
void VideoTargetReporting::switchTargetSurface(HID_TARGET targetType) {
    // 假设你的左侧靶面通过 stackedWidget_target 切换（根据实际UI调整）
    switch (targetType) {
    case HID_TARGET::TARGET_CHEST:
        ui->stackedWidget_target->setCurrentIndex(1); // 胸环靶页面索引
        m_historyCurrentTarget = HID_TARGET::TARGET_CHEST;
        break;
    case HID_TARGET::TARGET_IPSC:
        ui->stackedWidget_target->setCurrentIndex(2); // IPSC靶页面索引
        m_historyCurrentTarget = HID_TARGET::TARGET_IPSC;
        break;
    case HID_TARGET::TARGET_IDPA:
        ui->stackedWidget_target->setCurrentIndex(3); // IDPA靶页面索引
        m_historyCurrentTarget = HID_TARGET::TARGET_IDPA;
        break;
    default:
        ui->stackedWidget_target->setCurrentIndex(0); // 空白页
        m_historyCurrentTarget = HID_TARGET::TARGET_NONE;
        break;
    }
}

// 在历史模式下绘制弹孔（默认颜色：黑色，与当前射击的红色区分，可选）
void VideoTargetReporting::drawHistoryHoles(const QList<BulletRecord>& bulletList, HID_TARGET targetType) {
    // 1. 清空之前的历史弹孔
    clearHistoryHoles();

    // 2. 切换到当前历史记录的靶面
    switchTargetSurface(targetType);

    // 3. 获取当前历史靶面的绘制层
    HoleOverlay* historyOverlay = nullptr;
    switch (targetType) {
    case HID_TARGET::TARGET_CHEST: historyOverlay = m_chestHoleOverlay; break;
    case HID_TARGET::TARGET_IPSC: historyOverlay = m_ipscHoleOverlay; break;
    case HID_TARGET::TARGET_IDPA: historyOverlay = m_idpaHoleOverlay; break;
    default: return;
    }

    // 4. 绘制所有历史弹孔（默认颜色：黑色，可通过接口修改）
    QColor historyDefaultColor = QColor(0, 0, 0, 200); // 半透明黑色
    for (const auto& bullet : bulletList) {
        // 注意：历史弹孔存储的是原始坐标，需通过绘制层映射到靶面
        historyOverlay->addHole(bullet.x, bullet.y, historyDefaultColor);
    }

    // 5. 重置历史模式的高亮索引
    m_historyHighlightedIndex = -1;
}

void VideoTargetReporting::clearHistoryHoles() {
    // 清空所有靶面的历史弹孔（避免切换记录时残留）
    if (m_chestHoleOverlay) m_chestHoleOverlay->clearHoles();
    if (m_ipscHoleOverlay) m_ipscHoleOverlay->clearHoles();
    if (m_idpaHoleOverlay) m_idpaHoleOverlay->clearHoles();
    m_historyHighlightedIndex = -1;
}

void VideoTargetReporting::on_tableWidget_historyDetail_itemClicked(QTableWidgetItem *item) {
    // 步骤 1：基础校验（缺一不可）
    if (!item) { // 点击的 item 为空
        qWarning() << "[历史弹孔高亮] 点击的 item 为空";
        return;
    }
    if (!m_isHistoryMode) { // 未处于历史模式
        qWarning() << "[历史弹孔高亮] 未处于历史模式，忽略点击";
        return;
    }
    if (m_displayHistoryRecords.isEmpty()) { // 无历史记录
        qWarning() << "[历史弹孔高亮] 无历史记录，忽略点击";
        return;
    }

    // 步骤 2：获取当前选中的历史记录索引（UI 列表选中行）
    int currentHistoryIndex = ui->listWidget_historyTitles->currentRow();
    if (currentHistoryIndex < 0 || currentHistoryIndex >= m_displayHistoryRecords.size()) { // 索引无效
        qWarning() << "[历史弹孔高亮] 选中的历史记录索引无效：" << currentHistoryIndex;
        return;
    }

    // 步骤 3：获取当前历史记录的弹孔列表
    const HistoryRecord& currentRec = m_displayHistoryRecords[currentHistoryIndex];
    if (currentRec.bulletList.isEmpty()) { // 该记录无弹孔数据
        qWarning() << "[历史弹孔高亮] 选中的记录无弹孔数据";
        clearHistoryHoles();
        return;
    }

    // 步骤 4：获取点击的弹孔索引（表格行索引 = 弹孔列表索引）
    int bulletIndex = item->row();
    if (bulletIndex < 0 || bulletIndex >= currentRec.bulletList.size()) { // 弹孔索引越界
        qWarning() << "[历史弹孔高亮] 弹孔索引越界：" << bulletIndex << "，总弹孔数：" << currentRec.bulletList.size();
        return;
    }

    // 步骤 5：获取当前历史记录对应的靶面绘制层（关键！之前可能获取失败）
    // 从当前记录的第一个弹孔中获取靶型（确保靶型一致）
    HID_TARGET recordTarget = currentRec.bulletList.first().targetType;
    HoleOverlay* historyOverlay = nullptr;
    switch (recordTarget) {
    case HID_TARGET::TARGET_CHEST:
        historyOverlay = m_chestHoleOverlay;
        break;
    case HID_TARGET::TARGET_IPSC:
        historyOverlay = m_ipscHoleOverlay;
        break;
    case HID_TARGET::TARGET_IDPA:
        historyOverlay = m_idpaHoleOverlay;
        break;
    default:
        qWarning() << "[历史弹孔高亮] 无效靶型：" << static_cast<int>(recordTarget);
        return;
    }

    // 步骤 6：校验绘制层是否有效（避免空指针）
    if (!historyOverlay) {
        qWarning() << "[历史弹孔高亮] 靶面绘制层为空";
        return;
    }
    if (historyOverlay->holeCount() == 0) { // 绘制层未绘制弹孔（可能之前绘制失败）
        qWarning() << "[历史弹孔高亮] 绘制层无弹孔，重新绘制";
        drawHistoryHoles(currentRec.bulletList, recordTarget); // 重新绘制该记录的弹孔
        if (historyOverlay->holeCount() == 0) { // 重新绘制后仍无弹孔，直接返回
            qWarning() << "[历史弹孔高亮] 重新绘制后仍无弹孔";
            return;
        }
    }

    // 步骤 7：校验弹孔索引与绘制层的弹孔数匹配（避免越界）
    if (bulletIndex >= historyOverlay->holeCount()) {
        qWarning() << "[历史弹孔高亮] 弹孔索引与绘制层弹孔数不匹配：索引" << bulletIndex << "，绘制层弹孔数" << historyOverlay->holeCount();
        return;
    }

    // 步骤 8：执行高亮逻辑（恢复上一个，高亮当前）
    QColor historyDefaultColor = QColor(0, 0, 0, 200); // 历史弹孔默认颜色（半透明黑色）
    QColor historyHighlightColor = QColor(0, 255, 0, 220); // 高亮颜色（绿色）

    // 恢复上一个高亮弹孔的默认颜色
    if (m_historyHighlightedIndex != -1 && m_historyHighlightedIndex < historyOverlay->holeCount()) {
        historyOverlay->updateHoleColor(m_historyHighlightedIndex, historyDefaultColor);
    }

    // 高亮当前点击的弹孔
    historyOverlay->updateHoleColor(bulletIndex, historyHighlightColor);

    // 更新高亮索引
    m_historyHighlightedIndex = bulletIndex;
}

// 新增辅助函数：重新绘制当前射击的弹孔（返回主界面时调用）
void VideoTargetReporting::redrawCurrentShootingHoles() {
    // 1. 校验状态
    if (m_current_target == HID_TARGET::TARGET_NONE && m_currentBulletList.isEmpty()) {
        ui->textEdit_HID->append("[提示] 无射击数据，无需绘制");
        return;
    }

    // 2. 获取当前靶面绘制层（若未选中靶型，用最后一个弹孔的靶型）
    HoleOverlay* currentOverlay = getCurrentOverlay();
    if (!currentOverlay && !m_currentBulletList.isEmpty()) {
        m_current_target = m_currentBulletList.last().targetType; // 用最后一个弹孔的靶型
        currentOverlay = getCurrentOverlay();
    }
    if (!currentOverlay) return;

    // 3. 清空绘制层（避免历史弹孔残留）
    currentOverlay->clearHoles();
    currentOverlay->initLastHole();
    m_latestHoleIndex = -1;

    // 4. 重新绘制所有弹孔（包括历史模式期间新增的）
    for (int i = 0; i < m_currentBulletList.size(); ++i) {
        const auto& bullet = m_currentBulletList[i];
        QColor holeColor;

        // 最新弹孔（最后一个）用金色，其余用红色
        if (i == m_currentBulletList.size() - 1) {
            holeColor = currentOverlay->getLatestColor();
            m_latestHoleIndex = i;
        } else {
            holeColor = currentOverlay->getDefaultColor();
        }

        // 绘制弹孔（用原始坐标，绘制层自动映射）
        currentOverlay->addHole(bullet.x, bullet.y, holeColor);
    }

    // 5. 恢复选中弹孔高亮（如果有）
    if (m_selectedHoleIndex != -1 && m_selectedHoleIndex < currentOverlay->holeCount()) {
        currentOverlay->updateHoleColor(m_selectedHoleIndex, currentOverlay->getSelectedColor());
    }

    // 6. 同步当前靶型（确保后续操作正确）
    if (!m_currentBulletList.isEmpty()) {
        m_current_target = m_currentBulletList.last().targetType;
    }

    ui->textEdit_HID->append(QString("[提示] 重新绘制弹孔：共%1发（含历史模式新增）").arg(m_currentBulletList.size()));
}

void VideoTargetReporting::onHistoryRecordsChanged() {
    // 1. 清空 UI 列表（关键：先清空再填充，避免旧数据残留）
    ui->listWidget_historyTitles->clear();
    // 2. 清空详情表格和靶面
    ui->tableWidget_historyDetail->setRowCount(0);
    clearHistoryHoles();
    m_historyHighlightedIndex = -1;

    // 3. 重新加载数据
    m_displayHistoryRecords = m_historyManager->loadAllHistoryRecords();

    if (m_displayHistoryRecords.isEmpty()) {
        // 仅添加“暂无历史记录”项，无其他数据
        ui->listWidget_historyTitles->addItem("暂无历史记录");
        switchTargetSurface(HID_TARGET::TARGET_NONE);
        return;
    }

    // 4. 填充 UI 列表（确保列表长度 = 数据长度）
    for (const auto& rec : m_displayHistoryRecords) {
        new QListWidgetItem(rec.title, ui->listWidget_historyTitles);
    }

    // 5. 选中第一条（索引 0 一定有效，因为数据非空）
    ui->listWidget_historyTitles->setCurrentRow(0);
    const HistoryRecord& firstRec = m_displayHistoryRecords[0];
    showHistoryDetail(0);

    if (!firstRec.bulletList.isEmpty()) {
        HID_TARGET firstTarget = firstRec.bulletList.first().targetType;
        drawHistoryHoles(firstRec.bulletList, firstTarget);
    } else {
        clearHistoryHoles();
        switchTargetSurface(HID_TARGET::TARGET_NONE);
    }
}

// 辅助函数：更新历史模式的靶型状态（targetType=当前历史记录的靶型）
void VideoTargetReporting::updateConnectionState(HID_TARGET targetType) {
    if (!m_isHistoryMode) return; // 仅在历史模式下生效

    // 1. 获取对应的状态文本（默认「未选择靶型」）
    QString stateText = m_historyTargetStateMap.value(targetType, "未选择靶型");

    // 2. 设置状态颜色（历史模式统一用「已连接」的颜色，如绿色；可根据需求调整）
    QColor stateColor = QColor(0, 255, 255); // 绿色：表示历史记录靶型已加载
    if (targetType == HID_TARGET::TARGET_NONE) {
        stateColor = QColor(169, 169, 169); // 灰色：未选择靶型
    }

    // 3. 应用到 label_connectionState
    ui->label_connectionState->setText(stateText);
    QPalette palette = ui->label_connectionState->palette();
    palette.setColor(QPalette::WindowText, stateColor);
    ui->label_connectionState->setPalette(palette);

    // 可选：打印日志，便于调试
    qDebug() << "[历史模式] 靶型状态更新：" << stateText << "（靶型：" << static_cast<int>(targetType) << "）";
}

void VideoTargetReporting::refreshHistoryUI() {
    // 重新加载最新数据
    m_displayHistoryRecords = m_historyManager->loadAllHistoryRecords();

    // 清空 UI 残留
    ui->listWidget_historyTitles->clear();
    ui->tableWidget_historyDetail->setRowCount(0);
    ui->label_historyDetailTitle->setText("");
    clearHistoryHoles();
    m_historyHighlightedIndex = -1;

    if (m_displayHistoryRecords.isEmpty()) {
        ui->listWidget_historyTitles->addItem("暂无历史记录");
        switchTargetSurface(HID_TARGET::TARGET_NONE);
        // 强制更新状态为「未选择靶型」（关键！）
        updateConnectionState(HID_TARGET::TARGET_NONE);
    } else {
        // 非空场景：填充列表并选中第一条
        for (const auto& rec : m_displayHistoryRecords) {
            new QListWidgetItem(rec.title, ui->listWidget_historyTitles);
        }
        ui->listWidget_historyTitles->setCurrentRow(0);
        const HistoryRecord& firstRec = m_displayHistoryRecords[0];
        showHistoryDetail(0);
        HID_TARGET firstTarget = firstRec.bulletList.isEmpty() ? HID_TARGET::TARGET_NONE : firstRec.bulletList.first().targetType;
        drawHistoryHoles(firstRec.bulletList, firstTarget);
        updateConnectionState(firstTarget);
    }
}

void VideoTargetReporting::on_pushButton_minimize_clicked()
{
    showMinimized();
}
