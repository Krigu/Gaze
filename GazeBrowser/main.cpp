
#include <QtGui>
#include "ui/BrowserWindow.hpp"

int main(int argc, char * argv[]) {
    QApplication app(argc, argv);

    MainWindow *browser;
    if (argc > 1)
        browser = new MainWindow(QUrl(argv[1]));
    else
        browser = new MainWindow();

    browser->showFullScreen();
    return app.exec();
}