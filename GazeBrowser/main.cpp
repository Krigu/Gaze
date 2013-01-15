#include <QtGui>
#include "ui/BrowserWindow.hpp"
#include <string>

/*! \mainpage GazeBrowser API Documentation
 *
 * \section intro_sec Introduction
 *
 * This is the API Documentation of the GazeBrowser
 *
 * \section About GazeBrowser
 * GazeBrowser is an Application for browsing the internet with your eye movements. 
 * <br/> GazeBrowser uses the GazeLib (developed by the same authors) to measure the Gaze Tracking signals.
 * This application makes heavy use of Qt, OpenCV and the GazeLib API
 *
 * \subsection Usage
 * Start the Browser, Calibrate the System and use your eyes!
 */
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