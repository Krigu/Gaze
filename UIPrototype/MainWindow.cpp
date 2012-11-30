
 #include <QtGui>
 #include <QtNetwork>
 #include <QtWebKit>
 #include "mainwindow.h"

 
 MainWindow::MainWindow(const QUrl& url)
 {
     progress = 0;

     QFile file;
     file.setFileName(":/jquery.min.js");
     file.open(QIODevice::ReadOnly);
     jQuery = file.readAll();
     file.close();
     
     QNetworkProxyFactory::setUseSystemConfiguration(true);

     view = new QWebView(this);
     view->load(url);
     connect(view, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
     connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
     connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
     connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

     locationEdit = new QLineEdit(this);
     locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
     connect(locationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));

     QMenu *effectMenu = menuBar()->addMenu(tr("&Gaze Actions"));
     effectMenu->addAction("Calibration", this, SLOT(highlightAllLinks()));
     effectMenu->addAction("Scroll Up", this, SLOT(scrollUp()));
     effectMenu->addAction("Scroll Down", this, SLOT(scrollDown()));
     effectMenu->addAction("Find Links", this, SLOT(highlightAllLinks()));
     //effectMenu->addAction("Scroll Up", this, SLOT(highlightAllLinks()));

     rotateAction = new QAction(this);
     rotateAction->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
     rotateAction->setCheckable(true);
     rotateAction->setText(tr("Turn images upside down"));
     connect(rotateAction, SIGNAL(toggled(bool)), this, SLOT(rotateImages(bool)));
     effectMenu->addAction(rotateAction);

     /*QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));
     toolsMenu->addAction(tr("Remove GIF images"), this, SLOT(removeGifImages()));
     toolsMenu->addAction(tr("Remove all inline frames"), this, SLOT(removeInlineFrames()));
     toolsMenu->addAction(tr("Remove all object elements"), this, SLOT(removeObjectElements()));
     toolsMenu->addAction(tr("Remove all embedded elements"), this, SLOT(removeEmbeddedElements()));
    */
     setCentralWidget(view);
     setUnifiedTitleAndToolBarOnMac(true);
 }

 void MainWindow::adjustLocation()
 {
     locationEdit->setText(view->url().toString());
 }

 void MainWindow::changeLocation()
 {
     QUrl url = QUrl(locationEdit->text());
     view->load(url);
     view->setFocus();
 }

 void MainWindow::adjustTitle()
 {
     if (progress <= 0 || progress >= 100)
         setWindowTitle(view->title());
     else
         setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
 }

 void MainWindow::setProgress(int p)
 {
     progress = p;
     adjustTitle();
 }

 void MainWindow::finishLoading(bool)
 {
     progress = 100;
     adjustTitle();
     view->page()->mainFrame()->evaluateJavaScript(jQuery);

     rotateImages(rotateAction->isChecked());
 }

 void MainWindow::highlightAllLinks()
 {
     QString code = "$('a').each( function () { $(this).css('background-color', 'yellow') } )";
     view->page()->mainFrame()->evaluateJavaScript(code);
 }

 void MainWindow::scrollUp()
 {
     QString code = "$('html, body').animate({ scrollTop: 0 }, 'slow');";
     view->page()->mainFrame()->evaluateJavaScript(code);
 }
 
 void MainWindow::scrollDown()
 {
     QString code = "$('html, body').animate({ scrollTop: $(document).height() }, 'slow');";
     view->page()->mainFrame()->evaluateJavaScript(code);
 }
 
 void MainWindow::rotateImages(bool invert)
 {
     QString code;
     if (invert)
         code = "$('img').each( function () { $(this).css('-webkit-transition', '-webkit-transform 2s'); $(this).css('-webkit-transform', 'rotate(180deg)') } )";
     else
         code = "$('img').each( function () { $(this).css('-webkit-transition', '-webkit-transform 2s'); $(this).css('-webkit-transform', 'rotate(0deg)') } )";
     view->page()->mainFrame()->evaluateJavaScript(code);
 }

 void MainWindow::removeGifImages()
 {
     QString code = "$('[src*=gif]').remove()";
     view->page()->mainFrame()->evaluateJavaScript(code);
 }

 void MainWindow::removeInlineFrames()
 {
     QString code = "$('iframe').remove()";
     view->page()->mainFrame()->evaluateJavaScript(code);
 }

 void MainWindow::removeObjectElements()
 {
     QString code = "$('object').remove()";
     view->page()->mainFrame()->evaluateJavaScript(code);
 }

 void MainWindow::removeEmbeddedElements()
 {
     QString code = "$('embed').remove()";
     view->page()->mainFrame()->evaluateJavaScript(code);
 }