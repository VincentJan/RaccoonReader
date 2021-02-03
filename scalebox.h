#ifndef SCALEBOX_H
#define SCALEBOX_H

#include <QComboBox>

class ScaleBox : public QComboBox
{
    Q_OBJECT
public:
    ScaleBox(QWidget *parent);
    void setScale(double scale);
    void focusOutEvent(QFocusEvent* e) override;
private:
    void text2Scale(const QString& text);
signals:
    void scaleChanged(double scale);
};

#endif // SCALEBOX_H
