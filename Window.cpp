#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	mueveMustang = 8.0f;
	mueveHelicoptero = 0.0f;
	mueveNave = 0.0f;
	direccionNave = 0;
	lamparaEncendida = true;
	focoPezEncendido = false;  // apagado por defecto, encender con F
	spotlightRojoPezEncendido = false; // apagado por defecto, encender con R
	spotlightRojoPezRotX = 0.0f;
	spotlightRojoPezRotY = -90.0f; // Apunta hacia abajo por defecto
	spotlightRojoPezRotZ = 0.0f;
	// Pez: empieza en el borde derecho de la pecera (Z local = +1.8, cerca del Agave)
	pezPosZ    = 1.8f;
	pezTiempo  = 0.0f;
	direccionPez = 0;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Obtener resolucion del monitor primario y calcular 90% de la pantalla
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
	width  = (GLint)(videoMode->width  * 0.9f);
	height = (GLint)(videoMode->height * 0.8f);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 09: Animacion    -   Israel Hipolito Mejia Alba  ", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);

	// Centrar la ventana en el monitor
	int monitorX, monitorY;
	glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);
	glfwSetWindowPos(mainWindow,
		monitorX + (videoMode->width  - width)  / 2,
		monitorY + (videoMode->height - height) / 2);

	// Centrar el mouse en la ventana al iniciar
	glfwFocusWindow(mainWindow);
	glfwSetCursorPos(mainWindow, width / 2.0, height / 2.0);
	lastX = width / 2.0f;
	lastY = height / 2.0f;
	mouseFirstMoved = false;
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
	if (key == GLFW_KEY_Y)
	{ 
		theWindow->muevex -= 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow->muevex += 1.0; 
	}
	 
	if (key == GLFW_KEY_J)
	{
		theWindow->mueveMustang += 1.0;
	}
	if (key == GLFW_KEY_H)
	{
		theWindow->mueveMustang -= 1.0;
	}
	// Helicoptero: M avanza, N retrocede
	if (key == GLFW_KEY_M)
	{
		theWindow->mueveHelicoptero += 1.0;
	}
	if (key == GLFW_KEY_N)
	{
		theWindow->mueveHelicoptero -= 1.0;
	}
	// Z: toggle lampara
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		theWindow->lamparaEncendida = !theWindow->lamparaEncendida;
	}
	// F: toggle luz puntual del foco del pez abisal
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		theWindow->focoPezEncendido = !theWindow->focoPezEncendido;
	}
	// R: toggle spotlight rojo del pez abisal
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		theWindow->spotlightRojoPezEncendido = !theWindow->spotlightRojoPezEncendido;
	}
	// G: rotar spotlight rojo en eje X
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		theWindow->spotlightRojoPezRotX += 15.0f;
		if (theWindow->spotlightRojoPezRotX >= 360.0f)
			theWindow->spotlightRojoPezRotX -= 360.0f;
	}
	// H: rotar spotlight rojo en eje Y
	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		theWindow->spotlightRojoPezRotY += 15.0f;
		if (theWindow->spotlightRojoPezRotY >= 360.0f)
			theWindow->spotlightRojoPezRotY -= 360.0f;
	}
	// T: rotar spotlight rojo en eje Z
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		theWindow->spotlightRojoPezRotZ += 15.0f;
		if (theWindow->spotlightRojoPezRotZ >= 360.0f)
			theWindow->spotlightRojoPezRotZ -= 360.0f;
	}
	// V: nave avanza (-X), B: nave retrocede (+X)
	if (key == GLFW_KEY_V)
	{
		theWindow->mueveNave -= 1.0;
		theWindow->direccionNave = -1;
	}
	if (key == GLFW_KEY_B)
	{
		theWindow->mueveNave += 1.0;
		theWindow->direccionNave = 1;
	}
	if ((key == GLFW_KEY_V || key == GLFW_KEY_B) && action == GLFW_RELEASE)
	{
		theWindow->direccionNave = 0;
	}

	// O: pez avanza (-Z), P: pez retrocede (+Z)
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
		theWindow->direccionPez = -1;
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		theWindow->direccionPez = 1;
	if ((key == GLFW_KEY_O || key == GLFW_KEY_P) && action == GLFW_RELEASE)
		theWindow->direccionPez = 0;



	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
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
