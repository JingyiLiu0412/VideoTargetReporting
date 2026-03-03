// #ifndef VIDEOTARGETREPORTING_H
// #define VIDEOTARGETREPORTING_H

// #include <QMainWindow>

// QT_BEGIN_NAMESPACE
// namespace Ui {
// class VideoTargetReporting;
// }
// QT_END_NAMESPACE

// class VideoTargetReporting : public QMainWindow
// {
//     Q_OBJECT

// public:
//     VideoTargetReporting(QWidget *parent = nullptr);
//     ~VideoTargetReporting();

// protected:
//     void keyPressEvent(QKeyEvent *event) override;

// private slots:
//     void on_pushButton_10_clicked();

//     void on_btnExit_clicked();

//     void on_btnMinisize_clicked();

// private:
//     Ui::VideoTargetReporting *ui;
// };
// #endif // VIDEOTARGETREPORTING_H
#ifndef VIDEOTARGETREPORTING_H
#define VIDEOTARGETREPORTING_H

#include <QMainWindow>
#include "HIDReadThread.h"
#include "hidapi.h"
#include "HIDBusinessHandler.h"
#include "HIDReadThread.h"
#include "HoleOverlay.h"  // 包含弹孔绘制类
#include <QTimer>
#include <QDateTime>
#include <QList>
#include <QListWidgetItem>
#include <QByteArray>
#include <QMessageBox>
#include <QSettings>
#include <QResizeEvent>
#include <HIDCommandDef.h>
#include <BulletHistoryManager.h>

QT_BEGIN_NAMESPACE
namespace Ui { class VideoTargetReporting; }
QT_END_NAMESPACE

