#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_targetDisplay(new TargetDisplayWidget(this))
    , m_settingsPage(new SettingsPage(this))
{
    ui->setupUi(this);

    // Set 2:1 stretch ratio for left/right panels manually since Qt 5's uic
    // doesn't support the stretch list property in .ui files.
    ui->horizontalLayout->setStretch(0, 2);
    ui->horizontalLayout->setStretch(1, 1);

    // --- Embed TargetDisplayWidget into the container provided by the .ui ---
    {
        auto *layout = new QVBoxLayout(ui->targetDisplayContainer);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(m_targetDisplay);
        ui->targetDisplayContainer->setLayout(layout);
    }

    // --- Embed SettingsPage into its container ---
    {
        auto *layout = new QVBoxLayout(ui->settingsContainer);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(m_settingsPage);
        ui->settingsContainer->setLayout(layout);
    }

    // --- Connect signals ---
    connect(m_settingsPage, &SettingsPage::targetTypeSelected,
            this, &MainWindow::onTargetTypeSelected);
    connect(ui->btnAddHole,    &QPushButton::clicked,
            this, &MainWindow::onAddBulletHole);
    connect(ui->btnClearHoles, &QPushButton::clicked,
            this, &MainWindow::onClearBulletHoles);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTargetTypeSelected(TargetType type)
{
    m_targetDisplay->setTargetType(type);
    ui->statusLabel->setText(
        QStringLiteral("当前靶型: %1").arg(targetDisplayName(type)));
}

void MainWindow::onAddBulletHole()
{
    // Add a bullet hole at a random normalised position (demo only).
    const double x = 0.1 + QRandomGenerator::global()->generateDouble() * 0.8;
    const double y = 0.1 + QRandomGenerator::global()->generateDouble() * 0.8;
    m_targetDisplay->addBulletHoleNormalized(QPointF(x, y));
    ui->statusLabel->setText(
        QStringLiteral("弹孔已添加 (%.2f, %.2f)").arg(x).arg(y));
}

void MainWindow::onClearBulletHoles()
{
    m_targetDisplay->clearAllBulletHoles();
    ui->statusLabel->setText(QStringLiteral("弹孔已清除"));
}
