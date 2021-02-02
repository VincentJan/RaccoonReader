#include "pageslider.h"
#include <QMouseEvent>

PageSlider::PageSlider(QWidget* parent) : step_(1) {
    this->setOrientation(Qt::Horizontal);
    this->setMinimum(1);
    this->setSingleStep(1);
    this->setPageStep(1);
    this->setParent(parent);
}

void PageSlider::mousePressEvent(QMouseEvent *e) {
    QSlider::mousePressEvent(e);
    auto pos = e->pos().x() / static_cast<double>(width());
    auto val = pos * (maximum() - minimum()) + minimum();
    setValue(val);
}

void PageSlider::mouseReleaseEvent(QMouseEvent *e) {
    QSlider::mouseReleaseEvent(e);
    emit SlideFinished();
}

void PageSlider::wheelEvent(QWheelEvent *e) {
    if (e->angleDelta().y() > 0) {
        this->setValue(this->value() - step_);
    } else {
        this->setValue(this->value() + step_);
    }
    emit SlideFinished();
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
