#include "targetselectorbutton.h"
#include "targetpagewidget.h"   // re-use buildTargetPixmap via friend / static

#include <QPainter>
#include <QPaintEvent>
#include <QFontMetrics>

// ---------------------------------------------------------------------------
// TargetSelectorButton
// ---------------------------------------------------------------------------

TargetSelectorButton::TargetSelectorButton(TargetType type, QWidget *parent)
    : QAbstractButton(parent)
    , m_type(type)
    , m_originalSize(targetOriginalSize(type))
    , m_cachedThumbSize(-1, -1)
{
    setCheckable(true);
    setCursor(Qt::PointingHandCursor);
    setText(targetDisplayName(type));
}

QSize TargetSelectorButton::sizeHint() const
{
    // Show the button at a comfortable thumbnail size (120 px tall).
    const int thumbH = 120;
    const double ratio = static_cast<double>(m_originalSize.width())
                       / m_originalSize.height();
    return QSize(qRound(thumbH * ratio), thumbH + 24 /* label */);
}

QSize TargetSelectorButton::minimumSizeHint() const
{
    const int thumbH = 80;
    const double ratio = static_cast<double>(m_originalSize.width())
                       / m_originalSize.height();
    return QSize(qRound(thumbH * ratio), thumbH + 20);
}

void TargetSelectorButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int w = width();
    const int h = height();

    // --- 1. Reserve bottom strip for the text label ---
    const int labelH = 22;
    const int imageAreaH = h - labelH;

    // --- 2. Compute image rect maintaining aspect ratio ---
    const double origW = m_originalSize.width();
    const double origH = m_originalSize.height();
    const double ratio = origW / origH;

    int dispH = imageAreaH;
    int dispW = qRound(dispH * ratio);
    if (dispW > w) {
        dispW = w;
        dispH = qRound(dispW / ratio);
    }
    const int imgX = (w - dispW) / 2;
    const int imgY = (imageAreaH - dispH) / 2;
    const QRect imgRect(imgX, imgY, dispW, dispH);

    // --- 3. Draw highlight / border when hovered ---
    if (underMouse() || isChecked()) {
        painter.setPen(QPen(isChecked() ? Qt::red : QColor(80, 80, 80), 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(imgRect.adjusted(-3, -3, 3, 3), 4, 4);
    }

    // --- 4. Draw target thumbnail (cached; rebuilt only when display size changes) ---
    const QSize neededSize(dispW, dispH);
    if (m_cachedThumb.isNull() || m_cachedThumbSize != neededSize) {
        m_cachedThumb = TargetPageWidget::buildTargetPixmap(m_type)
                            .scaled(neededSize,
                                    Qt::IgnoreAspectRatio,
                                    Qt::SmoothTransformation);
        m_cachedThumbSize = neededSize;
    }
    painter.drawPixmap(imgRect, m_cachedThumb);

    // --- 5. Draw red checkmark when selected ---
    if (isChecked()) {
        // Draw a semi-transparent red rectangle
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255, 0, 0, 40));
        painter.drawRect(imgRect);

        // Draw the ✓ symbol
        QFont checkFont;
        checkFont.setPixelSize(qMax(24, dispH / 3));
        checkFont.setBold(true);
        painter.setFont(checkFont);
        painter.setPen(Qt::red);
        painter.drawText(imgRect, Qt::AlignCenter, QStringLiteral("✓"));
    }

    // --- 6. Draw text label ---
    QFont labelFont;
    labelFont.setPixelSize(13);
    painter.setFont(labelFont);
    painter.setPen(isChecked() ? Qt::red : Qt::black);
    const QRect labelRect(0, imageAreaH, w, labelH);
    painter.drawText(labelRect, Qt::AlignCenter, text());
}
