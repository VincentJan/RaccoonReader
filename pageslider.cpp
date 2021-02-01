#include "pageslider.h"
#include <QMouseEvent>

PageSlider::PageSlider() : step_(1) {
    this->setOrientation(Qt::Horizontal);
    this->setMinimum(1);
    this->setSingleStep(1);
    this->setPageStep(1);
}

void PageSlider::mousePressEvent(QMouseEvent *e) {
    QSlider::mousePressEvent(e);
    auto pos = e->pos().x() / static_cast<double>(width());
    auto val = pos * (maximum() - minimum()) + minimum();
    setValue(val);
}

void PageSlider::wheelEvent(QWheelEvent *e) {
    if (e->angleDelta().y() > 0) {
        this->setValue(this->value() - step_);
    } else {
        this->setValue(this->value() + step_);
    }
}

void PageSlider::keyPressEvent(QKeyEvent *e) {
    if(e->key() == Qt::Key_Shift) {
        step_ = 10;
    }
}

void PageSlider::keyReleaseEvent(QKeyEvent *e) {
    if(e->key() == Qt::Key_Shift) {
        step_ = 1;
    }
}
