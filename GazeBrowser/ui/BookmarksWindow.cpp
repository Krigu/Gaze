#include <QtGui>

#include "BookmarksWindow.hpp"
#include <iostream>

using namespace std;

BookmarksWindow::BookmarksWindow(QSettings * settings) : settings(settings) {
    settings->beginGroup("BOOKMARKS");

    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *widget = new QWidget();
    for (int i = 0; i < 9; i++) {
        layout->addWidget(createLinkGroup(i));
    }
    widget->setLayout(layout);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(1);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(widget);
    scrollArea->setWidgetResizable(true);
    scrollArea->resize(500, 400);

    // Pack everything in a BoxLayout
    QVBoxLayout *content = new QVBoxLayout;
    content->addWidget(scrollArea);
    content->addWidget(createButtonGroup());    
    setLayout(content);

    settings->endGroup();
}

QGroupBox * BookmarksWindow::createLinkGroup(int index) {

    QGroupBox *linkGroup = new QGroupBox(tr("Link"));
    QLabel *lCaption = new QLabel(tr("Caption"));
    QLineEdit *edCaption = new QLineEdit();
    edCaption->setText(settings->value("DESC_" + QString::number(index)).toString());

    QLabel *lUrl = new QLabel(tr("URL"));
    QLineEdit *edUrl = new QLineEdit();
    edUrl->setText(settings->value("URL_" + QString::number(index)).toString());

    QGridLayout *linkLayout = new QGridLayout;
    linkLayout->addWidget(lCaption, 0, 0);
    linkLayout->addWidget(edCaption, 0, 1);
    linkLayout->addWidget(lUrl, 1, 0);
    linkLayout->addWidget(edUrl, 1, 1);
    linkGroup->setLayout(linkLayout);

    return linkGroup;

}

QDialogButtonBox * BookmarksWindow::createButtonGroup() {

    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
            | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(save()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(cancel()));

    return buttonBox;
}

void BookmarksWindow::save() {
    QList<QLineEdit *> list = this->findChildren<QLineEdit *> ();

    settings->beginGroup("BOOKMARKS");
    QList<QLineEdit* >::iterator i;
    int j = 0;
    for (i = list.begin(); i != list.end(); ++i, j++) {
        settings->setValue("DESC_" + QString::number(j), (*i)->text());
        ++i;
        settings->setValue("URL_" + QString::number(j), (*i)->text());
    }
    settings->endGroup();


    this->close();
}

void BookmarksWindow::cancel() {
    this->close();
}

BookmarksWindow::~BookmarksWindow() {
}

