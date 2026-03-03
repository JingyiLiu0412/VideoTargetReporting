#include "HIDBusinessHandler.h"
#include "HIDResponseGenerator.h"
#include "ui_videotargetreporting.h"
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include <QToolTip>
#include <QSoundEffect>
HIDBusinessHandler::HIDBusinessHandler(Ui_VideoTargetReporting* ui,
                                       HoleOverlay* chestHole,
                                       HoleOverlay* ipscHole,
                                       HoleOverlay* idpaHole,
                                       QObject* parent)
    : QObject(parent),
    m_ui(ui),
    m_current_target(HID_TARGET::TARGET_CHEST),
    m_chestHoleOverlay(chestHole),
    m_ipscHoleOverlay(ipscHole),
    m_idpaHoleOverlay(idpaHole),
    m_state(HID_STATE::STATE_DISCONNECTED),
    m_heartbeatFailCount(0),
    m_shootingState(ShootingState::STATE_TO_START){
    // 初始化握手定时器（1秒发一次握手）
    m_handshakeTimer = new QTimer(this);
    m_handshakeTimer->setInterval(HID_CONST::HANDSHAKE_INTERVAL); // HID_CONST::HANDSHAKE_INTERVAL=1000ms

    // 绑定定时器信号（定时发握手）
    connect(m_handshakeTimer, &QTimer::timeout, this, &HIDBusinessHandler::sendHandshakeFrame);

    // 初始化心跳定时器（复用HID_CONST常量）
    m_heartbeatTimer = new QTimer(this);
    m_currentHeartbeatInterval = HID_CONST::HEARTBEAT_INIT_INTERVAL; // 初始40s
    m_heartbeatTimer->setInterval(m_currentHeartbeatInterval);
    // m_heartbeatTimer->setInterval(HID_CONST::HEARTBEAT_INTERVAL); // 40000ms
    m_heartbeatTimeoutTimer = new QTimer(this);
    m_heartbeatTimeoutTimer->setSingleShot(true);
    m_heartbeatTimeoutTimer->setInterval(HID_CONST::HEARTBEAT_TIMEOUT); // 500ms

    // 绑定信号槽（保持不变）
    connect(m_handshakeTimer, &QTimer::timeout, this, &HIDBusinessHandler::sendHandshakeFrame);
    connect(m_heartbeatTimer, &QTimer::timeout, this, &HIDBusinessHandler::sendHeartbeatFrame);
    connect(m_heartbeatTimeoutTimer, &QTimer::timeout, this, &HIDBusinessHandler::onHeartbeatTimeout);

    // 初始化UI：重连按钮默认禁用（程序启动时还没断开）
    // m_ui->pushButton_reconnection->setEnabled(false);
    // 初始化UI控件（如表格列名，若未在UI设计时设置）
    if (m_ui->tableWidget->columnCount() == 0) {
        m_ui->tableWidget->setColumnCount(3);
        m_ui->tableWidget->setHorizontalHeaderLabels({"序号", "成绩", "时间"});
    }
}

// 启动握手流程（主窗口初始化时调用）
void HIDBusinessHandler::startHandshake() {
    // 锁定状态：正在连接中，不重复启动握手
    if (m_state == HID_STATE::STATE_CONNECTING) {
        addLog("[握手流程] 已在连接中，无需重复启动握手");
        return;
    }
    m_state = HID_STATE::STATE_CONNECTING;
    m_heartbeatFailCount = 0;
    m_handshakeTimer->stop();
    m_heartbeatTimer->stop(); // 确保心跳未启动
    updateConnectionUI(m_state); // 显示“连接中...”
    m_handshakeTimer->start(HID_CONST::HANDSHAKE_INTERVAL);   // 开始1秒一次发握手
    addLog("[握手流程] 启动握手，等待开发板响应...");
}

