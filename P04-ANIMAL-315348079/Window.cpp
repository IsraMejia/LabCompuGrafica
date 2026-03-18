#include "Window.h"

Window::Window()
{
    width = 800;
    height = 600;
    mouseFirstMoved = true;
    for (size_t i = 0; i < 1024; i++) keys[i] = 0;

    // [CONCEPTO] POSTURA INICIAL (Bind Pose): 
    // Es la posición en la que el árbol jerárquico no tiene ninguna transformación 
    // de rotación aplicada a sus nodos hijos.
    hombro1 = codo1 = hombro2 = codo2 = hombro3 = codo3 = hombro4 = codo4 = 0.0f;
    baseCola = mediaCola = 0.0f;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;
    mouseFirstMoved = true;
    for (size_t i = 0; i < 1024; i++) keys[i] = 0;

    // Postura Inicial (Todos los ángulos en cero)
    hombro1 = codo1 = hombro2 = codo2 = hombro3 = codo3 = hombro4 = codo4 = 0.0f;
    baseCola = mediaCola = 0.0f;
}

int Window::Initialise()
{
    if (!glfwInit()) { printf("Falló inicializar GLFW"); glfwTerminate(); return 1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    width = (int)(mode->width * 0.9f);
    height = (int)(mode->height * 0.9f);
    
    mainWindow = glfwCreateWindow(width, height, "Practica 04: Cocodrilo Robot - Modelado Jerarquico", NULL, NULL);
    glfwSetWindowPos(mainWindow, (mode->width - width) / 2, (mode->height - height) / 2);

    if (!mainWindow) { printf("Fallo en crearse la ventana"); glfwTerminate(); return 1; }
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(mainWindow);
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    createCallbacks();
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { printf("Falló inicialización de GLEW"); glfwDestroyWindow(mainWindow); glfwTerminate(); return 1; }
    glEnable(GL_DEPTH_TEST); 
    glViewport(0, 0, bufferWidth, bufferHeight);
    glfwSetWindowUserPointer(mainWindow, this);
    return 0;
}

void Window::createCallbacks() { glfwSetKeyCallback(mainWindow, ManejaTeclado); glfwSetCursorPosCallback(mainWindow, ManejaMouse); }
GLfloat Window::getXChange() { GLfloat theChange = xChange; xChange = 0.0f; return theChange; }
GLfloat Window::getYChange() { GLfloat theChange = yChange; yChange = 0.0f; return theChange; }

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) { theWindow->keys[key] = true; }
        else if (action == GLFW_RELEASE) { theWindow->keys[key] = false; }
    }

    float velocidad = 2.0f;

    // ==============================================================================
    // [CONCEPTO] MANIPULACIÓN DEL ESTADO DE LA JERARQUÍA
    // Al presionar teclas, alteramos el valor de las articulaciones. Como el modelo
    // es jerárquico, alterar el "Hombro" automáticamente afectará la posición global
    // del "Codo" y la "Pantorrilla" durante la fase de renderizado.
    // NOTA: Movimiento continuo SIN topes anatómicos (restricciones).
    // ==============================================================================
    
    // Pata 1 (Delantera Izquierda) 
    if (theWindow->keys[GLFW_KEY_Z]) { theWindow->hombro1 += velocidad; }
    if (theWindow->keys[GLFW_KEY_X]) { theWindow->codo1 -= velocidad; }

    // Pata 2 (Delantera Derecha)
    if (theWindow->keys[GLFW_KEY_V]) { theWindow->hombro2 += velocidad; }
    if (theWindow->keys[GLFW_KEY_B]) { theWindow->codo2 -= velocidad; }

    // Pata 3 (Trasera Izquierda)
    if (theWindow->keys[GLFW_KEY_U]) { theWindow->hombro3 += velocidad; }
    if (theWindow->keys[GLFW_KEY_I]) { theWindow->codo3 -= velocidad; }

    // Pata 4 (Trasera Derecha) 
    if (theWindow->keys[GLFW_KEY_J]) { theWindow->hombro4 += velocidad; }
    if (theWindow->keys[GLFW_KEY_K]) { theWindow->codo4 -= velocidad; }

    // Cola Manual (H, Y) 
    if (theWindow->keys[GLFW_KEY_H]) { theWindow->baseCola += velocidad; }
    if (theWindow->keys[GLFW_KEY_Y]) { theWindow->baseCola -= velocidad; }

    // ==============================================================================
    // Movimientos Maestros (Alteran múltiples nodos de la jerarquía a la vez)
    // ==============================================================================

    if (theWindow->keys[GLFW_KEY_C]) { theWindow->hombro1 += velocidad; theWindow->codo1 -= velocidad; }
    if (theWindow->keys[GLFW_KEY_N]) { theWindow->hombro2 += velocidad; theWindow->codo2 -= velocidad; }
    if (theWindow->keys[GLFW_KEY_O]) { theWindow->hombro3 += velocidad; theWindow->codo3 -= velocidad; }
    if (theWindow->keys[GLFW_KEY_L]) { theWindow->hombro4 += velocidad; theWindow->codo4 -= velocidad; }
    
    // El coletazo maestro gira toda la jerarquía hacia el mismo lado
    if (theWindow->keys[GLFW_KEY_G]) { theWindow->baseCola += velocidad; theWindow->mediaCola += velocidad; }
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (theWindow->mouseFirstMoved) { theWindow->lastX = xPos; theWindow->lastY = yPos; theWindow->mouseFirstMoved = false; }
    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;
    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

Window::~Window() { glfwDestroyWindow(mainWindow); glfwTerminate(); }