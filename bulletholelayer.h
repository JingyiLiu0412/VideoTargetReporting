#ifndef BULLETHOLELAYER_H
#define BULLETHOLELAYER_H

#include <QWidget>
#include <QList>
#include <QPointF>

/**
 * @brief Transparent overlay widget that renders bullet holes on top of a
 *        target image.
 *
 * The layer is placed exactly on top of the displayed image rect inside a
 * TargetPageWidget.  Bullet-hole positions are stored as normalised
 * coordinates in the range [0, 1] relative to the layer's own width/height,
 * so they remain stable across resize events.
 */
class BulletHoleLayer : public QWidget
{
    Q_OBJECT
public:
    explicit BulletHoleLayer(QWidget *parent = nullptr);

    /**
     * @brief Add a bullet hole at a normalised position.
     * @param normPos  Point with x and y each in [0, 1], relative to the
     *                 original target image dimensions.
     */
    void addBulletHole(const QPointF &normPos);

    /** Remove all bullet holes from this layer. */
    void clearBulletHoles();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<QPointF> m_holes;   ///< normalised [0,1] coordinates
    static constexpr int kHoleRadius = 6; ///< display radius in pixels
};

#endif // BULLETHOLELAYER_H