void HIDBusinessHandler::onHeartbeatTimeout() {
    m_heartbeatCurrentFail++;
    addLog(QString("[心跳超时] 第%1次超时，当前失败计数：%2")
               .arg(m_heartbeatCurrentFail).arg(m_heartbeatCurrentFail));

    // 根据失败次数调整间隔
    if (m_heartbeatCurrentFail == 1) {
        // 第1次超时：切换为20s间隔
        m_currentHeartbeatInterval = HID_CONST::HEARTBEAT_SECOND_INTERVAL;
        m_heartbeatTimer->setInterval(m_currentHeartbeatInterval);
        addLog(QString("[间隔调整] 切换为%1ms发送心跳").arg(m_currentHeartbeatInterval));
    } else if (m_heartbeatCurrentFail == 2) {
        // 第2次超时：切换为10s间隔
        m_currentHeartbeatInterval = HID_CONST::HEARTBEAT_THIRD_INTERVAL;
        m_heartbeatTimer->setInterval(m_currentHeartbeatInterval);
        addLog(QString("[间隔调整] 切换为%1ms发送心跳").arg(m_currentHeartbeatInterval));
    } else if (m_heartbeatCurrentFail >= HID_CONST::MAX_HEARTBEAT_FAIL) {
        // 第3次超时：判定掉线
        m_state = HID_STATE::STATE_DISCONNECTED;
        updateConnectionUI(m_state);
        m_heartbeatTimer->stop();
        m_ui->pushButton_reconnection->setEnabled(true);
        addLog("[心跳失败] 累计3次超时，判定为掉线");
        QMessageBox::critical(nullptr, "连接断开", "连续3次心跳无响应，已断开连接！");
        m_ui->frame_connectionState->setStyleSheet("background-color:rgb(255,0,0);");
        // 核心新增：延迟1秒后启动握手（和程序初始逻辑一致）
        // 延迟是为了避免频繁重试占用资源，可根据需求调整（如500ms、2000ms）
        QTimer::singleShot(1000, this, &HIDBusinessHandler::startHandshake);
        m_ui->pushButton_start->setEnabled(true);
        m_ui->pushButton_pause->setEnabled(false);
        m_ui->pushButton_end->setEnabled(false);
        m_ui->label_currentState->setText("待开始");
        m_ui->label_currentState->setStyleSheet("color:rgb(255,0,0);");
        return; // 退出函数，避免重复启动
    }

    // 超时后不停止定时器，而是按新间隔继续发送（下次触发由定时器自动处理）
    addLog(QString("[继续发送] 按新间隔%1ms等待下一次心跳").arg(m_currentHeartbeatInterval));
}

void HIDBusinessHandler::sendHeartbeatFrame() {
    HID_Response resp = HIDResponseGenerator::generateHeartbeatReq();
    emit sendResponse(resp.raw_data);
    addLog(QString("[发送心跳] 按当前间隔%1ms发送").arg(m_currentHeartbeatInterval));
    m_heartbeatTimeoutTimer->start(HID_CONST::HEARTBEAT_TIMEOUT); // 500ms，使用常量
}

// 处理开发板的心跳确认（在 handleCommand 中调用）
void HIDBusinessHandler::handleHeartbeatAck() {
    m_heartbeatTimeoutTimer->stop(); // 停止超时检测
    // 重置失败计数和间隔为初始值（40s）
    m_heartbeatCurrentFail = 0;
    m_currentHeartbeatInterval = HID_CONST::HEARTBEAT_INIT_INTERVAL;
    m_heartbeatTimer->setInterval(m_currentHeartbeatInterval);
    addLog("[心跳正常] 收到确认，重置间隔为40s");
}

// 当收到对方的有效指令（非心跳响应）时调用
void HIDBusinessHandler::onValidCommandReceived() {
    // 重置心跳计时（从当前时间开始重新计算40s）
    m_heartbeatTimer->stop();
    m_heartbeatTimer->start(m_currentHeartbeatInterval); // 用当前间隔（通常是40s）重启计时
    addLog("[收到指令] 对方在线，重置心跳计时");

    // 同时重置失败计数和间隔（因为对方在线）
    if (m_heartbeatCurrentFail > 0) {
        m_heartbeatCurrentFail = 0;
        m_currentHeartbeatInterval = HID_CONST::HEARTBEAT_INIT_INTERVAL;
        m_heartbeatTimer->setInterval(m_currentHeartbeatInterval);
        addLog("[状态恢复] 因收到有效指令，心跳间隔恢复为40s");
    }
}

