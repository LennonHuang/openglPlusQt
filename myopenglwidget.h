#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QDebug>

#include <GLFW/glfw3.h>

class myopenglwidget: public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    myopenglwidget(QWidget*parent=nullptr);
protected:
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void initializeGL() override;
};

#endif // MYOPENGLWIDGET_H
