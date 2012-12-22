/* 
 * File:   BookmarksWindow.cpp
 * Author: krigu
 * 
 * Created on December 22, 2012, 3:54 PM
 */

#include <QtGui>

#include "BookmarksWindow.hpp"
#include <iostream>

using namespace std;

BookmarksWindow::BookmarksWindow(QSettings * settings) : settings(settings) {
    settings->beginGroup("BOOKMARKS");

    QVBoxLayout *layout = new QVBoxLayout;
    for (int i = 0; i < 9; i++) {
        layout->addWidget(createLinkGroup(i));
    }

    layout->addWidget(createButtonGroup());
    setLayout(layout);

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

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    return buttonBox;
}

void BookmarksWindow::accept() {
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

void BookmarksWindow::reject() {
    this->close();
}

BookmarksWindow::~BookmarksWindow() {
}