// 核心业务处理入口
HID_Response HIDBusinessHandler::handleCommand(const HID_Command& cmd) {
    if (!cmd.is_valid) {
        // 指令无效，生成错误反馈（格式错）
        addLog("[指令解析] 无效指令（格式/校验错）");
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_HANDSHAKE_REQ, HID_ERROR::ERROR_FORMAT);
    }
    // 关键拦截：未连接（未握手成功）时，只处理握手响应，其他指令直接拒绝
    if (m_state != HID_STATE::STATE_CONNECTED) {
        // 仅允许处理握手响应（CMD=0x02），其他指令返回“未连接”错误
        if (cmd.cmd != HID_CMD::CMD_HANDSHAKE_ACK) {
            addLog(QString("[通信拦截] 未握手成功，拒绝处理指令（CMD:0x%1）").arg(static_cast<uint8_t>(cmd.cmd)));
            // 可选：返回“未连接”错误反馈（需协议支持，若无则返回无需发送）
            HID_Response resp = {QByteArray(), false};
            return resp;
        }
    }

    // 新增：处理心跳确认指令（CMD=0x04）
    if (cmd.cmd == HID_CMD::CMD_HEARTBEAT_ACK) {
        handleHeartbeatAck();
        HID_Response resp = {QByteArray(), false};
        return resp;
    }

    // 优先处理握手响应（CMD=0x02）
    if (cmd.cmd == HID_CMD::CMD_HANDSHAKE_ACK) {
        return handleHandshakeAck(cmd);
    }


    // 根据指令类型分发处理
    switch (cmd.cmd) {
    case HID_CMD::CMD_HANDSHAKE_REQ:
        return handleHandshakeReq();
    case HID_CMD::CMD_HEARTBEAT_REQ:
        return handleHeartbeatReq();
    case HID_CMD::CMD_CHEST_SCORE:
        return handleChestScore(cmd.data);
    case HID_CMD::CMD_IPSC_SCORE:
        return handleIPSCScore(cmd.data);
    case HID_CMD::CMD_IDPA_SCORE:
        return handleIDPAScore(cmd.data);
    case HID_CMD::CMD_COMM_END:
        return handleCommEnd();
    case HID_CMD::CMD_RETRANSMIT_REQ:
        return handleRetransmitReq(cmd.data);
    case HID_CMD::CMD_RECONNECT_REQ:
        return handleReconnectReq();
    case HID_CMD::CMD_TARGET_ACK:
        return handleTargetSelect(cmd.data);
    case HID_CMD::CMD_START_ACK:
        return handleStartConfirm();
    case HID_CMD::CMD_PAUSE_ACK:
        return handlePauseConfirm();
    case HID_CMD::CMD_END_ACK:
        return handleEndConfirm();
    case HID_CMD::CMD_SHOOT_REQ:
        return handleShootConfirm();
    case HID_CMD::CMD_OCCLUSION_REQ:
        return handleOcclusion();
    default:
        // 未知指令，生成错误反馈（指令码错）
        addLog(QString("[指令解析] 未知指令（CMD:0x%1）").arg(static_cast<uint8_t>(cmd.cmd), 2, 16, QChar('0')));
        return HIDResponseGenerator::generateErrorFeedback(cmd.cmd, HID_ERROR::ERROR_CMD);
    }
}
// 发送握手指令（平板→开发板：55 AA 01 00 01）
void HIDBusinessHandler::sendHandshakeFrame() {
    // 调用响应生成器生成握手指令（确保HIDResponseGenerator有这个方法）
    HID_Response resp = HIDResponseGenerator::generateHandshakeReq();
    emit sendResponse(resp.raw_data); // 发送到开发板（通过信号连接到底层通信）
    addLog("[通信发送] 发送握手指令");
}

