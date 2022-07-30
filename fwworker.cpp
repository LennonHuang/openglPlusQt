#include "fwworker.h"
#include <QDebug>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include <iostream>

// camera
float camDis = 3.0;
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  camDis);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camPointDir = glm::normalize( - cameraPos + cameraTarget);
glm::vec3 camRight = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 camUp    = glm::cross(camRight, camPointDir);

float fov = 45.0;
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scollCallback(GLFWwindow* window, double xoffset, double yoffset);

float frtTriV[] = {
    //first triangle
    -0.9f, -0.5f, 0.0f,
    0.0f, -0.5f, 0.0f,
    -0.45f, 0.5f, 0.0f,
};

float sndTriV[] = {
    //second tri
    0.0f, -0.5f, 0.0f,
    0.9f, -0.5f, 0.0f,
    0.45f, 0.5f, 0.0f
};

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location=0) in vec3 aPos;\n"
                                 "uniform mat4 model;"
                                 "uniform mat4 view;"
                                 "uniform mat4 projection;"
                                 "void main(){\n"
                                 " gl_Position = projection*view*model*vec4(aPos, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                 "out vec4 FragColor;\n"
                                 "void main(){\n"
                                 " FragColor = vec4(1.0f, 0.5f, 0.5f, 1.0f);\n"
                                 "}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0, width, height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

fwWorker::fwWorker(QObject *parent) : QObject(parent)
{

}

void fwWorker::run()
{
    qDebug() << "openning glfw window";
    qDebug() << "cam point dir: " << QString::fromStdString(glm::to_string(camPointDir));
    qDebug() << "cam right: " << QString::fromStdString(glm::to_string(camRight));
    qDebug() << "cam up: " << QString::fromStdString(glm::to_string(camUp));
    //init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Create the window
    qDebug() << "Creating a OpenGL window";
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL,NULL);
    if(window == nullptr){
        qDebug() << "Failed to create a OpenGL window";
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    qDebug() << "A OpenGL window created.";

    //GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        qDebug() << "Failed to init GLAD";
        return ;
    }

    ////////////////////////////////////
    //build and compile shader program
    ////////////////////////////////////
    /// vertextShader
    unsigned int vertextShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertextShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertextShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertextShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertextShader, 512, NULL, infoLog);
        qDebug() << infoLog;
    }else{
        qDebug() << "vertex shader compiled successfully";
    }
    /// fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        qDebug() << infoLog;
    }else{
        qDebug() << "fragment shader compiled successfully";
    }
    //Link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertextShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }else{
        qDebug() << "Shaders linked successfully!";
    }
    //clean up vertex and fragment shader
    glDeleteShader(vertextShader);
    glDeleteShader(fragmentShader);

    //Vertice Buffer
    unsigned int VAO[2],VBO[2];
    //generate index
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    //Binding
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);//assign buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof (frtTriV), frtTriV, GL_STATIC_DRAW);//copy user-define data
    //Attribute
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //clean
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    //Binding
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);//assign buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof (sndTriV), sndTriV, GL_STATIC_DRAW);//copy user-define data
    //Attribute
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //clean
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    //Resize window event callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scollCallback);

    //Bind to the shader ID before setting matrix
    glUseProgram(shaderProgram);
    //apply model transformation
    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    //view, camera
    glm::mat4 view(1.0f);
    view = glm::lookAt(cameraPos, cameraTarget, camUp);
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    //projection
    glm::mat4 proj(1.0f);
    proj = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    //mouse setting, callback must be static function
    glfwSetCursorPosCallback(window, mouseCallback);

    //Keep app alive
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        //input
        processInput(window);

        //draw our triangle
        glUseProgram(shaderProgram);
        //view, camera
        glm::mat4 view(1.0f);
        view = glm::lookAt(cameraPos, cameraTarget, camUp);
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        //projection
        glm::mat4 proj(1.0f);
        proj = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(VAO[1]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
        emit updateq();
    }

    //clean up
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    static double xLast, yLast;
    static double yaw = 0;
    static double pitch = 90;
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
        //mouse release mode
        //record last position
        xLast = xpos;
        yLast = ypos;
        return;
    }
    //Holding, catch deltax and deltay
    double resolution = 0.1;
    double yawresolution = 2.0;
    double pitchresolution = 1.0;
    double dx = xpos - xLast;
    double dy = ypos - yLast;

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        //right press

    }else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        //left press
        pitch += dy*resolution*pitchresolution;
        yaw -= dx*resolution*yawresolution;
        if(pitch < 5.0){pitch = 5.0;}else if(pitch > 179.0){ pitch = 179.0;}
        camRight.x = sin(glm::radians(yaw));
        camRight.y = cos(glm::radians(yaw));
        cameraPos = glm::vec3(camDis * cos(glm::radians(pitch)) * cos(glm::radians(-yaw)),
                              camDis * cos(glm::radians(pitch)) * sin(glm::radians(-yaw)),
                              camDis * sin(glm::radians(pitch)));
        camPointDir = glm::normalize( -cameraPos + cameraTarget);//target always vec3(0,0,0) for now
        camUp = glm::normalize(glm::cross(camRight, camPointDir));

        //Debug print
        qDebug() << "*******************";
        qDebug() << "yaw: " << yaw;
        qDebug() << "pitch: " << pitch;
        qDebug() << "cam pos: " << QString::fromStdString(glm::to_string(cameraPos));
        qDebug() << "cam right: " << QString::fromStdString(glm::to_string(camRight));
        qDebug() << "cam up: " << QString::fromStdString(glm::to_string(camUp));
        qDebug() << "*******************";
    }
    xLast = xpos;
    yLast = ypos;
}

void scollCallback(GLFWwindow* window, double xoffset, double yoffset){
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
