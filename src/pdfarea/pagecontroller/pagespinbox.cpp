/*
**  This file is part of Raccoon Reader.
**
** 	mainwindow.h: Declaration of MainWindow class.
**
**  Copyright 2021 Yang XiLong
*/

#include "pdfarea/pagecontroller/pagespinbox.h"

PageSpinBox::PageSpinBox(QWidget* parent) {
    setParent(parent);
    setAlignment(Qt::AlignCenter);
    setValue(1);
}

void PageSpinBox::mouseReleaseEvent(QMouseEvent* e) {
    QSpinBox::mouseReleaseEvent(e);
    emit QSpinBox::editingFinished();
}

void PageSpinBox::wheelEvent(QWheelEvent *e) {
    QSpinBox::wheelEvent(e);
    emit QSpinBox::editingFinished();
}