// 处理开发板的握手响应（CMD=0x02）
HID_Response HIDBusinessHandler::handleHandshakeAck(const HID_Command& cmd) {
    HID_Response resp = {QByteArray(), false}; // 握手响应无需再回复开发板

    // 解析握手响应的状态（DATA第1字节：01=成功，00=失败）
    if (cmd.data.size() >= 1) {
        uint8_t status = static_cast<uint8_t>(cmd.data.at(0));
        if (status == 0x01) { // 握手成功（开发板返回：55 AA 02 01 01 02）
            m_state = HID_STATE::STATE_CONNECTED;
            updateConnectionUI(m_state); // 更新UI为“已连接”
            m_handshakeTimer->stop();    // 停止发送握手
            emit handshakeSuccessed();
            // m_ui->stackedWidget_target->setCurrentIndex(0);
            // m_ui->label_connectionState->setText("待选择");
            // 握手响应的三种情况,其一是首次登录程序，显示默认页面，靶面待选择，背景是红色；其二是非首次登陆程序，靶面由记忆靶型获取，握手成功即连接成功；
            // 其三是断线重连，靶型不变，握手成功即为连接成功。
            if(m_ui->stackedWidget_target->currentIndex() != 0){
                m_ui->frame_connectionState->setStyleSheet("background-color: rgb(0, 255, 0);");
            }
            // m_ui->pushButton_reconnection->setEnabled(false); // 禁用重连按钮
            m_heartbeatCurrentFail = 0;
            m_currentHeartbeatInterval = HID_CONST::HEARTBEAT_INIT_INTERVAL; // 40s
            m_heartbeatTimer->setInterval(m_currentHeartbeatInterval);
            m_heartbeatTimer->start(); // 关键：启动心跳（5秒一次）
            // m_heartbeatFailCount = 0;  // 重置失败计数
            addLog("[握手流程] 握手成功，启动心跳机制，初始间隔40s");
        } else { // 握手失败（开发板返回：55 AA 02 01 00 03）
            m_state = HID_STATE::STATE_DISCONNECTED; // 回到断开状态
            updateConnectionUI(m_state);
            addLog("[握手流程] 握手失败，继续重试...");
            // 保持“连接中”状态，继续发握手
        }
    }
    return resp;
}

// 更新连接状态UI（简洁版：只区分3种状态）
void HIDBusinessHandler::updateConnectionUI(HID_STATE state) {
    switch (state) {
    case HID_STATE::STATE_DISCONNECTED:
        m_ui->label_connect->setText("连接失败");
        m_ui->label_connect->setStyleSheet("color: red; font-weight: bold;");
        m_ui->pushButton_reconnection->setEnabled(true);
        break;
    case HID_STATE::STATE_CONNECTING:
        m_ui->label_connect->setText("连接中...");
        m_ui->label_connect->setStyleSheet("color: orange; font-weight: bold;");
        // m_ui->pushButton_reconnection->setEnabled(false);
        break;
    case HID_STATE::STATE_CONNECTED:
        m_ui->label_connect->setText("已连接");
        m_ui->label_connect->setStyleSheet("color: green; font-weight: bold;");
        // m_ui->pushButton_reconnection->setEnabled(false);
        break;
    }
}

// 处理握手请求：返回成功确认
HID_Response HIDBusinessHandler::handleHandshakeReq() {
    addLog("[业务处理] 收到握手请求，返回成功确认");
    return HIDResponseGenerator::generateHandshakeAck(true);  // 默认连接成功
}

// 处理心跳请求：返回心跳确认
HID_Response HIDBusinessHandler::handleHeartbeatReq() {
    addLog("[业务处理] 收到心跳请求，返回确认");
    return HIDResponseGenerator::generateHeartbeatAck();
}

