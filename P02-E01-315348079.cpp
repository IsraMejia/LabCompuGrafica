//Pr�ctica 2: �ndices, mesh, proyecciones, transformaciones geom�tricas
//Mejia Alba Israel Hipolito

//Ejercicio 1: Dibujar las iniciales de sus nombres, cada letra de un color diferente 

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>

// GLM para transformaciones  
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// Clases de utilidad
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

Window mainWindow;
std::vector<MeshColor*> meshColorList;
std::vector<Shader> shaderList;

// Rutas de Shaders
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";

void CrearLetrasIHM() {
    // Colores definidos en la práctica anterior
    float rI = 0.0f, gI = 0.7f, bI = 0.7f; // Cyan
    float rH = 0.9f, gH = 0.5f, bH = 0.0f; // Naranja
    float rM = 0.6f, gM = 0.8f, bM = 0.2f; // Lima

    GLfloat vertices[] = {
        // --- LETRA I (48 vértices) ---
        -0.7f, 0.3f, 0.0f, rI,gI,bI, -0.6f, 0.3f, 0.0f, rI,gI,bI, -0.6f, 0.4f, 0.0f, rI,gI,bI,
        -0.7f, 0.3f, 0.0f, rI,gI,bI, -0.6f, 0.4f, 0.0f, rI,gI,bI, -0.7f, 0.4f, 0.0f, rI,gI,bI,
        -0.6f, 0.3f, 0.0f, rI,gI,bI, -0.5f, 0.3f, 0.0f, rI,gI,bI, -0.5f, 0.4f, 0.0f, rI,gI,bI,
        -0.6f, 0.3f, 0.0f, rI,gI,bI, -0.5f, 0.4f, 0.0f, rI,gI,bI, -0.6f, 0.4f, 0.0f, rI,gI,bI,
        -0.5f, 0.3f, 0.0f, rI,gI,bI, -0.4f, 0.3f, 0.0f, rI,gI,bI, -0.4f, 0.4f, 0.0f, rI,gI,bI,
        -0.5f, 0.3f, 0.0f, rI,gI,bI, -0.4f, 0.4f, 0.0f, rI,gI,bI, -0.5f, 0.4f, 0.0f, rI,gI,bI,
        -0.6f, 0.2f, 0.0f, rI,gI,bI, -0.5f, 0.2f, 0.0f, rI,gI,bI, -0.5f, 0.3f, 0.0f, rI,gI,bI,
        -0.6f, 0.2f, 0.0f, rI,gI,bI, -0.5f, 0.3f, 0.0f, rI,gI,bI, -0.6f, 0.3f, 0.0f, rI,gI,bI,
        -0.6f, 0.1f, 0.0f, rI,gI,bI, -0.5f, 0.1f, 0.0f, rI,gI,bI, -0.5f, 0.2f, 0.0f, rI,gI,bI,
        -0.6f, 0.1f, 0.0f, rI,gI,bI, -0.5f, 0.2f, 0.0f, rI,gI,bI, -0.6f, 0.2f, 0.0f, rI,gI,bI,
        -0.7f, 0.0f, 0.0f, rI,gI,bI, -0.6f, 0.0f, 0.0f, rI,gI,bI, -0.6f, 0.1f, 0.0f, rI,gI,bI,
        -0.7f, 0.0f, 0.0f, rI,gI,bI, -0.6f, 0.1f, 0.0f, rI,gI,bI, -0.7f, 0.1f, 0.0f, rI,gI,bI,
        -0.6f, 0.0f, 0.0f, rI,gI,bI, -0.5f, 0.0f, 0.0f, rI,gI,bI, -0.5f, 0.1f, 0.0f, rI,gI,bI,
        -0.6f, 0.0f, 0.0f, rI,gI,bI, -0.5f, 0.1f, 0.0f, rI,gI,bI, -0.6f, 0.1f, 0.0f, rI,gI,bI,
        -0.5f, 0.0f, 0.0f, rI,gI,bI, -0.4f, 0.0f, 0.0f, rI,gI,bI, -0.4f, 0.1f, 0.0f, rI,gI,bI,
        -0.5f, 0.0f, 0.0f, rI,gI,bI, -0.4f, 0.1f, 0.0f, rI,gI,bI, -0.5f, 0.1f, 0.0f, rI,gI,bI,

        // --- LETRA H (54 vértices) ---
        -0.2f, 0.0f, 0.0f, rH,gH,bH, -0.1f, 0.0f, 0.0f, rH,gH,bH, -0.1f, 0.1f, 0.0f, rH,gH,bH,
        -0.2f, 0.0f, 0.0f, rH,gH,bH, -0.1f, 0.1f, 0.0f, rH,gH,bH, -0.2f, 0.1f, 0.0f, rH,gH,bH,
        -0.2f, 0.1f, 0.0f, rH,gH,bH, -0.1f, 0.1f, 0.0f, rH,gH,bH, -0.1f, 0.2f, 0.0f, rH,gH,bH,
        -0.2f, 0.1f, 0.0f, rH,gH,bH, -0.1f, 0.2f, 0.0f, rH,gH,bH, -0.2f, 0.2f, 0.0f, rH,gH,bH,
        -0.2f, 0.2f, 0.0f, rH,gH,bH, -0.1f, 0.2f, 0.0f, rH,gH,bH, -0.1f, 0.3f, 0.0f, rH,gH,bH,
        -0.2f, 0.2f, 0.0f, rH,gH,bH, -0.1f, 0.3f, 0.0f, rH,gH,bH, -0.2f, 0.3f, 0.0f, rH,gH,bH,
        -0.2f, 0.3f, 0.0f, rH,gH,bH, -0.1f, 0.3f, 0.0f, rH,gH,bH, -0.1f, 0.4f, 0.0f, rH,gH,bH,
        -0.2f, 0.3f, 0.0f, rH,gH,bH, -0.1f, 0.4f, 0.0f, rH,gH,bH, -0.2f, 0.4f, 0.0f, rH,gH,bH,
        -0.1f, 0.2f, 0.0f, rH,gH,bH,  0.0f, 0.2f, 0.0f, rH,gH,bH,  0.0f, 0.3f, 0.0f, rH,gH,bH,
        -0.1f, 0.2f, 0.0f, rH,gH,bH,  0.0f, 0.3f, 0.0f, rH,gH,bH, -0.1f, 0.3f, 0.0f, rH,gH,bH,
         0.0f, 0.0f, 0.0f, rH,gH,bH,  0.1f, 0.0f, 0.0f, rH,gH,bH,  0.1f, 0.1f, 0.0f, rH,gH,bH,
         0.0f, 0.0f, 0.0f, rH,gH,bH,  0.1f, 0.1f, 0.0f, rH,gH,bH,  0.0f, 0.1f, 0.0f, rH,gH,bH,
         0.0f, 0.1f, 0.0f, rH,gH,bH,  0.1f, 0.1f, 0.0f, rH,gH,bH,  0.1f, 0.2f, 0.0f, rH,gH,bH,
         0.0f, 0.1f, 0.0f, rH,gH,bH,  0.1f, 0.2f, 0.0f, rH,gH,bH,  0.0f, 0.2f, 0.0f, rH,gH,bH,
         0.0f, 0.2f, 0.0f, rH,gH,bH,  0.1f, 0.2f, 0.0f, rH,gH,bH,  0.1f, 0.3f, 0.0f, rH,gH,bH,
         0.0f, 0.2f, 0.0f, rH,gH,bH,  0.1f, 0.3f, 0.0f, rH,gH,bH,  0.0f, 0.3f, 0.0f, rH,gH,bH,
         0.0f, 0.3f, 0.0f, rH,gH,bH,  0.1f, 0.3f, 0.0f, rH,gH,bH,  0.1f, 0.4f, 0.0f, rH,gH,bH,
         0.0f, 0.3f, 0.0f, rH,gH,bH,  0.1f, 0.4f, 0.0f, rH,gH,bH,  0.0f, 0.4f, 0.0f, rH,gH,bH,

         // --- LETRA M (84 vértices) ---
         0.3f, 0.0f, 0.0f, rM,gM,bM, 0.4f, 0.0f, 0.0f, rM,gM,bM, 0.4f, 0.1f, 0.0f, rM,gM,bM,
         0.3f, 0.0f, 0.0f, rM,gM,bM, 0.4f, 0.1f, 0.0f, rM,gM,bM, 0.3f, 0.1f, 0.0f, rM,gM,bM,
         0.3f, 0.1f, 0.0f, rM,gM,bM, 0.4f, 0.1f, 0.0f, rM,gM,bM, 0.4f, 0.2f, 0.0f, rM,gM,bM,
         0.3f, 0.1f, 0.0f, rM,gM,bM, 0.4f, 0.2f, 0.0f, rM,gM,bM, 0.3f, 0.2f, 0.0f, rM,gM,bM,
         0.3f, 0.2f, 0.0f, rM,gM,bM, 0.4f, 0.2f, 0.0f, rM,gM,bM, 0.4f, 0.3f, 0.0f, rM,gM,bM,
         0.3f, 0.2f, 0.0f, rM,gM,bM, 0.4f, 0.3f, 0.0f, rM,gM,bM, 0.3f, 0.3f, 0.0f, rM,gM,bM,
         0.3f, 0.3f, 0.0f, rM,gM,bM, 0.4f, 0.3f, 0.0f, rM,gM,bM, 0.4f, 0.4f, 0.0f, rM,gM,bM,
         0.3f, 0.3f, 0.0f, rM,gM,bM, 0.4f, 0.4f, 0.0f, rM,gM,bM, 0.3f, 0.4f, 0.0f, rM,gM,bM,
         0.4f, 0.3f, 0.0f, rM,gM,bM, 0.5f, 0.3f, 0.0f, rM,gM,bM, 0.5f, 0.4f, 0.0f, rM,gM,bM,
         0.4f, 0.3f, 0.0f, rM,gM,bM, 0.5f, 0.4f, 0.0f, rM,gM,bM, 0.4f, 0.4f, 0.0f, rM,gM,bM,
         0.5f, 0.0f, 0.0f, rM,gM,bM, 0.6f, 0.0f, 0.0f, rM,gM,bM, 0.6f, 0.1f, 0.0f, rM,gM,bM,
         0.5f, 0.0f, 0.0f, rM,gM,bM, 0.6f, 0.1f, 0.0f, rM,gM,bM, 0.5f, 0.1f, 0.0f, rM,gM,bM,
         0.5f, 0.1f, 0.0f, rM,gM,bM, 0.6f, 0.1f, 0.0f, rM,gM,bM, 0.6f, 0.2f, 0.0f, rM,gM,bM,
         0.5f, 0.1f, 0.0f, rM,gM,bM, 0.6f, 0.2f, 0.0f, rM,gM,bM, 0.5f, 0.2f, 0.0f, rM,gM,bM,
         0.5f, 0.2f, 0.0f, rM,gM,bM, 0.6f, 0.2f, 0.0f, rM,gM,bM, 0.6f, 0.3f, 0.0f, rM,gM,bM,
         0.5f, 0.2f, 0.0f, rM,gM,bM, 0.6f, 0.3f, 0.0f, rM,gM,bM, 0.5f, 0.3f, 0.0f, rM,gM,bM,
         0.5f, 0.3f, 0.0f, rM,gM,bM, 0.6f, 0.3f, 0.0f, rM,gM,bM, 0.6f, 0.4f, 0.0f, rM,gM,bM,
         0.5f, 0.3f, 0.0f, rM,gM,bM, 0.6f, 0.4f, 0.0f, rM,gM,bM, 0.5f, 0.4f, 0.0f, rM,gM,bM,
         0.6f, 0.3f, 0.0f, rM,gM,bM, 0.7f, 0.3f, 0.0f, rM,gM,bM, 0.7f, 0.4f, 0.0f, rM,gM,bM,
         0.6f, 0.3f, 0.0f, rM,gM,bM, 0.7f, 0.4f, 0.0f, rM,gM,bM, 0.6f, 0.4f, 0.0f, rM,gM,bM,
         0.7f, 0.0f, 0.0f, rM,gM,bM, 0.8f, 0.0f, 0.0f, rM,gM,bM, 0.8f, 0.1f, 0.0f, rM,gM,bM,
         0.7f, 0.0f, 0.0f, rM,gM,bM, 0.8f, 0.1f, 0.0f, rM,gM,bM, 0.7f, 0.1f, 0.0f, rM,gM,bM,
         0.7f, 0.1f, 0.0f, rM,gM,bM, 0.8f, 0.1f, 0.0f, rM,gM,bM, 0.8f, 0.2f, 0.0f, rM,gM,bM,
         0.7f, 0.1f, 0.0f, rM,gM,bM, 0.8f, 0.2f, 0.0f, rM,gM,bM, 0.7f, 0.2f, 0.0f, rM,gM,bM,
         0.7f, 0.2f, 0.0f, rM,gM,bM, 0.8f, 0.2f, 0.0f, rM,gM,bM, 0.8f, 0.3f, 0.0f, rM,gM,bM,
         0.7f, 0.2f, 0.0f, rM,gM,bM, 0.8f, 0.3f, 0.0f, rM,gM,bM, 0.7f, 0.3f, 0.0f, rM,gM,bM,
         0.7f, 0.3f, 0.0f, rM,gM,bM, 0.8f, 0.3f, 0.0f, rM,gM,bM, 0.8f, 0.4f, 0.0f, rM,gM,bM,
         0.7f, 0.3f, 0.0f, rM,gM,bM, 0.8f, 0.4f, 0.0f, rM,gM,bM, 0.7f, 0.4f, 0.0f, rM,gM,bM
    };

    MeshColor* letras = new MeshColor();
    // 48 + 54 + 84 = 186 vértices * 6 (x,y,z,r,g,b) = 1116 floats
    letras->CreateMeshColor(vertices, 1116);
    meshColorList.push_back(letras);
}

void CreateShaders() {
    Shader* shaderColor = new Shader();
    shaderColor->CreateFromFiles(vShaderColor, fShaderColor);
    shaderList.push_back(*shaderColor);
}

int main() {
    mainWindow = Window(800, 800);
    mainWindow.Initialise();

    CrearLetrasIHM();
    CreateShaders();

    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;

    // MATRICES 
    // Usamos Ortho para que se vea 2D pero con profundidad para el pipeline
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

    while (!mainWindow.getShouldClose()) {
        glfwPollEvents();

        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].useShader();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectLocation();

        // Reiniciamos matriz de modelo (Identidad)
        glm::mat4 model(1.0f);

        // Traslación opcional para centrar o ajustar profundidad
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));

        // Enviamos las matrices al Shader
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        // Renderizado
        meshColorList[0]->RenderMeshColor();

        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}