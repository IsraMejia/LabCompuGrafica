//práctica 3: Modelado Geométrico y Cámara Sintética (Pyraminx Perfecto)
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al código
#include "Mesh.h"
#include "Shader.h"
#include "Sphere.h"
#include "Window.h"
#include "Camera.h"

using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; 
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
Sphere sp = Sphere(1.0, 20, 20); 

void CrearCubo() {
    unsigned int cubo_indices[] = {
        0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 7, 6, 5, 5, 4, 7, 
        4, 0, 3, 3, 7, 4, 4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3  
    };
    GLfloat cubo_vertices[] = {
        -0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f, 0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f
    };
    Mesh* cubo = new Mesh();
    cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
    meshList.push_back(cubo);
}

// Tetraedro regular perfecto (Geometría base para el Pyraminx)
void CrearPiramideTriangular() {
    unsigned int indices_piramide_triangular[] = {
            0,1,2, 1,3,2, 3,0,2, 1,0,3
    };
    GLfloat vertices_piramide_triangular[] = {
        -1.0f, 0.0f, 0.0f,    
         1.0f, 0.0f, 0.0f,    
         0.0f, 0.0f, -1.73205f, 
         0.0f, 1.63299f, -0.57735f, 
    };
    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
    meshList.push_back(obj1);
}

 

void CreateShaders() {
    Shader* shader1 = new Shader(); shader1->CreateFromFiles(vShader, fShader); shaderList.push_back(*shader1);
    Shader* shader2 = new Shader(); shader2->CreateFromFiles(vShaderColor, fShader); shaderList.push_back(*shader2);
}

// =========================================================================
//  Dibuja las caras de colores de la piramide
// =========================================================================
void DibujarCara(glm::mat4 matrizBaseCara, glm::vec3 colorFace, GLuint uniModel, GLuint uniColor, Mesh* mallaTetraedro, float escalaGap) {
    glUniform3fv(uniColor, 1, glm::value_ptr(colorFace));

    // 1. Vector Normal de la cara frontal. Nos servirá para "empujar" las piezas hacia afuera
    glm::vec3 frontNormal = glm::vec3(0.0f, 0.333333f, 0.942809f);
    
    // 2. Empujamos TODA la cara 0.005f hacia afuera. Sirve para evitar parpadeos de los bordes negros con las caras
    matrizBaseCara = glm::translate(matrizBaseCara, frontNormal * 0.005f);

    // 3. Centroide matemático de la cara frontal. Usaremos este punto para encoger
    // los triángulos hacia el centro, revelando el fondo negro en esquinas y bordes.
    glm::vec3 faceCentroid = glm::vec3(0.0f, 0.544331f, -0.192450f);

    // Posiciones exactas de las 6 pirámides apuntando hacia arriba
    glm::vec3 uprightPos[] = {
        glm::vec3(0.0f, 1.088662f, -0.384900f),
        glm::vec3(-0.333333f, 0.544331f, -0.192450f),
        glm::vec3(0.333333f, 0.544331f, -0.192450f),
        glm::vec3(-0.666667f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.666667f, 0.0f, 0.0f)
    };

    for (int i = 0; i < 6; i++) {
        glm::mat4 m = glm::translate(matrizBaseCara, uprightPos[i]);
        m = glm::scale(m, glm::vec3(0.333333f)); // Las vuelve 1/3 de su tamaño (encaje perfecto)
        
        // Magia: Encogemos la pieza hacia el centro de su propia cara frontal
        m = glm::translate(m, faceCentroid);
        m = glm::scale(m, glm::vec3(escalaGap)); 
        m = glm::translate(m, -faceCentroid);
        
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(m));
        mallaTetraedro->RenderMesh();
    }

    // Posiciones exactas de las 3 pirámides invertidas
    glm::vec3 invertedPos[] = {
        glm::vec3(0.0f, 1.088662f, -0.384900f),
        glm::vec3(-0.333333f, 0.544331f, -0.192450f),
        glm::vec3(0.333333f, 0.544331f, -0.192450f)
    };

    for (int i = 0; i < 3; i++) {
        glm::mat4 m = glm::translate(matrizBaseCara, invertedPos[i]);
        m = glm::rotate(m, glm::radians(180.0f), frontNormal); // La giramos para que apunte abajo
        m = glm::scale(m, glm::vec3(0.333333f));
        
        // Misma magia de encogimiento para mantener la simetría de los bordes negros
        m = glm::translate(m, faceCentroid);
        m = glm::scale(m, glm::vec3(escalaGap));
        m = glm::translate(m, -faceCentroid);
        
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(m));
        mallaTetraedro->RenderMesh();
    }
}
// =========================================================================

