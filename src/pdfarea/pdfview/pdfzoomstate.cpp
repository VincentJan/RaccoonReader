#include "pdfarea/pdfview/pdfzoomstate.h"
#include "pdfarea/pdfview/pdfviewstate.h"

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
    if(pdfView_->document() == nullptr) return;
    auto key = e->key();
    if(key == Qt::Key_0) {
        pdfView_->clearFitMode();
        pdfView_->setZoomLevel(1);
    } else if(key == Qt::Key_Equal) {
        zoomIn();
    } else if(key == Qt::Key_Minus) {
        zoomOut();
    }
}

void PdfScaleState::keyReleaseEvent(QKeyEvent *e)
{
    if(pdfView_->document() == nullptr) return;
    auto key = e->key();
    if(key == Qt::Key_Control) {
        next_ = new PdfViewState(pdfView_);
    }
}

void PdfScaleState::wheelEvent(QWheelEvent *e)
{
    if(pdfView_->document() == nullptr) return;
    auto delta = e->angleDelta().y();
    if(delta > 0) {
        zoomIn();
    } else {
        zoomOut();
    }
}

void PdfScaleState::focusOutEvent(QFocusEvent *)
{
    if(pdfView_->document() == nullptr) {
        return;
    }
    next_ = new PdfViewState(pdfView_);
}

void PdfScaleState::zoomIn()
{
    auto zoomLevel = pdfView_->zoomLevel();
    zoomLevel = (zoomLevel + 0.25 >= 2) ? 2 : zoomLevel + 0.25;
    pdfView_->clearFitMode();
    pdfView_->setZoomLevel(zoomLevel);
}

void PdfScaleState::zoomOut()
{
    auto zoomLevel = pdfView_->zoomLevel();
    zoomLevel = (zoomLevel - 0.25 <= 0.25) ? 0.25 : zoomLevel - 0.25;
    pdfView_->clearFitMode();
    pdfView_->setZoomLevel(zoomLevel);
}
