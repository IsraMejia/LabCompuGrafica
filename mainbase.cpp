#include <stdio.h>
#include <glew.h>
#include <glfw3.h>
//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
//Salu2
int main() 
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//****  LAS SIGUIENTES 4 LÍNEAS SE COMENTAN EN DADO CASO DE QUE AL USUARIO NO LE FUNCIONE LA VENTANA Y PUEDA CONOCER LA VERSIÓN DE OPENGL QUE TIENE ****/

	//Asignando variables de GLFW y propiedades de ventana
		// En caso de que parpadee la ventana, comentar las siguientes líneas
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);
	//Apuntador al nombre de la ventana, tamaño, monitor y compartir contexto. 
	// Aqui se puede quitar los null para que se cree una ventana a pantalla completa y comparta contexto con otra ventana respectivamente

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);
	//Memoria de la ventana para renderizar


	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);
	// El contexto es el espacio de memoria donde se guardan los datos de OpenGL, como los shaders, texturas, etc. 
	// Es necesario asignar un contexto a la ventana para poder usar OpenGL en ella.


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);	// El origen en el centro de la ventana, y el tamaño del viewport es igual al tamaño del buffer de la ventana (tamaño inicial).
	
	//Mensajes de información de la tarjeta gráfica en la terminal
	printf("Version de Opengl: %s \n",glGetString(GL_VERSION));
	printf("Marca: %s \n", glGetString(GL_VENDOR));
	printf("Renderer: %s \n", glGetString(GL_RENDERER));
	printf("Shaders: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow)) 
	{
		//Recibir eventos del usuario
		glfwPollEvents();

		//Limpiar la ventana
		glClearColor(0.0f, 0.2f, 0.0f, 1.0f); //RGB y alfa (transparencia) para limpiar la ventana, en este caso se limpia con verde
		// Matriz de color 4x1 , cada componente va de 0.0 a 1.0, donde 0.0 es el valor mínimo

		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow); // Intercambiar el buffer de la ventana, es decir, mostrar lo que se ha dibujado en la ventana.
		//Existe otro buffer donde se procesa lo que se va a mostrar en la ventana, y el buffer de la ventana es el que se muestra al usuario. 
		// Al intercambiar los buffers, se muestra lo que se ha dibujado en el buffer de procesamiento en la ventana.

		//Despues del glfwSwapBuffers ya no se escribe mas codigo ya que se muestra lo que se ha dibujado en la ventana, y si se escribe código después de esto, no se mostrará en la ventana hasta el siguiente intercambio de buffers.

	}// mainWindow = True   se cierra


	return 0;
}