// 处理胸环靶成绩：解析角度+距离+成绩，处理映射关系，添加到表格+绘制弹孔，返回确认
HID_Response HIDBusinessHandler::handleChestScore(const QByteArray& data) {
    onValidCommandReceived();

    if (m_shootingState != ShootingState::STATE_SHOOTING) {
        addLog("[业务处理] 非射击状态，忽略胸环靶成绩指令");
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_CHEST_SCORE, HID_ERROR::ERROR_SHOOTING_STATE);
    }

    if(m_current_target != HID_TARGET::TARGET_CHEST){
        addLog("[业务处理] 当前靶形非胸环靶，无法处理胸环靶弹孔数据!");
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_CHEST_SCORE, HID_ERROR::ERROR_TARGET_TYPE);
    }

    // 校验数据长度（保持5字节：XH XL YH YL S（成绩编码））
    if (data.size() != 5) {
        addLog(QString("[业务处理] 胸环靶成绩数据长度错误（需5字节，实际收到%1字节）")
                   .arg(data.size()));
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_CHEST_SCORE, HID_ERROR::ERROR_DATA_LEN);
    }

    // 1. 解析角度（10倍值，两字节）和距离（10倍值，两字节）
    uint16_t angle10x = convertCoordToDecimal(data, 0); // 角度*10
    uint16_t distance10x = convertCoordToDecimal(data, 2); // 距离*10

    // 2. 还原角度和距离
    double realAngle = angle10x / 10.0; // 角度（度）
    double realDistance = distance10x / 10.0; // 距离（基于640x658靶面）

    // 3. 转换为640x658靶面的原始坐标（中心(320,395)）
    QPointF center(320, 395);
    double rad = qDegreesToRadians(realAngle); // 角度转弧度
    int rawX = static_cast<int>(center.x() + realDistance * cos(rad));
    int rawY = static_cast<int>(center.y() - realDistance * sin(rad));

    // 4. 边界校验（限制在640x658范围内）
    rawX = qBound(0, rawX, 639); // 0~639（640宽度）
    rawY = qBound(0, rawY, 657); // 0~657（658高度）

    // 成绩解析（去掉除以10.0，编码值直接等于真实成绩）
    uint8_t score_code = static_cast<uint8_t>(data.at(4));
    int actual_score = score_code;  // 例：0x05→5环，0x06→6环，0x0A→10环（0x0A=10）

    // 成绩范围校验（调整为0~10环，对应编码0x00~0x0A）
    if (actual_score < 0 || actual_score > 10) {
        addLog(QString("[业务处理] 胸环靶成绩超出有效范围（编码：0x%1 → 成绩：%2环）")
                   .arg(score_code, 2, 16, QChar('0')).arg(actual_score));  // 编码以十六进制显示，更直观
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_CHEST_SCORE, HID_ERROR::ERROR_DATA_LEN);
    }

    // 关键修改3：日志和表格显示（调整为「X环」格式，与实际成绩一致）
    QString score_str = QString("%1环").arg(actual_score);  // 显示为「N环」，符合用户认知
    addLog(QString("[业务处理] 收到胸环靶成绩：%1").arg(score_str));

    emit bulletDataReceived(rawX, rawY, QString("%1环").arg(actual_score), HID_TARGET::TARGET_CHEST);

    // 返回成功确认
    return HIDResponseGenerator::generateScoreAck(HID_CMD::CMD_CHEST_SCORE, true);
}

