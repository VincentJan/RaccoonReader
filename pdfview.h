#ifndef PDFVIEW_H
#define PDFVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QMap>
#include <functional>

#include <poppler/qt5/poppler-qt5.h>

using Poppler::Document;
using std::function;

class PdfView : public QGraphicsView
{
    Q_OBJECT
public:
    enum FitMode {
        None, FitWidth, FitHeight, FitPage
    };
    PdfView(QWidget* parent = nullptr);
    PdfView(const QString& path, QWidget* parent = nullptr);
    PdfView(const PdfView&) = delete;
    PdfView(PdfView&&) = delete;
    PdfView& operator=(const PdfView&) = delete;
    PdfView& operator=(PdfView&&) = delete;
    ~PdfView();
    int pageNum() const;
    int pageCount() const;
    Document* document() const;
    void setPageNum(int n);
    void setDocument(Document* doc);
    void setScale(double scale);
    void setFitMode(FitMode fitMode);
    void clearFitMode();
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
    void focusOutEvent(QFocusEvent *e) override;
private:
    int pageNum_ = 0;
    double scale_ = 1;
    bool painting_ = false;
    FitMode fitMode_;
    QPoint startPot_;
    Document* doc_;
    QGraphicsScene* curScene_;

    bool ctrlPressed_ = false;
    bool altPressed_ = false;
    bool shiftPressed_ = false;

    void MoveUp(int n);
    void MoveDown(int n);
    void MoveLeft(int n);
    void MoveRight(int n);
signals:
    void pageChanged(int n);
    void scaleChanged(double scale);
};

#endif // PDFVIEW_H
