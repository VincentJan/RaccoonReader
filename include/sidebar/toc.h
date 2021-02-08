/*
**  This file is part of Raccoon Reader.
**
** 	mainwindow.h: Declaration of MainWindow class.
**
**  Copyright 2021 Yang XiLong
*/

#ifndef TOC_H
#define TOC_H

#include <QTreeWidget>
#include <poppler/qt5/poppler-qt5.h>
#include <QVector>

using Poppler::Document;
using Poppler::OutlineItem;

class Toc : public QTreeWidget{
    Q_OBJECT
public:
    Toc(Document* doc = nullptr, QWidget* parent = nullptr);
    Toc(const Toc&) = delete;
    Toc(Toc&&) = delete;
    Toc& operator=(const Toc&) = delete;
    Toc& operator=(Toc&&) = delete;
    ~Toc() = default;
    void setDocument(Document* doc);
    void highlightItem(int n);
protected:
    void keyPressEvent(QKeyEvent *e) override;
private:
    void generateSubItem(QTreeWidgetItem* parent, const QVector<OutlineItem>& outlines);
    QVector<OutlineItem> outlineItem_;
};

#endif // TOC_H
