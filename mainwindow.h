#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pdfview.h"
#include "pagecontroller.h"
#include "toc.h"

#include <QMainWindow>
#include <QString>
#include <QSplitter>
#include <QImage>
#include <QPdfDocument>
#include <QGraphicsView>
#include <QTreeWidget>

const QString appName = "Raccoon Reader";

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void OpenFile();
    void CloseFile();
private:
    PdfView* pdfView_;
    PageController* pageController_;
    QSplitter* mainSplitter_;
    QTabWidget* sideBar_;
    Toc* toc_;
    QSplitter* pdfArea_;
    void ContentSelected(QTreeWidgetItem* item, int col);
};
#endif // MAINWINDOW_H
