#include "pdfview.h"
#include <QDebug>

PdfView::PdfView(const QString& path) : pageNum_(1), doc_(new QPdfDocument)
  , pdfView_(new QGraphicsView), curScene_(new QGraphicsScene) {
    doc_->load(path);
    pdfView_->setScene(curScene_);
    SetPageNum(1);
}

PdfView::~PdfView() {
    delete pdfView_;
    delete doc_;
}

int PdfView::GetPageNum() const {
    return pageNum_;
}

void PdfView::SetPageNum(int n) {
    if(n <= 0) return;
    if(doc_ == nullptr) return;
    pageNum_ = n - 1;
    QSize tmp(doc_->pageSize(0).width() * 2, doc_->pageSize(0).height() * 2);
    QImage curPage = doc_->render(pageNum_, tmp);
    curScene_->clear();
    curScene_->addPixmap(QPixmap::fromImage(curPage));
}

void PdfView::SetDocument(QPdfDocument* doc) {
    doc_ = doc;
}

QPdfDocument* PdfView::GetDocument() const {
    return doc_;
}

QWidget* PdfView::GetLayout() const {
    return pdfView_;
}

int PdfView::GetPageCount() const {
    if(doc_ == nullptr) return 0;
    return doc_->pageCount();
}
