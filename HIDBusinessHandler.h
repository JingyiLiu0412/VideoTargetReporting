#ifndef HIDBUSINESSHANDLER_H
#define HIDBUSINESSHANDLER_H

#include "HIDCommandDef.h"
#include <QObject>
#include <QTableWidget>
#include <QTextEdit>
#include <QStackedWidget>
#include <QLabel>
#include <HoleOverlay.h>

// 前向声明UI类（避免循环引用，实际项目需包含UI头文件）
class Ui_VideoTargetReporting;

class HIDBusinessHandler : public QObject {
    Q_OBJECT
public:
    explicit HIDBusinessHandler(Ui_VideoTargetReporting* ui,
                                HoleOverlay* chestHole,
                                HoleOverlay* ipscHole,
                                HoleOverlay* idpaHole,
                                QObject* parent = nullptr);

    // 启动握手流程（程序启动时调用）
    void startHandshake();

    // 处理解析后的指令，返回响应
    HID_Response handleCommand(const HID_Command& cmd);
    HID_STATE getConnectionState() const { return m_state; } // 供外部获取状态
    HID_TARGET getCurrentTarget() const { return m_current_target; }
    ShootingState m_shootingState;
signals:
    // 发送响应数据的信号（连接到HID发送函数）
    void sendResponse(const QByteArray& response_data);
    void handshakeSuccessed();
    // 新增：发送弹孔数据信号（参数包含弹孔的所有必要信息）
    void bulletDataReceived(int x, int y, QString score, HID_TARGET targetType);
private slots:
    // 定时发送握手指令
    void sendHandshakeFrame();
    // 心跳相关（新增/补充）
    void sendHeartbeatFrame(); // 发送心跳帧
    void onHeartbeatTimeout(); // 心跳超时处理
    void handleHeartbeatAck(); // 处理心跳确认（CMD=0x04）

private:
    Ui_VideoTargetReporting* m_ui;          // UI指针，用于操作界面控件
    HID_TARGET m_current_target;        // 当前选中的靶型
    HoleOverlay* m_chestHoleOverlay;
    HoleOverlay* m_ipscHoleOverlay;
    HoleOverlay* m_idpaHoleOverlay;
    // 业务动作：处理握手请求
    HID_Response handleHandshakeReq();

    // 业务动作：处理心跳请求
    HID_Response handleHeartbeatReq();

    // 业务动作：处理胸环靶成绩
    HID_Response handleChestScore(const QByteArray& data);

    // 业务动作：处理IPSC靶成绩
    HID_Response handleIPSCScore(const QByteArray& data);

    // 业务动作：处理IDPA靶成绩
    HID_Response handleIDPAScore(const QByteArray& data);

    // 业务动作：处理通信结束
    HID_Response handleCommEnd();

    // 业务动作：处理错误重发请求
    HID_Response handleRetransmitReq(const QByteArray& data);

    // 业务动作：处理重连请求
    HID_Response handleReconnectReq();

    // 业务动作：处理靶型选择
    HID_Response handleTargetSelect(const QByteArray& data);

    // 处理开始确认指令
    HID_Response handleStartConfirm();

    // 处理暂停确认指令
    HID_Response handlePauseConfirm();

    // 处理结束确认指令
    HID_Response handleEndConfirm();

    // 处理开始射击指令
    HID_Response handleShootConfirm();

    // 处理遮挡指令
    HID_Response handleOcclusion();

    // 辅助函数：将16位坐标（高字节+低字节）转换为十进制
    uint16_t convertCoordToDecimal(const QByteArray& data, int start_idx);

    // 辅助函数：在textEdit中添加日志
    void addLog(const QString& log);

    // 辅助函数：在tableWidget中添加成绩记录
    // void addScoreToTable(const QString& score);

    HID_STATE m_state;                  // 连接状态
    QTimer* m_handshakeTimer;           // 握手定时器（循环发握手）
    // QList<QString> m_score_history;     // 成绩历史（可选，用于重发）
    QTimer* m_heartbeatTimer; // 心跳发送定时器（5秒一次）
    QTimer* m_heartbeatTimeoutTimer; // 心跳超时检测定时器（500ms）

    // 心跳失败计数（新增）
    int m_heartbeatFailCount;
    int m_currentHeartbeatInterval = HID_CONST::HEARTBEAT_INTERVAL;
    int m_heartbeatCurrentFail = 0;

    void onValidCommandReceived();
    // 更新连接状态UI（只处理“连接中/已连接/断开”）
    void updateConnectionUI(HID_STATE state);

    HID_Response handleHandshakeAck(const HID_Command& cmd);
};

#endif // HIDBUSINESSHANDLER_H
