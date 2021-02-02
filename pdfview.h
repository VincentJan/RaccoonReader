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
    void SetScale(const QString& scale);
    void FitSize(int, int);
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;
    void resizeEvent(QResizeEvent* e) override;
private:
    int pageNum_ = 0;
    double scale_ = 1;
    bool painting_ = false;
    QString fitMode_;
    QPoint startPot_;
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
