#ifndef HIDCOMMANDPARSER_H
#define HIDCOMMANDPARSER_H

#include "HIDCommandDef.h"

class HIDCommandParser {
public:
    // 解析原始字节流（输入：HID接收的原始数据，输出：解析后的指令结构体）
    static HID_Command parseRawData(const QByteArray& raw_data);

private:
    // 1. 校验帧格式（是否以55 AA开头，长度是否满足最小要求）
    static bool checkFrameFormat(const QByteArray& raw_data);

    // 2. 校验数据长度（DATALENGTH字段与实际数据长度是否匹配）
    static bool checkDataLength(const QByteArray& raw_data, uint8_t data_len);

    // 3. 计算校验和（从CMD到DATA所有字节异或，对比指令中的CS字段）
    static uint8_t calculateChecksum(const QByteArray& raw_data, uint8_t data_len);
};

#endif // HIDCOMMANDPARSER_H
