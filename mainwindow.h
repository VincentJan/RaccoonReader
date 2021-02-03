/*
**  This file is part of Raccoon Reader.
**
** 	mainwindow.h: Declaration of MainWindow class.
**
**  Copyright 2021 Yang XiLong
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pdfview.h"
#include "pagecontroller.h"
#include "toc.h"

#include <QMainWindow>
#include <QSplitter>
#include <QString>
#include <QIcon>


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;
    MainWindow(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    void openFile();
    void closeFile();
    void actualSize();
    void fitWidth();
    void fitHeight();
    void fitPage();
private:
    QSplitter *mainSplitter_;
    QTabWidget *sideBar_;
    Toc *toc_;
    PdfView *pdfView_;
    PageController *pageController_;
    QSplitter *pdfArea_;

    void generateMenuBar();
    void contentSelected(QTreeWidgetItem *item, int col);
    static const QString appName;
    static const QString appLogo;
signals:
    void documentChanged(Document *file);
};
#endif // MAINWINDOW_H
