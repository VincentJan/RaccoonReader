/*
**  This file is part of Raccoon Reader.
**
** 	mainwindow.h: Declaration of MainWindow class.
**
**  Copyright 2021 Yang XiLong
*/

#include "include/pdfarea/pdfview/pdfview.h"
#include "include/pdfarea/pdfview/pdfviewstate.h"

#include <QDebug>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QApplication>
#include <QScreen>
#include <QRect>
#include <QGraphicsRectItem>

PdfView::PdfView(QWidget *parent)
    : QGraphicsView(parent), fitMode_(), doc_()
{
    state_ = new PdfViewState(this);
    setScene(new QGraphicsScene);
}

PdfView::PdfView(const QString &path, QWidget *parent)
    : PdfView(parent)
{
    doc_ = Document::load(path);
    doc_->setPaperColor(QColor(248,248,248));
    doc_->setRenderHint(Poppler::Document::TextAntialiasing);
    setPageNum(1);
}

PdfView::~PdfView()
{
    delete doc_;
}

int PdfView::pageNum() const
{
    return pageNum_;
}

void PdfView::setPageNum(int n)
{
    if(n <= 0) return;
    if(doc_ == nullptr) {
        scene()->clear();
        return;
    }
    pageNum_ = n;
    emit pageChanged(n);
    double res = QApplication::primaryScreen()->logicalDotsPerInch() * zoomLevel_;
    auto page = doc_->page(pageNum_ - 1);
    QImage img = page->renderToImage(res, res);
    scene()->clear();
    scene()->addPixmap(QPixmap::fromImage(img));
    scene()->setSceneRect(0, 0, img.width(), img.height());
}

void PdfView::setDocument(Document* doc)
{
    doc_ = doc;
    if(doc_ == nullptr) {
        scene()->clear();
        scene()->setSceneRect(0,0,0,0);
        delete state_;
        state_ = new PdfViewState(this);
        return;
    }
    doc_->setPaperColor(QColor(248,248,248));
    doc_->setRenderHint(Poppler::Document::TextAntialiasing);
    setFitMode(None);
    setZoomLevel(1);
    setPageNum(1);
}

Document* PdfView::document() const
{
    return doc_;
}

int PdfView::pageCount() const
{
    if(doc_ == nullptr) return 0;
    return doc_->numPages();
}

double PdfView::zoomLevel() const
{
    return zoomLevel_;
}

void PdfView::setZoomLevel(double zoomLevel)
{
    auto originLevel = zoomLevel_;
    if(doc_ == nullptr) return;
    double height = size().height() - 2;
    double width = size().width() - 2;
    double pageWidth = scene()->sceneRect().width();
    double pageHeight = scene()->sceneRect().height();
    QMap<FitMode, function<void(double)>> scalePage;
    scalePage[None] = [this](double zoomLevel) {
        zoomLevel_ = zoomLevel;
    };
    scalePage[FitWidth] = [this, height, &width, pageHeight, pageWidth](double) {
        if(height / width < pageHeight / pageWidth) {
            width -= verticalScrollBar()->size().width();
        }
        zoomLevel_ = width / pageWidth * zoomLevel_;
    };
    scalePage[FitHeight] = [this, &height, width, pageHeight, pageWidth](double) {
        if(width / height < pageWidth / pageHeight) {
            height -= horizontalScrollBar()->size().height();
        }
        zoomLevel_ = height / pageHeight * zoomLevel_;
    };
    scalePage[FitPage] = [scalePage, height, width, pageHeight, pageWidth](double scale) {
        if(height / width < pageHeight / pageWidth) {
            scalePage[FitHeight](scale);
        } else {
            scalePage[FitWidth](scale);
        }
    };
    scalePage[fitMode_](zoomLevel);
    if(zoomLevel_ != originLevel) {
        emit scaleChanged(zoomLevel_);
    }
    setPageNum(pageNum_);
}

void PdfView::setFitMode(FitMode fitMode)
{
    fitMode_ = fitMode;
    setZoomLevel(zoomLevel_);
}

void PdfView::clearFitMode()
{
    setFitMode(None);
}

void PdfView::mousePressEvent(QMouseEvent* e)
{
    state_->mousePressEvent(e);
    changeState();
    e->ignore();
    QGraphicsView::mousePressEvent(e);
}

void PdfView::mouseMoveEvent(QMouseEvent* e)
{
    state_->mouseMoveEvent(e);
    changeState();
    e->ignore();
    QGraphicsView::mouseMoveEvent(e);
}

void PdfView::mouseReleaseEvent(QMouseEvent* e)
{
    state_->mouseReleaseEvent(e);
    changeState();
    e->ignore();
    QGraphicsView::mouseReleaseEvent(e);
}

void PdfView::wheelEvent(QWheelEvent *e)
{
    state_->wheelEvent(e);
    changeState();
    e->ignore();
}

void PdfView::keyPressEvent(QKeyEvent *e)
{
    state_->keyPressEvent(e);
    changeState();
    e->ignore();
}

void PdfView::keyReleaseEvent(QKeyEvent *e)
{
    state_->keyReleaseEvent(e);
    changeState();
    e->ignore();
    QGraphicsView::keyReleaseEvent(e);
}

void PdfView::resizeEvent(QResizeEvent *e)
{
    e->ignore();
    QGraphicsView::resizeEvent(e);
    if(fitMode_ != None)
    {
        setZoomLevel(zoomLevel_);
    }
}

void PdfView::focusOutEvent(QFocusEvent *e)
{
    state_->focusOutEvent(e);
    changeState();
    e->ignore();
    QGraphicsView::focusOutEvent(e);
}

void PdfView::changeState()
{
    auto tmp = state_;
    state_ = state_->nextState();
    if(state_ != tmp){
        delete tmp;
    }
}
