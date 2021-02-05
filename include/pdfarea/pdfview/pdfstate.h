#ifndef PDFSTATE_H
#define PDFSTATE_H

#include "pdfview.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QFocusEvent>

class PdfState
{
public:
    PdfState(PdfView *pdfView)
        : next_(this), pdfView_(pdfView) {}
    virtual ~PdfState() {}
    PdfState *nextState() {return next_;}

    virtual void keyPressEvent     (QKeyEvent    *) = 0;
    virtual void keyReleaseEvent   (QKeyEvent    *) {}
    virtual void mousePressEvent   (QMouseEvent  *) {}
    virtual void mouseMoveEvent    (QMouseEvent  *) {}
    virtual void mouseReleaseEvent (QMouseEvent  *) {}
    virtual void wheelEvent        (QWheelEvent  *) {}
    virtual void focusOutEvent     (QFocusEvent  *) {}
protected:
    PdfState *next_;
    PdfView *pdfView_;
};

#endif // PDFSTATE_H
