#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	mainWindow = nullptr; 
	bufferWidth = 0;
	bufferHeight = 0;
	cambioX = 0.0f;
	cambioY = 0.0f;
	ultimaX = 0.0f;
	ultimaY = 0.0f;
	primerMovimientoMouse = true;
	
	// ====== INICIALIZACIÓN DE CONTROL DEL CARRO ======
	movimientoZ = 0.0f;
	giroLlantas = 0.0f;
	giroCofre = 0.0f;

	velTraslacion = 0.1f; // Ajustar sensibilidad
	velGiro = 5.0f;       // Ajustar sensibilidad

	for (size_t i = 0; i < 1024; i++) { teclas[i] = 0; }
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	mainWindow = nullptr; 
	bufferWidth = 0;
	bufferHeight = 0;
	cambioX = 0.0f;
	cambioY = 0.0f;
	ultimaX = 0.0f;
	ultimaY = 0.0f;
	primerMovimientoMouse = true;

	// ====== INICIALIZACIÓN DE CONTROL DEL CARRO ======
	movimientoZ = 0.0f;
	giroLlantas = 0.0f;
	giroCofre = 0.0f;

	velTraslacion = 0.1f;
	velGiro = 5.0f;

	for (size_t i = 0; i < 1024; i++) { teclas[i] = 0; }
}

// ... (Métodos Initialise, createCallbacks, getXChange, getYChange, ManejaMouse quedan IGUAL que en Goddard) ...

 
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
	// Cambié el título de la ventana
	mainWindow = glfwCreateWindow(width, height, "Práctica 5 - Mustang Jerárquico - Israel Mejia", NULL, NULL);
	if (!mainWindow)
	{
		printf("Error: Falla en la creacion de la ventana");
		glfwTerminate();
		return 1;
	}
	GLFWmonitor* monitorPrimario = glfwGetPrimaryMonitor();
	const GLFWvidmode* modoVideo = glfwGetVideoMode(monitorPrimario);
	int posicionX = (modoVideo->width - width) / 2;
	int posicionY = (modoVideo->height - height) / 2;
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

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* miVentana = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// ==== NUEVOS CONTROLES DEL CARRO (Mano Derecha) ====

	// 1. Traslación del Chasis (I / K)
	if (key == GLFW_KEY_I)
	{
		miVentana->movimientoZ -= miVentana->velTraslacion; // Avanza
		miVentana->giroLlantas -= miVentana->velGiro;       // Hace que las llantas giren automáticamente
	}
	if (key == GLFW_KEY_K)
	{
		miVentana->movimientoZ += miVentana->velTraslacion; // Retrocede
		miVentana->giroLlantas += miVentana->velGiro;       // Llantas giran hacia atrás
	}

	// 2. Rotación independiente de Llantas (U / J) - Útil para probar Punto 2 sin moverse
	if (key == GLFW_KEY_U) { miVentana->giroLlantas -= miVentana->velGiro; }
	if (key == GLFW_KEY_J) { miVentana->giroLlantas += miVentana->velGiro; }

	// 3. Apertura/Cierre de Cofre (Y / H)
	// Limitamos la rotación para que no se pase del tope del motor o se meta a la cabina
	if (key == GLFW_KEY_Y && miVentana->giroCofre > -70.0f) 
	{ 
		miVentana->giroCofre -= miVentana->velGiro; // Abrir hacia arriba
	}
	if (key == GLFW_KEY_H && miVentana->giroCofre < 0.0f) 
	{ 
		miVentana->giroCofre += miVentana->velGiro; // Cerrar hasta 0
	}

	// Guardar estado de la tecla
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) { miVentana->teclas[key] = true; }
		else if (action == GLFW_RELEASE) { miVentana->teclas[key] = false; }
	}
}


Window::~Window()
{
	if (mainWindow != nullptr)
	{
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
	} 
}