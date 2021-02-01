#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include <QObject>
#include <QSplitter>
#include <QSpinBox>
#include <QLabel>
#include <QSize>
#include "pageslider.h"

class PageController : public QObject
{
    Q_OBJECT
public:
    PageController(const QSize& size, int pageCount);
    ~PageController();
    QWidget* GetLayout();
    void SetPageCount(int n);
private:
    QSplitter* layout_;
    PageSlider* slider_;
    QSpinBox* spinBox_;
    QLabel* pageCountLabel_;
    int pageCount_;

    void EmitChange(int n);
signals:
    void pageChanged(int n);
};

#endif // PAGECONTROLLER_H
