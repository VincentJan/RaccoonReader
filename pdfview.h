#ifndef PDFVIEW_H
#define PDFVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <poppler/qt5/poppler-qt5.h>

using Poppler::Document;

class PdfView : public QGraphicsView{
    Q_OBJECT
public:
    PdfView();
    PdfView(const QString& path);
    PdfView(const PdfView&) = delete;
    PdfView(PdfView&&) = delete;
    PdfView& operator=(const PdfView&) = delete;
    PdfView& operator=(PdfView&&) = delete;
    ~PdfView();
    int GetPageNum() const;
    void SetPageNum(int n);
    void SetDocument(Document* doc);
    Document* GetDocument() const;
    int GetPageCount() const;
    void wheelEvent(QWheelEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
private:
    int pageNum_ = 0;
    double scale = 1;
    Document* doc_;
    QGraphicsScene* curScene_;

    void MoveUp(int n);
    void MoveDown(int n);
    void MoveLeft(int n);
    void MoveRight(int n);
signals:
    void PageChanged(int n);
};

#endif // PDFVIEW_H
