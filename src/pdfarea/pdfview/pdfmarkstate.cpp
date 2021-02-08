#include "pdfarea/pdfview/pdfmarkstate.h"
#include "pdfarea/pdfview/pdfviewstate.h"
#include "pdfarea/pdfview/highlightarea.h"

#include <QApplication>
#include <QGraphicsRectItem>
#include <QScrollBar>

#include <poppler/qt5/poppler-qt5.h>

using Poppler::HighlightAnnotation;

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
    sPot.setX((sPot.x() < 0) ? 0 : sPot.x());
    sPot.setY((sPot.y() < 0) ? 0 : sPot.y());
    auto maxX = pdfView_->sceneRect().x() + pdfView_->sceneRect().width();
    auto maxY = pdfView_->sceneRect().y() + pdfView_->sceneRect().height();
    ePot.setX((ePot.x() > maxX) ? maxX : ePot.x());
    ePot.setY((ePot.y() > maxY) ? maxY : ePot.y());
    selectArea->setRect(QRectF(sPot, ePot));
}

void PdfMarkState::mouseReleaseEvent(QMouseEvent *)
{
    auto rect = selectArea->rect();
    selectArea->setRect(0, 0, 0, 0);
    auto img = new QImage(rect.width(), rect.height()
                          , QImage::Format_RGBA64);
    auto painter = new QPainter(img);
    pdfView_->scene()->render(painter, QRectF(), rect);
    auto mark = new HighlightArea(rect);
    mark->fixBackground(*img);
    pdfView_->scene()->addItem(mark);
}

void PdfMarkState::focusOutEvent(QFocusEvent *)
{
    next_ = new PdfViewState(pdfView_);
}
