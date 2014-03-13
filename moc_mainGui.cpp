/****************************************************************************
** Meta object code from reading C++ file 'mainGui.hpp'
**
** Created: Thu Mar 13 12:30:06 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "client/mainGui.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainGui.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainGui[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x08,
      17,    8,    8,    8, 0x08,
      24,    8,    8,    8, 0x08,
      32,    8,    8,    8, 0x08,
      41,    8,    8,    8, 0x08,
      52,    8,    8,    8, 0x08,
      66,    8,    8,    8, 0x08,
      78,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainGui[] = {
    "MainGui\0\0about()\0quit()\0login()\0"
    "logout()\0listMgrs()\0listPlayers()\0"
    "buildings()\0listAndChooseTournaments()\0"
};

void MainGui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainGui *_t = static_cast<MainGui *>(_o);
        switch (_id) {
        case 0: _t->about(); break;
        case 1: _t->quit(); break;
        case 2: _t->login(); break;
        case 3: _t->logout(); break;
        case 4: _t->listMgrs(); break;
        case 5: _t->listPlayers(); break;
        case 6: _t->buildings(); break;
        case 7: _t->listAndChooseTournaments(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MainGui::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainGui::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainGui,
      qt_meta_data_MainGui, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainGui::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainGui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainGui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainGui))
        return static_cast<void*>(const_cast< MainGui*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
