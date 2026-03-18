/*Práctica 4: Modelado Jerárquico.
FASE 2: Cinemática Directa Manual Simplificada (Sin Frenos).
*/
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtc\random.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "Sphere.h"
#include "Window.h"
#include "Camera.h"

using std::vector;
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
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

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}

// Crear cilindro
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

// función para crear un cono
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

// función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

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
void CreateShaders() { Shader* shader1 = new Shader(); shader1->CreateFromFiles(vShader, fShader); shaderList.push_back(*shader1); }


// ====================================================================================================
// [CONCEPTO] FUNCIONES MODULARES COMO "RAMAS" DEL ÁRBOL JERÁRQUICO
// Al pasar `glm::mat4 model` por valor (no por referencia), estamos creando una copia local 
// de la matriz. Esto equivale al antiguo `glPushMatrix()`. Así, las transformaciones 
// que hacemos dentro de estas funciones afectan solo a esta "rama" (hijo) y no al "tronco" (padre).
// ====================================================================================================

void DibujarOjo(glm::mat4 model, glm::vec3 offset, GLuint uniformModel, GLuint uniformColor, glm::vec3 colorGlobo, glm::vec3 colorPupila) {
    // 1. HERENCIA: Nos ubicamos respecto al sistema de coordenadas de la Cabeza (Padre)
    model = glm::translate(model, offset);

    // [CONCEPTO] SEPARACIÓN DE ESCALA:
    // Hacemos una matriz 'modelGlobo' solo para escalar y dibujar. Si escalaramos la matriz
    // 'model' principal, la pupila heredaría esa deformación.
    glm::mat4 modelGlobo = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelGlobo));
    glUniform3fv(uniformColor, 1, glm::value_ptr(colorGlobo));
    sp.render();
    
    // NODO HIJO: Pupila (Nace a partir del centro del ojo, no del centro del mundo)
    glm::mat4 modelPupila = glm::translate(model, glm::vec3(0.18f, 0.0f, 0.0f));
    modelPupila = glm::scale(modelPupila, glm::vec3(0.15f, 0.15f, 0.15f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPupila));
    glUniform3fv(uniformColor, 1, glm::value_ptr(colorPupila));
    sp.render();
}

void DibujarPata(glm::mat4 model, glm::vec3 offset, float anguloHombro, float anguloCodo, GLuint uniformModel, GLuint uniformColor, glm::vec3 colorPata, glm::vec3 colorArticulacion) {
    // 1. ESPACIO LOCAL: 'model' llega con las coordenadas y rotación del Tronco.
    // Al aplicar translate, el (0,0,0) local de la pata ahora es la esquina del tronco.
    model = glm::translate(model, offset); 
    
    // 2. Nivel 1 (Hombro): Aplicamos el Grado de Libertad del teclado.
    model = glm::rotate(model, glm::radians(anguloHombro), glm::vec3(0.0f, 0.0f, 1.0f));
    
    // 'modelHombro' guarda la matriz que ya incluye posición global + posición relativa + rotación
    glm::mat4 modelHombro = model; 
    
    // DIBUJAR ARTICULACIÓN
    glm::mat4 modelEsfera1 = glm::scale(modelHombro, glm::vec3(0.6f, 0.6f, 0.6f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsfera1));
    glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
    sp.render();
    
    // 3. Nivel 2 (Muslo): Depende del Hombro.
    // El vector (-0.55f) nos mueve hacia abajo en el eje Y "local" del hombro.
    modelHombro = glm::translate(modelHombro, glm::vec3(0.0f, -0.55f, 0.0f)); 
    glm::mat4 modelMuslo = glm::scale(modelHombro, glm::vec3(0.8f, 0.9f, 0.8f)); 
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMuslo));
    glUniform3fv(uniformColor, 1, glm::value_ptr(colorPata));
    meshList[0]->RenderMesh(); 
    
    // 4. Nivel 3 (Codo): Es HIJO del Muslo/Hombro.
    // Primero trasladamos al final del muslo (-0.55 local), LUEGO rotamos.
    // [CONCEPTO] ORDEN MATRICIAL: T * R. Esto asegura que rote como bisagra al final del hueso.
    glm::mat4 modelCodo = glm::translate(modelHombro, glm::vec3(0.0f, -0.55f, 0.0f)); 
    modelCodo = glm::rotate(modelCodo, glm::radians(anguloCodo), glm::vec3(0.0f, 0.0f, 1.0f));
    
    glm::mat4 modelEsfera2 = glm::scale(modelCodo, glm::vec3(0.5f, 0.5f, 0.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsfera2));
    glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
    sp.render();
    
    // 5. Nivel 4 (Pantorrilla): Es HIJO del Codo.
    // Hereda todas las transformaciones anteriores (rotación hombro + traslación muslo + rotación codo).
    glm::mat4 modelPantorrilla = glm::translate(modelCodo, glm::vec3(0.0f, -0.55f, 0.0f));
    modelPantorrilla = glm::scale(modelPantorrilla, glm::vec3(0.6f, 0.9f, 0.6f)); 
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPantorrilla));
    glUniform3fv(uniformColor, 1, glm::value_ptr(colorPata));
    meshList[0]->RenderMesh(); 
}

