#include "myopenglwidget.h"

const char *vss = "#version 140\n"
    "in vec3 aPos;\n"
    "in vec3 aColour;\n"
    "out vec3 vertexColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   vertexColor = aColour;\n"
    "}\0";

const char *fss = "#version 140\n"
    "out vec4 FragColor;\n"
    "in vec3 vertexColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(vertexColor, 1.0);\n"
    "}\n\0";

myopenglwidget::myopenglwidget(QWidget *parent)
{

}

myopenglwidget::~myopenglwidget(){
    if(shaderProgram!=nullptr){
        vbo.destroy();
        delete  shaderProgram;
        shaderProgram = nullptr;
    }
}

void myopenglwidget::initializeGL()
{
    qDebug() << "init OpenGL environment";
    initializeOpenGLFunctions();
    glPointSize(5);
    //////////////////////////////////////////////////
    // vertex shader
    shaderProgram = new QOpenGLShaderProgram;
    int ok = shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vss);
    if(ok){
        qDebug() << "compile vertext shader successfully.";
    }else{
        qDebug() << shaderProgram->log();
    }
    // fragment shader
    ok = shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fss);
    if(ok){
        qDebug() << "compile fragment shader successfully.";
    }else{
        qDebug() << shaderProgram->log();
    }
    //Bind the Shaders
    shaderProgram->bindAttributeLocation("vertex", 0);
    shaderProgram->bindAttributeLocation("fragment", 1);
    if(shaderProgram->link()){
        qDebug() << "vertex and fragment shaders have been linked";
    }else{
        qDebug() << "Shaders Linking ERROR.";
    }
    ////////////////////////////////////////////////
    //vertext array object
    vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
    //vertext buffer object
    vbo.create();
    vbo.bind();
    vbo.allocate(pointcloud.constData(), pointcloud.size() * sizeof(GLfloat));
    qDebug()  << "pointcloud vertex has been allocated, size is " <<  pointcloud.size();

    setupVAOattributes();

    shaderProgram->release();

}

void myopenglwidget::setupVAOattributes()
{
    vbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                             nullptr);//x,y,z
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                             reinterpret_cast<void *>(3 * sizeof(GLfloat)));//RGB
    vbo.release();
}

void myopenglwidget::paintGL()
{
    qDebug() << "painting opengl widget";
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //Binding vao
    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
    shaderProgram->bind();
    glDrawArrays(GL_POINTS, 0, 3);
    shaderProgram->release();
}

void myopenglwidget::resizeGL(int w, int h)
{
    qDebug() << "resizing opengl window";
}

