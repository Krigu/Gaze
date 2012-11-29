/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created: Thu Nov 29 21:28:31 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   28,   28,   28, 0x09,
      29,   28,   28,   28, 0x09,
      46,   28,   28,   28, 0x09,
      60,   77,   28,   28, 0x09,
      79,   28,   28,   28, 0x09,
      99,   28,   28,   28, 0x09,
     112,   28,   28,   28, 0x09,
     135,   28,   28,   28, 0x09,
     155,  174,   28,   28, 0x09,
     181,   28,   28,   28, 0x09,
     199,   28,   28,   28, 0x09,
     220,   28,   28,   28, 0x09,
     243,   28,   28,   28, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0adjustLocation()\0\0"
    "changeLocation()\0adjustTitle()\0"
    "setProgress(int)\0p\0finishLoading(bool)\0"
    "viewSource()\0slotSourceDownloaded()\0"
    "highlightAllLinks()\0rotateImages(bool)\0"
    "invert\0removeGifImages()\0removeInlineFrames()\0"
    "removeObjectElements()\0removeEmbeddedElements()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->adjustLocation(); break;
        case 1: _t->changeLocation(); break;
        case 2: _t->adjustTitle(); break;
        case 3: _t->setProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->finishLoading((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->viewSource(); break;
        case 6: _t->slotSourceDownloaded(); break;
        case 7: _t->highlightAllLinks(); break;
        case 8: _t->rotateImages((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->removeGifImages(); break;
        case 10: _t->removeInlineFrames(); break;
        case 11: _t->removeObjectElements(); break;
        case 12: _t->removeEmbeddedElements(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
