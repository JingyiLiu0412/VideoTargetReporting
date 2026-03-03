#ifndef HIDRESPONSEGENERATOR_H
#define HIDRESPONSEGENERATOR_H

#include "HIDCommandDef.h"

class HIDResponseGenerator {
public:
    // 生成握手确认响应（ST：01=成功，00=失败）
    static HID_Response generateHandshakeAck(bool is_success);

    // 生成心跳确认响应
    static HID_Response generateHeartbeatAck();

    // 生成成绩确认响应（CMD：胸环靶/IPSC/IDPA，ST：01=成功，00=失败）
    static HID_Response generateScoreAck(HID_CMD cmd, bool is_success);

    // 生成通信结束确认响应
    static HID_Response generateCommEndAck();

    // 生成数据错误反馈响应（CD：错误指令码，ER：错误码）
    static HID_Response generateErrorFeedback(HID_CMD error_cmd, HID_ERROR error_code);

    // 生成重连确认响应
    static HID_Response generateReconnectAck();

    // 生成靶型确认响应（TT：靶型类型）
    static HID_Response generateTargetAck(HID_TARGET target);

    // 生成主动握手指令
    static HID_Response generateHandshakeReq();

    // 生成心跳询问指令
    static HID_Response generateHeartbeatReq();

    // 生成靶型选择执行
    static HID_Response generateTargetSelectReq(HID_TARGET target);

    // 生成开始指令
    static HID_Response generateStartReq();
    static HID_Response generateStartAck();

    // 生成暂停指令
    static HID_Response generatePauseReq();
    static HID_Response generatePauseAck();

    // 生成结束指令
    static HID_Response generateEndReq();
    static HID_Response generateEndAck();

    // 射击指令
    static HID_Response generateShootReq();
    static HID_Response generateShootAck();

    static HID_Response generateOcclusionReq();
    static HID_Response generateOcclusionAck();
private:
    // 通用响应生成（输入：CMD、DATA，输出：带报告ID的原始响应）
    static QByteArray generateGeneralResponse(HID_CMD cmd, const QByteArray& data);

    // 计算响应的校验和（复用解析器的校验和逻辑，可直接调用）
    static uint8_t calculateResponseChecksum(HID_CMD cmd, const QByteArray& data);
};

#endif // HIDRESPONSEGENERATOR_H
