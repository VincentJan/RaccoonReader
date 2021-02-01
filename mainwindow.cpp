#include "mainwindow.h"

#include <QIcon>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QDebug>
#include <QtPdf>
#include <QTreeView>
#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto size = QGuiApplication::primaryScreen()->availableSize();
    this->resize(size.width()*0.8, size.height()*0.8);
    this->setWindowTitle(appName);
    this->setWindowIcon(QIcon(":/images/icon.png"));

    menuBar()->addMenu(new QMenu("File"));
    menuBar()->addMenu(new QMenu("Edit"));
    menuBar()->addMenu(new QMenu("View"));
    menuBar()->addMenu(new QMenu("Tools"));
    menuBar()->addMenu(new QMenu("Help"));

    pdfView = new PdfView("/home/vincent/Desktop/test.pdf");

    QSplitter* mainSplitter = new QSplitter(Qt::Vertical, this);
    setCentralWidget(mainSplitter);

    QSplitter* pdfArea = new QSplitter(Qt::Horizontal, mainSplitter);
    mainSplitter->addWidget(pdfArea);

    QPdfBookmarkModel* pdfBookmarkModel = new QPdfBookmarkModel(this);
    pdfBookmarkModel->setDocument(pdfView->GetDocument());

    QTreeView* bookmarkView = new QTreeView(this);
    bookmarkView->setModel(pdfBookmarkModel);
    bookmarkView->setHeaderHidden(true);
    bookmarkView->expandAll();

    connect(bookmarkView, &QTreeView::activated, this, &MainWindow::bookmarkSelected);

    QTabWidget* sideBar = new QTabWidget(this);
    sideBar->addTab(bookmarkView, "Contents");

    pdfArea->addWidget(sideBar);
    pdfArea->addWidget(pdfView->GetLayout());

    pageController = new PageController(this->size(), pdfView->GetPageCount());
    mainSplitter->addWidget(pageController->GetLayout());

    connect(pageController, &PageController::pageChanged, pdfView, &PdfView::SetPageNum);
}

MainWindow::~MainWindow()
{
    delete pdfView;
    delete pageController;
}

void MainWindow::bookmarkSelected(const QModelIndex& index) {
    if(!index.isValid()) {
        return;
    }
    qDebug() << index.data(QPdfBookmarkModel::PageNumberRole).toInt();
}
