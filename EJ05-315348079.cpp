/*
Práctica 5: Carga de modelos jerárquicos
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

// Referencias a los componentes de nuestro perro
Model CuerpoPrincipal;
Model ComponenteCabeza;
Model ComponenteMandibula;
Model ApendiceCola; // <-- Agregamos el modelo de la cola
Model ExtremidadFrontalDer;
Model ExtremidadFrontalIzq;
Model ExtremidadTraseraDer;
Model ExtremidadTraseraIzq;

Skybox cieloFondo;

GLfloat deltaTiempo = 0.0f;
GLfloat tiempoAnterior = 0.0f;
static double limiteFrames = 1.0 / 60.0;

// Rutas de Shaders
static const char* vertexShader = "shaders/shader_m.vert";
static const char* fragmentShader = "shaders/shader_m.frag";

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

void InicializarShaders()
{
    Shader* shaderBasico = new Shader();
    shaderBasico->CreateFromFiles(vertexShader, fragmentShader);
    listaShaders.push_back(*shaderBasico);
}

int main()
{
    // ========================================================
    // LOGICA PARA QUE LA VENTANA OCUPE EL 90% DE LA PANTALLA
    // ========================================================
    if (!glfwInit()) {
        printf("Error: Fallo crítico al invocar GLFW\n");
        return 1;
    }
    
    GLFWmonitor* monitorPrincipal = glfwGetPrimaryMonitor();
    const GLFWvidmode* modoVideo = glfwGetVideoMode(monitorPrincipal);
    
    int anchoCalculado = (int)(modoVideo->width * 0.9f);
    int altoCalculado = (int)(modoVideo->height * 0.9f);

    // Mandamos el ancho y alto calculados a nuestra clase Window
    mainWindow = Window(anchoCalculado, altoCalculado); 
    mainWindow.Initialise();

    PrepararGeometrias();
    InicializarShaders();

    camaraPrincipal = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.1f, 1.0f);

    // ========================================================
    // CARGA DE LOS ARCHIVOS .OBJ
    // ========================================================
    CuerpoPrincipal = Model();
    CuerpoPrincipal.LoadModel("Models/g_cuerpo.obj");
    
    ComponenteCabeza = Model();
    ComponenteCabeza.LoadModel("Models/g_cabeza.obj");
    
    ComponenteMandibula = Model();
    ComponenteMandibula.LoadModel("Models/g_mandibula.obj"); 
    
    ExtremidadTraseraIzq = Model();
    ExtremidadTraseraIzq.LoadModel("Models/g_pata_tra_izq.obj");
    
    ExtremidadTraseraDer = Model();
    ExtremidadTraseraDer.LoadModel("Models/g_pata_tra_der.obj");
    
    ExtremidadFrontalDer = Model();
    ExtremidadFrontalDer.LoadModel("Models/g_pata_del_der.obj");
    
    ExtremidadFrontalIzq = Model();
    ExtremidadFrontalIzq.LoadModel("Models/g_pata_del_izq.obj");

    // Skybox
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
    glm::mat4 pivoteBase(1.0); 
    glm::vec3 tinte = glm::vec3(1.0f, 1.0f, 1.0f);

    // Ciclo principal de renderizado
    while (!mainWindow.getShouldClose())
    {
        GLfloat momentoActual = glfwGetTime();
        deltaTiempo = momentoActual - tiempoAnterior;
        deltaTiempo += (momentoActual - tiempoAnterior) / limiteFrames;
        tiempoAnterior = momentoActual;

        glfwPollEvents();
        camaraPrincipal.keyControl(mainWindow.getsKeys(), deltaTiempo);
        camaraPrincipal.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        cieloFondo.DrawSkybox(camaraPrincipal.calculateViewMatrix(), proyeccionGlobal);

        listaShaders[0].UseShader();
        idModelo = listaShaders[0].GetModelLocation();
        idProyeccion = listaShaders[0].GetProjectionLocation();
        idVista = listaShaders[0].GetViewLocation();
        idColor = listaShaders[0].getColorLocation();

        glUniformMatrix4fv(idProyeccion, 1, GL_FALSE, glm::value_ptr(proyeccionGlobal));
        glUniformMatrix4fv(idVista, 1, GL_FALSE, glm::value_ptr(camaraPrincipal.calculateViewMatrix()));
        
        // --- DIBUJADO DEL PISO ---
        tinte = glm::vec3(0.3f, 0.3f, 0.3f); 
        modelo = glm::mat4(1.0);
        modelo = glm::translate(modelo, glm::vec3(0.0f, -2.0f, 0.0f));
        modelo = glm::scale(modelo, glm::vec3(30.0f, 1.0f, 30.0f));
        glUniformMatrix4fv(idModelo, 1, GL_FALSE, glm::value_ptr(modelo));
        glUniform3fv(idColor, 1, glm::value_ptr(tinte));
        arreglosMallas[0]->RenderMesh();

        // ========================================================
        // ENSAMBLAJE DEL MODELO (JERARQUÍA Y COLORIZACION)
        // ========================================================

        // 1. Torso Principal
        modelo = glm::mat4(1.0);
        modelo = glm::translate(modelo, glm::vec3(0.0f, 0.75f, -1.5f));
        pivoteBase = modelo; // Almacenamos el origen del cuerpo
        
        tinte = glm::vec3(0.2f, 0.4f, 0.8f); // Tono principal
        glUniform3fv(idColor, 1, glm::value_ptr(tinte));
        glUniformMatrix4fv(idModelo, 1, GL_FALSE, glm::value_ptr(modelo));
        CuerpoPrincipal.RenderModel();

        // 2. Sección del Craneo
        modelo = pivoteBase;
        modelo = glm::translate(modelo, glm::vec3(1.3f, 0.75f, 0.0f));
        modelo = glm::rotate(modelo, glm::radians(mainWindow.getGiroCabeza()), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 pivoteCabeza = modelo; // Necesario por si la mandíbula depende de la cabeza
        
        tinte = glm::vec3(0.5f, 0.1f, 0.5f);
        glUniform3fv(idColor, 1, glm::value_ptr(tinte));
        glUniformMatrix4fv(idModelo, 1, GL_FALSE, glm::value_ptr(modelo));
        ComponenteCabeza.RenderModel();

        // 3. Mandibula Inferior
        modelo = pivoteCabeza; // Relativo a la cabeza, no al cuerpo
        modelo = glm::translate(modelo, glm::vec3(1.5f, 0.25f, 0.0f));
        modelo = glm::rotate(modelo, glm::radians(mainWindow.getGiroMandibula()), glm::vec3(0.0f, 0.0f, -1.0f));
        
        tinte = glm::vec3(0.9f, 0.9f, 0.1f); 
        glUniform3fv(idColor, 1, glm::value_ptr(tinte));
        glUniformMatrix4fv(idModelo, 1, GL_FALSE, glm::value_ptr(modelo));
        ComponenteMandibula.RenderModel();
 

        // 5. Soporte Trasero Izquierdo
        modelo = pivoteBase;
        modelo = glm::translate(modelo, glm::vec3(-0.5f, -1.2f, -0.6f));
        modelo = glm::rotate(modelo, glm::radians(mainWindow.getGiroPataTrasIzq()), glm::vec3(0.0f, 0.0f, 1.0f));
        
        tinte = glm::vec3(0.1f, 0.8f, 0.8f); 
        glUniform3fv(idColor, 1, glm::value_ptr(tinte));
        glUniformMatrix4fv(idModelo, 1, GL_FALSE, glm::value_ptr(modelo));
        ExtremidadTraseraIzq.RenderModel();

        // 6. Soporte Trasero Derecho
        modelo = pivoteBase;
        modelo = glm::translate(modelo, glm::vec3(-0.5f, -1.2f, 0.6f));
        modelo = glm::rotate(modelo, glm::radians(mainWindow.getGiroPataTrasDer()), glm::vec3(0.0f, 0.0f, 1.0f));
        
        tinte = glm::vec3(0.1f, 0.8f, 0.1f);
        glUniform3fv(idColor, 1, glm::value_ptr(tinte));
        glUniformMatrix4fv(idModelo, 1, GL_FALSE, glm::value_ptr(modelo));
        ExtremidadTraseraDer.RenderModel();

        // 7. Soporte Frontal Izquierdo
        modelo = pivoteBase;
        modelo = glm::translate(modelo, glm::vec3(1.0f, -0.55f, -0.6f));
        modelo = glm::rotate(modelo, glm::radians(mainWindow.getGiroPataDelIzq()), glm::vec3(0.0f, 0.0f, 1.0f));
        
        tinte = glm::vec3(0.9f, 0.5f, 0.1f);
        glUniform3fv(idColor, 1, glm::value_ptr(tinte));
        glUniformMatrix4fv(idModelo, 1, GL_FALSE, glm::value_ptr(modelo));
        ExtremidadFrontalIzq.RenderModel();

        // 8. Soporte Frontal Derecho
        modelo = pivoteBase;
        modelo = glm::translate(modelo, glm::vec3(1.0f, -0.55f, 0.6f));
        modelo = glm::rotate(modelo, glm::radians(mainWindow.getGiroPataDelDer()), glm::vec3(0.0f, 0.0f, 1.0f));
        
        tinte = glm::vec3(0.9f, 0.1f, 0.1f);
        glUniform3fv(idColor, 1, glm::value_ptr(tinte));
        glUniformMatrix4fv(idModelo, 1, GL_FALSE, glm::value_ptr(modelo));
        ExtremidadFrontalDer.RenderModel();

        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}