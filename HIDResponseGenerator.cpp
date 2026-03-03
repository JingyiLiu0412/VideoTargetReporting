#include "HIDResponseGenerator.h"

// 通用响应生成：构建“报告ID + 55 AA + CMD + DATA_LEN + DATA + CS”结构
QByteArray HIDResponseGenerator::generateGeneralResponse(HID_CMD cmd, const QByteArray& data) {
    QByteArray response;
    const uint8_t REPORT_ID = 0x23;  // 固定报告ID
    const uint8_t START1 = 0x55;     // 起始符1
    const uint8_t START2 = 0xAA;     // 起始符2
    uint8_t data_len = static_cast<uint8_t>(data.size());  // 数据长度
    uint8_t cs = calculateResponseChecksum(cmd, data);     // 校验和

    // 拼接响应字节流
    response.append(static_cast<char>(REPORT_ID));  // 第1字节：报告ID
    response.append(static_cast<char>(START1));     // 第2字节：55
    response.append(static_cast<char>(START2));     // 第3字节：AA
    response.append(static_cast<char>(static_cast<uint8_t>(cmd)));  // 第4字节：CMD
    response.append(static_cast<char>(data_len));   // 第5字节：DATA_LEN
    response.append(data);                          // 第6~6+data_len-1字节：DATA
    response.append(static_cast<char>(cs));         // 最后1字节：CS

    return response;
}

// 计算响应校验和（CMD + DATA_LEN + DATA 异或）
uint8_t HIDResponseGenerator::calculateResponseChecksum(HID_CMD cmd, const QByteArray& data) {
    uint8_t cs = 0;
    cs ^= static_cast<uint8_t>(cmd);                  // CMD
    cs ^= static_cast<uint8_t>(data.size());          // DATA_LEN
    for (int i = 0; i < data.size(); i++) {
        cs ^= static_cast<uint8_t>(data.at(i));       // DATA字段
    }
    return cs;
}

// 握手确认响应（DATA：1字节ST）
HID_Response HIDResponseGenerator::generateHandshakeAck(bool is_success) {
    HID_Response resp;
    QByteArray data;
    data.append(static_cast<char>(is_success ? 0x01 : 0x00));  // ST：01成功，00失败
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_HANDSHAKE_ACK, data);
    resp.need_send = true;
    return resp;
}

// 心跳确认响应（无DATA）
HID_Response HIDResponseGenerator::generateHeartbeatAck() {
    HID_Response resp;
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_HEARTBEAT_ACK, QByteArray());
    resp.need_send = true;
    return resp;
}

// 成绩确认响应（DATA：1字节ST，CMD区分靶型）
HID_Response HIDResponseGenerator::generateScoreAck(HID_CMD cmd, bool is_success) {
    HID_Response resp;
    QByteArray data;
    data.append(static_cast<char>(is_success ? 0x01 : 0x00));  // ST：01成功，00失败

    // 根据原指令CMD选择响应CMD
    HID_CMD ack_cmd;
    if (cmd == HID_CMD::CMD_CHEST_SCORE) {
        ack_cmd = HID_CMD::CMD_CHEST_ACK;
    } else if (cmd == HID_CMD::CMD_IPSC_SCORE) {
        ack_cmd = HID_CMD::CMD_IPSC_ACK;
    } else if (cmd == HID_CMD::CMD_IDPA_SCORE) {
        ack_cmd = HID_CMD::CMD_IDPA_ACK;
    } else {
        resp.need_send = false;
        return resp;
    }

    resp.raw_data = generateGeneralResponse(ack_cmd, data);
    resp.need_send = true;
    return resp;
}

// 通信结束确认响应（无DATA）
HID_Response HIDResponseGenerator::generateCommEndAck() {
    HID_Response resp;
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_COMM_END_ACK, QByteArray());
    resp.need_send = true;
    return resp;
}

// 数据错误反馈响应（DATA：CD + ER，各1字节）
HID_Response HIDResponseGenerator::generateErrorFeedback(HID_CMD error_cmd, HID_ERROR error_code) {
    HID_Response resp;
    QByteArray data;
    data.append(static_cast<char>(static_cast<uint8_t>(error_cmd)));  // CD：错误指令码
    data.append(static_cast<char>(static_cast<uint8_t>(error_code)));  // ER：错误码
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_ERROR_FEEDBACK, data);
    resp.need_send = true;
    return resp;
}

