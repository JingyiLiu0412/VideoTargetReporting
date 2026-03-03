/****************************************************************************
** Meta object code from reading C++ file 'videotargetreporting.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../videotargetreporting.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videotargetreporting.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN20VideoTargetReportingE_t {};
} // unnamed namespace

template <> constexpr inline auto VideoTargetReporting::qt_create_metaobjectdata<qt_meta_tag_ZN20VideoTargetReportingE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "VideoTargetReporting",
        "on_pushButton_exit_clicked",
        "",
        "on_pushButton_HID_clicked",
        "on_pushButton_reset_clicked",
        "on_pushButton_settings_clicked",
        "on_pushButton_back_clicked",
        "setTargetSelected",
        "QLabel*",
        "currentLabel",
        "other1",
        "other2",
        "on_pushButton_reconnection_clicked",
        "on_pushButton_connect_clicked",
        "on_pushButton_start_clicked",
        "on_pushButton_pause_clicked",
        "on_pushButton_end_clicked",
        "on_pushButton_saveRecord_clicked",
        "on_pushButton_history_clicked",
        "on_listWidget_historyTitles_itemClicked",
        "QListWidgetItem*",
        "item",
        "onBulletDataReceived",
        "x",
        "y",
        "score",
        "HID_TARGET",
        "targetType",
        "on_pushButton_deleteRecord_clicked",
        "deleteHistoryRecord",
        "index",
        "on_tableWidget_itemClicked",
        "QTableWidgetItem*",
        "on_tableWidget_historyDetail_itemClicked",
        "onHistoryRecordsChanged",
        "on_pushButton_minimize_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_pushButton_exit_clicked'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_HID_clicked'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_reset_clicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_settings_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_back_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setTargetSelected'
        QtMocHelpers::SlotData<void(QLabel *, QLabel *, QLabel *)>(7, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 8, 9 }, { 0x80000000 | 8, 10 }, { 0x80000000 | 8, 11 },
        }}),
        // Slot 'on_pushButton_reconnection_clicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_connect_clicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_start_clicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_pause_clicked'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_end_clicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_saveRecord_clicked'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_history_clicked'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_listWidget_historyTitles_itemClicked'
        QtMocHelpers::SlotData<void(QListWidgetItem *)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 20, 21 },
        }}),
        // Slot 'onBulletDataReceived'
        QtMocHelpers::SlotData<void(int, int, QString, HID_TARGET)>(22, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 23 }, { QMetaType::Int, 24 }, { QMetaType::QString, 25 }, { 0x80000000 | 26, 27 },
        }}),
        // Slot 'on_pushButton_deleteRecord_clicked'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'deleteHistoryRecord'
        QtMocHelpers::SlotData<void(int)>(29, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 30 },
        }}),
        // Slot 'on_tableWidget_itemClicked'
        QtMocHelpers::SlotData<void(QTableWidgetItem *)>(31, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 32, 21 },
        }}),
        // Slot 'on_tableWidget_historyDetail_itemClicked'
        QtMocHelpers::SlotData<void(QTableWidgetItem *)>(33, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 32, 21 },
        }}),
        // Slot 'onHistoryRecordsChanged'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_minimize_clicked'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<VideoTargetReporting, qt_meta_tag_ZN20VideoTargetReportingE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject VideoTargetReporting::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20VideoTargetReportingE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20VideoTargetReportingE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20VideoTargetReportingE_t>.metaTypes,
    nullptr
} };

void VideoTargetReporting::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<VideoTargetReporting *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_pushButton_exit_clicked(); break;
        case 1: _t->on_pushButton_HID_clicked(); break;
        case 2: _t->on_pushButton_reset_clicked(); break;
        case 3: _t->on_pushButton_settings_clicked(); break;
        case 4: _t->on_pushButton_back_clicked(); break;
        case 5: _t->setTargetSelected((*reinterpret_cast< std::add_pointer_t<QLabel*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QLabel*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QLabel*>>(_a[3]))); break;
        case 6: _t->on_pushButton_reconnection_clicked(); break;
        case 7: _t->on_pushButton_connect_clicked(); break;
        case 8: _t->on_pushButton_start_clicked(); break;
        case 9: _t->on_pushButton_pause_clicked(); break;
        case 10: _t->on_pushButton_end_clicked(); break;
        case 11: _t->on_pushButton_saveRecord_clicked(); break;
        case 12: _t->on_pushButton_history_clicked(); break;
        case 13: _t->on_listWidget_historyTitles_itemClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 14: _t->onBulletDataReceived((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<HID_TARGET>>(_a[4]))); break;
        case 15: _t->on_pushButton_deleteRecord_clicked(); break;
        case 16: _t->deleteHistoryRecord((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 17: _t->on_tableWidget_itemClicked((*reinterpret_cast< std::add_pointer_t<QTableWidgetItem*>>(_a[1]))); break;
        case 18: _t->on_tableWidget_historyDetail_itemClicked((*reinterpret_cast< std::add_pointer_t<QTableWidgetItem*>>(_a[1]))); break;
        case 19: _t->onHistoryRecordsChanged(); break;
        case 20: _t->on_pushButton_minimize_clicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 2:
            case 1:
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QLabel* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *VideoTargetReporting::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoTargetReporting::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20VideoTargetReportingE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int VideoTargetReporting::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}
QT_WARNING_POP