int main() {
    mainWindow = Window(800, 800);
    mainWindow.Initialise();

    CrearCubo();                  
    CrearPiramideTriangular();    // Índice 1 (Tetraedro base) 
    CreateShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -10.0f, 0.0f, 0.4f, 0.4f);

    GLuint uniformProjection = 0; GLuint uniformModel = 0; GLuint uniformView = 0; GLuint uniformColor = 0;
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    sp.init(); sp.load();

    glm::mat4 baseModel(1.0);
    glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

    // ESCALA DE BORDES: 0.82f indica que los triángulos de color ocupan el 82% de su espacio.
    // El 18% restante se vuelve un borde negro grueso y visible en TODOS lados.
    float sc = 0.82f; 

    while (!mainWindow.getShouldClose()) {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        deltaTime += (now - lastTime) / limitFPS;
        lastTime = now;

        glfwPollEvents();
        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        // Fondo oscuro para contrastar
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].useShader();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectLocation();
        uniformView = shaderList[0].getViewLocation();
        uniformColor = shaderList[0].getColorLocation();

        // -----------------------------------------------------------------
        // 1. MATRIZ BASE (Pivote central de todo el Pyraminx)
        // -----------------------------------------------------------------
        baseModel = glm::mat4(1.0);
        baseModel = glm::translate(baseModel, glm::vec3(0.0f, -0.5f, -5.5f)); 
        baseModel = glm::scale(baseModel, glm::vec3(1.4f, 1.4f, 1.4f)); 
        baseModel = glm::rotate(baseModel, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
        baseModel = glm::rotate(baseModel, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
        baseModel = glm::rotate(baseModel, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

        // -----------------------------------------------------------------
        // 2. NÚCLEO NEGRO SÓLIDO (Escala 1.0 perfecta)
        // -----------------------------------------------------------------
        glm::mat4 modelAux = baseModel;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
        color = glm::vec3(0.05f, 0.05f, 0.05f); // Negro
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh();

        // -----------------------------------------------------------------
        // 3. CARA FRONTAL (ROJA)
        // -----------------------------------------------------------------
        DibujarCara(baseModel, glm::vec3(1.0f, 0.0f, 0.0f), uniformModel, uniformColor, meshList[1], sc);

        // -----------------------------------------------------------------
        // 4. CARA DERECHA (AZUL)
        // -----------------------------------------------------------------
        glm::mat4 faceRight = baseModel;
        faceRight = glm::translate(faceRight, glm::vec3(0.0f, 0.0f, -0.57735f)); 
        faceRight = glm::rotate(faceRight, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        faceRight = glm::translate(faceRight, glm::vec3(0.0f, 0.0f, 0.57735f));
        DibujarCara(faceRight, glm::vec3(0.0f, 0.0f, 1.0f), uniformModel, uniformColor, meshList[1], sc);

        // -----------------------------------------------------------------
        // 5. CARA IZQUIERDA (AMARILLA)
        // -----------------------------------------------------------------
        glm::mat4 faceLeft = baseModel;
        faceLeft = glm::translate(faceLeft, glm::vec3(0.0f, 0.0f, -0.57735f)); 
        faceLeft = glm::rotate(faceLeft, glm::radians(240.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        faceLeft = glm::translate(faceLeft, glm::vec3(0.0f, 0.0f, 0.57735f));
        DibujarCara(faceLeft, glm::vec3(1.0f, 1.0f, 0.0f), uniformModel, uniformColor, meshList[1], sc);

        // -----------------------------------------------------------------
        // 6. CARA INFERIOR/BASE (VERDE)
        // -----------------------------------------------------------------
        glm::mat4 faceBottom = baseModel;
        glm::vec3 ejeSimetria = glm::vec3(0.0f, 0.57735f, -0.81649f);
        faceBottom = glm::rotate(faceBottom, glm::radians(180.0f), ejeSimetria);
        DibujarCara(faceBottom, glm::vec3(0.0f, 1.0f, 0.0f), uniformModel, uniformColor, meshList[1], sc);

        glUseProgram(0);
        mainWindow.swapBuffers();
    }
    return 0;
}