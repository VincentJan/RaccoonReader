#include "include/pdfarea/pdfview/pdfviewstate.h"
#include "include/pdfarea/pdfview/pdfzoomstate.h"
#include "include/pdfarea/pdfview/pdfmarkstate.h"

#include <QScrollBar>

PdfViewState::PdfViewState(PdfView *pdfView)
    : PdfState(pdfView)
{
    pdfView_->setDragMode(PdfView::ScrollHandDrag);
}

PdfViewState::~PdfViewState()
{
    pdfView_->setDragMode(PdfView::NoDrag);
}

void PdfViewState::keyPressEvent(QKeyEvent *e)
{
    if(pdfView_->document() == nullptr) {
        return;
    }
    auto key = e->key();
    if (key == Qt::Key_J || key == Qt::Key_Down) {
        moveDown(50);
    } else if (key == Qt::Key_K || key == Qt::Key_Up) {
        moveUp(50);
    } else if (key == Qt::Key_H || key == Qt::Key_Left) {
        moveLeft(50);
    } if (key == Qt::Key_L || key == Qt::Key_Right) {
        moveRight(50);
    } else if (key == Qt::Key_Control) {
        next_ = new PdfScaleState(pdfView_);
    } else if (key == Qt::Key_Shift) {
        next_ = new PdfMarkState(pdfView_);
    }
}

void PdfViewState::wheelEvent(QWheelEvent *e)
{
    if(pdfView_->document() == nullptr) {
        return;
    }
    if(e->angleDelta().y() >= 0) {
        moveUp(e->angleDelta().y() * 0.5);
    } else {
        moveDown(e->angleDelta().y() * -0.5);
    }
    if(e->angleDelta().x() >= 0) {
        moveLeft(e->angleDelta().x() * 0.5);
    } else {
        moveRight((e->angleDelta().x() * -0.5));
    }
}

void PdfViewState::moveUp(int n) {
    if(n <= 0) return;
    auto vBar = pdfView_->verticalScrollBar();
    auto pageNumber = pdfView_->pageNum();
    if(vBar->value() == 0 && pageNumber != 1) {
        pdfView_->setPageNum(pageNumber - 1);
        vBar->setValue(vBar->maximum());
        return;
    }
    vBar->setValue(vBar->value() - n);
}

void PdfViewState::moveDown(int n) {
    if(n <= 0) return;
    auto vBar = pdfView_->verticalScrollBar();
    auto pageNumber = pdfView_->pageNum();
    if(vBar->value() == vBar->maximum()
            && pageNumber != pdfView_->pageCount()) {
        pdfView_->setPageNum(pageNumber + 1);
        vBar->setValue(0);
        return;
    }
    vBar->setValue(vBar->value() + n);
}

void PdfViewState::moveLeft(int n) {
    if(n <= 0) return;
    auto hBar = pdfView_->horizontalScrollBar();
    auto pageNumber = pdfView_->pageNum();
    if(hBar->value() == 0
            && pageNumber != 1) {
        pdfView_->setPageNum(pageNumber - 1);
        hBar->setValue(hBar->maximum());
        return;
    }
    hBar->setValue(hBar->value() - n);
}

void PdfViewState::moveRight(int n) {
    if(n <= 0) return;
    auto hBar = pdfView_->horizontalScrollBar();
    auto pageNumber = pdfView_->pageNum();
    if(hBar->value() == hBar->maximum()
            && pageNumber != pdfView_->pageCount()) {
        pdfView_->setPageNum(pageNumber + 1);
        hBar->setValue(0);
        return;
    }
    hBar->setValue(hBar->value() + n);
}
