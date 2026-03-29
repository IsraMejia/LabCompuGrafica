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
    bool* getsKeys() { return teclas; }
    GLfloat getXChange();
    GLfloat getYChange();
    void swapBuffers() { glfwSwapBuffers(mainWindow); }
    
    // Métodos para obtener los ángulos de nuestro perro
    GLfloat getGiroCabeza() { return giroCabeza; }
    GLfloat getGiroMandibula() { return giroMandibula; }
    GLfloat getGiroCola() { return giroCola; } // Nuevo método para la cola
    GLfloat getGiroPataTrasDer() { return giroPataTrasDer; }
    GLfloat getGiroPataTrasIzq() { return giroPataTrasIzq; }
    GLfloat getGiroPataDelDer() { return giroPataDelDer; }
    GLfloat getGiroPataDelIzq() { return giroPataDelIzq; }

    ~Window();

private:
    GLFWwindow* mainWindow;
    GLint width, height;
    
    // Variables de rotación personalizadas
    GLfloat giroCabeza, giroMandibula, giroCola;
    GLfloat giroPataTrasDer, giroPataTrasIzq, giroPataDelDer, giroPataDelIzq;
    
    bool teclas[1024];
    GLint bufferWidth, bufferHeight;
    GLfloat ultimaX, ultimaY;
    GLfloat cambioX, cambioY;
    bool primerMovimientoMouse;
    
    void createCallbacks();
    static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
    static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};