#include "pagecontroller.h"
#include <QDebug>

PageController::PageController(const QSize& size, const int pageCount) : pageCount_(pageCount)
  , lastPage_(0) {
    layout_ = new QSplitter(Qt::Horizontal);
    layout_->setFixedHeight(size.height() * 0.03);

    slider_ = new PageSlider(layout_);
    slider_->setFixedHeight(size.height() * 0.03);
    layout_->addWidget(slider_);

    spinBox_ = new PageSpinBox(layout_);
    spinBox_->setFixedHeight(size.height() * 0.03);
    spinBox_->setFixedWidth(size.width() * 0.05);
    layout_->addWidget(spinBox_);

    connect(slider_, &PageSlider::valueChanged, spinBox_, &QSpinBox::setValue);
    connect(slider_, &PageSlider::SlideFinished, this, &PageController::EmitChange);
    connect(spinBox_, &QSpinBox::editingFinished, this, &PageController::EmitChange);

    pageCountLabel_ = new QLabel(layout_);
    pageCountLabel_->setFixedWidth(size.width() * 0.05);
    pageCountLabel_->setFixedHeight(size.height() * 0.03);
    pageCountLabel_->setAlignment(Qt::AlignHCenter);
    layout_->addWidget(pageCountLabel_);

    SetPageCount(pageCount);
}

PageController::~PageController() {
    delete layout_;
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

void PageController::EmitChange() {
    int n = spinBox_->value();
    if(n == lastPage_) {
        return;
    }
    lastPage_ = n;
    slider_->setValue(n);
    emit pageChanged(n);
}

void PageController::SetPageNum(int n) {
    spinBox_->setValue(n);
    emit spinBox_->editingFinished();
}
