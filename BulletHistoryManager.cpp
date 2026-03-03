#include "BulletHistoryManager.h"
#include <QSettings>
#include <QDateTime>
#include <algorithm>

// 单例实例初始化
BulletHistoryManager* BulletHistoryManager::s_instance = nullptr;

BulletHistoryManager::BulletHistoryManager(QObject* parent) : QObject(parent) {
    // 构造时自动加载本地历史记录到内存
    loadFromFile();
}

BulletHistoryManager* BulletHistoryManager::getInstance(QObject* parent) {
    if (!s_instance) {
        s_instance = new BulletHistoryManager(parent);
    }
    return s_instance;
}

void BulletHistoryManager::saveHistoryRecord(const HistoryRecord& record) {
    // 添加到内存缓存
    m_historyRecords.append(record);
    // 按创建时间倒序排序（最新的在前面）
    std::sort(m_historyRecords.begin(), m_historyRecords.end(), [](const HistoryRecord& a, const HistoryRecord& b) {
        return a.createTime > b.createTime;
    });
    // 保存到本地文件
    saveToFile();
    // 发送信号通知UI更新
    emit historyRecordsChanged();
}

QList<HistoryRecord> BulletHistoryManager::loadAllHistoryRecords() {
    loadFromFile();
    std::sort(m_historyRecords.begin(), m_historyRecords.end(), [](const HistoryRecord& a, const HistoryRecord& b) {
        return a.createTime > b.createTime;
    });
    // 添加日志：打印加载到的记录数
    // qDebug() << "[历史记录管理器] 加载到" << m_historyRecords.size() << "条记录";
    return m_historyRecords;
}

bool BulletHistoryManager::deleteHistoryRecord(int index) {
    if (index < 0 || index >= m_historyRecords.size()) {
        return false;
    }
    // 从内存中删除
    m_historyRecords.removeAt(index);
    // 同步到本地文件
    saveToFile();
    // 发送信号通知UI更新
    emit historyRecordsChanged();
    return true;
}

void BulletHistoryManager::clearAllHistoryRecords() {
    m_historyRecords.clear();
    saveToFile();
    emit historyRecordsChanged();
}

HistoryRecord BulletHistoryManager::getHistoryRecord(int index) const {
    if (index >= 0 && index < m_historyRecords.size()) {
        return m_historyRecords[index];
    }
    return HistoryRecord(); // 索引无效时返回空记录
}

int BulletHistoryManager::getHistoryRecordCount() const {
    return m_historyRecords.size();
}

// 从本地文件加载（复用你原有的逻辑）
void BulletHistoryManager::loadFromFile() {
    QSettings settings("SpecialPie", "TargetReportingSystem");
    m_historyRecords.clear();

    int historyCount = settings.beginReadArray("HistoryRecords");
    for (int i = 0; i < historyCount; ++i) {
        settings.setArrayIndex(i);
        HistoryRecord rec;
        rec.title = settings.value("Title").toString();
        rec.createTime = settings.value("CreateTime").toDateTime();

        int bulletCount = settings.beginReadArray("BulletList");
        for (int j = 0; j < bulletCount; ++j) {
            settings.setArrayIndex(j);
            BulletRecord bullet;
            bullet.index = settings.value("Index").toInt();
            bullet.x = settings.value("X").toInt();
            bullet.y = settings.value("Y").toInt();
            bullet.score = settings.value("Score").toString();
            bullet.targetType = static_cast<HID_TARGET>(settings.value("TargetType").toUInt());
            bullet.time = settings.value("Time").toString();
            rec.bulletList.append(bullet);
        }
        settings.endArray();

        m_historyRecords.append(rec);
    }
    settings.endArray();
}

// 保存到本地文件（复用你原有的逻辑）
void BulletHistoryManager::saveToFile() {
    QSettings settings("SpecialPie", "TargetReportingSystem");
    settings.beginWriteArray("HistoryRecords");

    for (int i = 0; i < m_historyRecords.size(); ++i) {
        settings.setArrayIndex(i);
        const HistoryRecord& rec = m_historyRecords[i];
        settings.setValue("Title", rec.title);
        settings.setValue("CreateTime", rec.createTime);

        settings.beginWriteArray("BulletList");
        for (int j = 0; j < rec.bulletList.size(); ++j) {
            settings.setArrayIndex(j);
            const BulletRecord& bullet = rec.bulletList[j];
            settings.setValue("Index", bullet.index);
            settings.setValue("X", bullet.x);
            settings.setValue("Y", bullet.y);
            settings.setValue("Score", bullet.score);
            settings.setValue("TargetType", static_cast<uint8_t>(bullet.targetType));
            settings.setValue("Time", bullet.time);
        }
        settings.endArray();
    }
    settings.endArray();
}
