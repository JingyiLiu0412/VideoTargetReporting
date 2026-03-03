#ifndef TARGETSELECTORBUTTON_H
#define TARGETSELECTORBUTTON_H

#include "types.h"

#include <QAbstractButton>
#include <QPixmap>

/**
 * @brief A checkable button that displays a target image and paints a
 *        red ✓ (checkmark) on top when checked.
 *
 * Used by the SettingsPage to let the operator choose the active target type.
 * The button respects the original target aspect ratio when painting.
 */
class TargetSelectorButton : public QAbstractButton
{
    Q_OBJECT
public:
    explicit TargetSelectorButton(TargetType type, QWidget *parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    TargetType targetType() const { return m_type; }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    /** Generate a thumbnail-sized pixmap for the target type. */
    static QPixmap buildThumbnail(TargetType type, const QSize &sz);

    TargetType m_type;
    QSize      m_originalSize;
    mutable QPixmap m_cachedThumb;   ///< scaled thumbnail; invalidated on resize
    mutable QSize   m_cachedThumbSize;
};

#endif // TARGETSELECTORBUTTON_H
