
#include <QtGui>
#include "ui/BrowserWindow.hpp"

int main(int argc, char * argv[]) {
    QApplication app(argc, argv);

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