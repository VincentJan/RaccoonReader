#ifndef PDFMARKSTATE_H
#define PDFMARKSTATE_H

#include "pdfstate.h"

#include <QPoint>
#include <QGraphicsRectItem>

class PdfMarkState : public PdfState
{
public:
    PdfMarkState(PdfView *pdfView);
    ~PdfMarkState();
    virtual void keyPressEvent     (QKeyEvent    *);
    virtual void keyReleaseEvent   (QKeyEvent    *);
    virtual void mousePressEvent   (QMouseEvent  *);
    virtual void mouseMoveEvent    (QMouseEvent  *);
    virtual void mouseReleaseEvent (QMouseEvent  *);
    virtual void focusOutEvent     (QFocusEvent  *);
private:
    QPoint startPot_;
    QPoint endPot_;
    QGraphicsRectItem *selectArea;
};

#endif // PDFMARKSTATE_H
