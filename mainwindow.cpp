#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mazenode.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    setupUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI(){
    ui->setupUi(this);
    this->setMouseTracking(true);
    ui->tabWidget->setMouseTracking(true);
    ui->tabWidget->setGeometry(1,1,ui->tabWidget->width(),ui->tabWidget->height());
    maze = new MazeUi(ui->tabWidget->widget(0));
    this->showMinimized();
}

