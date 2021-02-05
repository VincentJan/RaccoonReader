#include "include/pdfarea/pdfview/pdfzoomstate.h"
#include "include/pdfarea/pdfview/pdfviewstate.h"

#include <QApplication>
#include <QScrollBar>

PdfScaleState::PdfScaleState(PdfView *pdfView)
    : PdfState(pdfView)
{
    QApplication::setOverrideCursor(Qt::SizeVerCursor);
}

PdfScaleState::~PdfScaleState()
{
    QApplication::restoreOverrideCursor();
}

void PdfScaleState::keyPressEvent(QKeyEvent *e)
{
    if(pdfView_->document() == nullptr) {
        return;
    }
    auto zoomLevel = pdfView_->zoomLevel();
    auto key = e->key();
    if(key == Qt::Key_0) {
        pdfView_->clearFitMode();
        pdfView_->setZoomLevel(1);
    } else if(key == Qt::Key_Plus) {
        pdfView_->clearFitMode();
        pdfView_->setZoomLevel(zoomLevel + 0.15);
    } else if(key == Qt::Key_Minus) {
        pdfView_->clearFitMode();
        pdfView_->setZoomLevel(zoomLevel - 0.15);
    }
}

void PdfScaleState::keyReleaseEvent(QKeyEvent *e)
{
    if(pdfView_->document() == nullptr) {
        return;
    }
    auto key = e->key();
    if(key == Qt::Key_Control) {
        next_ = new PdfViewState(pdfView_);
    }
}

void PdfScaleState::wheelEvent(QWheelEvent *e)
{
    if(pdfView_->document() == nullptr) {
        return;
    }
    auto zoomLevel = pdfView_->zoomLevel();
    auto delta = e->angleDelta().y();
    if(delta > 0) {
        pdfView_->clearFitMode();
        pdfView_->setZoomLevel(zoomLevel + 0.15);
    } else {
        pdfView_->clearFitMode();
        pdfView_->setZoomLevel(zoomLevel - 0.15);
    }
}

void PdfScaleState::focusOutEvent(QFocusEvent *)
{
    if(pdfView_->document() == nullptr) {
        return;
    }
    next_ = new PdfViewState(pdfView_);
}
