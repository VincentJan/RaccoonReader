/*
**  This file is part of Raccoon Reader.
**
** 	mainwindow.h: Declaration of MainWindow class.
**
**  Copyright 2021 Yang XiLong
*/

#include "include/pdfarea/pagecontroller/pageslider.h"
#include <QMouseEvent>

PageSlider::PageSlider(QWidget* parent) : step_(1) {
    setOrientation(Qt::Horizontal);
    setMinimum(1);
    setSingleStep(1);
    setPageStep(1);
    setParent(parent);
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
        setValue(value() - step_);
    } else {
        setValue(value() + step_);
    }
    emit SlideFinished();
}
