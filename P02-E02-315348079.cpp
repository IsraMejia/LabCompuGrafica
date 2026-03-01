// Práctica 3: Composición de Ilustración 3D de una Casa con Shaders de Colores Independientes y Prueba de Profundidad
//Mejia Alba Israel Hipolito
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>

// GLM para transformaciones matemáticas (Lo nuevo de Práctica 2)
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// Clases de utilidad del profesor para abstraer la complejidad de OpenGL
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

// Variables globales para la ventana y las listas de objetos y shaders
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

// Rutas de los Vertex Shaders (Un archivo independiente por color para evitar el "color clamp")
static const char* vRojo = "shaders/shaderrojo.vert";
static const char* vVerde = "shaders/shaderverde.vert";
static const char* vAzul = "shaders/shaderazul.vert";
static const char* vCafe = "shaders/shadercafe.vert";
static const char* vVerdeOscuro = "shaders/shaderverdeoscuro.vert";

// Ruta del Fragment Shader básico para que simplemente pasa el color calculado
static const char* fOriginal = "shaders/shader.frag";

// Función para crear las figuras geométricas básicas: Pirámide y Cubo
// 1. Pirámide triangular (Índice 0 en la lista)
void CreaPiramide() {
    unsigned int indices[] = {
        0,1,2,  1,3,2,  3,0,2,  1,0,3 // 4 caras triangulares
    };
    GLfloat vertices[] = {
        // X,      Y,      Z
        -0.5f, -0.5f,  0.0f,   // Vértice 0 (Base frontal izquierda)
         0.5f, -0.5f,  0.0f,   // Vértice 1 (Base frontal derecha)
         0.0f,  0.5f, -0.25f,  // Vértice 2 (Ápice o punta)
         0.0f, -0.5f, -0.5f    // Vértice 3 (Base trasera central)
    };
    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1); // Agregado como primer objeto
}

// 2. Cubo genérico (Índice 1 en la lista)
void CrearCubo() {
    // Índices para definir las 6 caras con 12 triángulos
    unsigned int cubo_indices[] = {
        0, 1, 2, 2, 3, 0, // Cara Frontal
        1, 5, 6, 6, 2, 1, // Cara Derecha
        7, 6, 5, 5, 4, 7, // Cara Trasera
        4, 0, 3, 3, 7, 4, // Cara Izquierda
        4, 5, 1, 1, 0, 4, // Cara Inferior
        3, 2, 6, 6, 7, 3  // Cara Superior
    };
    // Vértices para un cubo de 1x1x1 centrado en el origen (de -0.5 a 0.5)
    GLfloat cubo_vertices[] = {
        // X,      Y,      Z
        -0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,   0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f, // Vértices Frontales (0-3)
        -0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,   0.5f,  0.5f, -0.5f,  -0.5f,  0.5f, -0.5f  // Vértices Traseros (4-7)
    };
    Mesh* cubo = new Mesh();
    cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
    meshList.push_back(cubo); // Agregado como segundo objeto
}

// Función para cargar y vincular los programas de shaders. 
void CreateShaders() {
    const char* verts[] = {
        vRojo,         // Shaders de color independientes
        vVerde,
        vAzul,
        vCafe,
        vVerdeOscuro
    };

    for (int i = 0; i < 5; i++) {
        Shader* sh = new Shader();
        sh->CreateFromFiles(verts[i], fOriginal); // Vinculación de archivos
        shaderList.push_back(*sh); // Guardado en la lista
    }
}

