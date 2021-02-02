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
