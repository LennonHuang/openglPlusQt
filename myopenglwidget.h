#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <qopenglshaderprogram.h>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QDebug>

#include <GLFW/glfw3.h>

class myopenglwidget: public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    myopenglwidget(QWidget*parent=nullptr);
    ~myopenglwidget();
    QOpenGLShaderProgram *shaderProgram = nullptr;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QVector<GLfloat> pointcloud{
            //position          colour
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
             0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
        };

protected:
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void initializeGL() override;
private:
    void setupVAOattributes();
};

#endif // MYOPENGLWIDGET_H
