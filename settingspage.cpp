#include "settingspage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QButtonGroup>

SettingsPage::SettingsPage(QWidget *parent)
    : QWidget(parent)
    , m_selected(TargetType::ChestRing)
    , m_buttonGroup(new QButtonGroup(this))
{
    // --- Title label ---
    auto *titleLabel = new QLabel(QStringLiteral("靶型选择"), this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont;
    titleFont.setPixelSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    // --- Horizontal row of selector buttons ---
    auto *btnRow = new QHBoxLayout;
    btnRow->setSpacing(16);
    btnRow->addStretch();

    const TargetType types[] = {
        TargetType::ChestRing,
        TargetType::IPSC,
        TargetType::IDPA
    };
    for (TargetType t : types) {
        auto *btn = new TargetSelectorButton(t, this);
        m_buttons.insert(t, btn);
        m_buttonGroup->addButton(btn);
        btnRow->addWidget(btn);
    }
    btnRow->addStretch();

    m_buttonGroup->setExclusive(true);

    // Select the default target.
    if (auto *b = m_buttons.value(m_selected, nullptr))
        b->setChecked(true);

    // --- Separator line ---
    auto *sep = new QFrame(this);
    sep->setFrameShape(QFrame::HLine);
    sep->setFrameShadow(QFrame::Sunken);

    // --- Main layout ---
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(btnRow);
    mainLayout->addWidget(sep);
    mainLayout->addStretch();
    setLayout(mainLayout);

    connect(m_buttonGroup,
            QOverload<QAbstractButton*, bool>::of(&QButtonGroup::buttonToggled),
            this, &SettingsPage::onButtonToggled);
}

void SettingsPage::setSelectedTarget(TargetType type)
{
    if (m_selected == type)
        return;
    m_selected = type;
    if (auto *b = m_buttons.value(type, nullptr))
        b->setChecked(true);
}

void SettingsPage::onButtonToggled(QAbstractButton *button, bool checked)
{
    if (!checked)
        return;
    auto *tsb = qobject_cast<TargetSelectorButton *>(button);
    if (!tsb)
        return;
    m_selected = tsb->targetType();
    emit targetTypeSelected(m_selected);
}
