#ifndef HOLEOVERLAY_H
#define HOLEOVERLAY_H

#include <QWidget>
#include <QList>
#include <QPoint>
#include <QPainter>
#include <QColor>

// 新增：弹孔信息结构体（坐标+颜色）
struct HoleData {
    QPoint pos;       // 靶面坐标
    QColor color;     // 弹孔颜色
};

class HoleOverlay : public QWidget {
    Q_OBJECT
public:
    explicit HoleOverlay(QWidget *parent = nullptr)
        : QWidget(parent),
        m_targetWidth(640),    // 靶面默认宽度（与UI中frame尺寸一致）
        m_targetHeight(640),   // 靶面默认高度
        m_holeRadius(5),        // 弹孔半径（像素）
        // 初始化最后一个弹孔为无效坐标（-1,-1），表示无历史记录
        m_lastHole(-1, -1)
    {
        // 设置透明背景，避免遮挡靶面图片
        setAttribute(Qt::WA_TransparentForMouseEvents); // 鼠标事件穿透
        setStyleSheet("background-color: transparent;");
        // 初始化默认颜色（原有弹孔颜色：半透明红色）
        m_defaultColor = QColor(0, 0, 0, 200);
        m_latestColor = QColor(255, 0, 0, 220); // 最新弹孔：红色（突出）
        m_selectedColor = QColor(0, 255, 0, 220); // 选中弹孔：绿色（醒目）
    }
    // 新增：校验新弹孔是否有效（不重复）
    bool isHoleValid(uint16_t rawX, uint16_t rawY) const {
        QPoint mappedPos = mapRawToTarget(rawX, rawY);
        return mappedPos != m_lastHole; // 和绘制层用同一套去重逻辑
    }

    // 1. 新增：添加弹孔（支持指定颜色，默认用默认色）
    void addHole(uint16_t rawX, uint16_t rawY, QColor color = QColor()) {
        QPoint mappedPos = mapRawToTarget(rawX, rawY);
        if (mappedPos == m_lastHole) {
            return; // 去重逻辑保留
        }
        m_lastHole = mappedPos;

        // 若未指定颜色，使用默认色
        HoleData hole;
        hole.pos = mappedPos;
        hole.color = color.isValid() ? color : m_defaultColor;
        m_holes.append(hole);

        update();
    }

    // 2. 新增：更新指定索引弹孔的颜色
    void updateHoleColor(int index, QColor color) {
        if (index >= 0 && index < m_holes.size()) {
            m_holes[index].color = color;
            update(); // 触发重绘
        }
    }

    // 3. 新增：获取弹孔总数（用于索引校验）
    int holeCount() const {
        return m_holes.size();
    }

    // 4. 新增：设置三种核心颜色（外部可配置）
    void setDefaultColor(const QColor& color) { m_defaultColor = color; }
    void setLatestColor(const QColor& color) { m_latestColor = color; }
    void setSelectedColor(const QColor& color) { m_selectedColor = color; }

    // 原有接口保留（兼容旧逻辑）
    void clearHoles() {
        m_holes.clear();
        m_lastHole = QPoint(-1, -1);
        update();
    }
    void initLastHole() { m_lastHole = QPoint(-1, -1); }
    void setTargetSize(int width, int height) {
        m_targetWidth = width;
        m_targetHeight = height;
        resize(width, height);
    }
    void setRawCoordRange(uint16_t max_x, uint16_t max_y) {
        m_rawCoordMax_x = max_x;
        m_rawCoordMax_y = max_y;
    }
    void setHoleRadius(int radius) { m_holeRadius = radius; }
    QPoint mapRawToTarget(uint16_t rawX, uint16_t rawY) const {
        int targetX = static_cast<int>((static_cast<double>(rawX) / m_rawCoordMax_x) * m_targetWidth);
        int targetY = static_cast<int>((static_cast<double>(rawY) / m_rawCoordMax_y) * m_targetHeight);
        targetX = qBound(0, targetX, m_targetWidth - 1);
        targetY = qBound(0, targetY, m_targetHeight - 1);
        return QPoint(targetX, targetY);
    }
    // QPoint getLastHole() const { return m_lastHole; }

    // 新增：获取三种颜色的 public 接口（外部类通过接口访问）
    QColor getDefaultColor() const { return m_defaultColor; }
    QColor getLatestColor() const { return m_latestColor; }  // 关键：获取最新弹孔颜色
    QColor getSelectedColor() const { return m_selectedColor; }
protected:
    // void paintEvent(QPaintEvent *event) override {
    //     Q_UNUSED(event);
    //     QPainter painter(this);
    //     painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿

    //     // 绘制弹孔（红色实心圆）
    //     painter.setPen(Qt::NoPen); // 无边框
    //     painter.setBrush(QColor(255, 0, 0, 200)); // 半透明红色（R,G,B,透明度）

    //     for (const QPoint &hole : m_holes) {
    //         painter.drawEllipse(hole, m_holeRadius, m_holeRadius);
    //     }
    // }
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen); // 无边框，保持原有风格

        // 遍历所有弹孔，按各自颜色绘制
        for (const HoleData &hole : m_holes) {
            painter.setBrush(hole.color);
            painter.drawEllipse(hole.pos, m_holeRadius, m_holeRadius);
        }
    }

private:
    QList<HoleData> m_holes;       // 弹孔列表（靶面坐标）
    int m_targetWidth;           // 靶面宽度（像素）
    int m_targetHeight;          // 靶面高度（像素）
    uint16_t m_rawCoordMax_x;      // 原始坐标最大值（硬件范围）
    uint16_t m_rawCoordMax_y;
    int m_holeRadius;            // 弹孔半径（像素）
    QPoint m_lastHole;           // 新增：记录最后一个弹孔的坐标（用于去重）

    // 新增：三种核心颜色
    QColor m_defaultColor;    // 普通弹孔颜色
    QColor m_latestColor;     // 最新弹孔颜色
    QColor m_selectedColor;   // 选中弹孔颜色
};

#endif // HOLEOVERLAY_H
