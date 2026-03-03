#ifndef HIDCOMMANDDEF_H
#define HIDCOMMANDDEF_H

#include <QByteArray>
#include <cstdint>

// 射击状态枚举（核心新增）
enum class ShootingState : uint8_t {
    STATE_TO_START = 0x00,  // 待开始（初始状态）
    STATE_READY = 0x01,     // 就绪（点击开始后，等待开发板就绪指令）
    STATE_SHOOTING = 0x02,  // 射击中（收到开发板就绪指令后，可处理弹孔成绩）
    STATE_PAUSED = 0x03     // 暂停（暂停射击，不处理弹孔成绩）
};


// 新增：通信状态枚举
enum class HID_STATE : uint8_t {
    STATE_DISCONNECTED,  // 未连接
    STATE_CONNECTING,    // 连接中（握手阶段）
    STATE_CONNECTED      // 已连接（心跳阶段）
};

// 新增：常量定义
namespace HID_CONST {
const int HANDSHAKE_INTERVAL = 1000;    // 握手发送间隔（1s）
const int HEARTBEAT_INTERVAL = 40000;    // 心跳发送间隔（5s）
const int HEARTBEAT_INIT_INTERVAL = 40000; // 初始心跳间隔40s
const int HEARTBEAT_SECOND_INTERVAL = 20000; // 第二次失败后间隔20s
const int HEARTBEAT_THIRD_INTERVAL = 10000;  // 第三次失败后间隔10s
const int HEARTBEAT_TIMEOUT = 500;      // 心跳超时时间（500ms）
const int MAX_HEARTBEAT_FAIL = 3;       // 最大心跳失败次数
}

// 指令CMD枚举（对应文档中“响应CMD”字段）
enum class HID_CMD : uint8_t {
    CMD_HANDSHAKE_REQ = 0x01,    // 开发板：握手请求
    CMD_HANDSHAKE_ACK = 0x02,    // 平板：握手确认
    CMD_HEARTBEAT_REQ = 0x03,    // 开发板：心跳请求
    CMD_HEARTBEAT_ACK = 0x04,    // 平板：心跳确认
    CMD_CHEST_SCORE = 0x10,      // 开发板：胸环靶成绩
    CMD_CHEST_ACK = 0x11,        // 平板：胸环靶确认
    CMD_IPSC_SCORE = 0x12,       // 开发板：IPSC靶成绩
    CMD_IPSC_ACK = 0x13,         // 平板：IPSC靶确认
    CMD_IDPA_SCORE = 0x14,       // 开发板：IDPA靶成绩
    CMD_IDPA_ACK = 0x15,         // 平板：IDPA靶确认
    CMD_COMM_END = 0x20,         // 开发板：通信结束
    CMD_COMM_END_ACK = 0x21,     // 平板：结束确认
    CMD_ERROR_FEEDBACK = 0x30,   // 平板：数据错误反馈
    CMD_RETRANSMIT_REQ = 0x31,   // 开发板：错误重发请求
    CMD_RECONNECT_REQ = 0x32,    // 开发板：重连请求
    CMD_RECONNECT_ACK = 0x33,    // 平板：重连确认
    CMD_TARGET_SELECT = 0x40,    // 开发板：靶型选择
    CMD_TARGET_ACK = 0x41,        // 平板：靶型确认
    CMD_START_REQ = 0x42,              // 开始指令
    CMD_START_ACK = 0x43,              // 开始确认
    CMD_PAUSE_REQ = 0x44,              // 暂停指令
    CMD_PAUSE_ACK = 0x45,              // 暂停确认
    CMD_END_REQ = 0x46,                // 结束指令
    CMD_END_ACK = 0x47,                 // 结束确认
    CMD_SHOOT_REQ = 0x48,               // 可以射击指令
    CMD_SHOOT_ACK = 0x49,                // 射击指令确认
    CMD_OCCLUSION_REQ = 0x50,    // 开发板检测到遮挡请求暂停指令
    CMD_OCCLUSION_ACK = 0x51     // 处理遮挡确认指令
};

// 错误码枚举（对应文档“ER”字段）
enum class HID_ERROR : uint8_t {
    ERROR_FORMAT = 0x00,     // 帧格式错
    ERROR_CHECKSUM = 0x01,   // 校验错
    ERROR_CMD = 0x02,        // 指令码错
    ERROR_DATA_LEN = 0x03,   // 数据长度错
    ERROR_SHOOTING_STATE = 0x04,  // 射击状态错误
    ERROR_COORD = 0x05,       // 坐标范围错
    ERROR_TARGET_TYPE = 0x06  // 靶型不一致
};

// 靶型枚举（对应文档“TT”字段）
enum class HID_TARGET : uint8_t {
    TARGET_NONE = 0x00,      // 默认
    TARGET_CHEST = 0x01,     // 胸环靶
    TARGET_IPSC = 0x02,      // IPSC靶
    TARGET_IDPA = 0x03       // IDPA靶
};

// 指令结构体（解析后的数据格式）
struct HID_Command {
    HID_CMD cmd;              // 指令类型
    uint8_t data_len;         // 数据长度（DATALENGTH）
    QByteArray data;          // 数据字段（DATA）
    uint8_t checksum;         // 校验和（CS）
    bool is_valid;            // 指令是否有效（格式+校验通过）
};

// 响应结构体（待发送的响应数据）
struct HID_Response {
    QByteArray raw_data;      // 响应的原始字节流（含报告ID）
    bool need_send;           // 是否需要发送给开发板
};

#endif // HIDCOMMANDDEF_H