// 重连确认响应（无DATA）
HID_Response HIDResponseGenerator::generateReconnectAck() {
    HID_Response resp;
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_RECONNECT_ACK, QByteArray());
    resp.need_send = true;
    return resp;
}

// 靶型确认响应（DATA：1字节TT）
HID_Response HIDResponseGenerator::generateTargetAck(HID_TARGET target) {
    HID_Response resp;
    QByteArray data;
    data.append(static_cast<char>(static_cast<uint8_t>(target)));  // TT：靶型
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_TARGET_ACK, data);
    resp.need_send = false;
    return resp;
}

// 生成平板→开发板的握手指令（55 AA 01 00 01）
HID_Response HIDResponseGenerator::generateHandshakeReq() {
    HID_Response resp;
    QByteArray data;
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_HANDSHAKE_REQ, data);
    resp.need_send = true;
    return resp;
}

// 生成平板->开发板的心跳指令(55 AA 03 00 03)
HID_Response HIDResponseGenerator::generateHeartbeatReq() {
    HID_Response resp;
    QByteArray data;
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_HEARTBEAT_REQ, data);
    resp.need_send = true;
    return resp;
}

// 生成平板->开发板的靶形选择指令（55 AA 40 01 TT CHKSUM） TT -01 胸环靶 -02 IPSC -03 IDPA
HID_Response HIDResponseGenerator::generateTargetSelectReq(HID_TARGET target) {
    HID_Response resp;
    QByteArray data;
    data.append(static_cast<char>(static_cast<uint8_t>(target)));
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_TARGET_SELECT, data);
    resp.need_send = true;
    return resp;
}

// 生成平板->开发板的开始指令55 AA 42 00 42
HID_Response HIDResponseGenerator::generateStartReq() {
    HID_Response resp;
    QByteArray data;
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_START_REQ, data);
    resp.need_send = true;
    return resp;
}

HID_Response HIDResponseGenerator::generateStartAck() {
    HID_Response resp;
    QByteArray data;
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_START_ACK, data);
    resp.need_send = false;
    return resp;
}

// 生成平板->开发板的暂停指令55 AA 44 00 44
HID_Response HIDResponseGenerator::generatePauseReq() {
    HID_Response resp;
    QByteArray data;
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_PAUSE_REQ, data);
    resp.need_send = true;
    return resp;
}

HID_Response HIDResponseGenerator::generatePauseAck() {
    HID_Response resp;
    QByteArray data;
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_PAUSE_ACK, data);
    resp.need_send = false;
    return resp;
}

// 生成平板->开发板的结束指令55 AA 46 00 46
HID_Response HIDResponseGenerator::generateEndReq() {
    HID_Response resp;
    QByteArray data;
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_END_REQ, data);
    resp.need_send = true;
    return resp;
}

HID_Response HIDResponseGenerator::generateEndAck() {
    HID_Response resp;
    QByteArray data;
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_END_ACK, data);
    resp.need_send = false;
    return resp;
}

HID_Response HIDResponseGenerator::generateShootReq(){
    HID_Response resp;
    QByteArray data;
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_SHOOT_REQ, data);
    resp.need_send = false;
    return resp;
}

HID_Response HIDResponseGenerator::generateShootAck(){
    HID_Response resp;
    QByteArray data;
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_SHOOT_ACK, data);
    resp.need_send = true;
    return resp;
}

HID_Response HIDResponseGenerator::generateOcclusionReq(){
    HID_Response resp;
    QByteArray data;
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_OCCLUSION_REQ, data);
    resp.need_send = false;
    return resp;
}

HID_Response HIDResponseGenerator::generateOcclusionAck(){
    HID_Response resp;
    QByteArray data;
    resp.raw_data = generateGeneralResponse(HID_CMD::CMD_OCCLUSION_ACK, data);
    resp.need_send = true;
    return resp;
}
