#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include "pageslider.h"
#include "pagespinbox.h"
#include "scalebox.h"

#include <QObject>
#include <QSplitter>
#include <QLabel>
#include <QSize>

#include <poppler/qt5/poppler-qt5.h>

using Poppler::Document;

class PageController : public QSplitter
{
    Q_OBJECT
public:
    PageController(const QSize& size, int pageCount = 1);
    PageController(const PageController&) = delete;
    PageController(PageController&&) = delete;
    ~PageController() = default;
    void setPageCount(int n);
    void setPageNum(int n);
    void setFromDocument(Document *doc);
    void setScale(double scale);
private:
    ScaleBox* scaleBox_;
    PageSlider* slider_;
    PageSpinBox* spinBox_;
    QLabel* pageCountLabel_;
    int pageCount_;
    int lastPage_;

    void emitChange();
signals:
    void pageChanged(int pageNum);
    void scaleChanged(double scale);
    void scaleSelected();
};

#endif // PAGECONTROLLER_H
