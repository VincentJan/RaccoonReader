#include "pdfview.h"

#include <QDebug>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QApplication>
#include <QScreen>
#include <QRect>
#include <QApplication>

PdfView::PdfView(QWidget *parent)
    : QGraphicsView(parent), fitMode_(), doc_(), curScene_(new QGraphicsScene)
{
    setScene(curScene_);
    setStyleSheet("background-color:gray");
    verticalScrollBar()->setStyleSheet("background-color:lightgray");
    horizontalScrollBar()->setStyleSheet("background-color:lightgray");
    setDragMode(QGraphicsView::ScrollHandDrag);
    setMouseTracking(true);
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
        curScene_->clear();
        return;
    }
    pageNum_ = n;
    emit pageChanged(n);
    double res = QApplication::primaryScreen()->logicalDotsPerInch() * scale_;
    auto page = doc_->page(pageNum_ - 1);
    QImage pageImg = page->renderToImage(res, res);
    curScene_->clear();
    curScene_->addPixmap(QPixmap::fromImage(pageImg));
    curScene_->setSceneRect(0, 0, pageImg.width(), pageImg.height());
}

void PdfView::setDocument(Document* doc)
{
    doc_ = doc;
    if(doc == nullptr) {
        curScene_->clear();
        return;
    }
    doc_->setPaperColor(QColor(248,248,248));
    doc_->setRenderHint(Poppler::Document::TextAntialiasing);
    setFitMode(None);
    setScale(1);
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

void PdfView::setScale(double scale)
{
    if(doc_ == nullptr) return;
    double height = size().height() - 2;
    double width = size().width() - 2;
    double pageWidth = curScene_->sceneRect().width();
    double pageHeight = curScene_->sceneRect().height();
    QMap<FitMode, function<void(double)>> scalePage;
    scalePage[None] = [this](double scale) {
        scale_ = scale;
    };
    scalePage[FitWidth] = [this, height, &width, pageHeight, pageWidth](double) {
        if(height / width < pageHeight / pageWidth) {
            width -= verticalScrollBar()->size().width();
        }
        scale_ = width / pageWidth * scale_;
        emit scaleChanged(scale_);
    };
    scalePage[FitHeight] = [this, &height, width, pageHeight, pageWidth](double) {
        if(width / height < pageWidth / pageHeight) {
            height -= horizontalScrollBar()->size().height();
        }
        scale_ = height / pageHeight * scale_;
        emit scaleChanged(scale_);
    };
    scalePage[FitPage] = [scalePage, height, width, pageHeight, pageWidth](double scale) {
        if(height / width < pageHeight / pageWidth) {
            scalePage[FitHeight](scale);
        } else {
            scalePage[FitWidth](scale);
        }
    };
    scalePage[fitMode_](scale);
    setPageNum(pageNum_);
}

void PdfView::setFitMode(FitMode fitMode)
{
    fitMode_ = fitMode;
    setScale(scale_);
}

void PdfView::clearFitMode()
{
    setFitMode(None);
}

void PdfView::mousePressEvent(QMouseEvent* e)
{
    e->ignore();
    QGraphicsView::mousePressEvent(e);
}

void PdfView::mouseMoveEvent(QMouseEvent* e)
{
    e->ignore();
    QGraphicsView::mouseMoveEvent(e);
}

void PdfView::mouseReleaseEvent(QMouseEvent* e)
{
    e->ignore();
    QGraphicsView::mouseReleaseEvent(e);
}

void PdfView::wheelEvent(QWheelEvent *e)
{
    e->ignore();
    if(ctrlPressed_) {
        clearFitMode();
        if(e->angleDelta().y() >= 0) {
            if(scale_ < 10 - 0.15)
                setScale(scale_ + 0.15);
        } else {
            if(scale_ > 0.15)
                setScale(scale_ - 0.15);
        }
        emit scaleChanged(scale_);
        return;
    }
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

void PdfView::keyPressEvent(QKeyEvent *e)
{
    e->ignore();
    if (e->key() == Qt::Key_J || e->key() == Qt::Key_Down) {
        MoveDown(50);
    }
    if (e->key() == Qt::Key_K || e->key() == Qt::Key_Up) {
        MoveUp(50);
    }
    if (e->key() == Qt::Key_H || e->key() == Qt::Key_Left) {
        MoveLeft(50);
    }
    if (e->key() == Qt::Key_L || e->key() == Qt::Key_Right) {
        MoveRight(50);
    }
    if(e->key() == Qt::Key_Control) {
        ctrlPressed_ = true;
    }
    if(e->key() == Qt::Key_Alt) {
        altPressed_ = true;
    }
    if(e->key() == Qt::Key_Shift) {
        shiftPressed_ = true;
        QApplication::setOverrideCursor(Qt::CrossCursor);
    }

}

void PdfView::keyReleaseEvent(QKeyEvent *e)
{
    e->ignore();
    QGraphicsView::keyReleaseEvent(e);
    if(e->key() == Qt::Key_Control) {
        ctrlPressed_ = false;
    }
    if(e->key() == Qt::Key_Alt) {
        altPressed_ = false;
    }
    if(e->key() == Qt::Key_Shift) {
        shiftPressed_ = false;
        QApplication::restoreOverrideCursor();
    }
}

void PdfView::resizeEvent(QResizeEvent *e)
{
    e->ignore();
    QGraphicsView::resizeEvent(e);
    if(fitMode_ != None)
    {
        setScale(scale_);
        emit scaleChanged(scale_);
    }
}

void PdfView::focusOutEvent(QFocusEvent *e)
{
    e->ignore();
    QGraphicsView::focusOutEvent(e);
    QApplication::restoreOverrideCursor();
}

void PdfView::MoveUp(int n)
{
    if(n <= 0) return;
    auto vBar = verticalScrollBar();
    if(vBar->value() == 0 && pageNum_ != 1) {
        setPageNum(pageNum_ - 1);
        vBar->setValue(vBar->maximum());
        return;
    }
    vBar->setValue(vBar->value() - n);
}

void PdfView::MoveDown(int n)
{
    if(n <= 0) return;
    auto vBar = verticalScrollBar();
    if(vBar->value() == vBar->maximum() && pageNum_ != pageCount()) {
        setPageNum(pageNum_ + 1);
        vBar->setValue(0);
        return;
    }
    vBar->setValue(vBar->value() + n);
}

void PdfView::MoveLeft(int n)
{
    if(n <= 0) return;
    auto hBar = horizontalScrollBar();
    if(hBar->value() == 0 && pageNum_ != 1) {
        setPageNum(pageNum_ - 1);
        hBar->setValue(hBar->maximum());
        return;
    }
    hBar->setValue(hBar->value() - n);
}

void PdfView::MoveRight(int n)
{
    if(n <= 0) return;
    auto hBar = horizontalScrollBar();
    if(hBar->value() == hBar->maximum() && pageNum_ != pageCount()) {
        setPageNum(pageNum_ + 1);
        hBar->setValue(0);
        return;
    }
    hBar->setValue(hBar->value() + n);
}