// 处理IPSC靶成绩：解析坐标和成绩，添加到表格
HID_Response HIDBusinessHandler::handleIPSCScore(const QByteArray& data) {
    onValidCommandReceived();

    if (m_shootingState != ShootingState::STATE_SHOOTING) {
        addLog("[业务处理] 非射击状态，忽略IPSC靶成绩指令");
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_IPSC_SCORE, HID_ERROR::ERROR_SHOOTING_STATE);
    }

    if(m_current_target != HID_TARGET::TARGET_IPSC){
        addLog("[业务处理] 当前靶形非IPSC靶，无法处理IPSC靶弹孔数据!");
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_IPSC_SCORE, HID_ERROR::ERROR_TARGET_TYPE);
    }

    // 校验数据长度（文档要求DATA占5字节：XH XL YH YL SC）
    if (data.size() != 5) {
        addLog("[业务处理] IPSC成绩数据长度错误");
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_IPSC_SCORE, HID_ERROR::ERROR_DATA_LEN);
    }

    // 解析坐标和成绩
    uint16_t x = convertCoordToDecimal(data, 0);
    uint16_t y = convertCoordToDecimal(data, 2);
    uint8_t sc = static_cast<uint8_t>(data.at(4));
    QString sc_str = (sc == 0x00) ? "A" : (sc == 0x01) ? "B" : (sc == 0x02) ? "C" : (sc == 0x03) ? "D" : "未知";

    // 校验坐标和成绩范围
    if (x > 460 || y > 580) {
        addLog(QString("[业务处理] IPSC坐标超出范围（X:%1, Y:%2）").arg(x).arg(y));
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_IPSC_SCORE, HID_ERROR::ERROR_COORD);
    }
    if (sc > 0x03 || sc == 0x01) {
        addLog(QString("[业务处理] IPSC成绩无效（SC:0x%1）").arg(sc, 2, 16, QChar('0')));
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_IPSC_SCORE, HID_ERROR::ERROR_DATA_LEN);
    }

    // 业务动作：添加到表格和日志
    QString score_str = QString("IPSC - X:%1, Y:%2, 成绩:%3").arg(x).arg(y).arg(sc_str);
    addLog(QString("[业务处理] 收到IPSC成绩：%1").arg(score_str));

    // addScoreToTable(score_str);
    emit bulletDataReceived(x, y, sc_str,HID_TARGET::TARGET_IPSC);

    // 返回成功确认
    return HIDResponseGenerator::generateScoreAck(HID_CMD::CMD_IPSC_SCORE, true);
}

// 处理IDPA靶成绩（逻辑与IPSC类似）
HID_Response HIDBusinessHandler::handleIDPAScore(const QByteArray& data) {
    onValidCommandReceived();

    if (m_shootingState != ShootingState::STATE_SHOOTING) {
        addLog("[业务处理] 非射击状态，忽略IDPA靶成绩指令");
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_IDPA_SCORE, HID_ERROR::ERROR_SHOOTING_STATE);
    }

    if(m_current_target != HID_TARGET::TARGET_IDPA){
        addLog("[业务处理] 当前靶形非IDPA靶，无法处理IDPA靶弹孔数据!");
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_IDPA_SCORE, HID_ERROR::ERROR_TARGET_TYPE);
    }

    if (data.size() != 5) {
        addLog("[业务处理] IDPA成绩数据长度错误");
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_IDPA_SCORE, HID_ERROR::ERROR_DATA_LEN);
    }

    uint16_t x = convertCoordToDecimal(data, 0);
    uint16_t y = convertCoordToDecimal(data, 2);
    uint8_t sc = static_cast<uint8_t>(data.at(4));
    QString sc_str = (sc == 0x00) ? "A" : (sc == 0x01) ? "B" : (sc == 0x02) ? "C" : (sc == 0x03) ? "D" : "未知";

    if (x > 460 || y > 780) {
        addLog(QString("[业务处理] IDPA坐标超出范围（X:%1, Y:%2）").arg(x).arg(y));
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_IDPA_SCORE, HID_ERROR::ERROR_COORD);
    }
    if (sc > 0x02) {
        addLog(QString("[业务处理] IDPA成绩无效（SC:0x%1）").arg(sc, 2, 16, QChar('0')));
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_IDPA_SCORE, HID_ERROR::ERROR_DATA_LEN);
    }
    QString score_str = QString("IDPA - X:%1, Y:%2, 成绩:%3").arg(x).arg(y).arg(sc_str);
    addLog(QString("[业务处理] 收到IDPA成绩：%1").arg(score_str));

    emit bulletDataReceived(x, y, sc_str,HID_TARGET::TARGET_IDPA);

    return HIDResponseGenerator::generateScoreAck(HID_CMD::CMD_IDPA_SCORE, true);
}

// 处理通信结束：更新状态，返回确认
HID_Response HIDBusinessHandler::handleCommEnd() {
    addLog("[业务处理] 收到通信结束请求，返回确认");
    m_ui->label_currentTarget->setText("通信结束");
    m_ui->label_currentTarget->setStyleSheet("color: gray;");
    return HIDResponseGenerator::generateCommEndAck();
}

