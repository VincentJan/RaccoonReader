#include "pdfview.h"
#include <QDebug>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QPdfPageNavigation>
#include <QApplication>
#include <QScreen>
#include <QRect>

PdfView::PdfView() : QGraphicsView(), fitMode_(), doc_()
  , curScene_(new QGraphicsScene){
    this->setScene(curScene_);
    this->setStyleSheet("background-color:gray");
    this->verticalScrollBar()->setStyleSheet("background-color:lightgray");
    this->horizontalScrollBar()->setStyleSheet("background-color:lightgray");
    this->setDragMode(QGraphicsView::ScrollHandDrag);
}

PdfView::PdfView(const QString& path) : PdfView() {
    doc_ = Document::load(path);
    doc_->setPaperColor(QColor(248,248,248));
    doc_->setRenderHint(Poppler::Document::TextAntialiasing);
    SetPageNum(1);
}

PdfView::~PdfView() {
    delete doc_;
}

int PdfView::GetPageNum() const {
    return pageNum_;
}

void PdfView::SetPageNum(int n) {
    if(n <= 0) return;
    if(doc_ == nullptr) {
        curScene_->clear();
        return;
    }
    pageNum_ = n;
    emit PageChanged(n);
    double res = QApplication::primaryScreen()->logicalDotsPerInch() * scale_;
    auto page = doc_->page(pageNum_ - 1);
    QImage pageImg = page->renderToImage(res, res);
    curScene_->clear();
    curScene_->addPixmap(QPixmap::fromImage(pageImg));
    curScene_->setSceneRect(0, 0, pageImg.width(), pageImg.height());
}

void PdfView::SetDocument(Document* doc) {
    doc_ = doc;
    if(doc == nullptr) {
        return;
    }
    doc_->setPaperColor(QColor(248,248,248));
    doc_->setRenderHint(Poppler::Document::TextAntialiasing);
}

Document* PdfView::GetDocument() const {
    return doc_;
}

int PdfView::GetPageCount() const {
    if(doc_ == nullptr) return 0;
    return doc_->numPages();
}

void PdfView::SetScale(const QString &scale) {
    if(doc_ == nullptr || scale.size() == 0) {
        return;
    }
    double height = this->size().height() - 2;
    double width = this->size().width() - 2;
    double pageWidth = curScene_->sceneRect().width();
    double pageHeight = curScene_->sceneRect().height();
    if(scale == "Fit Width") {
        fitMode_ = "Fit Width";
        if(height / width < pageHeight / pageWidth) {
            width -= this->verticalScrollBar()->size().width();
        }
        scale_ = width / pageWidth * scale_;
    } else if(scale == "Fit Height") {
        fitMode_ = "Fit Height";
        if(width / height < pageWidth / pageHeight) {
            height -= this->horizontalScrollBar()->size().height();
        }
        scale_ = height / pageHeight * scale_;
    } else if(scale == "Fit Page"){
        if(height / width < pageHeight / pageWidth) {
            SetScale("Fit Height");
        } else {
            SetScale("Fit Width");
        }
        fitMode_ = "Fit Page";
    } else {
        fitMode_.clear();
        auto percent = scale.split("%");
        scale_ = percent.at(0).toDouble() / 100;
    }
    SetPageNum(pageNum_);
}

void PdfView::mousePressEvent(QMouseEvent* e) {
    QGraphicsView::mousePressEvent(e);
}

void PdfView::mouseMoveEvent(QMouseEvent* e) {
    QGraphicsView::mouseMoveEvent(e);
}

void PdfView::mouseReleaseEvent(QMouseEvent* e) {
    QGraphicsView::mouseReleaseEvent(e);
}

void PdfView::wheelEvent(QWheelEvent *e) {
    if(e->angleDelta().y() >= 0) {
        MoveUp(e->angleDelta().y() * 0.5);
    } else {
        MoveDown(e->angleDelta().y() * -0.5);
    }
    if(e->angleDelta().x() >= 0) {
        MoveLeft(e->angleDelta().x() * 0.5);
    } else {
        MoveRight((e->angleDelta().x() * -0.5));
    }
}

void PdfView::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_J || e->key() == Qt::DownArrow) {
        MoveDown(50);
    }
    if (e->key() == Qt::Key_K || e->key() == Qt::UpArrow) {
        MoveUp(50);
    }
    if (e->key() == Qt::Key_H || e->key() == Qt::LeftArrow) {
        MoveLeft(50);
    }
    if (e->key() == Qt::Key_L || e->key() == Qt::RightArrow) {
        MoveRight(50);
    }
    if(e->key() == Qt::Key_Q) {
    }
    if(e->key() == Qt::Key_E) {
    }
}

void PdfView::keyReleaseEvent(QKeyEvent *e) {
    QGraphicsView::keyReleaseEvent(e);
}

void PdfView::resizeEvent(QResizeEvent *e) {
    QGraphicsView::resizeEvent(e);
    if(fitMode_.size() != 0) {
        SetScale(fitMode_);
    }
}

void PdfView::MoveUp(int n) {
    if(n <= 0) return;
    auto vBar = this->verticalScrollBar();
    if(vBar->value() == 0 && pageNum_ != 1) {
        SetPageNum(pageNum_ - 1);
        vBar->setValue(vBar->maximum());
    }
    vBar->setValue(vBar->value() - n);
}

void PdfView::MoveDown(int n) {
    if(n <= 0) return;
    auto vBar = this->verticalScrollBar();
    if(vBar->value() == vBar->maximum() && pageNum_ != GetPageCount()) {
        SetPageNum(pageNum_ + 1);
        this->verticalScrollBar()->setValue(0);
    }
    vBar->setValue(vBar->value() + n);
}

void PdfView::MoveLeft(int n) {
    if(n <= 0) return;
    auto hBar = this->horizontalScrollBar();
    hBar->setValue(hBar->value() - n);
}

void PdfView::MoveRight(int n) {
    if(n <= 0) return;
    auto hBar = this->horizontalScrollBar();
    hBar->setValue(hBar->value() + n);
}
