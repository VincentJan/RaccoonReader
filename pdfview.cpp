#include "pdfview.h"

#include <QDebug>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QApplication>
#include <QScreen>
#include <QRect>
#include <QGraphicsRectItem>
#include <QApplication>

PdfView::PdfView(QWidget *parent)
    : QGraphicsView(parent), fitMode_(), doc_(), curScene_(new QGraphicsScene)
{
    curSelect_ = new QGraphicsRectItem();
    setScene(curScene_);
    setDragMode(QGraphicsView::ScrollHandDrag);
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
    QImage img = page->renderToImage(res, res);
    curScene_->clear();
    curScene_->addPixmap(QPixmap::fromImage(img));
    curScene_->setSceneRect(0, 0, img.width(), img.height());
    curSelect_ = new QGraphicsRectItem();
    curScene_->addItem(curSelect_);
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

void PdfView::enterScaleMode()
{
    clearEditMode();
    scaleMode_ = true;
    QApplication::setOverrideCursor(Qt::SizeVerCursor);
    setDragMode(QGraphicsView::NoDrag);
}

void PdfView::enterSelectMode() {
    clearEditMode();
    selectMode_ = true;
    QApplication::setOverrideCursor(Qt::CrossCursor);
    setDragMode(QGraphicsView::NoDrag);
}

void PdfView::clearEditMode() {
    QApplication::restoreOverrideCursor();
    setDragMode(QGraphicsView::ScrollHandDrag);
    curSelect_->setRect(0, 0, 0, 0);
    selectMode_ = false;
    scaleMode_ = false;
}

void PdfView::mousePressEvent(QMouseEvent* e)
{
    if(selectMode_) {
        startPos_ = e->pos();
    }
    e->ignore();
    QGraphicsView::mousePressEvent(e);
}

void PdfView::mouseMoveEvent(QMouseEvent* e)
{
    if(selectMode_) {
        endPos_ = e->pos();
        auto sceneStartPos = mapToScene(startPos_);
        auto sceneEndPos = mapToScene(endPos_);
        auto spos = curSelect_->mapFromScene(sceneStartPos);
        auto epos = curSelect_->mapFromScene(sceneEndPos);
        curSelect_->setRect(QRectF(spos, epos));
        auto brush = QBrush();
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(QColor(150, 150, 200, 100));
        curSelect_->setBrush(brush);
    }
    e->ignore();
    QGraphicsView::mouseMoveEvent(e);
}

void PdfView::mouseReleaseEvent(QMouseEvent* e)
{
    if(selectMode_) {
        endPos_ = e->pos();
        auto sceneStartPos = mapToScene(startPos_);
        auto sceneEndPos = mapToScene(endPos_);
        auto spos = curSelect_->mapFromScene(sceneStartPos);
        auto epos = curSelect_->mapFromScene(sceneEndPos);
        curSelect_->setRect(QRectF(spos, epos));
        auto selectArea = new QGraphicsRectItem();
        selectArea->setRect(curSelect_->rect());
        selectArea->setBrush(curSelect_->brush());
        curScene_->addItem(selectArea);
    }
    e->ignore();
    QGraphicsView::mouseReleaseEvent(e);
}

void PdfView::wheelEvent(QWheelEvent *e)
{
    e->ignore();
    if(scaleMode_) {
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
    if(e->key() == Qt::Key_Shift) {
        enterSelectMode();
    }
    if(e->key() == Qt::Key_Control) {
        enterScaleMode();
    }
}

void PdfView::keyReleaseEvent(QKeyEvent *e)
{
    e->ignore();
    QGraphicsView::keyReleaseEvent(e);
    if((selectMode_ && e->key() == Qt::Key_Shift)
            ||(scaleMode_ && e->key() == Qt::Key_Control))
    {
        clearEditMode();
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
    clearEditMode();
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
