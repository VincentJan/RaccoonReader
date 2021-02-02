#include "mainwindow.h"

#include <QGuiApplication>
#include <QScreen>
#include <QIcon>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QDebug>

using Poppler::OptContentModel;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto size = QGuiApplication::primaryScreen()->availableSize();
    this->resize(size.width()*0.8, size.height() * 0.9);
    this->setWindowTitle(appName);
    this->setWindowIcon(QIcon(":/images/icon.png"));

    auto fileMenu = new QMenu("File", menuBar());
    menuBar()->addMenu(fileMenu);

    auto openFile = new QAction("Open File", fileMenu);
    openFile->setShortcut(QKeySequence::Open);
    connect(openFile, &QAction::triggered, this, &MainWindow::OpenFile);
    fileMenu->addAction(openFile);

    auto closeFile = new QAction("Close File", fileMenu);
    closeFile->setShortcut(QKeySequence::Close);
    connect(closeFile, &QAction::triggered, this, &MainWindow::CloseFile);
    fileMenu->addAction(closeFile);

    menuBar()->addMenu(new QMenu("Edit"));
    menuBar()->addMenu(new QMenu("View"));
    menuBar()->addMenu(new QMenu("Tools"));
    menuBar()->addMenu(new QMenu("Help"));

    pdfView_ = new PdfView();

    mainSplitter_ = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(mainSplitter_);

    sideBar_ = new QTabWidget(this);
    mainSplitter_->addWidget(sideBar_);
    sideBar_->setVisible(false);

    pdfArea_ = new QSplitter(Qt::Vertical, mainSplitter_);
    pdfArea_->addWidget(pdfView_);

    pageController_ = new PageController(this->size(), 1);
    pdfArea_->addWidget(pageController_->GetWidget());

    connect(pageController_, &PageController::pageChanged, pdfView_, &PdfView::SetPageNum);
    connect(pdfView_, &PdfView::PageChanged, pageController_, &PageController::SetPageNum);
    connect(pageController_, &PageController::scaleChanged, pdfView_, &PdfView::SetScale);

    mainSplitter_->addWidget(pdfArea_);
}

MainWindow::~MainWindow()
{
    delete pdfView_;
    delete pageController_;
}

void MainWindow::OpenFile() {
    auto fileName = QFileDialog::getOpenFileName(this,
         tr("Open Document"), "$HOME", tr("Pdf Files (*.pdf)"));

    if(fileName == "") {
        return;
    }

    CloseFile();

    pdfView_->SetDocument(Document::load(fileName));
    pdfView_->SetPageNum(1);

    if (toc_ == nullptr) {
        toc_ = new Toc(pdfView_->GetDocument(), sideBar_);
        sideBar_->addTab(toc_, "Contents");

        connect(toc_, &Toc::itemActivated, this, &MainWindow::ContentSelected);
        connect(pdfView_, &PdfView::PageChanged, toc_, &Toc::HighLightItem);
    } else {
        toc_->SetDocument(pdfView_->GetDocument());
    }
    sideBar_->setVisible(true);
    mainSplitter_->setStretchFactor(0, 1);
    mainSplitter_->setStretchFactor(1, 3);
    toc_->expandAll();

    pageController_->SetPageCount(pdfView_->GetPageCount());
}

void MainWindow::CloseFile() {
    pdfView_->SetDocument(nullptr);
    pdfView_->SetPageNum(1);

    if (toc_ != nullptr) {
        toc_->SetDocument(nullptr);
    }
    sideBar_->setVisible(false);

    pageController_->SetPageCount(0);
}

void MainWindow::ContentSelected(QTreeWidgetItem* item, int) {
    if(item->columnCount() < 2) return;
    int pageNum = item->text(1).toInt();
    pageController_->SetPageNum(pageNum);
}
