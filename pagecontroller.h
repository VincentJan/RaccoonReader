#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include <QObject>
#include <QSplitter>
#include <QLabel>
#include <QSize>
#include "pageslider.h"
#include "pagespinbox.h"

class PageController : public QObject
{
    Q_OBJECT
public:
    PageController(const QSize& size, int pageCount);
    ~PageController();
    QWidget* GetLayout();
    void SetPageCount(int n);
    void SetPageNum(int n);
private:
    QSplitter* layout_;
    PageSlider* slider_;
    PageSpinBox* spinBox_;
    QLabel* pageCountLabel_;
    int pageCount_;
    int lastPage_;

    void EmitChange();
signals:
    void pageChanged(int n);
};

#endif // PAGECONTROLLER_H
