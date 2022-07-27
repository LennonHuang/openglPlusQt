#ifndef FWWORKER_H
#define FWWORKER_H

#include <QObject>
#include <QDebug>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class fwWorker : public QObject
{
    Q_OBJECT
public:
    explicit fwWorker(QObject *parent = nullptr);
    // settings
    const unsigned int screenWidth = 800;
    const unsigned int screenHeight = 600;
    // camera
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
signals:
    void updateq();
public slots:
    void run();

};

#endif // FWWORKER_H
