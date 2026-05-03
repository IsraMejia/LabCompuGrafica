#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }

	GLfloat getmueveMustang() { return mueveMustang; }

	GLfloat getmueveHelicoptero() { return mueveHelicoptero; }

	GLfloat getmueveNave() { return mueveNave; }
	// -1 = avanzando (V), +1 = retrocediendo (B), 0 = quieta
	int getDireccionNave() { return direccionNave; }

	bool getLamparaEncendida() { return lamparaEncendida; }
	bool getFocoPezEncendido() { return focoPezEncendido; }
	bool getSpotlightRojoPezEncendido() { return spotlightRojoPezEncendido; }

	// Spotlight rojo del pez: rotaciones en cada eje
	GLfloat getSpotlightRojoPezRotX() { return spotlightRojoPezRotX; }
	GLfloat getSpotlightRojoPezRotY() { return spotlightRojoPezRotY; }
	GLfloat getSpotlightRojoPezRotZ() { return spotlightRojoPezRotZ; }

	// Pez abisal: posicion Z local y tiempo senoidal
	GLfloat getPezPosZ()   { return pezPosZ; }
	GLfloat getPezTiempo() { return pezTiempo; }
	// -1 = avanzando (O), +1 = retrocediendo (P), 0 = quieto
	int getDireccionPez()  { return direccionPez; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;

	GLfloat mueveMustang;
	GLfloat mueveHelicoptero;
	GLfloat mueveNave;
	int direccionNave;
	bool lamparaEncendida;
	bool focoPezEncendido;  // tecla F: toggle luz puntual del foco del pez
	bool spotlightRojoPezEncendido; // tecla R: toggle spotlight rojo del pez
	
	// Spotlight rojo del pez: rotaciones en grados
	GLfloat spotlightRojoPezRotX; // tecla G: rotar en eje X
	GLfloat spotlightRojoPezRotY; // tecla H: rotar en eje Y
	GLfloat spotlightRojoPezRotZ; // tecla T: rotar en eje Z

	// Pez abisal
	GLfloat pezPosZ;    // posicion Z local dentro de la pecera (-1.8 a +1.8)
	GLfloat pezTiempo;  // acumulador de tiempo para la senoidal en Y
	int     direccionPez; // -1=O avanza, +1=P retrocede, 0=quieto

	bool mouseFirstMoved;

	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

