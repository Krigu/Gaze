#ifndef IMAGELINKLABEL_H
#define	IMAGELINKLABEL_H

#include <QtGui>

struct Link {
    QImage image;
    QString href;
};

/**
 * the ImageLinkLabel displays a link and the region around it in an image
 */
class ImageLinkLabel : public QLabel {
    Q_OBJECT
public:
    ImageLinkLabel();
    virtual ~ImageLinkLabel();
    void setLink(Link link);

signals:
    void clicked(QString href);

protected:
    void mousePressEvent(QMouseEvent * event);

private:
    Link link;

};

#endif	/* IMAGELINKLABEL_H */

