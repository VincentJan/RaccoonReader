#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pdfview.h"
#include "pagecontroller.h"

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
private:
    PdfView* pdfView;
    PageController* pageController;

    void ContentSelected(QTreeWidgetItem* item, int col);
};
#endif // MAINWINDOW_H
