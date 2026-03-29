/*
Práctica 5: Carga de modelos jerárquicos - Transformación de Goddard a Mustang
Actualizado por CompuGrafica 👾 - Versión Gigante, Colores y Ajustes Finales de Jerarquía
*/
#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

const float conversorRadianes = 3.14159265f / 180.0f;
Window mainWindow;
std::vector<Mesh*> arreglosMallas;
std::vector<Shader> listaShaders;
Camera camaraPrincipal;

// ====== REFERENCIAS A LOS COMPONENTES DEL MUSTANG ======
Model CarroChasis;
Model CarroCofre;
Model LlantaDelDer;
Model LlantaDelIzq;
Model LlantaTraDer;
Model LlantaTraIzq;

Skybox cieloFondo;

GLfloat deltaTiempo = 0.0f;
GLfloat tiempoAnterior = 0.0f;
static double limiteFrames = 1.0 / 60.0;

// Rutas de Shaders (Se mantienen igual)
static const char* vertexShader = "shaders/shader_m.vert";
static const char* fragmentShader = "shaders/shader_m.frag";

// Función PrepararGeometrias (Piso) queda IGUAL
void PrepararGeometrias()
{
	unsigned int indicesSuelo[] = { 0, 2, 1, 1, 2, 3 };
	GLfloat verticesSuelo[] = {
		-10.0f, 0.0f, -10.0f,   0.0f, 0.0f,     0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,    10.0f, 0.0f,    0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,    0.0f, 10.0f,    0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,     10.0f, 10.0f,   0.0f, -1.0f, 0.0f
	};
	Mesh* mallaPiso = new Mesh();
	mallaPiso->CreateMesh(verticesSuelo, indicesSuelo, 32, 6);
	arreglosMallas.push_back(mallaPiso);
}

// Función InicializarShaders queda IGUAL
void InicializarShaders()
{
	Shader* shaderBasico = new Shader();
	shaderBasico->CreateFromFiles(vertexShader, fragmentShader);
	listaShaders.push_back(*shaderBasico);
}

