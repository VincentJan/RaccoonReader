#include "pdfview.h"
#include <QDebug>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QPdfPageNavigation>
#include <QApplication>
#include <QScreen>

PdfView::PdfView() : QGraphicsView(), doc_()
  , curScene_(new QGraphicsScene){
    this->setScene(curScene_);
    this->setStyleSheet("background-color:gray");
}

PdfView::PdfView(const QString& path) : PdfView() {
    doc_ = Document::load(path);
    doc_->setPaperColor(QColor("white"));
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
    if(doc_ == nullptr) return;
    pageNum_ = n;
    emit PageChanged(n);
    int res = QApplication::primaryScreen()->logicalDotsPerInch();
    auto page = doc_->page(pageNum_ - 1);
    QImage pageImg = page->renderToImage(res, res);
    curScene_->clear();
    curScene_->addPixmap(QPixmap::fromImage(pageImg));
}

void PdfView::SetDocument(Document* doc) {
    doc_ = doc;
}

Document* PdfView::GetDocument() const {
    return doc_;
}

int PdfView::GetPageCount() const {
    if(doc_ == nullptr) return 0;
    return doc_->numPages();
}

void PdfView::wheelEvent(QWheelEvent *e) {
    if(e->angleDelta().y() >= 0) {
        MoveUp(e->angleDelta().y());
    } else {
        MoveDown(e->angleDelta().y() * -1);
    }
    if(e->angleDelta().x() >= 0) {
        MoveLeft(e->angleDelta().x());
    } else {
        MoveRight((e->angleDelta().x() * -1));
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
        scale += 0.05;
        SetPageNum(pageNum_);
    }
    if(e->key() == Qt::Key_E) {
        scale -= 0.05;
        SetPageNum(pageNum_);
    }
}

void PdfView::keyReleaseEvent(QKeyEvent *e) {
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
