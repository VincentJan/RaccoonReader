#include "pagecontroller.h"
#include <QDebug>

PageController::PageController(const QSize& size, const int pageCount)
    : pageCount_(pageCount), lastPage_(0)
{
    setOrientation(Qt::Horizontal);
    setFixedHeight(size.height() * 0.03);

    scaleBox_ = new ScaleBox(this);
    scaleBox_->setFixedHeight(size.height() * 0.03);
    scaleBox_->setFixedWidth(size.width() * 0.1);
    connect(scaleBox_, &ScaleBox::scaleChanged, this
            , [this](double s){emit scaleChanged(s);});
    connect(scaleBox_, &ScaleBox::textActivated, this
            , [this](){emit scaleSelected();});

    slider_ = new PageSlider(this);
    slider_->setFixedHeight(size.height() * 0.03);
    addWidget(slider_);

    spinBox_ = new PageSpinBox(this);
    spinBox_->setFixedHeight(size.height() * 0.03);
    spinBox_->setFixedWidth(size.width() * 0.05);
    addWidget(spinBox_);

    connect(slider_, &PageSlider::valueChanged, spinBox_, &QSpinBox::setValue);
    connect(slider_, &PageSlider::SlideFinished, this, &PageController::emitChange);
    connect(spinBox_, &QSpinBox::editingFinished, this, &PageController::emitChange);

    pageCountLabel_ = new QLabel(this);
    pageCountLabel_->setFixedWidth(size.width() * 0.05);
    pageCountLabel_->setFixedHeight(size.height() * 0.03);
    pageCountLabel_->setAlignment(Qt::AlignHCenter);
    addWidget(pageCountLabel_);

    setPageCount(pageCount);
}

void PageController::setPageCount(int n)
{
    if(n <= 0) return;
    pageCount_ = n;
    slider_->setMaximum(pageCount_);
    spinBox_->setRange(1, pageCount_);
    pageCountLabel_->setText("/ " + QString::number(pageCount_));
}

void PageController::setPageNum(int n)
{
    spinBox_->setValue(n);
    emit spinBox_->editingFinished();
}

void PageController::setFromDocument(Document *doc)
{
    int pageCount = (doc == nullptr) ? 1 : doc->numPages();
    setPageCount(pageCount);
    setPageNum(1);
    setScale(1);
}

void PageController::setScale(double scale)
{
    scaleBox_->setScale(scale);
}

void PageController::emitChange()
{
    int n = spinBox_->value();
    if(n == lastPage_) {
        return;
    }
    lastPage_ = n;
    slider_->setValue(n);
    emit pageChanged(n);
}
