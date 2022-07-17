#include "myopenglwidget.h"


myopenglwidget::myopenglwidget(QWidget *parent)
{

}

void myopenglwidget::initializeGL()
{
    qDebug() << "init OpenGL environment";

}

void myopenglwidget::paintGL()
{
    qDebug() << "painting opengl widget";
}

void myopenglwidget::resizeGL(int w, int h)
{
    qDebug() << "resizing opengl window";
}

