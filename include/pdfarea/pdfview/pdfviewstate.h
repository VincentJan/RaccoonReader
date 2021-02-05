#ifndef PDFVIEWSTATE_H
#define PDFVIEWSTATE_H

#include "pdfstate.h"

class PdfViewState : public PdfState
{
public:
    PdfViewState(PdfView *pdfView);
    ~PdfViewState();
    void keyPressEvent     (QKeyEvent    *) override;
    void wheelEvent        (QWheelEvent  *) override;
private:
    void moveUp   (int n);
    void moveDown (int n);
    void moveLeft (int n);
    void moveRight(int n);
};

#endif // PDFVIEWSTATE_H
