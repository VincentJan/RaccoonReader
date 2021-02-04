/*
**  This file is part of Raccoon Reader.
**
** 	pagespinbox.h: Declaration of PageSpinBox class which modifyed
** 		from QSpinBox class. This class provide a better way to edit
** 		numbers.
**
**  Copyright 2021 Yang XiLong
*/

#ifndef PAGESPINBOX_H
#define PAGESPINBOX_H

#include <QSpinBox>

class PageSpinBox : public QSpinBox {
    Q_OBJECT
public:
    PageSpinBox(QWidget* parent = nullptr);
    void mouseReleaseEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;
};

#endif // PAGESPINBOX_H
