#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include "types.h"
#include "targetselectorbutton.h"

#include <QWidget>
#include <QButtonGroup>
#include <QMap>

/**
 * @brief The settings page widget for the video target reporting system.
 *
 * It presents a row of TargetSelectorButton instances (one per target type)
 * and emits targetTypeSelected() when the operator clicks a different target.
 *
 * The red ✓ is rendered by TargetSelectorButton when it is in the checked
 * state.  A QButtonGroup enforces exclusive selection.
 */
class SettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsPage(QWidget *parent = nullptr);

    /** Programmatically select a target type. */
    void setSelectedTarget(TargetType type);

    /** Return the currently selected target type. */
    TargetType selectedTarget() const { return m_selected; }

signals:
    /** Emitted when the operator selects a different target type. */
    void targetTypeSelected(TargetType type);

private slots:
    void onButtonToggled(QAbstractButton *button, bool checked);

private:
    TargetType                          m_selected;
    QButtonGroup                       *m_buttonGroup;
    QMap<TargetType, TargetSelectorButton *> m_buttons;
};

#endif // SETTINGSPAGE_H
