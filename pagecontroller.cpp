#include "pagecontroller.h"
#include <QDebug>
#include <QMap>

PageController::PageController(const QSize& size, const int pageCount) : pageCount_(pageCount)
  , lastPage_(0) {
    widget_ = new QSplitter(Qt::Horizontal);
    widget_->setFixedHeight(size.height() * 0.03);

    comboBox_ = new QComboBox(widget_);
    QStringList items;
    items << "Fit Width" << "Fit Height" << "Fit Page" << "25%" << "50%"
        << "75%" << "100%" << "125%" << "150%" << "175%" << "200%" << "300%" << "400%" << "800%";
    comboBox_->addItems(items);
    comboBox_->setCurrentIndex(6);
    comboBox_->setFixedHeight(size.height() * 0.03);
    comboBox_->setFixedWidth(size.width() * 0.1);
    connect(comboBox_, &QComboBox::textActivated, this, &PageController::EmitScale);

    slider_ = new PageSlider(widget_);
    slider_->setFixedHeight(size.height() * 0.03);
    widget_->addWidget(slider_);

    spinBox_ = new PageSpinBox(widget_);
    spinBox_->setFixedHeight(size.height() * 0.03);
    spinBox_->setFixedWidth(size.width() * 0.05);
    widget_->addWidget(spinBox_);

    connect(slider_, &PageSlider::valueChanged, spinBox_, &QSpinBox::setValue);
    connect(slider_, &PageSlider::SlideFinished, this, &PageController::EmitChange);
    connect(spinBox_, &QSpinBox::editingFinished, this, &PageController::EmitChange);

    pageCountLabel_ = new QLabel(widget_);
    pageCountLabel_->setFixedWidth(size.width() * 0.05);
    pageCountLabel_->setFixedHeight(size.height() * 0.03);
    pageCountLabel_->setAlignment(Qt::AlignHCenter);
    widget_->addWidget(pageCountLabel_);

    SetPageCount(pageCount);
}

PageController::~PageController() {
    delete widget_;
}

QWidget* PageController::GetWidget() {
    return widget_;
}

void PageController::SetPageCount(int n) {
    if(n <= 0) return;
    pageCount_ = n;
    slider_->setMaximum(pageCount_);
    spinBox_->setRange(1, pageCount_);
    pageCountLabel_->setText("/ " + QString::number(pageCount_));
}

void PageController::SetPageNum(int n) {
    spinBox_->setValue(n);
    emit spinBox_->editingFinished();
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

void PageController::EmitScale(const QString& scale) {
    emit scaleChanged(scale);
}
