/*
**  This file is part of Raccoon Reader.
**
** 	pagecontroller.cpp: Definition of PageController class.
**
**  Copyright 2021 Yang XiLong
*/

#include "pdfarea/pagecontroller/pagecontroller.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>

// Conturctor accept the size of parent widget use to make size of itself,
// and a page count of current document
PageController::PageController(const QSize& size, const int pageCount)
    : pageCount_(pageCount), lastPage_(0), fitMode_(PdfView::None)
{
    setFixedHeight(size.height() * 0.03);

    auto layout = new QHBoxLayout(this);
    setLayout(layout);
    layout->setContentsMargins(10, 0, 10, 10);

    scaleBox_ = new ScaleBox(this);
    scaleBox_->setFixedHeight(size.height() * 0.03);
    scaleBox_->setMaximumWidth(size.width() * 0.08);
    layout->addWidget(scaleBox_);

    connect(scaleBox_, &ScaleBox::scaleChanged, this
            , [this](double s){emit scaleChanged(s);});
    connect(scaleBox_, &ScaleBox::textActivated, this
            , [this](){emit scaleSelected();});

    initFitButtons();

    // only slider not set to fixed width, that means only slider can fit
    // width while the widget's size have change.
    slider_ = new PageSlider(this);
    slider_->setFixedHeight(size.height() * 0.03);
    layout->addWidget(slider_);

    spinBox_ = new PageSpinBox(this);
    spinBox_->setFixedHeight(size.height() * 0.03);
    spinBox_->setFixedWidth(size.width() * 0.05);
    layout->addWidget(spinBox_);

    // binding values in spinbox and slider.
    connect(slider_, &PageSlider::valueChanged, spinBox_, &QSpinBox::setValue);
    connect(slider_, &PageSlider::SlideFinished, this, &PageController::emitChange);
    connect(spinBox_, &QSpinBox::editingFinished, this, &PageController::emitChange);

    pageCountLabel_ = new QLabel(this);
    pageCountLabel_->setMaximumWidth(size.width() * 0.05);
    pageCountLabel_->setFixedHeight(size.height() * 0.03);
    pageCountLabel_->setAlignment(Qt::AlignCenter);
    layout->addWidget(pageCountLabel_);

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
    setFitMode(PdfView::None);
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

void PageController::initFitButtons()
{
    fitButtons_[PdfView::FitWidth] = new QPushButton(QIcon(":/images/fitWidth.svg"),"", this);
    fitButtons_[PdfView::FitWidth]->setToolTip("Fit Width");
    fitButtons_[PdfView::FitHeight] = new QPushButton(QIcon(":/images/fitHeight.svg"),"", this);
    fitButtons_[PdfView::FitHeight]->setToolTip("Fit Height");
    fitButtons_[PdfView::FitPage] = new QPushButton(QIcon(":/images/fitPage.svg"), "", this);
    fitButtons_[PdfView::FitPage]->setToolTip("Fit Page");
    for(auto i = fitButtons_.begin(); i != fitButtons_.end(); ++i) {
        auto button = *i;
        button->setFixedHeight(size().height());
        button->setFixedWidth(size().height());
        button->setCheckable(true);
        layout()->addWidget(button);
        auto key = i.key();
        connect(button, &QPushButton::toggled, this, [this, key](bool checked) {
            if(checked) {
                emit fitModeChanged(key);
            } else if(fitMode_ == key) {
                emit fitModeChanged(PdfView::None);
            }
        });
    }
}

void PageController::setFitMode(PdfView::FitMode fitMode)
{
    for (auto i = fitButtons_.begin(); i != fitButtons_.end(); ++i) {
        (*i)->setChecked(i.key() == fitMode);
    }
}
