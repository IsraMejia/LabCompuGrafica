#include "Window.h"

Window::Window()
{
    width = 800;
    height = 600;

    // --- EL SALVAVIDAS ---
    mainWindow = nullptr; 

    // Inicializando variables de control
    bufferWidth = 0;
    bufferHeight = 0;
    cambioX = 0.0f;
    cambioY = 0.0f;
    ultimaX = 0.0f;
    ultimaY = 0.0f;
    primerMovimientoMouse = true;
    
    // Iniciar el perro en posición neutral
    giroCabeza = 0.0f;
    giroMandibula = 0.0f;
    giroCola = 0.0f; 
    giroPataDelDer = 0.0f;
    giroPataDelIzq = 0.0f;
    giroPataTrasDer = 0.0f;
    giroPataTrasIzq = 0.0f;

    for (size_t i = 0; i < 1024; i++) { teclas[i] = 0; }
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;

    // --- EL SALVAVIDAS ---
    mainWindow = nullptr; 
    
    // Inicializando variables de control
    bufferWidth = 0;
    bufferHeight = 0;
    cambioX = 0.0f;
    cambioY = 0.0f;
    ultimaX = 0.0f;
    ultimaY = 0.0f;
    primerMovimientoMouse = true;

    // Iniciar el perro en posición neutral
    giroCabeza = 0.0f;
    giroMandibula = 0.0f;
    giroCola = 0.0f; 
    giroPataDelDer = 0.0f;
    giroPataDelIzq = 0.0f;
    giroPataTrasDer = 0.0f;
    giroPataTrasIzq = 0.0f;

    for (size_t i = 0; i < 1024; i++) { teclas[i] = 0; }
}

int Window::Initialise()
{
    if (!glfwInit())
    {
        printf("Error: No se pudo inicializar GLFW");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "Ej 5 - Goddard Israel Mejia", NULL, NULL);

    if (!mainWindow)
    {
        printf("Error: Falla en la creacion de la ventana");
        glfwTerminate();
        return 1;
    }

    GLFWmonitor* monitorPrimario = glfwGetPrimaryMonitor();
    const GLFWvidmode* modoVideo = glfwGetVideoMode(monitorPrimario);
    
    // Calculamos el centro exacto de la pantalla
    int posicionX = (modoVideo->width - width) / 2;
    int posicionY = (modoVideo->height - height) / 2;
    
    // Movemos la ventana a esa posición
    glfwSetWindowPos(mainWindow, posicionX, posicionY);

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(mainWindow);

    createCallbacks();

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        printf("Error: Fallo al inicializar GLEW");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, bufferWidth, bufferHeight);
    glfwSetWindowUserPointer(mainWindow, this);
    
    return 0;
}

void Window::createCallbacks()
{
    glfwSetKeyCallback(mainWindow, ManejaTeclado);
    glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
    GLfloat cambioActual = cambioX;
    cambioX = 0.0f;
    return cambioActual;
}

GLfloat Window::getYChange()
{
    GLfloat cambioActual = cambioY;
    cambioY = 0.0f;
    return cambioActual;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
    Window* miVentana = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // ==== CONTROLES DEL MODELO ROBOTICO ====

    // Cabeza (Y/U)
    if (key == GLFW_KEY_Y && miVentana->giroCabeza < 45.0f) { miVentana->giroCabeza += 10.0f; }
    if (key == GLFW_KEY_U && miVentana->giroCabeza > -45.0f) { miVentana->giroCabeza -= 10.0f; }

    // Mandibula (I/O)
    if (key == GLFW_KEY_I && miVentana->giroMandibula < 45.0f) { miVentana->giroMandibula += 10.0f; }
    if (key == GLFW_KEY_O && miVentana->giroMandibula > -10.0f) { miVentana->giroMandibula -= 10.0f; }

    // Cola (T/G) -> Mover de lado a lado (-45 a 45 grados)
    if (key == GLFW_KEY_T && miVentana->giroCola < 45.0f) { miVentana->giroCola += 10.0f; }
    if (key == GLFW_KEY_G && miVentana->giroCola > -45.0f) { miVentana->giroCola -= 10.0f; }

    // Piernas Delanteras (H/J para la Derecha | K/L para la Izquierda)
    if (key == GLFW_KEY_H && miVentana->giroPataDelDer < 45.0f) { miVentana->giroPataDelDer += 10.0f; }
    if (key == GLFW_KEY_J && miVentana->giroPataDelDer > 0.0f) { miVentana->giroPataDelDer -= 10.0f; }
    
    if (key == GLFW_KEY_K && miVentana->giroPataDelIzq < 45.0f) { miVentana->giroPataDelIzq += 10.0f; }
    if (key == GLFW_KEY_L && miVentana->giroPataDelIzq > 0.0f) { miVentana->giroPataDelIzq -= 10.0f; }

    // Piernas Traseras (V/B para la Derecha | N/M para la Izquierda)
    if (key == GLFW_KEY_V && miVentana->giroPataTrasDer < 45.0f) { miVentana->giroPataTrasDer += 10.0f; }
    if (key == GLFW_KEY_B && miVentana->giroPataTrasDer > 0.0f) { miVentana->giroPataTrasDer -= 10.0f; }

    if (key == GLFW_KEY_N && miVentana->giroPataTrasIzq < 45.0f) { miVentana->giroPataTrasIzq += 10.0f; }
    if (key == GLFW_KEY_M && miVentana->giroPataTrasIzq > 0.0f) { miVentana->giroPataTrasIzq -= 10.0f; }

    // Guardar estado de la tecla
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS) { miVentana->teclas[key] = true; }
        else if (action == GLFW_RELEASE) { miVentana->teclas[key] = false; }
    }
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
    Window* miVentana = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (miVentana->primerMovimientoMouse)
    {
        miVentana->ultimaX = xPos;
        miVentana->ultimaY = yPos;
        miVentana->primerMovimientoMouse = false;
    }

    miVentana->cambioX = xPos - miVentana->ultimaX;
    miVentana->cambioY = miVentana->ultimaY - yPos;

    miVentana->ultimaX = xPos;
    miVentana->ultimaY = yPos;
}

Window::~Window()
{
    // --- LA VALIDACIÓN DE VIDA O MUERTE ---
    if (mainWindow != nullptr)
    {
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
    }
}