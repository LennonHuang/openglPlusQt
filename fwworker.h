#ifndef FWWORKER_H
#define FWWORKER_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <glm/common.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h> //make sure glad is in front of GLFW
#include <GLFW/glfw3.h>

class fwWorker : public QObject
{
    Q_OBJECT
public:
    explicit fwWorker(QObject *parent = nullptr);
    // settings
    const unsigned int screenWidth = 800;
    const unsigned int screenHeight = 600;

signals:
    void updateq();
public slots:
    void run();
private:
};

#endif // FWWORKER_H
