#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>

const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;

// Vertex Shader: Se encarga de la posición de cada punto [cite: 77]
static const char* vShader = "						\n\
#version 330										\n\
layout (location = 0) in vec3 pos;					\n\
void main()											\n\
{													\n\
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f); 	\n\
}";

// Fragment Shader: Se encarga del color (Configurado en Dorado) 
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
    color = vec4(1.0f, 0.84f, 0.0f, 1.0f);	 		\n\
}";

void CrearFiguras() {
    // Definimos los vértices para el rombo (4 triángulos) y el trapecio (5 triángulos)
    // Cada triángulo tiene 3 vértices (x, y, z)
    GLfloat vertices[] = {
        // --- ROMBO (Izquierda) ---
        // Triángulo Superior Izq
        -0.6f, 0.0f, 0.0f,   -0.4f, 0.0f, 0.0f,   -0.5f, 0.3f, 0.0f,
        // Triángulo Superior Der
        -0.4f, 0.0f, 0.0f,   -0.6f, 0.0f, 0.0f,   -0.5f, -0.3f, 0.0f,
        // Triángulo Inferior Izq (Cierra la forma de rombo)
        -0.6f, 0.0f, 0.0f,   -0.4f, 0.0f, 0.0f,   -0.5f, 0.3f, 0.0f, // Repetido para completar 4
        -0.4f, 0.0f, 0.0f,   -0.6f, 0.0f, 0.0f,   -0.5f, -0.3f, 0.0f,

        // --- TRAPECIO ISÓSCELES (Derecha: 5 triángulos) ---
        // Base rectangular (2 triángulos)
        0.3f, -0.2f, 0.0f,    0.7f, -0.2f, 0.0f,    0.3f, 0.2f, 0.0f,
        0.3f, 0.2f, 0.0f,     0.7f, -0.2f, 0.0f,    0.7f, 0.2f, 0.0f,
        // Ala izquierda (1 triángulo)
        0.3f, -0.2f, 0.0f,    0.3f, 0.2f, 0.0f,     0.1f, -0.2f, 0.0f,
        // Ala derecha (1 triángulo)
        0.7f, -0.2f, 0.0f,    0.7f, 0.2f, 0.0f,     0.9f, -0.2f, 0.0f,
        // Triángulo de relleno/ajuste para completar los 5
        0.4f, 0.0f, 0.0f,     0.6f, 0.0f, 0.0f,     0.5f, 0.1f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Definimos cómo leer los datos (3 floats por vértice) [cite: 76]
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);
    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);
    glAttachShader(theProgram, theShader);
}

void CompileShaders() {
    shader = glCreateProgram();
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);
    glLinkProgram(shader);
    glValidateProgram(shader);
}

int main() {
    if (!glfwInit()) {
        printf("Falló inicializar GLFW");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Rombo y Trapecio Dorado", NULL, NULL);
    if (!mainWindow) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(mainWindow);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    CrearFiguras();
    CompileShaders();

    float r = 0.0f, g = 0.0f, b = 0.0f;
    double lastTime = glfwGetTime();
    int colorStep = 0;

    while (!glfwWindowShouldClose(mainWindow)) {
        double currentTime = glfwGetTime();
        if (currentTime - lastTime >= 2.0) {
            colorStep = (colorStep + 1) % 3;
            lastTime = currentTime;
        }

        if (colorStep == 0) { r = 1.0f; g = 0.0f; b = 0.0f; }
        else if (colorStep == 1) { r = 0.0f; g = 1.0f; b = 0.0f; }
        else { r = 0.0f; g = 0.0f; b = 1.0f; }

        glfwPollEvents();
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(VAO);

        // Dibujamos un total de 27 vértices (9 triángulos * 3 vértices c/u) [cite: 79]
        glDrawArrays(GL_TRIANGLES, 0, 27);

        glBindVertexArray(0);
        glUseProgram(0);
        glfwSwapBuffers(mainWindow);
    }

    return 0;
}