#ifndef TARGETPAGEWIDGET_H
#define TARGETPAGEWIDGET_H

#include "types.h"
#include "bulletholelayer.h"

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QRect>

/**
 * @brief A single target page that stacks a target-image label and a
 *        BulletHoleLayer on top of each other without using a layout manager.
 *
 * Layout rules (applied in resizeEvent):
 *   - The image is scaled so that its HEIGHT equals the parent widget's height
 *     while preserving the original aspect ratio.
 *   - If the scaled width exceeds the widget width the image is instead scaled
 *     to fit the width (fallback, so nothing is clipped).
 *   - The image rect is then centred horizontally.
 *   - The BulletHoleLayer is given the exact same geometry as the image rect.
 */
class TargetPageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TargetPageWidget(TargetType type, QWidget *parent = nullptr);

    /**
     * @brief Add a bullet hole given pixel coordinates inside the original
     *        target image (i.e. as if the image were full-size).
     */
    void addBulletHoleImageCoords(const QPointF &imagePos);

    /**
     * @brief Add a bullet hole given normalised coordinates [0,1].
     */
    void addBulletHoleNormalized(const QPointF &normPos);

    /** Clear all bullet holes from this page. */
    void clearBulletHoles();

    /** Return the target type this page represents. */
    TargetType targetType() const { return m_type; }

    /** Build a placeholder pixmap for the given target type. */
    static QPixmap buildTargetPixmap(TargetType type);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    /** Recompute child geometries based on the current widget size. */
    void updateChildGeometry();

    TargetType       m_type;
    QLabel          *m_imageLabel;
    BulletHoleLayer *m_holeLayer;
    QPixmap          m_pixmap;
    QSize            m_originalSize;
};

#endif // TARGETPAGEWIDGET_H
