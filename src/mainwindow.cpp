/*
**  This file is part of Raccoon Reader.
**
** 	mainwindow.h: Declaration of MainWindow class.
**
**  Copyright 2021 Yang XiLong
*/

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
    mainSplitter_->setHandleWidth(0);
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
    pdfArea_->setHandleWidth(0);
    pdfArea_->addWidget(pdfView_);

    pageController_ = new PageController(this->size());
    connect(this, &MainWindow::documentChanged, pageController_, &PageController::setFromDocument);
    connect(pageController_, &PageController::pageChanged, pdfView_, &PdfView::setPageNum);
    connect(pdfView_, &PdfView::pageChanged, pageController_, &PageController::setPageNum);
    connect(pageController_, &PageController::scaleChanged, pdfView_, &PdfView::setZoomLevel);
    connect(pdfView_, &PdfView::scaleChanged, pageController_, &PageController::setScale);
    connect(pageController_, &PageController::scaleSelected, pdfView_, &PdfView::clearFitMode);
    connect(pageController_, &PageController::fitModeChanged, pdfView_, &PdfView::setFitMode);
    connect(pdfView_, &PdfView::fitModeChanged, pageController_, &PageController::setFitMode);
    pdfArea_->addWidget(pageController_);

    mainSplitter_->addWidget(pdfArea_);
}

void MainWindow::generateMenuBar()
{
    auto fileMenu = new QMenu(tr("File"), menuBar());
    menuBar()->addMenu(fileMenu);

    auto openFile = new QAction(tr("Open File"), fileMenu);
    openFile->setShortcut(QKeySequence::Open);
    openFile->setIcon(QIcon(":/images/open.svg"));
    connect(openFile, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openFile);

    auto closeFile = new QAction(tr("Close File"), fileMenu);
    closeFile->setShortcut(QKeySequence::Close);
    closeFile->setIcon(QIcon(":/images/close.svg"));
    connect(closeFile, &QAction::triggered, this, &MainWindow::closeFile);
    fileMenu->addAction(closeFile);

    auto viewMenu = new QMenu(tr("View"), menuBar());
    menuBar()->addMenu(viewMenu);

    auto actualSize = new QAction(tr("Actual Size"), viewMenu);
    actualSize->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_0));
    connect(actualSize, &QAction::triggered, this, &MainWindow::actualSize);
    viewMenu->addAction(actualSize);

    auto fitWidth = new QAction(tr("Fit Width"), viewMenu);
    fitWidth->setIcon(QIcon(":/images/fitWidth.svg"));
    fitWidth->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    connect(fitWidth, &QAction::triggered, this, &MainWindow::fitWidth);
    viewMenu->addAction(fitWidth);

    auto fitHeight = new QAction(tr("Fit Height"), viewMenu);
    fitHeight->setIcon(QIcon(":/images/fitHeight.svg"));
    fitHeight->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_8));
    connect(fitHeight, &QAction::triggered, this, &MainWindow::fitHeight);
    viewMenu->addAction(fitHeight);

    auto fitPage = new QAction(tr("Fit Page"), viewMenu);
    fitPage->setIcon(QIcon(":/images/fitPage.svg"));
    fitPage->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_9));
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
}

void MainWindow::closeFile()
{
    emit documentChanged(nullptr);
    sideBar_->setVisible(false);
}

void MainWindow::actualSize()
{
    pdfView_->setFitMode(PdfView::None);
    pdfView_->setZoomLevel(1);
    pageController_->setScale(1);
}

void MainWindow::fitWidth()
{
    if(pdfView_->fitMode() == PdfView::FitWidth) {
        pdfView_->setFitMode(PdfView::None);
    } else {
        pdfView_->setFitMode(PdfView::FitWidth);
    }
}

void MainWindow::fitHeight()
{
    if(pdfView_->fitMode() == PdfView::FitHeight) {
        pdfView_->setFitMode(PdfView::None);
    } else {
        pdfView_->setFitMode(PdfView::FitHeight);
    }
}

void MainWindow::fitPage()
{
    if(pdfView_->fitMode() == PdfView::FitPage) {
        pdfView_->setFitMode(PdfView::None);
    } else {
        pdfView_->setFitMode(PdfView::FitPage);
    }
}

void MainWindow::contentSelected(QTreeWidgetItem* item, int)
{
    if(item->columnCount() < 2) return;
    int pageNum = item->text(1).toInt();
    pageController_->setPageNum(pageNum);
}
