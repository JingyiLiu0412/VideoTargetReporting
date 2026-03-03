#include "targetpagewidget.h"

#include <QPainter>
#include <QResizeEvent>

// ---------------------------------------------------------------------------
// Placeholder target image generator
// ---------------------------------------------------------------------------

static void drawChestRingTarget(QPainter &p, int w, int h)
{
    // White background
    p.fillRect(0, 0, w, h, Qt::white);

    // Outer border
    p.setPen(QPen(Qt::black, 2));
    p.drawRect(1, 1, w - 2, h - 2);

    const QPointF centre(w / 2.0, h * 0.52);
    const double unitR = qMin(w, h) * 0.42;

    // Concentric rings (10 → 1 from inside out)
    const QColor ringColors[] = {
        QColor(255,255,255), QColor(255,255,255),   // 10, 9 – white
        QColor(0,0,0),       QColor(0,0,0),          // 8, 7 – black
        QColor(255,255,255), QColor(255,255,255),    // 6, 5 – white
        QColor(255,255,255), QColor(255,255,255),    // 4, 3
        QColor(255,255,255), QColor(255,255,255)     // 2, 1
    };
    for (int ring = 10; ring >= 1; --ring) {
        double r = unitR * ring / 10.0;
        p.setBrush(ringColors[ring - 1]);
        p.setPen(QPen(Qt::black, 1));
        p.drawEllipse(centre, r, r);
    }
    // Label "胸环靶"
    p.setPen(Qt::black);
    QFont f;
    f.setPixelSize(qMax(12, h / 25));
    p.setFont(f);
    p.drawText(QRectF(0, h * 0.88, w, h * 0.10),
               Qt::AlignHCenter | Qt::AlignVCenter, QStringLiteral("胸环靶"));
}

static void drawIPSCTarget(QPainter &p, int w, int h)
{
    p.fillRect(0, 0, w, h, QColor(210, 180, 140)); // tan / cardboard

    p.setPen(QPen(Qt::black, 2));
    p.drawRect(1, 1, w - 2, h - 2);

    // Simple silhouette: head + torso
    const double cx = w / 2.0;

    // Head circle
    double headR  = w * 0.14;
    double headCY = h * 0.12;
    p.setBrush(QColor(210, 180, 140));
    p.setPen(QPen(Qt::black, 1.5));
    p.drawEllipse(QPointF(cx, headCY), headR, headR);

    // Torso rectangle (rounded)
    double torsoX = w * 0.15;
    double torsoY = h * 0.24;
    double torsoW = w * 0.70;
    double torsoH = h * 0.55;
    p.drawRoundedRect(QRectF(torsoX, torsoY, torsoW, torsoH), 8, 8);

    // A-zone oval in centre of torso
    double azoneW = torsoW * 0.60;
    double azoneH = torsoH * 0.55;
    p.setPen(QPen(Qt::black, 1));
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(QPointF(cx, torsoY + torsoH * 0.42), azoneW / 2, azoneH / 2);

    p.setPen(Qt::black);
    QFont f;
    f.setPixelSize(qMax(12, h / 25));
    p.setFont(f);
    p.drawText(QRectF(0, h * 0.82, w, h * 0.10),
               Qt::AlignHCenter | Qt::AlignVCenter, QStringLiteral("IPSC"));
}

static void drawIDPATarget(QPainter &p, int w, int h)
{
    p.fillRect(0, 0, w, h, QColor(176, 196, 222)); // steel blue

    p.setPen(QPen(Qt::black, 2));
    p.drawRect(1, 1, w - 2, h - 2);

    const double cx = w / 2.0;

    // Head
    double headR  = w * 0.13;
    double headCY = h * 0.09;
    p.setBrush(QColor(176, 196, 222));
    p.setPen(QPen(Qt::black, 1.5));
    p.drawEllipse(QPointF(cx, headCY), headR, headR);

    // Torso (taller than IPSC because 780 px height vs 580)
    double torsoX = w * 0.15;
    double torsoY = h * 0.18;
    double torsoW = w * 0.70;
    double torsoH = h * 0.62;
    p.drawRoundedRect(QRectF(torsoX, torsoY, torsoW, torsoH), 8, 8);

    // -0 zone circle
    double zoneR = qMin(torsoW, torsoH) * 0.25;
    p.setPen(QPen(Qt::black, 1));
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(QPointF(cx, torsoY + torsoH * 0.35), zoneR, zoneR);

    p.setPen(Qt::black);
    QFont f;
    f.setPixelSize(qMax(12, h / 25));
    p.setFont(f);
    p.drawText(QRectF(0, h * 0.85, w, h * 0.10),
               Qt::AlignHCenter | Qt::AlignVCenter, QStringLiteral("IDPA"));
}

// ---------------------------------------------------------------------------
// TargetPageWidget
// ---------------------------------------------------------------------------

TargetPageWidget::TargetPageWidget(TargetType type, QWidget *parent)
    : QWidget(parent)
    , m_type(type)
    , m_imageLabel(new QLabel(this))
    , m_holeLayer(new BulletHoleLayer(this))
    , m_originalSize(targetOriginalSize(type))
{
    m_pixmap = buildTargetPixmap(type);

    // Image label – no layout, geometry is set manually in resizeEvent.
    m_imageLabel->setScaledContents(true);
    m_imageLabel->setPixmap(m_pixmap);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setStyleSheet(QStringLiteral("background: transparent;"));

    // Hole layer must be on top; raise it above the label.
    m_holeLayer->raise();
}

void TargetPageWidget::addBulletHoleImageCoords(const QPointF &imagePos)
{
    // Convert pixel coords in the original image to normalised [0,1].
    QPointF norm(imagePos.x() / m_originalSize.width(),
                 imagePos.y() / m_originalSize.height());
    m_holeLayer->addBulletHole(norm);
}

void TargetPageWidget::addBulletHoleNormalized(const QPointF &normPos)
{
    m_holeLayer->addBulletHole(normPos);
}

void TargetPageWidget::clearBulletHoles()
{
    m_holeLayer->clearBulletHoles();
}

void TargetPageWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateChildGeometry();
}

void TargetPageWidget::updateChildGeometry()
{
    const int availW = width();
    const int availH = height();
    if (availW <= 0 || availH <= 0)
        return;

    const double origW = m_originalSize.width();
    const double origH = m_originalSize.height();
    const double ratio = origW / origH;

    // Priority: fill height, maintain aspect ratio.
    int dispH = availH;
    int dispW = qRound(dispH * ratio);

    // Fallback: if the width overflows, scale to fit width instead.
    if (dispW > availW) {
        dispW = availW;
        dispH = qRound(dispW / ratio);
    }

    // Centre horizontally (and vertically in the fallback case).
    int x = (availW - dispW) / 2;
    int y = (availH - dispH) / 2;

    const QRect imageRect(x, y, dispW, dispH);
    m_imageLabel->setGeometry(imageRect);
    m_holeLayer->setGeometry(imageRect);
}

QPixmap TargetPageWidget::buildTargetPixmap(TargetType type)
{
    const QSize sz = targetOriginalSize(type);
    QPixmap pix(sz);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);

    switch (type) {
    case TargetType::ChestRing:
        drawChestRingTarget(painter, sz.width(), sz.height());
        break;
    case TargetType::IPSC:
        drawIPSCTarget(painter, sz.width(), sz.height());
        break;
    case TargetType::IDPA:
        drawIDPATarget(painter, sz.width(), sz.height());
        break;
    }
    return pix;
}
