
#include <QtGui>
#include "ui/BrowserWindow.hpp"

int main(int argc, char * argv[]) {
    QApplication app(argc, argv);

    BrowserWindow *browser;
    if (argc > 1)
        browser = new BrowserWindow(QUrl(argv[1]));
    else
        browser = new BrowserWindow();

    browser->showFullScreen();
    return app.exec();
}