void DibujarCola(glm::mat4 model, glm::vec3 offset, float anguloBase, float anguloMedio, GLuint uniformModel, GLuint uniformColor, glm::vec3 colorCuerpo, glm::vec3 colorEspina, glm::vec3 colorArticulacion) {
    // 1. Nivel Base Cola
    model = glm::translate(model, offset);
    model = glm::rotate(model, glm::radians(anguloBase), glm::vec3(0.0f, 1.0f, 0.0f)); // Rota en Y para barrer izq/der
    glm::mat4 modelBaseCola = model;
    
    glm::mat4 modelEsfera1 = glm::scale(modelBaseCola, glm::vec3(0.7f, 0.7f, 0.7f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsfera1));
    glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
    sp.render();

    // 2. Segmento 1
    modelBaseCola = glm::translate(modelBaseCola, glm::vec3(-1.5f, 0.0f, 0.0f));
    glm::mat4 modelSeg1 = glm::scale(modelBaseCola, glm::vec3(3.0f, 1.2f, 1.2f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelSeg1));
    glUniform3fv(uniformColor, 1, glm::value_ptr(colorCuerpo));
    meshList[0]->RenderMesh();

    // 2.1 Hijas del Segmento 1 (Espinas)
    for (int i = 0; i < 2; i++) {
        glm::mat4 mEspina = glm::translate(modelBaseCola, glm::vec3(-0.5f + (i * 1.0f), 0.7f, 0.0f));
        mEspina = glm::scale(mEspina, glm::vec3(0.8f, 0.8f, 0.8f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mEspina));
        glUniform3fv(uniformColor, 1, glm::value_ptr(colorEspina));
        meshList[4]->RenderMesh(); 
    }

    // 3. Articulación Media (Hijo del Segmento 1)
    glm::mat4 modelMedia = glm::translate(modelBaseCola, glm::vec3(-1.5f, 0.0f, 0.0f));
    modelMedia = glm::rotate(modelMedia, glm::radians(anguloMedio), glm::vec3(0.0f, 1.0f, 0.0f));
    
    glm::mat4 modelEsfera2 = glm::scale(modelMedia, glm::vec3(0.5f, 0.5f, 0.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsfera2));
    glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
    sp.render();

    // 4. Segmento 2 (Hijo de la Articulación Media)
    glm::mat4 modelSeg2 = glm::translate(modelMedia, glm::vec3(-1.2f, 0.0f, 0.0f));
    glm::mat4 modelDrawSeg2 = glm::scale(modelSeg2, glm::vec3(2.4f, 0.8f, 0.8f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelDrawSeg2));
    glUniform3fv(uniformColor, 1, glm::value_ptr(colorCuerpo));
    meshList[0]->RenderMesh();

    // 4.1 Hija del Segmento 2 (Espina Final)
    glm::mat4 mEspina2 = glm::translate(modelSeg2, glm::vec3(0.0f, 0.5f, 0.0f));
    mEspina2 = glm::scale(mEspina2, glm::vec3(0.6f, 0.6f, 0.6f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mEspina2));
    glUniform3fv(uniformColor, 1, glm::value_ptr(colorEspina));
    meshList[4]->RenderMesh();
}

