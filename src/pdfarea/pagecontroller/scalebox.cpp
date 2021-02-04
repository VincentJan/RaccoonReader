/*
**  This file is part of Raccoon Reader.
**
** 	mainwindow.h: Declaration of MainWindow class.
**
**  Copyright 2021 Yang XiLong
*/

#include "include/pdfarea/pagecontroller/scalebox.h"

#include <QStringList>
#include <QRegExpValidator>
#include <QDebug>

ScaleBox::ScaleBox(QWidget *parent)
    : QComboBox(parent)
{
    QStringList items;
    items << "25%" << "50%" << "75%" << "100%" << "125%" << "150%"
        << "175%" << "200%" << "300%" << "400%" << "800%";
    addItems(items);
    setCurrentIndex(3);
    setEditable(true);
    QRegExp reg("[0-9]{1,3}%?");;
    setValidator(new QRegExpValidator(reg));
    connect(this, &QComboBox::textActivated, this, &ScaleBox::text2Scale);
}

void ScaleBox::setScale(double scale)
{
    setEditText(QString::number(scale * 100, 'f', 0) + "%");
}

void ScaleBox::text2Scale(const QString& text)
{
    auto subText = text.split("%");
    auto newText = subText.first() + "%";
    setEditText(newText);
    auto scale = subText.at(0).toDouble() / 100;
    emit scaleChanged(scale);
}

void ScaleBox::focusOutEvent(QFocusEvent *e)
{
    QComboBox::focusOutEvent(e);
    text2Scale(currentText());
}
