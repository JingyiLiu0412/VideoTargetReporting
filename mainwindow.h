#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "types.h"
#include "targetdisplaywidget.h"
#include "settingspage.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The application's main window.
 *
 * Layout:
 *   Left panel  (stretch 2) – TargetDisplayWidget with adaptive target image
 *                             and bullet-hole overlay layer.
 *   Right panel (stretch 1) – SettingsPage for target-type selection, plus
 *                             demo controls.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTargetTypeSelected(TargetType type);
    void onAddBulletHole();
    void onClearBulletHoles();

private:
    Ui::MainWindow      *ui;
    TargetDisplayWidget *m_targetDisplay;
    SettingsPage        *m_settingsPage;
};

#endif // MAINWINDOW_H
