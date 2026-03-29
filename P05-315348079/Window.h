#pragma once
#include <stdio.h>
#include <glew.h>
#include <glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	bool* getsKeys() { return teclas; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { glfwSwapBuffers(mainWindow); }
	
	// ====== MÉTODOS PARA OBTENER LOS VALORES DE CONTROL DEL CARRO ======
	GLfloat getMovimientoZ() { return movimientoZ; }
	GLfloat getGiroLlantas() { return giroLlantas; }
	GLfloat getGiroCofre() { return giroCofre; }

	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	
	// ====== VARIABLES DE CONTROL PERSONALIZADAS ======
	GLfloat movimientoZ; // Traslación del auto en el eje Z
	GLfloat giroLlantas; // Rotación de las llantas sobre su eje (rodar)
	GLfloat giroCofre;   // Rotación del cofre (abrir/cerrar)
	
	// Velocidades de movimiento/rotación
	GLfloat velTraslacion;
	GLfloat velGiro;

	bool teclas[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat ultimaX, ultimaY;
	GLfloat cambioX, cambioY;
	bool primerMovimientoMouse;
	
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};