#include "pdfarea/pdfview/highlightarea.h"

#include <QPainter>
#include <QFocusEvent>
#include <QDebug>

HighlightArea::HighlightArea(QRectF rect)
    : rect_(rect), brush_()
    , pen_(), shape_(RECT), hasImage_(false)
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    brush_.setColor(QColor(255,180,0,50));
    brush_.setStyle(Qt::SolidPattern);
    pen_.setColor(QColor(255,180,50,100));
    pen_.setWidth(5);
}

void HighlightArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(pen_);
    painter->setBrush(brush_);
    if(shape_ == RECT) {
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect_);
    } else if (shape_ == LINE) {
        auto pos1 = QPointF(rect_.x(), rect_.y());
        auto pos2 = QPointF(rect_.x() + rect_.width(), rect_.y() + rect_.height());
        painter->drawLine(pos1, pos2);
    }else if(shape_ == Highlight) {
        painter->drawImage(rect_, img_);
    }
}

QRectF HighlightArea::boundingRect() const
{
    return rect_;
}

void HighlightArea::focusInEvent(QFocusEvent *e)
{
    e->ignore();
    QGraphicsItem::focusInEvent(e);
}

void HighlightArea::fixBackground(QImage background)
{
    auto color = QColor(255,180,0,100);
    img_ = background;
    for(int x = 0; x < img_.width(); ++x) {
        for(int y = 0; y < img_.height(); ++y) {
            auto curColor = img_.pixelColor(x, y);
            int average = (curColor.red() + curColor.green() + curColor.blue()) / 3;
            if(average > 150) {
                img_.setPixelColor(x, y, color);
            }
        }
    }
    hasImage_ = true;
}