int main()
{
    mainWindow = Window(800, 600);
    mainWindow.Initialise();
    
    // Carga de primitivas en arreglos Vertex Buffer Objects (VBO) y Vertex Array Objects (VAO)
    CrearCubo();                  
    CrearPiramideTriangular();    
    CrearCilindro(30, 1.0f);      
    CrearCono(25, 2.0f);          
    CrearPiramideCuadrangular();  
    CreateShaders();
    
    // Cámara y Matrices Globales
    camera = Camera(glm::vec3(-5.0f, 5.0f, 15.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -5.0f, 0.2f, 0.2f);
    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformColor = 0;
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
    
    sp.init(); sp.load(); 
    
    // 'model' es nuestra Matriz de Transformación base (Identidad)
    glm::mat4 model(1.0), modelaux(1.0);
    
    // Paleta de colores
    glm::vec3 colorVerdeOscuro = glm::vec3(0.0f, 0.4f, 0.0f), colorPatas = glm::vec3(0.1f, 0.3f, 0.2f);
    glm::vec3 colorEspinas = glm::vec3(0.0f, 0.2f, 0.0f), colorArticulacion = glm::vec3(0.3f, 0.6f, 0.15f);
    glm::vec3 colorOjos = glm::vec3(0.80f, 0.80f, 1.0f);

    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        deltaTime += (now - lastTime) / limitFPS;
        lastTime = now;

        glfwPollEvents();
        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shaderList[0].useShader();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectLocation();
        uniformView = shaderList[0].getViewLocation();
        uniformColor = shaderList[0].getColorLocation();
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

        // ====================================================================================================
        // [CONCEPTO] CONSTRUCCIÓN DEL ÁRBOL JERÁRQUICO
        // Las operaciones matriciales en OpenGL (GLM) se acumulan. 
        // Empezamos posicionando el "Padre de Todos" (Tronco) y a partir de él colgamos los hijos.
        // ====================================================================================================
        
        model = glm::mat4(1.0);
        // Movemos el sistema de coordenadas global hacia atrás (Z = -15)
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -15.0f)); 
        
        // [CONCEPTO] ANCLAJE (Save State):
        // modelaux guarda la posición central del cuerpo ANTES de escalar el tronco.
        // Equivalente a glPushMatrix(). Así los hijos no heredan la deformación de la escala.
        modelaux = model; 

        // 1. TRONCO (Nodo Raíz)
        model = glm::scale(model, glm::vec3(6.0f, 2.0f, 3.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(uniformColor, 1, glm::value_ptr(colorVerdeOscuro));
        meshList[0]->RenderMesh(); 
        
        // 2. ESPINAS LOMO (Hijas del Tronco)
        for (int i = 0; i < 5; i++) {
            model = modelaux; // Restauramos la posición central del tronco
            model = glm::translate(model, glm::vec3(-2.0f + (i * 1.0f), 1.3f, 0.0f)); // Posición local relativa
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniform3fv(uniformColor, 1, glm::value_ptr(colorEspinas));
            meshList[4]->RenderMesh(); 
        }

        // 3. CABEZA (Hija del Tronco)
        model = modelaux;
        model = glm::translate(model, glm::vec3(4.0f, 0.5f, 0.0f)); // Movimiento relativo (Adelante y arriba)
        glm::mat4 modelCabeza = model; // Nuevo anclaje para los sub-hijos (Hocico y Ojos)
        
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(uniformColor, 1, glm::value_ptr(colorVerdeOscuro));
        meshList[0]->RenderMesh();
        
        // 3.1 HOCICO (Hijo de la Cabeza)
        model = modelCabeza; // Partimos del sistema local de la cabeza
        model = glm::translate(model, glm::vec3(2.0f, -0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(2.5f, 1.0f, 1.5f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(uniformColor, 1, glm::value_ptr(colorVerdeOscuro));
        meshList[0]->RenderMesh();

        // 3.2 OJOS (Hijos de la Cabeza)
        // Ojo Derecho
        glm::mat4 baseOjoDer = glm::translate(modelCabeza, glm::vec3(0.8f, 1.0f, 1.0f));
        baseOjoDer = glm::rotate(baseOjoDer, glm::radians(-75.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        DibujarOjo(baseOjoDer, glm::vec3(0.0f, 0.0f, 0.0f), uniformModel, uniformColor, colorOjos, colorVerdeOscuro);

        // Ojo Izquierdo
        glm::mat4 baseOjoIzq = glm::translate(modelCabeza, glm::vec3(0.8f, 1.0f, -1.0f));
        baseOjoIzq = glm::rotate(baseOjoIzq, glm::radians(75.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        DibujarOjo(baseOjoIzq, glm::vec3(0.0f, 0.0f, 0.0f), uniformModel, uniformColor, colorOjos, colorVerdeOscuro);
        
        // --- 4. PATAS Y COLA (Hijas del Tronco Principal) ---
        // Se leen las variables dinámicas manipuladas por el teclado
        
        DibujarPata(modelaux, glm::vec3(2.5f, -0.5f, 1.8f), mainWindow.getHombro1(), mainWindow.getCodo1(), uniformModel, uniformColor, colorPatas, colorArticulacion);
        DibujarPata(modelaux, glm::vec3(2.5f, -0.5f, -1.8f), mainWindow.getHombro2(), mainWindow.getCodo2(), uniformModel, uniformColor, colorPatas, colorArticulacion);
        DibujarPata(modelaux, glm::vec3(-2.5f, -0.5f, 1.8f), mainWindow.getHombro3(), mainWindow.getCodo3(), uniformModel, uniformColor, colorPatas, colorArticulacion);
        DibujarPata(modelaux, glm::vec3(-2.5f, -0.5f, -1.8f), mainWindow.getHombro4(), mainWindow.getCodo4(), uniformModel, uniformColor, colorPatas, colorArticulacion);
        
        DibujarCola(modelaux, glm::vec3(-3.0f, 0.0f, 0.0f), mainWindow.getBaseCola(), mainWindow.getMediaCola(), uniformModel, uniformColor, colorVerdeOscuro, colorEspinas, colorArticulacion);

        glUseProgram(0);
        mainWindow.swapBuffers();
    }
    return 0;
}