/****************************************************************************
** Meta object code from reading C++ file 'server.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../server/server.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'server.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_server_t {
    QByteArrayData data[8];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_server_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_server_t qt_meta_stringdata_server = {
    {
QT_MOC_LITERAL(0, 0, 6), // "server"
QT_MOC_LITERAL(1, 7, 18), // "incomingConnection"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 7), // "qintptr"
QT_MOC_LITERAL(4, 35, 16), // "socketDescriptor"
QT_MOC_LITERAL(5, 52, 12), // "sendToClient"
QT_MOC_LITERAL(6, 65, 14), // "readFromClient"
QT_MOC_LITERAL(7, 80, 8) // "sockDisc"

    },
    "server\0incomingConnection\0\0qintptr\0"
    "socketDescriptor\0sendToClient\0"
    "readFromClient\0sockDisc"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_server[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a /* Public */,
       5,    0,   37,    2, 0x0a /* Public */,
       6,    0,   38,    2, 0x0a /* Public */,
       7,    0,   39,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void server::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<server *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->incomingConnection((*reinterpret_cast< qintptr(*)>(_a[1]))); break;
        case 1: _t->sendToClient(); break;
        case 2: _t->readFromClient(); break;
        case 3: _t->sockDisc(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject server::staticMetaObject = { {
    QMetaObject::SuperData::link<QTcpServer::staticMetaObject>(),
    qt_meta_stringdata_server.data,
    qt_meta_data_server,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *server::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *server::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_server.stringdata0))
        return static_cast<void*>(this);
    return QTcpServer::qt_metacast(_clname);
}

int server::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
