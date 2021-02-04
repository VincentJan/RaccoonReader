#include "include/pdfarea/pdfview/selectrect.h"

#include <QHBoxLayout>
#include <QPushButton>

SelectRect::SelectRect(QWidget *parent)
    : QWidget(parent), selectArea_(new QGraphicsRectItem())
{
    selectArea_->setFlags(QGraphicsRectItem::ItemIsFocusable);

    settingDialog_ = new QDialog(this);
    settingDialog_->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    auto layout = new QHBoxLayout(settingDialog_);
    settingDialog_->setLayout(layout);

    auto del = new QPushButton(settingDialog_);
    del->setIcon(QIcon(":/images/delete.svg"));
    connect(del, &QPushButton::clicked, this, [this](){
        emit babyPleaseKillMe(selectArea_);
        selectArea_ = new QGraphicsRectItem();
        settingDialog_->close();
    });
    layout->addWidget(del);
    settingDialog_->show();
}


SelectRect::~SelectRect()
{
    delete selectArea_;
}

QGraphicsRectItem *SelectRect::graphicsItem()
{
    return selectArea_;
}

void SelectRect::show()
{
    settingDialog_->show();
}