int main() {
    // Inicialización de la ventana de 800x800 píxeles
    mainWindow = Window(800, 800);
    mainWindow.Initialise();

    // Creación de los objetos geométricos básicos e inicialización de shaders
    CreaPiramide(); // meshList[0]
    CrearCubo();    // meshList[1]
    CreateShaders();

    // Variables para almacenar las ubicaciones de las matrices uniformes en los shaders
    GLuint uniformModel = 0;
    GLuint uniformProjection = 0;

    // MATRIZ DE PROYECCIÓN EN PERSPECTIVA:Usada para dar la ilusión de profundidad real
    // Se calcula usando el ancho/alto de la ventana para mantener la relación de aspecto.
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    // Le dice a OpenGL que respete las profundidades y no encime figuras
    glEnable(GL_DEPTH_TEST);

    // Loop de renderizado mientras no se cierre la ventana
    while (!mainWindow.getShouldClose()) {
        glfwPollEvents(); // Recibir eventos del usuario (teclado/ratón)

        //Fondo blanco  
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        // Limpiamos tanto el buffer de color como el buffer de profundidad
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activamos el primer shader (rojo para la casa)
        shaderList[0].useShader();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectLocation();
        // Enviamos la matriz de proyección uniforme al shader activado
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1.0f); // Matriz de modelo base (identidad)

        // ==========================================
        // 1. CUERPO DE LA CASA (CUBO ROJO)
        // ==========================================
        model = glm::mat4(1.0f); // Reiniciar matriz
        // Traslación: Posicionar la casa en el espacio. El centro está en Y=-0.4 y Z=-3.5 para darle profundidad.
        model = glm::translate(model, glm::vec3(0.0f, -0.4f, -3.5f));
        // Escala: Mantener el tamaño de 1x1x1 para el cubo de la casa.
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        // Enviamos la matriz de modelo uniforme al shader
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        // Renderizamos el cubo
        meshList[1]->RenderMesh();



        // ==========================================
        // 2. TECHO (PIRÁMIDE AZUL)
        // ==========================================
        // Activamos el shader azul (Índice 2)
        shaderList[2].useShader();
        uniformModel = shaderList[2].getModelLocation();
        uniformProjection = shaderList[2].getProjectLocation();
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        model = glm::mat4(1.0f); // Reiniciar matriz 
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, -3.2f)); 
        model = glm::scale(model, glm::vec3(1.5f, 0.8f, 1.5f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        // Renderizamos la pirámide
        meshList[0]->RenderMesh();



        // ==========================================
        // 3. PUERTA Y VENTANAS (CUBOS VERDES)
        // ==========================================
        // Activamos el shader verde (Índice 1)
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
 
        glm::vec3 itemScale = glm::vec3(0.27f, 0.27f, 0.1f);
         

        // --- Ventana Izquierda ---
        model = glm::mat4(1.0f); // Reiniciar matriz 
        model = glm::translate(model, glm::vec3(-0.2f, -0.1f, -2.99f)); // Posicionar arriba a la izquierda
        model = glm::scale(model, itemScale); // Aplicar escala genérica cuadrada
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->RenderMesh();

        // --- Ventana Derecha ---
        model = glm::mat4(1.0f); // Reiniciar matriz 
        model = glm::translate(model, glm::vec3(0.2f, -0.1f, -2.99f)); // Posicionar arriba a la derecha
        model = glm::scale(model, itemScale); // Aplicar escala genérica cuadrada
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->RenderMesh();

        // --- Puerta ---
        model = glm::mat4(1.0f); // Reiniciar matriz 
        model = glm::translate(model, glm::vec3(0.0f, -0.75f, -2.99f));
        model = glm::scale(model, itemScale);  
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->RenderMesh();



        // ==========================================
        // 4. TRONCOS DE LOS PINOS (CUBOS CAFÉ)
        // ==========================================
        // Activamos el shader café (Índice 3)
        shaderList[3].useShader();
        uniformModel = shaderList[3].getModelLocation();
        uniformProjection = shaderList[3].getProjectLocation();
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        // Escala cuadrada para los troncos (mismo ancho y altura visual).
        glm::vec3 trunkScale = glm::vec3(0.235f, 0.235f, 0.235f);

        // --- Tronco Izquierdo ---
        model = glm::mat4(1.0f); // Reiniciar matriz 
        model = glm::translate(model, glm::vec3(-0.9f, -0.9f, -3.5f));  
        model = glm::scale(model, trunkScale); // Aplicar escala cuadrada
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->RenderMesh();

        // --- Tronco Derecho ---
        model = glm::mat4(1.0f); // Reiniciar matriz 
        model = glm::translate(model, glm::vec3(0.9f, -0.9f, -3.5f));  
        model = glm::scale(model, trunkScale); // Aplicar escala cuadrada
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->RenderMesh();

            // ==========================================
            // 5. COPAS DE LOS PINOS (PIRÁMIDES VERDE OSCURO)
            // ==========================================
            // Activamos el shader verde oscuro (Índice 4)
        shaderList[4].useShader();
        uniformModel = shaderList[4].getModelLocation();
        uniformProjection = shaderList[4].getProjectLocation();
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        //Dimensiones pino 
        glm::vec3 pineScale = glm::vec3(0.5f, 0.65f, 0.5f);

        // --- Pino Izquierdo ---
        model = glm::mat4(1.0f); // Reiniciar matriz 
        model = glm::translate(model, glm::vec3(-0.9f, -0.46f, -3.37f));  
        model = glm::scale(model, pineScale); // Aplicar escala reducida y alineada
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        // Renderizamos la pirámide
        meshList[0]->RenderMesh();

        // --- Pino Derecho ---
        model = glm::mat4(1.0f); // Reiniciar matriz 
        model = glm::translate(model, glm::vec3(0.9f, -0.46f, -3.37f));  
        model = glm::scale(model, pineScale); // Aplicar escala reducida y alineada
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        // Renderizamos la pirámide
        meshList[0]->RenderMesh();

        // Desactivamos el shader para evitar conflictos y hacemos swap de los buffers
        glUseProgram(0); 
        mainWindow.swapBuffers();
    }

    return 0;
}