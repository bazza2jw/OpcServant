/****************************************************************************
** Meta object code from reading C++ file 'Correlator.hpp'
**
** Created: Wed Jul 2 12:10:31 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Correlator.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Correlator.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_CorrelatorThread[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,
      42,   17,   17,   17, 0x05,
      58,   56,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      82,   17,   17,   17, 0x0a,
      90,   17,   17,   17, 0x0a,
      98,   17,   17,   17, 0x0a,
     105,   17,   17,   17, 0x0a,
     117,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CorrelatorThread[] = {
    "CorrelatorThread\0\0CurrentCounts(unsigned)\0"
    "HaveResults()\0,\0Fault(unsigned,QString)\0"
    "Clear()\0Start()\0Stop()\0CountRate()\0"
    "GetResults()\0"
};

const QMetaObject CorrelatorThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CorrelatorThread,
      qt_meta_data_CorrelatorThread, 0 }
};

const QMetaObject *CorrelatorThread::metaObject() const
{
    return &staticMetaObject;
}

void *CorrelatorThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CorrelatorThread))
	return static_cast<void*>(const_cast< CorrelatorThread*>(this));
    return QThread::qt_metacast(_clname);
}

int CorrelatorThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: CurrentCounts((*reinterpret_cast< unsigned(*)>(_a[1]))); break;
        case 1: HaveResults(); break;
        case 2: Fault((*reinterpret_cast< unsigned(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: Clear(); break;
        case 4: Start(); break;
        case 5: Stop(); break;
        case 6: CountRate(); break;
        case 7: GetResults(); break;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CorrelatorThread::CurrentCounts(unsigned  _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CorrelatorThread::HaveResults()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CorrelatorThread::Fault(unsigned  _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
static const uint qt_meta_data_CorrelatorMain[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      24,   15,   15,   15, 0x0a,
      31,   15,   15,   15, 0x0a,
      38,   15,   15,   15, 0x0a,
      45,   15,   15,   15, 0x0a,
      53,   15,   15,   15, 0x0a,
      65,   15,   15,   15, 0x0a,
      89,   15,   15,   15, 0x0a,
     105,  103,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CorrelatorMain[] = {
    "CorrelatorMain\0\0Start()\0Stop()\0Exit()\0"
    "Tick()\0Clear()\0CountRate()\0"
    "CurrentCounts(unsigned)\0HaveResults()\0"
    ",\0Fault(unsigned,QString)\0"
};

const QMetaObject CorrelatorMain::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CorrelatorMain,
      qt_meta_data_CorrelatorMain, 0 }
};

const QMetaObject *CorrelatorMain::metaObject() const
{
    return &staticMetaObject;
}

void *CorrelatorMain::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CorrelatorMain))
	return static_cast<void*>(const_cast< CorrelatorMain*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CorrelatorMain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Start(); break;
        case 1: Stop(); break;
        case 2: Exit(); break;
        case 3: Tick(); break;
        case 4: Clear(); break;
        case 5: CountRate(); break;
        case 6: CurrentCounts((*reinterpret_cast< unsigned(*)>(_a[1]))); break;
        case 7: HaveResults(); break;
        case 8: Fault((*reinterpret_cast< unsigned(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        }
        _id -= 9;
    }
    return _id;
}