int main()
{
	// ========================================================
	// LOGICA DE VENTANA (Reutilizada igual)
	// ========================================================
	if (!glfwInit()) {
		printf("Error: Fallo crítico al invocar GLFW\n");
		return 1;
	}

	GLFWmonitor* monitorPrincipal = glfwGetPrimaryMonitor();
	const GLFWvidmode* modoVideo = glfwGetVideoMode(monitorPrincipal);

	int anchoCalculado = (int)(modoVideo->width * 0.9f);
	int altoCalculado = (int)(modoVideo->height * 0.9f);
	mainWindow = Window(anchoCalculado, altoCalculado);
	mainWindow.Initialise();

	PrepararGeometrias();
	InicializarShaders();

	// Ajuste de cámara por escala jerárquica gigante (10x)
	camaraPrincipal = Camera(glm::vec3(0.0f, 15.0f, 80.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -10.0f, 1.0f, 1.0f);

	// ========================================================
	// CARGA DE MODELOS (Igual, son las mismas rutas)
	// ========================================================
	printf("Cargando modelos del Mustang Gigante...\n");

	CarroChasis = Model();
	CarroChasis.LoadModel("Models/mustang_sin_cofre.obj");

	CarroCofre = Model();
	CarroCofre.LoadModel("Models/cofre.obj");

	LlantaTraDer = Model();
	LlantaTraDer.LoadModel("Models/llanta_derecha_trasera.obj");

	LlantaDelIzq = Model();
	LlantaDelIzq.LoadModel("Models/llanta_izquierda.obj");
	

	LlantaTraIzq = Model();
	LlantaTraIzq.LoadModel("Models/llanta_izquierda_trasera.obj");

	LlantaDelDer = Model();
	LlantaDelDer.LoadModel("Models/llanta_derecha.obj");

	

	printf("Modelos cargados exitosamente.\n");

	// Skybox (Se mantiene igual, lake naranjo)
	std::vector<std::string> carasCielo;
	carasCielo.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	carasCielo.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	carasCielo.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	carasCielo.push_back("Textures/Skybox/cupertin-lake_up.tga");
	carasCielo.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	carasCielo.push_back("Textures/Skybox/cupertin-lake_ft.tga");
	cieloFondo = Skybox(carasCielo);

	GLuint idProyeccion = 0, idModelo = 0, idVista = 0;
	GLuint idColor = 0;
	glm::mat4 proyeccionGlobal = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 modelo(1.0);
	glm::mat4 pivoteChasis(1.0);

	glm::vec3 tinte = glm::vec3(1.0f, 1.0f, 1.0f);

	// Ciclo principal
	while (!mainWindow.getShouldClose())
	{
		GLfloat momentoActual = glfwGetTime();
		deltaTiempo = momentoActual - tiempoAnterior;
		// Corrección explícita de cast para eliminar advertencia C4244
		deltaTiempo += (GLfloat)((momentoActual - tiempoAnterior) / limiteFrames);
		tiempoAnterior = momentoActual;

		glfwPollEvents();
		camaraPrincipal.keyControl(mainWindow.getsKeys(), deltaTiempo);
		camaraPrincipal.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Fondo gris oscuro
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cieloFondo.DrawSkybox(camaraPrincipal.calculateViewMatrix(), proyeccionGlobal);

		listaShaders[0].UseShader();
		idModelo = listaShaders[0].GetModelLocation();
		idProyeccion = listaShaders[0].GetProjectionLocation();
		idVista = listaShaders[0].GetViewLocation();
		idColor = listaShaders[0].getColorLocation();

		glUniformMatrix4fv(idProyeccion, 1, GL_FALSE, glm::value_ptr(proyeccionGlobal));
		glUniformMatrix4fv(idVista, 1, GL_FALSE, glm::value_ptr(camaraPrincipal.calculateViewMatrix()));

		// --- 1. DIBUJO DEL PISO ---
		tinte = glm::vec3(0.01f, 0.9f, 0.02f); // Asfalto
		modelo = glm::mat4(1.0); 
		modelo = glm::translate(modelo, glm::vec3(0.0f, -2.5f, 0.0f));
		// Hacemos el piso mucho más grande.
		modelo = glm::scale(modelo, glm::vec3(200.0f, 1.0f, 200.0f));
		glUniformMatrix4fv(idModelo, 1, GL_FALSE, glm::value_ptr(modelo));
		glUniform3fv(idColor, 1, glm::value_ptr(tinte));
		arreglosMallas[0]->RenderMesh();



		float traslacionZ = mainWindow.getMovimientoZ();
		float rotacionLlantas = mainWindow.getGiroLlantas();
		float rotacionCofre = mainWindow.getGiroCofre(); 


		const float posX_WidthOffset = 4.9f;  // Ancho estimado del chasis (local)

		// !!! [NUEVOS VALORES] Llantas longitudinalmente: Aumentados para 'abrirlas' hacia los extremos de la salpicadera.
		const float posZ_RearOffset = 4.5f;  // Llantas traseras (Aumentado de 6.0f)
		const float posZ_FrontOffset = -4.5f; // Llantas delanteras (Aumentado de -7.0f)

		const float posY_HeightOffset = 0.5f; // Altura de ejes local (asume chasis centro en 0)


		// --------------------------------------------------------
		// 1. Chasis Principal (Padre) - COLOR DORADO
		// --------------------------------------------------------
		modelo = glm::mat4(1.0);
		// 1.1 Traslación (Controles del teclado)
		modelo = glm::translate(modelo, glm::vec3(0.0f, 0.0f, traslacionZ));
		// 1.2 Rotar 90 grados para arreglar el control invertido 
		modelo = glm::rotate(modelo, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		// 1.3 ESCALA GLOBAL GIGANTE (10x) (Punto 1)
		modelo = glm::scale(modelo, glm::vec3(10.0f, 10.0f, 10.0f));

		pivoteChasis = modelo; // Frame base para hijos (heredan escala, traslaciónZ y rotación)

		tinte = glm::vec3(1.0f, 0.84f, 0.0f); // Dorado
		glUniform3fv(idColor, 1, glm::value_ptr(tinte));
		glUniformMatrix4fv(idModelo, 1, GL_FALSE, glm::value_ptr(modelo));
		CarroChasis.RenderModel();

		// --------------------------------------------------------
		// 2. Cofre (Hijo) - COLOR AZUL
		// --------------------------------------------------------
		tinte = glm::vec3(0.0f, 0.0f, 1.0f); // Azul Puro

		modelo = pivoteChasis; // Heredamos 
		modelo = glm::translate(modelo, glm::vec3(0.0f, 0.005f, 0.875f));
		modelo = glm::rotate(modelo, glm::radians(rotacionCofre), glm::vec3(0.0f, 0.0f, -1.0f));
 
		glUniform3fv(idColor, 1, glm::value_ptr(tinte));
		glUniformMatrix4fv(idModelo, 1, GL_FALSE, glm::value_ptr(modelo));
		CarroCofre.RenderModel();


		// --------------------------------------------------------
		// 3. Llantas (Hijos) - COLOR MORADO
		// -------------------------------------------------------- 
		glUniform3fv(idColor, 1, glm::value_ptr(glm::vec3(0.5f, 0.0f, 0.5f))); // Morado

		// --- 3.1 Llanta Delantera Derecha ---
		modelo = pivoteChasis; // Herencia (escala 10x y traslaciónZ)
		// 1. Posicionar localmente en la salpicadera (Usando nuevos valores más cerrados y abiertos)
		modelo = glm::translate(modelo, glm::vec3(posX_WidthOffset, posY_HeightOffset, posZ_FrontOffset));
		// 2. Rotar para que ruede (Controles del teclado)
		modelo = glm::rotate(modelo, glm::radians(rotacionLlantas), glm::vec3(0.0f, 0.0f, 0.1f));

		glUniformMatrix4fv(idModelo, 1, GL_FALSE, glm::value_ptr(modelo));
		LlantaDelDer.RenderModel();

		// --- 3.2 Llanta Trasera Derecha ---
		modelo = pivoteChasis;
		modelo = glm::translate(modelo, glm::vec3(posX_WidthOffset, posY_HeightOffset, posZ_RearOffset));
		modelo = glm::rotate(modelo, glm::radians(rotacionLlantas), glm::vec3(0.0f, 0.0f, 0.1f));
		glUniformMatrix4fv(idModelo, 1, GL_FALSE, glm::value_ptr(modelo));
		LlantaTraDer.RenderModel();

		// --- 3.3 Llanta Delantera Izquierda ---
		modelo = pivoteChasis;
		modelo = glm::translate(modelo, glm::vec3(-posX_WidthOffset, posY_HeightOffset, posZ_FrontOffset));
		modelo = glm::rotate(modelo, glm::radians(rotacionLlantas), glm::vec3(0.0f, 0.0f, 0.1f));
		glUniformMatrix4fv(idModelo, 1, GL_FALSE, glm::value_ptr(modelo));
		LlantaDelIzq.RenderModel();

		// --- 3.4 Llanta Trasera Izquierda ---
		modelo = pivoteChasis;
		modelo = glm::translate(modelo, glm::vec3(-posX_WidthOffset, posY_HeightOffset, posZ_RearOffset));
		modelo = glm::rotate(modelo, glm::radians(rotacionLlantas), glm::vec3(0.0f, 0.0f, 0.1f));
		glUniformMatrix4fv(idModelo, 1, GL_FALSE, glm::value_ptr(modelo));
		LlantaTraIzq.RenderModel();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}