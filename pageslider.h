#ifndef PAGESLIDER_H
#define PAGESLIDER_H

#include <QSlider>

class PageSlider : public QSlider {
    Q_OBJECT
public:
    PageSlider(QWidget* parent = nullptr);
    ~PageSlider() = default;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
private:
    int step_ = 1;
signals:
    void SlideFinished();
};

#endif // PAGESLIDER_H
