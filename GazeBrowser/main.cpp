
#include <QtGui>
#include "ui/BrowserWindow.hpp"
#include <string>

int main(int argc, char * argv[]) {
    QApplication app(argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
            QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load(
           //QString::fromStdString(GazeConfig::inWorkingDir("translation/gazebrowser_de")));
           "gazebrowser_" + QLocale::system().name());
    app.installTranslator(&myappTranslator);
    
    BrowserWindow *browser;
    
    // was the camera number passed as an argument?
    bool conversion_ok = false;
    int camera = 0;
    if (argc > 1){
        camera = QString(argv[1]).toInt(&conversion_ok, 10);
        if(!conversion_ok)
            camera = 0;
    } 
    
    browser = new BrowserWindow(camera);

    browser->showFullScreen();
    return app.exec();
}