/*
**  This file is part of Raccoon Reader.
**
** 	pagecontroller.h:
** 		Declaration of PageController class, which provide a
**  interface for scaling page and changing num of pages;
** 		This class consider 4 components:
** 		ScaleBox: which to modify page scale by choose items or input a
** 	certainly number that has a '%' at end.
** 		PageSlider: which can modify page number by roll mouse wheel,
** 	click an position or drag the slider. For better performance, the
** 	slider emit a signal only if drag finished.
** 		PageSpinBox: which can modify pagenumber by click buttons or input
** 	a number. For better performance, the component emit a signal only if
** 	input finished or focus out.
**
**  Copyright 2021 Yang XiLong
*/

#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include "pageslider.h"
#include "pagespinbox.h"
#include "scalebox.h"
#include "QPushButton"

#include "pdfarea/pdfview/pdfview.h"

#include <QLabel>
#include <QSize>
#include <QMap>

#include <poppler/qt5/poppler-qt5.h>

using Poppler::Document;

class PageController : public QWidget
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
    void setFitMode(PdfView::FitMode fitMode);
private:
    ScaleBox* scaleBox_;
    QMap<PdfView::FitMode, QPushButton*> fitButtons_;
    PageSlider* slider_;
    PageSpinBox* spinBox_;
    QLabel* pageCountLabel_;
    int pageCount_;
    int lastPage_;
    PdfView::FitMode fitMode_;

    void emitChange();
    void initFitButtons();
signals:
    void pageChanged(int pageNum);
    void scaleChanged(double scale);
    void scaleSelected();
    void fitModeChanged(PdfView::FitMode);
};

#endif // PAGECONTROLLER_H
