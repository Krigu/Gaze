/* 
 * File:   ImageLinkLabel.h
 * Author: krigu
 *
 * Created on January 2, 2013, 7:28 PM
 */

#ifndef IMAGELINKLABEL_H
#define	IMAGELINKLABEL_H

#include <QtGui>

struct Link {
    QImage image;
    QString href;
};

class ImageLinkLabel : public QLabel {
    Q_OBJECT
public:
    ImageLinkLabel(Link link);
    virtual ~ImageLinkLabel();

signals:
    void clicked(QString href);

protected:
    void mousePressEvent(QMouseEvent * event);

private:
    Link link;

};

#endif	/* IMAGELINKLABEL_H */

