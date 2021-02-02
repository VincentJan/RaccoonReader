#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include <QObject>
#include <QSplitter>
#include <QLabel>
#include <QSize>
#include <QComboBox>
#include "pageslider.h"
#include "pagespinbox.h"

class PageController : public QObject
{
    Q_OBJECT
public:
    PageController(const QSize& size, int pageCount);
    ~PageController();
    QWidget* GetWidget();
    void SetPageCount(int n);
    void SetPageNum(int n);
private:
    QSplitter* widget_;
    QComboBox* comboBox_;
    PageSlider* slider_;
    PageSpinBox* spinBox_;
    QLabel* pageCountLabel_;
    int pageCount_;
    int lastPage_;

    void EmitChange();
    void EmitScale(const QString& scale);
signals:
    void pageChanged(int pageNum);
    void scaleChanged(const QString& scale);
};

#endif // PAGECONTROLLER_H
