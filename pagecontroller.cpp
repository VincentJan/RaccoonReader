#include "pagecontroller.h"
#include <QDebug>

PageController::PageController(const QSize& size, const int pageCount) : pageCount_(pageCount) {
    layout_ = new QSplitter(Qt::Horizontal);
    layout_->setFixedHeight(size.height() * 0.03);
    layout_->setFixedWidth(size.width());

    slider_ = new PageSlider();
    slider_->setFixedHeight(size.height() * 0.03);
    layout_->addWidget(slider_);

    spinBox_ = new QSpinBox();
    spinBox_->setAlignment(Qt::AlignCenter);
    spinBox_->setFixedHeight(size.height() * 0.03);
    spinBox_->setFixedWidth(size.width() * 0.05);
    spinBox_->setValue(1);
    layout_->addWidget(spinBox_);

    connect(slider_, &PageSlider::valueChanged, spinBox_, &QSpinBox::setValue);
    void (QSpinBox::*intValueChange)(int) = &QSpinBox::valueChanged;
    connect(spinBox_, intValueChange, slider_, &QSlider::setValue);
    connect(spinBox_, intValueChange, this, &PageController::EmitChange);

    pageCountLabel_ = new QLabel();
    pageCountLabel_->setFixedWidth(size.width() * 0.05);
    pageCountLabel_->setFixedHeight(size.height() * 0.03);
    pageCountLabel_->setAlignment(Qt::AlignHCenter);
    layout_->addWidget(pageCountLabel_);

    SetPageCount(pageCount);
}

PageController::~PageController() {
    delete layout_;
    delete slider_;
    delete spinBox_;
    delete pageCountLabel_;
}

QWidget* PageController::GetLayout() {
    return layout_;
}

void PageController::SetPageCount(int n) {
    if(n <= 0) return;
    pageCount_ = n;
    slider_->setMaximum(pageCount_);
    spinBox_->setRange(1, pageCount_);
    pageCountLabel_->setText("/ " + QString::number(pageCount_));
}

void PageController::EmitChange(int n) {
    emit pageChanged(n);
}
