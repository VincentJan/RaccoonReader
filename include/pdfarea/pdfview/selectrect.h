#ifndef SELECTRECT_H
#define SELECTRECT_H

#include <QGraphicsRectItem>
#include <QDialog>

class SelectRect
        : public QWidget
{
    Q_OBJECT
public:
    explicit SelectRect(QWidget *parent = nullptr);
    ~SelectRect();
    SelectRect(const SelectRect&) = delete;
    SelectRect(SelectRect&&) = delete;
    QGraphicsRectItem *graphicsItem();
    void show();
private:
    QGraphicsRectItem *selectArea_;
    QDialog *settingDialog_;
signals:
    void babyPleaseKillMe(QGraphicsItem *item);
};

#endif // SELECTRECT_H
