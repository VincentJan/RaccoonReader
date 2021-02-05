#include "include/pdfarea/pdfview/pdfmarkstate.h"
#include "include/pdfarea/pdfview/pdfviewstate.h"

#include <QApplication>
#include <QGraphicsRectItem>
#include <QScrollBar>

PdfMarkState::PdfMarkState(PdfView *pdfView)
    : PdfState(pdfView)
{
    QApplication::setOverrideCursor(Qt::CrossCursor);
    selectArea = new QGraphicsRectItem();
    selectArea->setPen(QPen(QColor(150, 150, 200, 100)));
    selectArea->setBrush(QBrush(QColor(150, 150, 200,100)));
    pdfView_->scene()->addItem(selectArea);
}

PdfMarkState::~PdfMarkState()
{
    QApplication::restoreOverrideCursor();
    selectArea->setRect(0, 0, 0, 0);
}

void PdfMarkState::keyPressEvent(QKeyEvent *e)
{
    if(pdfView_->document() == nullptr) {
        return;
    }
    auto key = e->key();
    auto vBar = pdfView_->verticalScrollBar();
    auto hBar = pdfView_->horizontalScrollBar();
    if(key == Qt::Key_Shift) {
        next_ = new PdfViewState(pdfView_);
    } else if (key == Qt::Key_Escape) {
        next_ = new PdfViewState(pdfView_);
    } else if (key == Qt::Key_J || key == Qt::Key_Down) {
        vBar->setValue(vBar->value() + 50);
    } else if (key == Qt::Key_K || key == Qt::Key_Up) {
        vBar->setValue(vBar->value() - 50);
    } else if (key == Qt::Key_H || key == Qt::Key_Left) {
        hBar->setValue(hBar->value() - 50);
    } else if (key == Qt::Key_L || key == Qt::Key_Right) {
        hBar->setValue(hBar->value() + 50);
    }
}

void PdfMarkState::keyReleaseEvent(QKeyEvent *) {}

void PdfMarkState::mousePressEvent(QMouseEvent *e)
{
    startPot_ = e->pos();
}

void PdfMarkState::mouseMoveEvent(QMouseEvent *e)
{
    if(pdfView_->document() == nullptr) {
        return;
    }
    endPot_ = e->pos();
    auto sceneStartPot = pdfView_->mapToScene(startPot_);
    auto sceneEndPot = pdfView_->mapToScene(endPot_);
    auto sPot = selectArea->mapFromScene(sceneStartPot);
    auto ePot = selectArea->mapFromScene(sceneEndPot);
    if(sPot.x() > ePot.x()) {
        auto tmp = sPot.x();
        sPot.setX(ePot.x());
        ePot.setX(tmp);
    }
    if(sPot.y() > ePot.y()) {
        auto tmp = sPot.y();
        sPot.setY(ePot.y());
        ePot.setY(tmp);
    }
    selectArea->setRect(QRectF(sPot, ePot));
}

void PdfMarkState::mouseReleaseEvent(QMouseEvent *)
{
    selectArea->setRect(0, 0, 0, 0);
}

void PdfMarkState::focusOutEvent(QFocusEvent *)
{
    next_ = new PdfViewState(pdfView_);
}
