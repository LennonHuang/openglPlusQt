#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if(fwThread.isRunning()){
        delete glfwWindowWorker;
        glfwWindowWorker = nullptr;
        fwThread.quit();
        while(!fwThread.isFinished()){}
    }
    delete ui;
}

void MainWindow::on_glfw_pushButton_clicked()
{
    glfwWindowWorker = new fwWorker(nullptr);
    glfwWindowWorker->moveToThread(&fwThread);
    QObject::connect(&fwThread, &QThread::started, glfwWindowWorker, &fwWorker::run);
    fwThread.start();//start a thread running glfw window
}