// 处理错误重发请求：返回历史成绩（示例逻辑，可根据实际调整）
HID_Response HIDBusinessHandler::handleRetransmitReq(const QByteArray& data) {
    // 解析需要重发的指令码（DATA占1字节：CD）
    if (data.size() != 1) {
        addLog("[业务处理] 重发请求数据长度错误");
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_RETRANSMIT_REQ, HID_ERROR::ERROR_DATA_LEN);
    }

    HID_CMD retrans_cmd = static_cast<HID_CMD>(static_cast<uint8_t>(data.at(0)));
    addLog(QString("[业务处理] 收到重发请求（指令:0x%1）").arg(static_cast<uint8_t>(retrans_cmd), 2, 16, QChar('0')));

    // 重发请求无需单独响应（开发板等待重发数据），故返回不发送
    HID_Response resp = {QByteArray(), false};
    return resp;
}

// 处理重连请求：更新状态，返回确认
HID_Response HIDBusinessHandler::handleReconnectReq() {
    addLog("[业务处理] 收到重连请求，返回确认");
    m_ui->label_currentTarget->setText("重连中...");
    m_ui->label_currentTarget->setStyleSheet("color: orange;");
    return HIDResponseGenerator::generateReconnectAck();
}

// 处理靶型选择：切换UI靶型页面，返回确认
HID_Response HIDBusinessHandler::handleTargetSelect(const QByteArray& data) {
    onValidCommandReceived();
    // 解析靶型（DATA占1字节：TT）
    if (data.size() != 1) {
        addLog("[业务处理] 靶型选择数据长度错误");
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_TARGET_SELECT, HID_ERROR::ERROR_DATA_LEN);
    }

    HID_TARGET target = static_cast<HID_TARGET>(static_cast<uint8_t>(data.at(0)));
    QString target_str;
    int stacked_index;
    QUrl url;

    // 映射靶型到UI页面索引
    switch (target) {
    case HID_TARGET::TARGET_CHEST:
        target_str = "胸环靶";
        stacked_index = 1;  // 对应UI中stackedWidget_target的page_2（胸环靶页面）
        url = "qrc:/new/prefix1/sound/CT_XHB.wav";
        break;
    case HID_TARGET::TARGET_IPSC:
        target_str = "IPSC靶";
        stacked_index = 2;  // 对应page_3（IPSC页面）
        url = "qrc:/new/prefix1/sound/CT_IPSC.wav";
        break;
    case HID_TARGET::TARGET_IDPA:
        target_str = "IDPA靶";
        stacked_index = 3;  // 对应page_4（IDPA页面）
        url = "qrc:/new/prefix1/sound/CT_IDPA.wav";
        break;
    default:
        addLog(QString("[业务处理] 无效靶型（TT:0x%1）").arg(static_cast<uint8_t>(target), 2, 16, QChar('0')));
        return HIDResponseGenerator::generateErrorFeedback(HID_CMD::CMD_TARGET_SELECT, HID_ERROR::ERROR_DATA_LEN);
    }

    // 业务动作：切换UI页面，更新状态
    m_current_target = target;
    m_ui->stackedWidget_target->setCurrentIndex(stacked_index);
    m_ui->label_currentTarget->setText(QString("%1").arg(target_str));
    m_ui->frame_connectionState->setStyleSheet("background-color: rgb(0, 255, 0);");
    QSoundEffect *sound = new QSoundEffect(this);
    sound->setSource(url);  // 资源路径
    sound->play();
    addLog(QString("[业务处理] 切换靶型为：%1").arg(target_str));

    // 返回确认
    return HIDResponseGenerator::generateTargetAck(target);
}

// 辅助函数：高字节+低字节 → 十进制（小端/大端？文档中XH=高字节，XL=低字节，故为大端）
uint16_t HIDBusinessHandler::convertCoordToDecimal(const QByteArray& data, int start_idx) {
    uint8_t high_byte = static_cast<uint8_t>(data.at(start_idx));
    uint8_t low_byte = static_cast<uint8_t>(data.at(start_idx + 1));
    return (static_cast<uint16_t>(high_byte) << 8) | low_byte;  // 高字节左移8位 + 低字节
}

