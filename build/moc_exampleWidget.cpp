/****************************************************************************
** Meta object code from reading C++ file 'exampleWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../exampleWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'exampleWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_exampleWidget_t {
    QByteArrayData data[11];
    char stringdata[179];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_exampleWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_exampleWidget_t qt_meta_stringdata_exampleWidget = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 16),
QT_MOC_LITERAL(2, 31, 0),
QT_MOC_LITERAL(3, 32, 23),
QT_MOC_LITERAL(4, 56, 28),
QT_MOC_LITERAL(5, 85, 9),
QT_MOC_LITERAL(6, 95, 7),
QT_MOC_LITERAL(7, 103, 17),
QT_MOC_LITERAL(8, 121, 23),
QT_MOC_LITERAL(9, 145, 16),
QT_MOC_LITERAL(10, 162, 16)
    },
    "exampleWidget\0onRunManipAction\0\0"
    "onSelectManipActionFile\0"
    "onManipActionActionCompleted\0EcBoolean\0"
    "success\0onLoadManipAction\0"
    "onActionSeriesCompleted\0onRunManipSeries\0"
    "onRunProgramCode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_exampleWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x09 /* Protected */,
       3,    0,   50,    2, 0x09 /* Protected */,
       4,    1,   51,    2, 0x09 /* Protected */,
       7,    0,   54,    2, 0x09 /* Protected */,
       8,    1,   55,    2, 0x09 /* Protected */,
       9,    0,   58,    2, 0x09 /* Protected */,
      10,    0,   59,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void exampleWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        exampleWidget *_t = static_cast<exampleWidget *>(_o);
        switch (_id) {
        case 0: _t->onRunManipAction(); break;
        case 1: _t->onSelectManipActionFile(); break;
        case 2: _t->onManipActionActionCompleted((*reinterpret_cast< EcBoolean(*)>(_a[1]))); break;
        case 3: _t->onLoadManipAction(); break;
        case 4: _t->onActionSeriesCompleted((*reinterpret_cast< EcBoolean(*)>(_a[1]))); break;
        case 5: _t->onRunManipSeries(); break;
        case 6: _t->onRunProgramCode(); break;
        default: ;
        }
    }
}

const QMetaObject exampleWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_exampleWidget.data,
      qt_meta_data_exampleWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *exampleWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *exampleWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_exampleWidget.stringdata))
        return static_cast<void*>(const_cast< exampleWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int exampleWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
