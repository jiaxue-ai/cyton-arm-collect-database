/****************************************************************************
** Meta object code from reading C++ file 'cytonExampleGUIPlugin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cytonExampleGUIPlugin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cytonExampleGUIPlugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_cytonExampleGUIPlugin_t {
    QByteArrayData data[13];
    char stringdata[200];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_cytonExampleGUIPlugin_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_cytonExampleGUIPlugin_t qt_meta_stringdata_cytonExampleGUIPlugin = {
    {
QT_MOC_LITERAL(0, 0, 21),
QT_MOC_LITERAL(1, 22, 20),
QT_MOC_LITERAL(2, 43, 0),
QT_MOC_LITERAL(3, 44, 21),
QT_MOC_LITERAL(4, 66, 9),
QT_MOC_LITERAL(5, 76, 7),
QT_MOC_LITERAL(6, 84, 15),
QT_MOC_LITERAL(7, 100, 21),
QT_MOC_LITERAL(8, 122, 11),
QT_MOC_LITERAL(9, 134, 21),
QT_MOC_LITERAL(10, 156, 7),
QT_MOC_LITERAL(11, 164, 11),
QT_MOC_LITERAL(12, 176, 23)
    },
    "cytonExampleGUIPlugin\0manipActionCompleted\0"
    "\0actionSeriesCompleted\0EcBoolean\0"
    "success\0actionCompleted\0seriesActionCompleted\0"
    "exampleSlot\0onActionButtonPressed\0"
    "checked\0closeWindow\0onSeriesActionCompleted"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cytonExampleGUIPlugin[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    1,   55,    2, 0x06 /* Public */,
       6,    1,   58,    2, 0x06 /* Public */,
       7,    1,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   64,    2, 0x0a /* Public */,
       9,    1,   65,    2, 0x09 /* Protected */,
      11,    0,   68,    2, 0x09 /* Protected */,
      12,    1,   69,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,

       0        // eod
};

void cytonExampleGUIPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        cytonExampleGUIPlugin *_t = static_cast<cytonExampleGUIPlugin *>(_o);
        switch (_id) {
        case 0: _t->manipActionCompleted(); break;
        case 1: _t->actionSeriesCompleted((*reinterpret_cast< EcBoolean(*)>(_a[1]))); break;
        case 2: _t->actionCompleted((*reinterpret_cast< EcBoolean(*)>(_a[1]))); break;
        case 3: _t->seriesActionCompleted((*reinterpret_cast< EcBoolean(*)>(_a[1]))); break;
        case 4: _t->exampleSlot(); break;
        case 5: _t->onActionButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->closeWindow(); break;
        case 7: _t->onSeriesActionCompleted((*reinterpret_cast< EcBoolean(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (cytonExampleGUIPlugin::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&cytonExampleGUIPlugin::manipActionCompleted)) {
                *result = 0;
            }
        }
        {
            typedef void (cytonExampleGUIPlugin::*_t)(EcBoolean );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&cytonExampleGUIPlugin::actionSeriesCompleted)) {
                *result = 1;
            }
        }
        {
            typedef void (cytonExampleGUIPlugin::*_t)(EcBoolean );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&cytonExampleGUIPlugin::actionCompleted)) {
                *result = 2;
            }
        }
        {
            typedef void (cytonExampleGUIPlugin::*_t)(EcBoolean );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&cytonExampleGUIPlugin::seriesActionCompleted)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject cytonExampleGUIPlugin::staticMetaObject = {
    { &Ec::PluginGUI::staticMetaObject, qt_meta_stringdata_cytonExampleGUIPlugin.data,
      qt_meta_data_cytonExampleGUIPlugin,  qt_static_metacall, 0, 0}
};


const QMetaObject *cytonExampleGUIPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cytonExampleGUIPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_cytonExampleGUIPlugin.stringdata))
        return static_cast<void*>(const_cast< cytonExampleGUIPlugin*>(this));
    return Ec::PluginGUI::qt_metacast(_clname);
}

int cytonExampleGUIPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Ec::PluginGUI::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void cytonExampleGUIPlugin::manipActionCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void cytonExampleGUIPlugin::actionSeriesCompleted(EcBoolean _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void cytonExampleGUIPlugin::actionCompleted(EcBoolean _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void cytonExampleGUIPlugin::seriesActionCompleted(EcBoolean _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
