#ifndef TARGETDISPLAYWIDGET_H
#define TARGETDISPLAYWIDGET_H

#include "types.h"
#include "targetpagewidget.h"

#include <QStackedWidget>
#include <QMap>

/**
 * @brief A QStackedWidget that holds one TargetPageWidget per target type.
 *
 * Consumers should call setTargetType() to switch between targets and
 * addBulletHole*() to plot a new bullet hole on the active target.
 */
class TargetDisplayWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit TargetDisplayWidget(QWidget *parent = nullptr);

    /** Switch the visible target page. */
    void setTargetType(TargetType type);

    /** Return the currently displayed target type. */
    TargetType currentTargetType() const { return m_currentType; }

    /**
     * @brief Add a bullet hole to the current target page using pixel
     *        coordinates in the original target image space.
     */
    void addBulletHoleImageCoords(const QPointF &imagePos);

    /**
     * @brief Add a bullet hole to the current target page using normalised
     *        [0, 1] coordinates.
     */
    void addBulletHoleNormalized(const QPointF &normPos);

    /** Clear all bullet holes from the current target page. */
    void clearBulletHoles();

    /** Clear bullet holes on all pages. */
    void clearAllBulletHoles();

signals:
    /** Emitted after the displayed target type changes. */
    void targetTypeChanged(TargetType type);

private:
    TargetType                    m_currentType;
    QMap<TargetType, TargetPageWidget *> m_pages;
};

#endif // TARGETDISPLAYWIDGET_H
