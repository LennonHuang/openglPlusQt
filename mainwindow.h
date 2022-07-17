#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QThread>
#include "fwworker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_glfw_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QThread fwThread;
    fwWorker* glfwWindowWorker = nullptr;
};
#endif // MAINWINDOW_H
