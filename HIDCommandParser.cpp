#include "HIDCommandParser.h"
#include <QDebug>

// 核心解析函数
HID_Command HIDCommandParser::parseRawData(const QByteArray& raw_data) {
    HID_Command cmd = {HID_CMD::CMD_HANDSHAKE_REQ, 0, QByteArray(), 0, false};

    // 校验原始数据至少7字节（报告ID(1) + 指令(6)）
    if (raw_data.size() < 7) {
        return cmd;
    }

    // 跳过报告ID（索引0），解析指令起始符（索引1=55，索引2=AA）
    uint8_t start1 = static_cast<uint8_t>(raw_data.at(1));
    uint8_t start2 = static_cast<uint8_t>(raw_data.at(2));
    if (start1 != 0x55 || start2 != 0xAA) {
        return cmd;
    }

    // 提取CMD（索引3）、DATA_LEN（索引4）
    uint8_t cmd_code = static_cast<uint8_t>(raw_data.at(3));
    cmd.data_len = static_cast<uint8_t>(raw_data.at(4));

    // 校验DATA_LEN对应的索引是否存在（避免越界）
    if (raw_data.size() < (5 + cmd.data_len + 1)) { // 5=报告ID+起始符+CMD+DATA_LEN，+1=CS
        return cmd;
    }

    // 提取DATA（索引5开始，长度=DATA_LEN）、CS（索引5+DATA_LEN）
    cmd.data = raw_data.mid(5, cmd.data_len);
    cmd.checksum = static_cast<uint8_t>(raw_data.at(5 + cmd.data_len));

    // 校验校验和（CMD ^ DATA_LEN ^ DATA所有字节）
    uint8_t calc_cs = cmd_code ^ cmd.data_len;
    for (int i = 0; i < cmd.data.size(); i++) {
        calc_cs ^= static_cast<uint8_t>(cmd.data.at(i));
    }
    if (calc_cs != cmd.checksum) {
        return cmd;
    }

    // 指令有效
    cmd.cmd = static_cast<HID_CMD>(cmd_code);
    cmd.is_valid = true;
    return cmd;
}

// 校验帧格式：是否以55 AA开头，且长度≥5字节
bool HIDCommandParser::checkFrameFormat(const QByteArray& raw_data) {
    if (raw_data.size() < 5) return false;  // 最小长度：报告ID(1) + 指令头(4) + CS(1)？不，原始数据包含报告ID，指令本身最小5字节（55 AA CMD DATA_LEN CS），所以raw_data最小6字节？
    // 修正：原始数据结构 = 报告ID(1字节) + 指令内容(N字节)，指令内容最小5字节（55 AA CMD DATA_LEN CS），所以raw_data最小6字节
    if (raw_data.size() < 6) return false;
    if (static_cast<uint8_t>(raw_data.at(1)) != 0x55 || static_cast<uint8_t>(raw_data.at(2)) != 0xAA) {
        return false;  // 起始符错误（索引1、2为55 AA）
    }
    return true;
}

// 校验数据长度：实际数据长度是否等于DATALENGTH
bool HIDCommandParser::checkDataLength(const QByteArray& raw_data, uint8_t data_len) {
    // 指令内容总长度 = 5（头+CS） + data_len（数据）？不，指令内容结构：55 AA CMD DATA_LEN DATA CS → 总长度=2+1+1+data_len+1=5+data_len
    // 原始数据长度 = 1（报告ID） + 指令内容长度 → 1 + (5+data_len) = 6+data_len
    // 原始数据结构 = 报告ID(1) + 有效指令(5 + data_len) + 冗余00
    // 最小长度要求：1 + 5 + data_len = 6 + data_len（有效部分足够）
    if (raw_data.size() < (6 + data_len)) {
        return false;
    }
    return true;
}

// 计算校验和：从CMD到DATA所有字节异或
uint8_t HIDCommandParser::calculateChecksum(const QByteArray& raw_data, uint8_t data_len) {
    uint8_t cs = 0;
    // 校验和计算范围：CMD（索引2）、DATA_LEN（索引3）、DATA（索引4~4+data_len-1）
    cs ^= static_cast<uint8_t>(raw_data.at(2));  // CMD
    cs ^= static_cast<uint8_t>(raw_data.at(3));  // DATA_LEN
    for (int i = 0; i < data_len; i++) {
        cs ^= static_cast<uint8_t>(raw_data.at(4 + i));  // DATA字段
    }
    return cs;
}
