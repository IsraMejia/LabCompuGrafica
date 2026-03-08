//práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"

using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;

//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2, 2, 3, 0,
		// right
		1, 5, 6, 6, 2, 1,
		// back
		7, 6, 5, 5, 4, 7,
		// left
		4, 0, 3, 3, 7, 4,
		// bottom
		4, 5, 1, 1, 0, 4,
		// top
		3, 2, 6, 6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2, 1,3,2, 3,0,2, 1,0,3
	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, -0.25f,
		0.0f, -0.5f, -0.5f,
	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);
}

void CrearCilindro(int res, float R) {
	int n, i;
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0: vertices.push_back(x); break;
			case 1: vertices.push_back(y); break;
			case 2: vertices.push_back(z); break;
			case 3: vertices.push_back(x); break;
			case 4: vertices.push_back(0.5); break;
			case 5: vertices.push_back(z); break;
			}
		}
	}

	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0: vertices.push_back(x); break;
			case 1: vertices.push_back(-0.5f); break;
			case 2: vertices.push_back(z); break;
			}
		}
	}

	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0: vertices.push_back(x); break;
			case 1: vertices.push_back(0.5); break;
			case 2: vertices.push_back(z); break;
			}
		}
	}

	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

void CrearCono(int res, float R) {
	int n, i;
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0: vertices.push_back(x); break;
			case 1: vertices.push_back(y); break;
			case 2: vertices.push_back(z); break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);

	for (i = 0; i < res + 2; i++) indices.push_back(i);

	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4, 3,2,4, 2,1,4, 1,0,4, 0,1,2, 0,2,4
	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}

