#ifndef PAGESLIDER_H
#define PAGESLIDER_H

#include <QSlider>

class PageSlider : public QSlider
{
public:
    PageSlider();
    void mousePressEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
private:
    int step_;
};

#endif // PAGESLIDER_H
