#include "bulletholelayer.h"

#include <QPainter>
#include <QPaintEvent>

BulletHoleLayer::BulletHoleLayer(QWidget *parent)
    : QWidget(parent)
{
    // Transparent background – do NOT fill with any colour.
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);
}

void BulletHoleLayer::addBulletHole(const QPointF &normPos)
{
    m_holes.append(normPos);
    update();
}

void BulletHoleLayer::clearBulletHoles()
{
    m_holes.clear();
    update();
}

void BulletHoleLayer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    if (m_holes.isEmpty())
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const double w = width();
    const double h = height();

    for (const QPointF &norm : m_holes) {
        // Map normalised [0,1] to layer pixel coordinates.
        const double cx = norm.x() * w;
        const double cy = norm.y() * h;

        // Draw a filled red circle with a dark border.
        painter.setPen(QPen(Qt::black, 1.5));
        painter.setBrush(Qt::red);
        painter.drawEllipse(QPointF(cx, cy), kHoleRadius, kHoleRadius);
    }
}
