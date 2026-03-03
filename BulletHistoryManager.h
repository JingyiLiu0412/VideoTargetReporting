#ifndef BULLETHISTORYMANAGER_H
#define BULLETHISTORYMANAGER_H

#include <QObject>
#include <QList>
#include <QDateTime>
#include <QString>
#include "HIDCommandDef.h" // 包含 HID_TARGET 枚举

// 复用你原有的结构体（与 target_reporting 保持一致）
struct BulletRecord {
    int index;          // 序号
    int x;              // X坐标
    int y;              // Y坐标
    QString score;      // 成绩（A/B/C/D 或环数）
    HID_TARGET targetType; // 靶型
    QString time;       // 射击时间（HH:mm:ss）
};

struct HistoryRecord {
    QString title;      // 记录标题（如“2024-05-20 15:30 胸环靶训练”）
    QDateTime createTime; // 创建时间（用于排序和唯一标识）
    QList<BulletRecord> bulletList; // 该记录下的所有弹孔数据
};

class BulletHistoryManager : public QObject {
    Q_OBJECT
public:
    // 单例模式（全局唯一，避免多实例数据不一致）
    static BulletHistoryManager* getInstance(QObject* parent = nullptr);

    // 1. 保存单条历史记录（内存+本地文件）
    void saveHistoryRecord(const HistoryRecord& record);

    // 2. 加载所有历史记录（从本地文件到内存）
    QList<HistoryRecord> loadAllHistoryRecords();

    // 3. 删除指定索引的历史记录
    bool deleteHistoryRecord(int index);

    // 4. 清空所有历史记录
    void clearAllHistoryRecords();

    // 5. 按索引获取单条历史记录
    HistoryRecord getHistoryRecord(int index) const;

    // 6. 获取历史记录总数
    int getHistoryRecordCount() const;

signals:
    // 历史记录变化信号（通知UI更新列表）
    void historyRecordsChanged();

private:
    explicit BulletHistoryManager(QObject* parent = nullptr);
    ~BulletHistoryManager() override = default;

    // 禁止拷贝构造和赋值（确保单例唯一性）
    BulletHistoryManager(const BulletHistoryManager&) = delete;
    BulletHistoryManager& operator=(const BulletHistoryManager&) = delete;

    // 内部存储：内存缓存的历史记录
    QList<HistoryRecord> m_historyRecords;

    // 单例实例
    static BulletHistoryManager* s_instance;

    // 辅助函数：从本地文件加载（复用你原有的 QSettings 逻辑）
    void loadFromFile();

    // 辅助函数：保存到本地文件（复用你原有的 QSettings 逻辑）
    void saveToFile();
};

#endif // BULLETHISTORYMANAGER_H
