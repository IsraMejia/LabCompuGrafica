#include "Window.h"

Window::Window()
{
    width = 800;
    height = 600;

    rueda1 = 0.0f;
    rueda2 = 0.0f;
    rueda3 = 0.0f;
    rueda4 = 0.0f;

    mouseFirstMoved = true;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;
    rotax = 0.0f;
    rotay = 0.0f;
    rotaz = 0.0f;
    articulacion1 = 0.0f;
    articulacion2 = 0.0f;
    articulacion3 = 0.0f;
    articulacion4 = 0.0f;
    articulacion5 = 0.0f;
    articulacion6 = 0.0f;

    rueda1 = 0.0f;
    rueda2 = 0.0f;
    rueda3 = 0.0f;
    rueda4 = 0.0f;
    
    mouseFirstMoved = true;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
}

int Window::Initialise()
{
    //Inicialización de GLFW
    if (!glfwInit())
    {
        printf("Falló inicializar GLFW");
        glfwTerminate();
        return 1;
    }
    //Asignando variables de GLFW y propiedades de ventana
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //para solo usar el core profile de OpenGL y no tener retrocompatibilidad
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // --- CÓDIGO ORIGINAL COMENTADO PARA USO A FUTURO (Modo Ventana Pequeña) ---
    /*
    mainWindow = glfwCreateWindow(800, 600, "Practica 04: Modelado Jerárquico - Israel Mejia", NULL, NULL);
    */
    // --------------------------------------------------------------------------

    // --- NUEVA CREACIÓN DE VENTANA AL 90% DE LA PANTALLA ---
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    
    // Calculamos el 90% del ancho y alto del monitor actual
    width = (int)(mode->width * 0.9f);
    height = (int)(mode->height * 0.9f);
    
    // Creamos la ventana (al usar NULL en el penúltimo parámetro, forzamos que sea modo ventana y no Full Screen)
    mainWindow = glfwCreateWindow(width, height, "Practica 04: Modelado Jerárquico - Israel Mejia", NULL, NULL);
    
    // Centramos la ventana en el monitor
    glfwSetWindowPos(mainWindow, (mode->width - width) / 2, (mode->height - height) / 2);
    // -------------------------------------------------------

    if (!mainWindow)
    {
        printf("Fallo en crearse la ventana con GLFW");
        glfwTerminate();
        return 1;
    }
    //Obtener tamaño de Buffer
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    //asignar el contexto
    glfwMakeContextCurrent(mainWindow);

    // --- ATRAPA Y OCULTA EL MOUSE EN EL CENTRO ---
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // ---------------------------------------------

    //MANEJAR TECLADO y MOUSE
    createCallbacks();

    //permitir nuevas extensiones
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("Falló inicialización de GLEW");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
                             
    //Asignar Viewport
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    //Callback para detectar que se está usando la ventana
    glfwSetWindowUserPointer(mainWindow, this);
    
    return 0; // Retorno exitoso
}

void Window::createCallbacks()
{
    glfwSetKeyCallback(mainWindow, ManejaTeclado);
    glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat Window::getYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if (key == GLFW_KEY_E)
    {
        theWindow->rotax += 10.0;
    }
    if (key == GLFW_KEY_R)
    {
        theWindow->rotay += 10.0; //rotar sobre el eje y 10 grados
    }
    if (key == GLFW_KEY_T)
    {
        theWindow->rotaz += 10.0;
    }
    if (key == GLFW_KEY_F)
    {
        theWindow->articulacion1 += 10.0;
    }

    if (key == GLFW_KEY_G)
    {
        theWindow->articulacion2 += 10.0;
    }
    if (key == GLFW_KEY_H)
    {
        theWindow->articulacion3 += 10.0;
    }
    if (key == GLFW_KEY_J)
    {
        theWindow->articulacion4 += 10.0;
    }
    if (key == GLFW_KEY_K)
    {
        theWindow->articulacion5 += 10.0;
    }
    if (key == GLFW_KEY_L)
    {
        theWindow->articulacion6 += 10.0;
    }
    
    if (key == GLFW_KEY_V)
    {
        theWindow->rueda1 += 5.0f;
    }
    if (key == GLFW_KEY_B)
    {
        theWindow->rueda2 += 5.0f;
    }
    if (key == GLFW_KEY_N)
    {
        theWindow->rueda3 += 5.0f;
    }
    if (key == GLFW_KEY_M)
    {
        theWindow->rueda4 += 5.0f;
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
        }
    }
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMoved)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
} 