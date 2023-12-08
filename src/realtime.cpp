#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "settings.h"
#include "bezier.h"
#include <unistd.h>
#include "glm/glm.hpp"

// ================== Project 5: Lights, Camera
GLuint defaultFBO = 2;

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;

    // If you must use this function, do not edit anything above this
    prev_param1 = settings.shapeParameter1;
    prev_param2 = settings.shapeParameter2;
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here
    textureShader.deleteShader();
    phongShader.deleteShader();
    fbo.deleteFBO();

    this->doneCurrent();
}

void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();
    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    // Initializing GL.
    // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GL: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION) << std::endl;

    // Allows OpenGL to draw objects appropriately on top of one another
    glEnable(GL_DEPTH_TEST);
    // Tells OpenGL to only draw the front face
    glEnable(GL_CULL_FACE);
    // Tells OpenGL how big the screen is
    glViewport(0, 0, m_screen_width, m_screen_height);

    // Students: anything requiring OpenGL calls when the program starts should be done here

    // set background color
    glClearColor(0, 0, 0, 1);

    fbo.makeFBO(defaultFBO, m_screen_width, m_screen_height);

    // create shader program
    phongShader.createShader(m_screen_width, m_screen_height, fbo.getID());
    textureShader.createShader(m_screen_width, m_screen_height, fbo.getID());

    textureShader.updateTexture(fbo.getTexID());

    phongShader.updateScene(&currentScene);
    currentScene = Scene();

}

void Realtime::paintGL() {
    // Students: anything requiring OpenGL calls every frame should be done here

    // draw the current scene
    phongShader.draw();
    textureShader.draw();
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here
    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;

    // Task 34: Regenerate your FBOs
    fbo.deleteFBO();
    fbo.makeFBO(defaultFBO, m_screen_width, m_screen_height);

    phongShader.updateViewport(m_screen_width, m_screen_height, fbo.getID(), defaultFBO);
    textureShader.updateViewport(m_screen_width, m_screen_height, fbo.getID(), defaultFBO);

    width = w;
    height = h;
    currentScene.updateCamProjMatrix(m_screen_width, m_screen_height, settings.nearPlane, settings.farPlane);
}

void Realtime::sceneChanged() {
    RenderData metaData;
    SceneParser::parse(settings.sceneFilePath, metaData);
    currentScene = Scene(width, height, metaData);
    currentScene.updateCamProjMatrix(width, height, settings.nearPlane, settings.farPlane);
    phongShader.updateShapeData(settings.shapeParameter1, settings.shapeParameter2);

    update(); // asks for a PaintGL() call to occur
}

void Realtime::settingsChanged() {
    if (currentScene.getCamera().getNear() != settings.nearPlane ||
        currentScene.getCamera().getFar() != settings.farPlane) {
        currentScene.updateCamProjMatrix(width, height, settings.nearPlane, settings.farPlane);
    }

    if (prev_param1 != settings.shapeParameter1 || prev_param2 != settings.shapeParameter2) {
        prev_param1 = settings.shapeParameter1;
        prev_param2 = settings.shapeParameter2;
        // update shape data
        phongShader.updateShapeData(settings.shapeParameter1, settings.shapeParameter2);
    }

    update(); // asks for a PaintGL() call to occur
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = false;
    }
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        // Use deltaX and deltaY here to rotate
        currentScene.rotateCam(deltaX, deltaY);

        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();

    std::cout << "Camera Position.x: " << currentScene.getCamera().getPosition().x << " Camera Position.y: " << currentScene.getCamera().getPosition().y << " Camera Position.z: " << currentScene.getCamera().getPosition().z << std::endl;

    // enable/disable camera motion
    if (!settings.extraCredit1) {
        // Use deltaTime and m_keyMap here to move around
        float unitsMoved = 5.f * deltaTime;
        float forward = 0.f, right = 0.f , up = 0.f;

        if (m_keyMap[Qt::Key_W])  forward += unitsMoved;
        if (m_keyMap[Qt::Key_S])  forward -= unitsMoved;
        if (m_keyMap[Qt::Key_D])    right += unitsMoved;
        if (m_keyMap[Qt::Key_A])    right -= unitsMoved;
        if (m_keyMap[Qt::Key_Space])   up += unitsMoved;
        if (m_keyMap[Qt::Key_Control]) up -= unitsMoved;

        currentScene.translateCam(forward, right, up);
        update();
        return;
    }

    // -------- test code ----------
//    static float dist = glm::length(currentScene.getCamera().getPosition());
//    static glm::vec3 dir = -glm::normalize(glm::vec3(currentScene.getCamera().getPosition()));
//    currentScene.updateCamPos(currentScene.getCamera().getPosition() + (dist / 600.f) * glm::vec4(dir, 0.f));
    // -----------------------------

    // -------- recommended code ---------
    static std::vector<glm::vec4> testPoints = Bezier::testPoints();
    static int index = 0;
    currentScene.updateCamPos(testPoints[index++]);

    if (index > 0) {
        glm::vec3 newUp = glm::normalize(glm::dot(tan(testPoints[index-1]), tan(testPoints[index])) * currentScene.getUpVector());
        currentScene.updateCamLookUp(glm::normalize(glm::vec3(1, -1, -1)), newUp);
    }

    else {
        currentScene.updateCamLookUp(glm::normalize(glm::vec3(1, -1, -1)), currentScene.getUpVector());
    }

    //45 degrees downward in the positive X Z direction
    // currentScene.rotateCam(2, 0);
    // -----------------------------------

    // ------------- UNCOMMENT HERE ------------
//    for (glm::vec4 point : testPoints) {
//        currentScene.updateCamPos(point);
//        std::cout << "Camera Position.x: " << currentScene.getCamera().getPosition().x << " Camera Position.y: " << currentScene.getCamera().getPosition().y << " Camera Position.z: " << currentScene.getCamera().getPosition().z << std::endl;
//        update();
//    }
     // ------------------------------------------
    std::cout << index << std::endl;
    std::cout << "Camera Position.x: " << currentScene.getCamera().getPosition().x << " Camera Position.y: " << currentScene.getCamera().getPosition().y << " Camera Position.z: " << currentScene.getCamera().getPosition().z << std::endl;
    update(); // asks for a PaintGL() call to occur
}

// DO NOT EDIT
void Realtime::saveViewportImage(std::string filePath) {
    // Make sure we have the right context and everything has been drawn
    makeCurrent();

    int fixedWidth = 1024;
    int fixedHeight = 768;

    // Create Frame Buffer
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create a color attachment texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fixedWidth, fixedHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Optional: Create a depth buffer if your rendering uses depth testing
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fixedWidth, fixedHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    // Adjust the default FBO
    GLuint oldFBO = defaultFBO;
    defaultFBO = fbo;

    // resize the openGL viewport and propagate new default FBO
    resizeGL(fixedWidth, fixedHeight);

    // Clear and render your scene here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    paintGL();

    // Read pixels from framebuffer
    std::vector<unsigned char> pixels(fixedWidth * fixedHeight * 3);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glReadPixels(0, 0, fixedWidth, fixedHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // Unbind the framebuffer and return the default FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    defaultFBO = oldFBO;
    resizeGL(size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Convert to QImage
    QImage image(pixels.data(), fixedWidth, fixedHeight, QImage::Format_RGB888);
    QImage flippedImage = image.mirrored(); // Flip the image vertically

    // Save to file using Qt
    QString qFilePath = QString::fromStdString(filePath);
    if (!flippedImage.save(qFilePath)) {
        std::cerr << "Failed to save image to " << filePath << std::endl;
    }

    // Clean up
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
}
