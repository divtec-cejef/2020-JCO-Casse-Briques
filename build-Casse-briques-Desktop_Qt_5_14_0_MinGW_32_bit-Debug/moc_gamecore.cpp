/****************************************************************************
** Meta object code from reading C++ file 'gamecore.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/gamecore.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gamecore.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GameCore_t {
    QByteArrayData data[12];
    char stringdata0[173];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GameCore_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GameCore_t qt_meta_stringdata_GameCore = {
    {
QT_MOC_LITERAL(0, 0, 8), // "GameCore"
QT_MOC_LITERAL(1, 9, 16), // "notifyMouseMoved"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 16), // "newMousePosition"
QT_MOC_LITERAL(4, 44, 24), // "notifyMouseButtonPressed"
QT_MOC_LITERAL(5, 69, 13), // "mousePosition"
QT_MOC_LITERAL(6, 83, 16), // "Qt::MouseButtons"
QT_MOC_LITERAL(7, 100, 7), // "buttons"
QT_MOC_LITERAL(8, 108, 25), // "notifyMouseButtonReleased"
QT_MOC_LITERAL(9, 134, 16), // "notifyKeyPressed"
QT_MOC_LITERAL(10, 151, 3), // "key"
QT_MOC_LITERAL(11, 155, 17) // "notifyKeyReleased"

    },
    "GameCore\0notifyMouseMoved\0\0newMousePosition\0"
    "notifyMouseButtonPressed\0mousePosition\0"
    "Qt::MouseButtons\0buttons\0"
    "notifyMouseButtonReleased\0notifyKeyPressed\0"
    "key\0notifyKeyReleased"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GameCore[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    2,   42,    2, 0x06 /* Public */,
       8,    2,   47,    2, 0x06 /* Public */,
       9,    1,   52,    2, 0x06 /* Public */,
      11,    1,   55,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPointF,    3,
    QMetaType::Void, QMetaType::QPointF, 0x80000000 | 6,    5,    7,
    QMetaType::Void, QMetaType::QPointF, 0x80000000 | 6,    5,    7,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,

       0        // eod
};

void GameCore::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GameCore *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->notifyMouseMoved((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 1: _t->notifyMouseButtonPressed((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< Qt::MouseButtons(*)>(_a[2]))); break;
        case 2: _t->notifyMouseButtonReleased((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< Qt::MouseButtons(*)>(_a[2]))); break;
        case 3: _t->notifyKeyPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->notifyKeyReleased((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GameCore::*)(QPointF );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GameCore::notifyMouseMoved)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GameCore::*)(QPointF , Qt::MouseButtons );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GameCore::notifyMouseButtonPressed)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (GameCore::*)(QPointF , Qt::MouseButtons );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GameCore::notifyMouseButtonReleased)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (GameCore::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GameCore::notifyKeyPressed)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (GameCore::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GameCore::notifyKeyReleased)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GameCore::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_GameCore.data,
    qt_meta_data_GameCore,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GameCore::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GameCore::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GameCore.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GameCore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void GameCore::notifyMouseMoved(QPointF _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GameCore::notifyMouseButtonPressed(QPointF _t1, Qt::MouseButtons _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GameCore::notifyMouseButtonReleased(QPointF _t1, Qt::MouseButtons _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GameCore::notifyKeyPressed(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void GameCore::notifyKeyReleased(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