int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();

	CrearCubo();                  // índice 0
	CrearPiramideTriangular();    // índice 1 
	CrearCilindro(20, 1.0f);      // índice 2 (aumenté la resolución a 20 para que parezca cilindro)
	CrearCono(25, 2.0f);          // índice 3 
	CrearPiramideCuadrangular();  // índice 4 
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	sp.init();
	sp.load();

	glm::mat4 baseModel(1.0);
	glm::mat4 modelAux(1.0);
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Un fondo un poco más claro
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		// MATRIZ BASE PARA ROTAR TODA LA ESCENA JUNTA
		baseModel = glm::mat4(1.0);
		baseModel = glm::translate(baseModel, glm::vec3(0.0f, -0.5f, -6.0f));
		baseModel = glm::rotate(baseModel, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		baseModel = glm::rotate(baseModel, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		baseModel = glm::rotate(baseModel, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// -----------------------------------------------------------------
		// 1. EL PISO (Cubo aplanado)
		// -----------------------------------------------------------------
		modelAux = glm::translate(baseModel, glm::vec3(0.0f, -0.55f, 0.0f));
		modelAux = glm::scale(modelAux, glm::vec3(5.0f, 0.1f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
		color = glm::vec3(0.5f, 0.5f, 0.5f); // Gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		// -----------------------------------------------------------------
		// 2. CUERPO DE LA CASA (Cubo rojo)
		// -----------------------------------------------------------------
		modelAux = baseModel; // Posición original (0,0,0) respecto al centro
		modelAux = glm::scale(modelAux, glm::vec3(1.5f, 1.5f, 1.5f)); // Escala general de la casa
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
		color = glm::vec3(1.0f, 0.0f, 0.0f); // Rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		// -----------------------------------------------------------------
		// 3. TECHO (Pirámide cuadrangular azul)
		// -----------------------------------------------------------------
		modelAux = glm::translate(baseModel, glm::vec3(0.0f, 1.29f, 0.0f)); // Sube arriba del cubo
		modelAux = glm::scale(modelAux, glm::vec3(2.0f, 1.0f, 2.0f)); // Un poco más ancha que la casa
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
		color = glm::vec3(0.0f, 0.0f, 1.0f); // Azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMeshGeometry(); // Índice 4 es pirámide cuadrangular

		// -----------------------------------------------------------------
		// 4. PUERTA FRONTAL (Cubo verde)
		// -----------------------------------------------------------------
		modelAux = glm::translate(baseModel, glm::vec3(0.0f, -0.4f, 0.76f)); // Enfrente, al ras del piso
		modelAux = glm::scale(modelAux, glm::vec3(0.4f, 0.7f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
		color = glm::vec3(0.0f, 1.0f, 0.0f); // Verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		// -----------------------------------------------------------------
		// 5. VENTANA CIRCULAR TRASERA (Esfera azul aplanada)
		// -----------------------------------------------------------------
		modelAux = glm::translate(baseModel, glm::vec3(0.0f, 0.2f, -0.76f)); // Atrás, centrada
		modelAux = glm::scale(modelAux, glm::vec3(0.3f, 0.3f, 0.02f)); // Esfera aplanada en Z
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
		color = glm::vec3(0.0f, 0.0f, 1.0f); // Azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		// -----------------------------------------------------------------
		// 6. VENTANAS LATERALES (Cubos verdes, 2 por lado)
		// -----------------------------------------------------------------
		color = glm::vec3(0.0f, 1.0f, 0.0f); // Verde para todas las ventanas
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// -----------------------------------------------------------------
		// VENTANAS FRONTALES (Cubos verdes, 2 al frente)
		// -----------------------------------------------------------------

		// Frontal Izquierda
		modelAux = glm::translate(baseModel, glm::vec3(-0.35f, 0.2f, 0.76f)); // Desplazada a la izquierda en X, al frente en Z
		modelAux = glm::scale(modelAux, glm::vec3(0.4f, 0.4f, 0.05f));        // Plana en Z
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
		meshList[0]->RenderMesh();

		// Frontal Derecha
		modelAux = glm::translate(baseModel, glm::vec3(0.35f, 0.2f, 0.76f));  // Desplazada a la derecha en X, al frente en Z
		modelAux = glm::scale(modelAux, glm::vec3(0.4f, 0.4f, 0.05f));        // Plana en Z
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
		meshList[0]->RenderMesh();


		//Laterales

		// Izquierda 1
		modelAux = glm::translate(baseModel, glm::vec3(-0.76f, 0.2f, 0.3f));
		modelAux = glm::scale(modelAux, glm::vec3(0.05f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
		meshList[0]->RenderMesh();
		// Izquierda 2
		modelAux = glm::translate(baseModel, glm::vec3(-0.76f, 0.2f, -0.3f));
		modelAux = glm::scale(modelAux, glm::vec3(0.05f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
		meshList[0]->RenderMesh();

		// Derecha 1
		modelAux = glm::translate(baseModel, glm::vec3(0.76f, 0.2f, 0.3f));
		modelAux = glm::scale(modelAux, glm::vec3(0.05f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
		meshList[0]->RenderMesh();
		// Derecha 2
		modelAux = glm::translate(baseModel, glm::vec3(0.76f, 0.2f, -0.3f));
		modelAux = glm::scale(modelAux, glm::vec3(0.05f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
		meshList[0]->RenderMesh();

		// -----------------------------------------------------------------
		// 7. PINOS (Izquierdo y Derecho, centrados horizontalmente)
		// -----------------------------------------------------------------

		// TRONCO IZQUIERDO (Cilindro marrón)
		modelAux = glm::translate(baseModel, glm::vec3(-1.8f, -0.19f, 0.0f));
		modelAux = glm::scale(modelAux, glm::vec3(0.15f, 0.6f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
		color = glm::vec3(0.4f, 0.2f, 0.0f); // Marrón
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); // Índice 2 es Cilindro

		// COPA IZQUIERDA (Cono verde oscuro)
		modelAux = glm::translate(baseModel, glm::vec3(-1.8f, 0.3f, 0.0f));
		modelAux = glm::scale(modelAux, glm::vec3(0.3f, 1.0f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
		color = glm::vec3(0.0f, 0.5f, 0.0f); // Verde más oscuro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); 
		meshList[3]->RenderMeshGeometry(); // Índice 3 es Cono

		// TRONCO DERECHO
		modelAux = glm::translate(baseModel, glm::vec3(1.8f, -0.2f, 0.0f));
		modelAux = glm::scale(modelAux, glm::vec3(0.15f, 0.6f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
		color = glm::vec3(0.4f, 0.2f, 0.0f); // Marrón
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		// COPA DERECHA
		modelAux = glm::translate(baseModel, glm::vec3(1.8f, 0.3f, 0.0f));
		modelAux = glm::scale(modelAux, glm::vec3(0.3f, 1.0f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
		color = glm::vec3(0.0f, 0.5f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}