class VideoTargetReporting : public QMainWindow {
    Q_OBJECT

public:
    VideoTargetReporting(QWidget *parent = nullptr);
    ~VideoTargetReporting();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    // 重写事件过滤器，监听 Label 点击
    bool eventFilter(QObject *watched, QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void on_pushButton_exit_clicked();

    void on_pushButton_HID_clicked();

    // void on_pushButton_HIDsend_clicked();

    void on_pushButton_reset_clicked();

    void on_pushButton_settings_clicked();

    void on_pushButton_back_clicked();

    void setTargetSelected(QLabel* currentLabel, QLabel* other1, QLabel* other2);

    void on_pushButton_reconnection_clicked();

    void on_pushButton_connect_clicked();

    void on_pushButton_start_clicked();

    void on_pushButton_pause_clicked();

    void on_pushButton_end_clicked();

    // 手动保存按钮点击事件
    void on_pushButton_saveRecord_clicked();
    // 历史记录按钮点击事件
    void on_pushButton_history_clicked();
    // 历史记录列表点击事件（联动详情）
    void on_listWidget_historyTitles_itemClicked(QListWidgetItem *item);

    void onBulletDataReceived(int x, int y, QString score, HID_TARGET targetType);

    void on_pushButton_deleteRecord_clicked(); // 删除按钮点击事件
    void deleteHistoryRecord(int index); // 实际执行删除的辅助函数


    void on_tableWidget_itemClicked(QTableWidgetItem *item);

    void on_tableWidget_historyDetail_itemClicked(QTableWidgetItem *item);

    // 新增：接收历史记录变化信号，更新UI
    void onHistoryRecordsChanged();

    void on_pushButton_minimize_clicked();

private:
    Ui::VideoTargetReporting *ui;
    HID_TARGET m_current_target;        // 当前选中的靶型
    class HoleOverlay *holeOverlay;
    QList<QPoint> holes; // 存储弹孔位置
    void startHIDReadThread(); // 启动线程的函数
    hid_device* hid_dev = nullptr; // HID设备句柄，初始为断开状态
    HIDReadThread* m_hid_thread = nullptr;    // 原有HID接收线程
    HIDBusinessHandler* m_business_handler;  // 新增业务处理器
    const unsigned short VID = 0x4476; // 你的设备VID
    const unsigned short PID = 0x5101; // 你的设备PID
    bool isHexStringValid(const QString& str);

    // 弹孔绘制层（三个靶型分别对应）
    HoleOverlay* m_chestHoleOverlay;  // 胸环靶弹孔层
    HoleOverlay* m_ipscHoleOverlay;   // IPSC靶弹孔层
    HoleOverlay* m_idpaHoleOverlay;   // IDPA靶弹孔层
    // 新增：弹孔状态记录
    int m_latestHoleIndex = -1;    // 最新弹孔的索引（-1表示无）
    int m_selectedHoleIndex = -1;  // 选中弹孔的索引（-1表示无）

    // 辅助函数：获取当前激活的靶面绘制层
    HoleOverlay* getCurrentOverlay() {
        switch (m_current_target) {
        case HID_TARGET::TARGET_CHEST: return m_chestHoleOverlay;
        case HID_TARGET::TARGET_IPSC: return m_ipscHoleOverlay;
        case HID_TARGET::TARGET_IDPA: return m_idpaHoleOverlay;
        default: return nullptr;
        }
    }

    // 历史记录相关成员变量
    QList<HistoryRecord> m_historyRecords; // 所有历史记录
    QList<BulletRecord> m_currentBulletList; // 当前训练的弹孔数据
    // 辅助函数：添加弹孔到表格并同步到当前列表（需调用）
    void addBulletToTable(int x, int y, QString score, HID_TARGET targetType);
    // 历史记录核心函数
    void saveHistoryRecord(const HistoryRecord& history); // 保存单条记录到本地
    QList<HistoryRecord> loadAllHistoryRecords(); // 读取所有历史记录
    void showHistoryDetail(int index); // 显示指定索引的历史记录详情

    // 封装HID连接逻辑（返回是否连接成功）
    bool connectHIDDevice();
    // 封装HID断开逻辑
    void disconnectHIDDevice();
    QString getTargetName(HID_TARGET target);
    HID_TARGET m_lastSelectedTarget;

    // 新增：历史记录模式状态（与当前射击状态隔离）
    bool m_isHistoryMode = false; // 是否处于历史记录模式（区分当前射击/历史查看）
    int m_historyHighlightedIndex = -1; // 历史模式下选中的弹孔索引（-1表示无）
    HID_TARGET m_historyCurrentTarget = HID_TARGET::TARGET_NONE; // 历史模式下当前选中的靶型

    // 辅助函数：新增——切换靶面显示（当前射击/历史记录通用）
    void switchTargetSurface(HID_TARGET targetType);
    // 辅助函数：新增——在历史模式下绘制弹孔
    void drawHistoryHoles(const QList<BulletRecord>& bulletList, HID_TARGET targetType);
    // 辅助函数：新增——清空历史模式下的弹孔和高亮
    void clearHistoryHoles();

    void redrawCurrentShootingHoles();

    QList<HistoryRecord> m_displayHistoryRecords; // 用于UI展示的历史记录（内存缓存）
    // 历史记录管理器实例（核心修改：通过单例获取）
    BulletHistoryManager* m_historyManager;

    // 新增：存储主界面原始状态（返回时还原）
    QString m_mainConnStateText;  // 主界面原始的 label_connectionState 文本
    QColor m_mainConnStateColor;  // 主界面原始的 label_connectionState 颜色
    bool m_fromHistoryToSettings = false;    // 是否是从历史页面跳转到设置页面
    // 新增：历史模式靶型状态文本映射（key=靶型，value=状态文本）
    QMap<HID_TARGET, QString> m_historyTargetStateMap;

    void updateConnectionState(HID_TARGET targetType);
    void refreshHistoryUI();
    void updateTargetSurfaceLayout();
    void updateSettingsSelectionOverlayLayout();
    void refreshOverlayAfterLayoutChanged();
};

#endif
