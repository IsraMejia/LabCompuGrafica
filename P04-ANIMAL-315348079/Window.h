#pragma once
#include <stdio.h>
#include <glew.h>
#include <glfw3.h>

class Window
{
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight);
    int Initialise();
    GLfloat getBufferWidth() { return bufferWidth; }
    GLfloat getBufferHeight() { return bufferHeight; }
    bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
    bool* getsKeys() { return keys; }
    GLfloat getXChange();
    GLfloat getYChange();
    void swapBuffers() { return glfwSwapBuffers(mainWindow); }

    // ==============================================================================
    // [CONCEPTO] CINEMÁTICA DIRECTA (Forward Kinematics):
    // La Cinemática Directa consiste en calcular la posición final de un objeto 
    // (ej. la punta de la pata) a partir de los ángulos de todas sus articulaciones.
    // Estos "getters" permiten que nuestro ciclo de dibujado lea esos ángulos.
    // ==============================================================================
    GLfloat getHombro1() { return hombro1; }
    GLfloat getCodo1() { return codo1; }
    GLfloat getHombro2() { return hombro2; }
    GLfloat getCodo2() { return codo2; }
    GLfloat getHombro3() { return hombro3; }
    GLfloat getCodo3() { return codo3; }
    GLfloat getHombro4() { return hombro4; }
    GLfloat getCodo4() { return codo4; }
    GLfloat getBaseCola() { return baseCola; }
    GLfloat getMediaCola() { return mediaCola; }

    ~Window();

private: 
    GLFWwindow *mainWindow;
    GLint width, height;
    bool keys[1024];
    GLint bufferWidth, bufferHeight;
    GLfloat lastX, lastY, xChange, yChange;
    bool mouseFirstMoved;

    // ==============================================================================
    // [CONCEPTO] GRADOS DE LIBERTAD (Degrees of Freedom - DOF):
    // Cada una de estas variables representa un Grado de Libertad de nuestro modelo.
    // Almacenan el "Estado" de la jerarquía. El Hombro y el Codo tienen 1 DOF cada uno
    // ya que solo rotan sobre un solo eje local (Z).
    // ==============================================================================
    GLfloat hombro1, codo1, hombro2, codo2, hombro3, codo3, hombro4, codo4;
    GLfloat baseCola, mediaCola;

    void createCallbacks();
    static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
    static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};