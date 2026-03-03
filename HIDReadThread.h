#ifndef HIDREADTHREAD_H
#define HIDREADTHREAD_H
#include <QThread>
#include <QMutex>
#include "hidapi.h"

class HIDReadThread : public QThread
{
    Q_OBJECT
public:
    explicit HIDReadThread(hid_device* dev, QObject *parent = nullptr)
        : QThread(parent), hid_dev(dev), is_running(true) {}

    void stop() { is_running = false; } // 停止线程的接口

signals:
    // 接收到数据后，发送信号给UI线程更新文本框
    void dataReceived(const QString& data);

protected:
    void run() override
    {
        unsigned char buf[64] = {0}; // HID报告长度通常为64字节（根据设备调整）
        int bytes_read = 0;

        while (is_running && hid_dev != nullptr)
        {
            // 读取HID设备数据（超时100ms，避免无限阻塞）
            bytes_read = hid_read_timeout(hid_dev, buf, sizeof(buf), 100);

            if (bytes_read > 0)
            {
                // 将字节数据转换为十六进制字符串，方便查看
                QString recv_data = "收到数据：";
                for (int i = 0; i < bytes_read; i++)
                {
                    recv_data += QString("%1 ").arg(buf[i], 2, 16, QChar('0')).toUpper();
                }
                // 发送信号给UI线程（线程安全）
                emit dataReceived(recv_data);
            }
            else if (bytes_read < 0)
            {
                emit dataReceived("HID数据读取错误！");
                break; // 读取错误时退出循环
            }
        }
    }

private:
    hid_device* hid_dev; // HID设备句柄（由主线程传入）
    bool is_running;     // 线程运行标志
    QMutex mutex;        // 互斥锁（避免线程安全问题）
};
#endif // HIDREADTHREAD_H
