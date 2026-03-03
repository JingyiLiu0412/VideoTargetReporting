#ifndef TYPES_H
#define TYPES_H

#include <QSize>
#include <QString>

/**
 * @brief Supported target types for the video target reporting system.
 *
 * Each target type has a specific original image size (width x height).
 * The display widget maintains the aspect ratio and fills the parent height.
 */
enum class TargetType {
    ChestRing,  ///< 胸环靶  – original size 640 × 658
    IPSC,       ///< IPSC   – original size 460 × 580
    IDPA        ///< IDPA   – original size 460 × 780
};

/** Original (reference) image sizes for each target type. */
inline QSize targetOriginalSize(TargetType type)
{
    switch (type) {
    case TargetType::ChestRing: return {640, 658};
    case TargetType::IPSC:      return {460, 580};
    case TargetType::IDPA:      return {460, 780};
    }
    Q_UNREACHABLE();
}

/** Display name for each target type. */
inline QString targetDisplayName(TargetType type)
{
    switch (type) {
    case TargetType::ChestRing: return QStringLiteral("胸环靶");
    case TargetType::IPSC:      return QStringLiteral("IPSC");
    case TargetType::IDPA:      return QStringLiteral("IDPA");
    }
    Q_UNREACHABLE();
}

#endif // TYPES_H
