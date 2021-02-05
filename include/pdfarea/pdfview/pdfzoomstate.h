#ifndef PDFZOOMSTATE_H
#define PDFZOOMSTATE_H

#include "pdfstate.h"

class PdfScaleState : public PdfState
{
public:
    PdfScaleState(PdfView *pdfView);
    ~PdfScaleState();

    virtual void keyPressEvent     (QKeyEvent    *);
    virtual void keyReleaseEvent   (QKeyEvent    *);
    virtual void wheelEvent        (QWheelEvent  *);
    virtual void focusOutEvent     (QFocusEvent  *);
};

#endif // PDFZOOMSTATE_H
