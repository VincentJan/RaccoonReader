#ifndef HIGHLIGHTAREA_H
#define HIGHLIGHTAREA_H

#include <QGraphicsObject>

class HighlightArea : public QGraphicsObject
{
    Q_OBJECT
public:
    enum Shape {
        LINE, RECT
    };
    HighlightArea();
    void paint(QPainter *painter);
private:
    QPointF startPot_;
    QPointF endPot_;
    QBrush *brush_;
    QPen   *pen_;
    Shape  shape_;
signals:
    void killMe();
    void clicked();
};

#endif // HIGHLIGHTAREA_H
