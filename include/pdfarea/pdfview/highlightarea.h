#ifndef HIGHLIGHTAREA_H
#define HIGHLIGHTAREA_H

#include <QGraphicsObject>
#include <QPen>
#include <QBrush>

class HighlightArea : public QGraphicsObject
{
    Q_OBJECT
public:
    enum Shape {
        LINE, RECT, Highlight
    };
    HighlightArea(QRectF rect);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option
               , QWidget *widget) override;
    QRectF boundingRect() const override;
    void fixBackground(QImage background);
protected:
    void focusInEvent(QFocusEvent *e) override;
private:
    QRectF rect_;
    QBrush brush_;
    QPen   pen_;
    Shape  shape_;
    QImage img_;
    bool hasImage_;
signals:
    void killMe();
    void clicked();
};

#endif // HIGHLIGHTAREA_H
