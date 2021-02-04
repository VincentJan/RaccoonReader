#include "mainwindow.h"

#include <QGuiApplication>
#include <QScreen>
#include <QIcon>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QDebug>
#include <QKeyEvent>

const QString MainWindow::appName(tr("Raccoon Reader"));
const QString MainWindow::appLogo(tr(":/images/icon.png"));

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto size = QGuiApplication::primaryScreen()->availableSize();
    resize(size.width()*0.8, size.height() * 0.9);
    setWindowTitle(appName);
    setWindowIcon(QIcon(appLogo));

    generateMenuBar();

    pdfView_ = new PdfView(this);
    connect(this, &MainWindow::documentChanged, pdfView_, &PdfView::setDocument);

    mainSplitter_ = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(mainSplitter_);

    sideBar_ = new QTabWidget(this);
    mainSplitter_->addWidget(sideBar_);
    sideBar_->setVisible(false);

    toc_ = new Toc(nullptr, sideBar_);
    toc_->expandAll();
    connect(this, &MainWindow::documentChanged, toc_, &Toc::setDocument);
    connect(toc_, &Toc::itemActivated, this, &MainWindow::contentSelected);
    connect(pdfView_, &PdfView::pageChanged, toc_, &Toc::highlightItem);
    sideBar_->addTab(toc_, tr("Contents"));

    pdfArea_ = new QSplitter(Qt::Vertical, mainSplitter_);
    pdfArea_->addWidget(pdfView_);

    pageController_ = new PageController(this->size());
    connect(this, &MainWindow::documentChanged, pageController_, &PageController::setFromDocument);
    connect(pageController_, &PageController::pageChanged, pdfView_, &PdfView::setPageNum);
    connect(pdfView_, &PdfView::pageChanged, pageController_, &PageController::setPageNum);
    connect(pageController_, &PageController::scaleChanged, pdfView_, &PdfView::setScale);
    connect(pdfView_, &PdfView::scaleChanged, pageController_, &PageController::setScale);
    connect(pageController_, &PageController::scaleSelected, pdfView_, &PdfView::clearFitMode);
    pdfArea_->addWidget(pageController_);

    mainSplitter_->addWidget(pdfArea_);
}

void MainWindow::generateMenuBar()
{
    auto fileMenu = new QMenu(tr("File"), menuBar());
    menuBar()->addMenu(fileMenu);

    auto openFile = new QAction(tr("Open File"), fileMenu);
    openFile->setShortcut(QKeySequence::Open);
    connect(openFile, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openFile);

    auto closeFile = new QAction(tr("Close File"), fileMenu);
    closeFile->setShortcut(QKeySequence::Close);
    connect(closeFile, &QAction::triggered, this, &MainWindow::closeFile);
    fileMenu->addAction(closeFile);

    auto viewMenu = new QMenu(tr("View"), menuBar());
    menuBar()->addMenu(viewMenu);

    auto actualSize = new QAction(tr("Actual Size"), viewMenu);
    connect(actualSize, &QAction::triggered, this, &MainWindow::actualSize);
    viewMenu->addAction(actualSize);

    auto fitWidth = new QAction(tr("Fit Width"), viewMenu);
    connect(fitWidth, &QAction::triggered, this, &MainWindow::fitWidth);
    viewMenu->addAction(fitWidth);

    auto fitHeight = new QAction(tr("Fit Height"), viewMenu);
    connect(fitHeight, &QAction::triggered, this, &MainWindow::fitHeight);
    viewMenu->addAction(fitHeight);

    auto fitPage = new QAction(tr("Fit Page"), viewMenu);
    connect(fitPage, &QAction::triggered, this, &MainWindow::fitPage);
    viewMenu->addAction(fitPage);

    menuBar()->addMenu(new QMenu("Edit"));
    menuBar()->addMenu(new QMenu("Tools"));
    menuBar()->addMenu(new QMenu("Help"));
}

void MainWindow::openFile()
{
    auto fileName = QFileDialog::getOpenFileName(this,
         tr("Open Document"), "$HOME", tr("Pdf Files (*.pdf)"));
    if(fileName == "") return;
    closeFile();
    emit documentChanged(Document::load(fileName));
    sideBar_->setVisible(true);
    mainSplitter_->setStretchFactor(0, 1);
    mainSplitter_->setStretchFactor(1, 3);
}

void MainWindow::closeFile()
{
    emit documentChanged(nullptr);
    sideBar_->setVisible(false);
}

void MainWindow::actualSize()
{
    pdfView_->setFitMode(PdfView::None);
    pdfView_->setScale(1);
    pageController_->setScale(1);
}

void MainWindow::fitWidth()
{
    pdfView_->setFitMode(PdfView::FitWidth);
}

void MainWindow::fitHeight()
{
    pdfView_->setFitMode(PdfView::FitHeight);
}

void MainWindow::fitPage()
{
    pdfView_->setFitMode(PdfView::FitPage);
}

void MainWindow::contentSelected(QTreeWidgetItem* item, int)
{
    if(item->columnCount() < 2) return;
    int pageNum = item->text(1).toInt();
    pageController_->setPageNum(pageNum);
}
