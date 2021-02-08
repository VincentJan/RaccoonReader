/*
**  This file is part of Raccoon Reader.
**
** 	mainwindow.h: Declaration of MainWindow class.
**
**  Copyright 2021 Yang XiLong
*/

#include "sidebar/toc.h"
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QKeyEvent>

namespace  {
    QString extractPageNum(const QString& str) {
        auto list = str.split(";");
        return list.at(1);
    }
}

Toc::Toc(Document* doc, QWidget* parent) {
    setParent(parent);
    setDocument(doc);
    QStringList headers;
    headers << "Name" << "Page";
    setHeaderLabels(headers);
    setHeaderHidden(true);
    setColumnHidden(1, true);
}

void Toc::setDocument(Document* doc) {
    clear();
    if (doc == nullptr) return;
    outlineItem_ = doc->outline();
    for (int i = 0; i < outlineItem_.size(); ++i) {
        auto curItem = outlineItem_.at(i);
        auto tmp = new QTreeWidgetItem(this);
        tmp->setText(0, curItem.name());
        auto pageData = curItem.destination().data();
        if(pageData != nullptr) {
            auto pageNum = extractPageNum(pageData->toString());
            tmp->setText(1, pageNum);
        }
        if (curItem.hasChildren()) {
            generateSubItem(tmp, curItem.children());
        }
    }
    expandAll();
}

void Toc::highlightItem(int n) {
    QTreeWidgetItemIterator it(this);
    while (*it) {
        if((*it)->text(1).toInt() > n) {
            --it;
            break;
        }
        ++it;
    }
    setCurrentItem(*it);
}

void Toc::keyPressEvent(QKeyEvent *e)
{
    auto key = e->key();
    QTreeWidgetItemIterator it(this);
    while (*it) {
        if((*it) == currentItem()) {
            break;
        }
        ++it;
    }
    if(key == Qt::Key_J) {
        ++it;
        if(*it == nullptr) return;
        setCurrentItem(*it);
    } else if(key == Qt::Key_K) {
        --it;
        if(*it == nullptr) return;
        setCurrentItem(*it);
    }
    e->ignore();
    QTreeWidget::keyPressEvent(e);
}

void Toc::generateSubItem(QTreeWidgetItem* parent, const QVector<OutlineItem>& outlines) {
    for(int i = 0; i < outlines.size(); ++i) {
        auto curItem = outlines.at(i);
        auto tmp = new QTreeWidgetItem(parent);
        tmp->setText(0, curItem.name());
        auto pageData = curItem.destination().data();
        if(pageData != nullptr) {
            auto pageNum = extractPageNum(pageData->toString());
            tmp->setText(1, pageNum);
        }
        if(curItem.hasChildren()) {
            generateSubItem(tmp, curItem.children());
        }
    }
}
