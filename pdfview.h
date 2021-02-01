#ifndef PDFVIEW_H
#define PDFVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QPdfDocument>

class PdfView : public QObject {
    Q_OBJECT
public:
    PdfView(const QString& path = "");
    ~PdfView();
    int GetPageNum() const;
    void SetPageNum(int n);
    void SetDocument(QPdfDocument* doc);
    QPdfDocument* GetDocument() const;
    QWidget* GetLayout() const;
    int GetPageCount() const;
private:
    int pageNum_;
    QPdfDocument* doc_;
    QGraphicsView* pdfView_;
    QGraphicsScene* curScene_;
};

#endif // PDFVIEW_H
