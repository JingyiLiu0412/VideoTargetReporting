#include "targetdisplaywidget.h"

TargetDisplayWidget::TargetDisplayWidget(QWidget *parent)
    : QStackedWidget(parent)
    , m_currentType(TargetType::ChestRing)
{
    // Create one page for every target type.
    const TargetType types[] = {
        TargetType::ChestRing,
        TargetType::IPSC,
        TargetType::IDPA
    };
    for (TargetType t : types) {
        auto *page = new TargetPageWidget(t, this);
        addWidget(page);
        m_pages.insert(t, page);
    }

    // Show the default target.
    setCurrentWidget(m_pages.value(m_currentType));
}

void TargetDisplayWidget::setTargetType(TargetType type)
{
    if (m_currentType == type)
        return;
    m_currentType = type;
    TargetPageWidget *page = m_pages.value(type, nullptr);
    if (page)
        setCurrentWidget(page);
    emit targetTypeChanged(type);
}

void TargetDisplayWidget::addBulletHoleImageCoords(const QPointF &imagePos)
{
    if (auto *p = m_pages.value(m_currentType, nullptr))
        p->addBulletHoleImageCoords(imagePos);
}

void TargetDisplayWidget::addBulletHoleNormalized(const QPointF &normPos)
{
    if (auto *p = m_pages.value(m_currentType, nullptr))
        p->addBulletHoleNormalized(normPos);
}

void TargetDisplayWidget::clearBulletHoles()
{
    if (auto *p = m_pages.value(m_currentType, nullptr))
        p->clearBulletHoles();
}

void TargetDisplayWidget::clearAllBulletHoles()
{
    for (auto *p : m_pages)
        p->clearBulletHoles();
}
