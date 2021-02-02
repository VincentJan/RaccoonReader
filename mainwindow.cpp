#include "mainwindow.h"
#include "toc.h"

#include <QGuiApplication>
#include <QScreen>
#include <QIcon>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QDebug>

using Poppler::OptContentModel;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto size = QGuiApplication::primaryScreen()->availableSize();
    this->resize(size.width()*0.8, size.height() * 0.9);
    this->setWindowTitle(appName);
    this->setWindowIcon(QIcon(":/images/icon.png"));

    menuBar()->addMenu(new QMenu("File"));
    menuBar()->addMenu(new QMenu("Edit"));
    menuBar()->addMenu(new QMenu("View"));
    menuBar()->addMenu(new QMenu("Tools"));
    menuBar()->addMenu(new QMenu("Help"));

    pdfView = new PdfView("/home/vincent/Desktop/test3.pdf");

    QSplitter* mainSplitter = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(mainSplitter);

    QTabWidget* sideBar = new QTabWidget(this);

    Toc* toc = new Toc(pdfView->GetDocument(), sideBar);
    toc->expandAll();
    connect(toc, &Toc::itemActivated, this, &MainWindow::ContentSelected);

    sideBar->addTab(toc, "Contents");
    mainSplitter->addWidget(sideBar);

    QSplitter* pdfArea = new QSplitter(Qt::Vertical, mainSplitter);
    pdfArea->addWidget(pdfView);

    mainSplitter->addWidget(pdfArea);
    mainSplitter->setStretchFactor(0, 3);
    mainSplitter->setStretchFactor(1, 2);

    pageController = new PageController(this->size(), pdfView->GetPageCount());
    pdfArea->addWidget(pageController->GetLayout());

    connect(pageController, &PageController::pageChanged, pdfView, &PdfView::SetPageNum);
    connect(pdfView, &PdfView::PageChanged, pageController, &PageController::SetPageNum);
}

MainWindow::~MainWindow()
{
    delete pdfView;
    delete pageController;
}

void MainWindow::ContentSelected(QTreeWidgetItem* item, int) {
    if(item->columnCount() < 2) return;
    int pageNum = item->text(1).toInt();
    pageController->SetPageNum(pageNum);
}
