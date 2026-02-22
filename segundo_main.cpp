#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>

const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;
GLint uniformBlack;

// Vertex Shader: Recibe posición (0) y color (1) del VAO [cite: 28, 77]
static const char* vShader = "#version 330\n layout (location = 0) in vec3 pos; layout (location = 1) in vec3 inColor; out vec3 fragColor; void main() { gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f); fragColor = inColor; }";

// Fragment Shader: Toggle entre color de letra o Negro para la malla [cite: 28, 78]
static const char* fShader = "#version 330\n in vec3 fragColor; out vec4 color; uniform int isBlack; void main() { if(isBlack == 1) color = vec4(0.0f, 0.0f, 0.0f, 1.0f); else color = vec4(fragColor, 1.0f); }";

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);
    glShaderSource(theShader, 1, &shaderCode, NULL);
    glCompileShader(theShader);
    glAttachShader(theProgram, theShader);
}

void CompileShaders() {
    shader = glCreateProgram();
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);
    glLinkProgram(shader);
    uniformBlack = glGetUniformLocation(shader, "isBlack");
}

void CrearIHM() {
    // 1. Todas las letras del mismo color (Dorado)  
    float r = 1.0f, g = 0.8f, b = 0.0f; 

    GLfloat vertices[] = {
        // --- LETRA I (Esquina inferior izquierda) ---
        // Desplazada a la posición X: -0.9 a -0.6, Y: -0.9 a -0.5
        // Top (3 cuadros)
        -0.9f, -0.6f, 0.0f, r,g,b, -0.8f, -0.6f, 0.0f, r,g,b, -0.8f, -0.5f, 0.0f, r,g,b,
        -0.9f, -0.6f, 0.0f, r,g,b, -0.8f, -0.5f, 0.0f, r,g,b, -0.9f, -0.5f, 0.0f, r,g,b,
        -0.8f, -0.6f, 0.0f, r,g,b, -0.7f, -0.6f, 0.0f, r,g,b, -0.7f, -0.5f, 0.0f, r,g,b,
        -0.8f, -0.6f, 0.0f, r,g,b, -0.7f, -0.5f, 0.0f, r,g,b, -0.8f, -0.5f, 0.0f, r,g,b,
        -0.7f, -0.6f, 0.0f, r,g,b, -0.6f, -0.6f, 0.0f, r,g,b, -0.6f, -0.5f, 0.0f, r,g,b,
        -0.7f, -0.6f, 0.0f, r,g,b, -0.6f, -0.5f, 0.0f, r,g,b, -0.7f, -0.5f, 0.0f, r,g,b,
        // Poste (2 cuadros)
        -0.8f, -0.7f, 0.0f, r,g,b, -0.7f, -0.7f, 0.0f, r,g,b, -0.7f, -0.6f, 0.0f, r,g,b,
        -0.8f, -0.7f, 0.0f, r,g,b, -0.7f, -0.6f, 0.0f, r,g,b, -0.8f, -0.6f, 0.0f, r,g,b,
        -0.8f, -0.8f, 0.0f, r,g,b, -0.7f, -0.8f, 0.0f, r,g,b, -0.7f, -0.7f, 0.0f, r,g,b,
        -0.8f, -0.8f, 0.0f, r,g,b, -0.7f, -0.7f, 0.0f, r,g,b, -0.8f, -0.7f, 0.0f, r,g,b,
        // Base (3 cuadros)
        -0.9f, -0.9f, 0.0f, r,g,b, -0.8f, -0.9f, 0.0f, r,g,b, -0.8f, -0.8f, 0.0f, r,g,b,
        -0.9f, -0.9f, 0.0f, r,g,b, -0.8f, -0.8f, 0.0f, r,g,b, -0.9f, -0.8f, 0.0f, r,g,b,
        -0.8f, -0.9f, 0.0f, r,g,b, -0.7f, -0.9f, 0.0f, r,g,b, -0.7f, -0.8f, 0.0f, r,g,b,
        -0.8f, -0.9f, 0.0f, r,g,b, -0.7f, -0.8f, 0.0f, r,g,b, -0.8f, -0.8f, 0.0f, r,g,b,
        -0.7f, -0.9f, 0.0f, r,g,b, -0.6f, -0.9f, 0.0f, r,g,b, -0.6f, -0.8f, 0.0f, r,g,b,
        -0.7f, -0.9f, 0.0f, r,g,b, -0.6f, -0.8f, 0.0f, r,g,b, -0.7f, -0.8f, 0.0f, r,g,b,

        // --- LETRA H (Centro) ---
        // Desplazada a la posición X: -0.2 a 0.1, Y: -0.2 a 0.2
        // Poste Izq (4 cuadros)
        -0.2f, -0.2f, 0.0f, r,g,b, -0.1f, -0.2f, 0.0f, r,g,b, -0.1f, -0.1f, 0.0f, r,g,b,
        -0.2f, -0.2f, 0.0f, r,g,b, -0.1f, -0.1f, 0.0f, r,g,b, -0.2f, -0.1f, 0.0f, r,g,b,
        -0.2f, -0.1f, 0.0f, r,g,b, -0.1f, -0.1f, 0.0f, r,g,b, -0.1f,  0.0f, 0.0f, r,g,b,
        -0.2f, -0.1f, 0.0f, r,g,b, -0.1f,  0.0f, 0.0f, r,g,b, -0.2f,  0.0f, 0.0f, r,g,b,
        -0.2f,  0.0f, 0.0f, r,g,b, -0.1f,  0.0f, 0.0f, r,g,b, -0.1f,  0.1f, 0.0f, r,g,b,
        -0.2f,  0.0f, 0.0f, r,g,b, -0.1f,  0.1f, 0.0f, r,g,b, -0.2f,  0.1f, 0.0f, r,g,b,
        -0.2f,  0.1f, 0.0f, r,g,b, -0.1f,  0.1f, 0.0f, r,g,b, -0.1f,  0.2f, 0.0f, r,g,b,
        -0.2f,  0.1f, 0.0f, r,g,b, -0.1f,  0.2f, 0.0f, r,g,b, -0.2f,  0.2f, 0.0f, r,g,b,
        // Puente (1 cuadro)
        -0.1f,  0.0f, 0.0f, r,g,b,  0.0f,  0.0f, 0.0f, r,g,b,  0.0f,  0.1f, 0.0f, r,g,b,
        -0.1f,  0.0f, 0.0f, r,g,b,  0.0f,  0.1f, 0.0f, r,g,b, -0.1f,  0.1f, 0.0f, r,g,b,
        // Poste Der (4 cuadros)
         0.0f, -0.2f, 0.0f, r,g,b,  0.1f, -0.2f, 0.0f, r,g,b,  0.1f, -0.1f, 0.0f, r,g,b,
         0.0f, -0.2f, 0.0f, r,g,b,  0.1f, -0.1f, 0.0f, r,g,b,  0.0f, -0.1f, 0.0f, r,g,b,
         0.0f, -0.1f, 0.0f, r,g,b,  0.1f, -0.1f, 0.0f, r,g,b,  0.1f,  0.0f, 0.0f, r,g,b,
         0.0f, -0.1f, 0.0f, r,g,b,  0.1f,  0.0f, 0.0f, r,g,b,  0.0f,  0.0f, 0.0f, r,g,b,
         0.0f,  0.0f, 0.0f, r,g,b,  0.1f,  0.0f, 0.0f, r,g,b,  0.1f,  0.1f, 0.0f, r,g,b,
         0.0f,  0.0f, 0.0f, r,g,b,  0.1f,  0.1f, 0.0f, r,g,b,  0.0f,  0.1f, 0.0f, r,g,b,
         0.0f,  0.1f, 0.0f, r,g,b,  0.1f,  0.1f, 0.0f, r,g,b,  0.1f,  0.2f, 0.0f, r,g,b,
         0.0f,  0.1f, 0.0f, r,g,b,  0.1f,  0.2f, 0.0f, r,g,b,  0.0f,  0.2f, 0.0f, r,g,b,

        // --- LETRA M (Esquina superior derecha) ---
        // Desplazada a la posición X: 0.3 a 0.8, Y: 0.3 a 0.7
        // Poste 1 (4 cuadros)
         0.3f,  0.3f, 0.0f, r,g,b,  0.4f,  0.3f, 0.0f, r,g,b,  0.4f,  0.4f, 0.0f, r,g,b,
         0.3f,  0.3f, 0.0f, r,g,b,  0.4f,  0.4f, 0.0f, r,g,b,  0.3f,  0.4f, 0.0f, r,g,b,
         0.3f,  0.4f, 0.0f, r,g,b,  0.4f,  0.4f, 0.0f, r,g,b,  0.4f,  0.5f, 0.0f, r,g,b,
         0.3f,  0.4f, 0.0f, r,g,b,  0.4f,  0.5f, 0.0f, r,g,b,  0.3f,  0.5f, 0.0f, r,g,b,
         0.3f,  0.5f, 0.0f, r,g,b,  0.4f,  0.5f, 0.0f, r,g,b,  0.4f,  0.6f, 0.0f, r,g,b,
         0.3f,  0.5f, 0.0f, r,g,b,  0.4f,  0.6f, 0.0f, r,g,b,  0.3f,  0.6f, 0.0f, r,g,b,
         0.3f,  0.6f, 0.0f, r,g,b,  0.4f,  0.6f, 0.0f, r,g,b,  0.4f,  0.7f, 0.0f, r,g,b,
         0.3f,  0.6f, 0.0f, r,g,b,  0.4f,  0.7f, 0.0f, r,g,b,  0.3f,  0.7f, 0.0f, r,g,b,
        // Conector 1
         0.4f,  0.6f, 0.0f, r,g,b,  0.5f,  0.6f, 0.0f, r,g,b,  0.5f,  0.7f, 0.0f, r,g,b,
         0.4f,  0.6f, 0.0f, r,g,b,  0.5f,  0.7f, 0.0f, r,g,b,  0.4f,  0.7f, 0.0f, r,g,b,
        // Poste 2 - Centro (4 cuadros)
         0.5f,  0.3f, 0.0f, r,g,b,  0.6f,  0.3f, 0.0f, r,g,b,  0.6f,  0.4f, 0.0f, r,g,b,
         0.5f,  0.3f, 0.0f, r,g,b,  0.6f,  0.4f, 0.0f, r,g,b,  0.5f,  0.4f, 0.0f, r,g,b,
         0.5f,  0.4f, 0.0f, r,g,b,  0.6f,  0.4f, 0.0f, r,g,b,  0.6f,  0.5f, 0.0f, r,g,b,
         0.5f,  0.4f, 0.0f, r,g,b,  0.6f,  0.5f, 0.0f, r,g,b,  0.5f,  0.5f, 0.0f, r,g,b,
         0.5f,  0.5f, 0.0f, r,g,b,  0.6f,  0.5f, 0.0f, r,g,b,  0.6f,  0.6f, 0.0f, r,g,b,
         0.5f,  0.5f, 0.0f, r,g,b,  0.6f,  0.6f, 0.0f, r,g,b,  0.5f,  0.6f, 0.0f, r,g,b,
         0.5f,  0.6f, 0.0f, r,g,b,  0.6f,  0.6f, 0.0f, r,g,b,  0.6f,  0.7f, 0.0f, r,g,b,
         0.5f,  0.6f, 0.0f, r,g,b,  0.6f,  0.7f, 0.0f, r,g,b,  0.5f,  0.7f, 0.0f, r,g,b,
        // Conector 2
         0.6f,  0.6f, 0.0f, r,g,b,  0.7f,  0.6f, 0.0f, r,g,b,  0.7f,  0.7f, 0.0f, r,g,b,
         0.6f,  0.6f, 0.0f, r,g,b,  0.7f,  0.7f, 0.0f, r,g,b,  0.6f,  0.7f, 0.0f, r,g,b,
        // Poste 3 (4 cuadros)
         0.7f,  0.3f, 0.0f, r,g,b,  0.8f,  0.3f, 0.0f, r,g,b,  0.8f,  0.4f, 0.0f, r,g,b,
         0.7f,  0.3f, 0.0f, r,g,b,  0.8f,  0.4f, 0.0f, r,g,b,  0.7f,  0.4f, 0.0f, r,g,b,
         0.7f,  0.4f, 0.0f, r,g,b,  0.8f,  0.4f, 0.0f, r,g,b,  0.8f,  0.5f, 0.0f, r,g,b,
         0.7f,  0.4f, 0.0f, r,g,b,  0.8f,  0.5f, 0.0f, r,g,b,  0.7f,  0.5f, 0.0f, r,g,b,
         0.7f,  0.5f, 0.0f, r,g,b,  0.8f,  0.5f, 0.0f, r,g,b,  0.8f,  0.6f, 0.0f, r,g,b,
         0.7f,  0.5f, 0.0f, r,g,b,  0.8f,  0.6f, 0.0f, r,g,b,  0.7f,  0.6f, 0.0f, r,g,b,
         0.7f,  0.6f, 0.0f, r,g,b,  0.8f,  0.6f, 0.0f, r,g,b,  0.8f,  0.7f, 0.0f, r,g,b,
         0.7f,  0.6f, 0.0f, r,g,b,  0.8f,  0.7f, 0.0f, r,g,b,  0.7f,  0.7f, 0.0f, r,g,b
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Stride de 6 floats (3 pos + 3 color) [cite: 76]
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int main() {
    if (!glfwInit()) return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Practica 1 - IHM", NULL, NULL);
    if (!mainWindow) { glfwTerminate(); return 1; }

    glfwMakeContextCurrent(mainWindow);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { glfwDestroyWindow(mainWindow); glfwTerminate(); return 1; }

    glViewport(0, 0, WIDTH, HEIGHT);
    CrearIHM(); // Llamar después de glewInit() para evitar VAO = 0
    CompileShaders();

    float r = 0, g = 0, b = 0;
    double lastTime = glfwGetTime();
    int colorStep = 0;

    while (!glfwWindowShouldClose(mainWindow)) {
        double currentTime = glfwGetTime();
        if (currentTime - lastTime >= 2.0) {
            colorStep = (colorStep + 1) % 3;
            lastTime = currentTime;
        }
        if (colorStep == 0) { r = 0.5f; g = 0.1f; b = 0.1f; }
        else if (colorStep == 1) { r = 0.1f; g = 0.4f; b = 0.1f; }
        else { r = 0.1f; g = 0.1f; b = 0.5f; }

        glfwPollEvents();
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(VAO);

        // Paso 1: Relleno (isBlack = 0)  
        glUniform1i(uniformBlack, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, 186); // 16+18+28 triángulos 

        // Paso 2: Malla (isBlack = 1)
        glUniform1i(uniformBlack, 1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f);
        glDrawArrays(GL_TRIANGLES, 0, 186);

        glBindVertexArray(0);
        glfwSwapBuffers(mainWindow);
    }
    return 0;
}