// 辅助函数：添加日志到textEdit
void HIDBusinessHandler::addLog(const QString& log) {
    QString time_str = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    // m_ui->textEdit_HID->append(QString("[%1] %2").arg(time_str).arg(log));
}

HID_Response HIDBusinessHandler::handleStartConfirm(){
    onValidCommandReceived();
    m_shootingState = ShootingState::STATE_READY;
    m_ui->pushButton_start->setEnabled(false);
    m_ui->pushButton_pause->setEnabled(true);
    m_ui->pushButton_end->setEnabled(true);
    QSoundEffect *sound = new QSoundEffect(this);
    sound->setSource(QUrl("qrc:/new/prefix1/sound/wait_start.wav"));  // 资源路径
    sound->play();
    m_ui->label_currentState->setText("等待开始指令");
    m_ui->label_currentState->setStyleSheet("color: rgb(0, 255, 0);");
    return HIDResponseGenerator::generateStartAck();
}

HID_Response HIDBusinessHandler::handlePauseConfirm(){
    onValidCommandReceived();
    m_shootingState = ShootingState::STATE_PAUSED;
    m_ui->pushButton_pause->setEnabled(false);
    m_ui->pushButton_start->setEnabled(true);
    QSoundEffect *sound = new QSoundEffect(this);
    sound->setSource(QUrl("qrc:/new/prefix1/sound/pause.wav"));  // 资源路径
    sound->play();
    m_ui->label_currentState->setText("暂停中");
    m_ui->label_currentState->setStyleSheet("color: rgb(255, 255, 0);");
    // 暂停后若贴靶贴，再打同一位置仍然显示
    m_chestHoleOverlay->initLastHole();
    m_ipscHoleOverlay->initLastHole();
    m_idpaHoleOverlay->initLastHole();
    return HIDResponseGenerator::generatePauseAck();
}

HID_Response HIDBusinessHandler::handleEndConfirm(){
    onValidCommandReceived();
    m_shootingState = ShootingState::STATE_TO_START;
    m_ui->pushButton_pause->setEnabled(false);
    m_ui->pushButton_start->setEnabled(true);
    m_ui->pushButton_end->setEnabled(false);
    QSoundEffect *sound = new QSoundEffect(this);
    sound->setSource(QUrl("qrc:/new/prefix1/sound/end.wav"));  // 资源路径
    sound->play();
    m_ui->label_currentState->setText("待开始");
    m_ui->label_currentState->setStyleSheet("color: rgb(255, 0, 0);");
    return HIDResponseGenerator::generateEndAck();
}

HID_Response HIDBusinessHandler::handleShootConfirm(){
    onValidCommandReceived();
    m_shootingState = ShootingState::STATE_SHOOTING;
    // 显示可以开始射击
    // QToolTip::showText(QPoint(200, 200), "可以开始射击！", m_ui->stackedWidget_right, QRect(), 3000);
    QSoundEffect *sound = new QSoundEffect(this);
    sound->setSource(QUrl("qrc:/new/prefix1/sound/start.wav"));  // 资源路径
    sound->play();
    m_ui->label_currentState->setText("射击进行中");
    m_ui->label_currentState->setStyleSheet("color:rgb(0, 255, 0);");
    return HIDResponseGenerator::generateShootAck();
}

HID_Response HIDBusinessHandler::handleOcclusion(){
    onValidCommandReceived();
    if(m_ui->label_currentState->text() != "待开始"){
        m_ui->pushButton_start->setEnabled(true);
        m_ui->pushButton_pause->setEnabled(false);
        m_ui->label_currentState->setText("检测到遮挡，已暂停");
        m_ui->label_currentState->setStyleSheet("color:rgb(255, 0, 0);");
        QSoundEffect *sound = new QSoundEffect(this);
        sound->setSource(QUrl("qrc:/new/prefix1/sound/occlusion.wav"));  // 资源路径
        sound->play();

    }
    return HIDResponseGenerator::generateOcclusionAck();
}
