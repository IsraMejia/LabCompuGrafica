/*
Pr�ctica 7: Iluminaci�n 1 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

// Estructura para los colores del ciclo de faros del Mustang
struct ColorFaro { GLfloat r, g, b; };
static const ColorFaro cicloFaros[] = {
    {1.0f, 0.0f, 0.0f},  // rojo
    {1.0f, 1.0f, 0.0f},  // amarillo
    {0.0f, 1.0f, 0.0f},  // verde
    {0.0f, 1.0f, 1.0f},  // cian
    {0.0f, 0.0f, 1.0f},  // azul
    {1.0f, 0.0f, 1.0f},  // magenta
};
static const int NUM_COLORES_FARO = 6;
static int indiceFaro = 0;
static bool teclaXPresionada = false;

// Posicion local del bulbo del pez respecto al cuerpo (ajustar segun el modelo)
static const glm::vec3 posBulboPez(0.0f, 1.2f, 1.8f);
// Posicion de la pecera en mundo (raiz de la jerarquia del pez)
static const glm::vec3 posPecera(-6.5f, 4.5f, -4.5f);

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dadoTexture;
Texture dado8Texture;
Texture lamparaTexture;
Texture paceraTexture;

// Textura del pez abisal
Texture pezTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model Mustang_M;
Model Laterne_M;
Model Nave_M;

// Modelos del pez abisal
Model CuerpoPez_M;
Model AntenaPez_M;
Model FocoPez_M;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//funci�n de calculo de normales por promedio de v�rtices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
			10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};	
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados,  
    GLfloat cubo_vertices[] = {
        // front Alegria
        //Coordenadas vertices  Textura             Iluminacion
        //x     y       z       S       T           NX      NY      NZ
        -0.5f, -0.5f,  0.5f,    0.35f,  0.51f,      0.0f,   0.0f,   1.0f, //0 ab Izq
         0.5f, -0.5f,  0.5f,    0.63f,  0.51f,      0.0f,   0.0f,   1.0f, //1 ab Der
         0.5f,  0.5f,  0.5f,    0.63f,  0.74f,      0.0f,   0.0f,   1.0f, //2 Ar Der
        -0.5f,  0.5f,  0.5f,    0.35f,  0.74f,      0.0f,   0.0f,   1.0f, //3 Ar Izq

        // right Ansiedad
        //x     y       z       S       T
         0.5f, -0.5f,  0.5f,    0.69f,  0.51f,      1.0f,   0.0f,   0.0f, //4 ab Izq
         0.5f, -0.5f, -0.5f,    0.98f,  0.51f,      1.0f,   0.0f,   0.0f, //5 ab Der
         0.5f,  0.5f, -0.5f,    0.98f,  0.74f,      1.0f,   0.0f,   0.0f, //6 Ar Der 
         0.5f,  0.5f,  0.5f,    0.69f,  0.74f,      1.0f,   0.0f,   0.0f, //7 Ar Izq

         // back Tristeza
         -0.5f, -0.5f, -0.5f,   0.63f,  0.02f,      0.0f,   0.0f,  -1.0f, //8 ab Izq
          0.5f, -0.5f, -0.5f,   0.35f,  0.02f,      0.0f,   0.0f,  -1.0f, //9 ab Der
          0.5f,  0.5f, -0.5f,   0.35f,  0.23f,      0.0f,   0.0f,  -1.0f, //10 Ar Der
         -0.5f,  0.5f, -0.5f,   0.63f,  0.23f,      0.0f,   0.0f,  -1.0f, //11 Ar Izq

         // left Miedo
         //x        y       z       S       T
         -0.5f, -0.5f, -0.5f,   0.02f,  0.51f,     -1.0f,   0.0f,   0.0f, //12 ab Izq
         -0.5f, -0.5f,  0.5f,   0.32f,  0.51f,     -1.0f,   0.0f,   0.0f, //13 ab Der
         -0.5f,  0.5f,  0.5f,   0.32f,  0.74f,     -1.0f,   0.0f,   0.0f, //14 Ar Der
         -0.5f,  0.5f, -0.5f,   0.02f,  0.74f,     -1.0f,   0.0f,   0.0f, //15 Ar Izq

         // bottom  Enojo
        //x     y       z       S       T
        -0.5f, -0.5f,  0.5f,    0.35f,  0.26f,      0.0f,  -1.0f,   0.0f, //16 ab Izq
         0.5f, -0.5f,  0.5f,    0.63f,  0.26f,      0.0f,  -1.0f,   0.0f, //17 ab Der
         0.5f, -0.5f, -0.5f,    0.63f,  0.48f,      0.0f,  -1.0f,   0.0f, //18 Ar Der
        -0.5f, -0.5f, -0.5f,    0.35f,  0.48f,      0.0f,  -1.0f,   0.0f, //19 Ar Izq

        // UP Desagrado
        //x     y       z       S       T
        -0.5f,  0.5f,  0.5f,    0.35f,  0.76f,      0.0f,   1.0f,   0.0f, //20 ab Izq
         0.5f,  0.5f,  0.5f,    0.63f,  0.76f,      0.0f,   1.0f,   0.0f, //21 ab Der
         0.5f,  0.5f, -0.5f,    0.63f,  0.98f,      0.0f,   1.0f,   0.0f, //22 Ar Der
        -0.5f,  0.5f, -0.5f,    0.35f,  0.98f,      0.0f,   1.0f,   0.0f, //23 Ar Izq
    };

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}




void CrearDado8Caras()
{
    // Normales calculadas con calc_normals_oct.py (cross(v1-v0, v2-v0) por cara)
    //   x       y       z        S       T        NX        NY        NZ
    GLfloat oct_vertices[] = {
        // Cara 1: Top(+Y), +X, +Z   normal = (-0.5774, -0.5774, -0.5774)
         0.0f,  0.5f,  0.0f,   0.493f, 0.750f,   -0.5774f, -0.5774f, -0.5774f,
         0.5f,  0.0f,  0.0f,   0.251f, 0.510f,   -0.5774f, -0.5774f, -0.5774f,
         0.0f,  0.0f,  0.5f,   0.730f, 0.510f,   -0.5774f, -0.5774f, -0.5774f,
        // Cara 2: Top(+Y), -X, +Z   normal = ( 0.5774, -0.5774, -0.5774)
         0.0f,  0.5f,  0.0f,   0.249f, 0.490f,    0.5774f, -0.5774f, -0.5774f,
         0.0f,  0.0f,  0.5f,   0.010f, 0.258f,    0.5774f, -0.5774f, -0.5774f,
        -0.5f,  0.0f,  0.0f,   0.490f, 0.258f,    0.5774f, -0.5774f, -0.5774f,
        // Cara 3: Top(+Y), -X, -Z   normal = ( 0.5774, -0.5774,  0.5774)
         0.0f,  0.5f,  0.0f,   0.250f, 0.510f,    0.5774f, -0.5774f,  0.5774f,
        -0.5f,  0.0f,  0.0f,   0.010f, 0.750f,    0.5774f, -0.5774f,  0.5774f,
         0.0f,  0.0f, -0.5f,   0.490f, 0.750f,    0.5774f, -0.5774f,  0.5774f,
        // Cara 4: Top(+Y), +X, -Z   normal = (-0.5774, -0.5774,  0.5774)
         0.0f,  0.5f,  0.0f,   0.510f, 0.280f,   -0.5774f, -0.5774f,  0.5774f,
         0.0f,  0.0f, -0.5f,   0.251f, 0.490f,   -0.5774f, -0.5774f,  0.5774f,
         0.5f,  0.0f,  0.0f,   0.749f, 0.490f,   -0.5774f, -0.5774f,  0.5774f,
        // Cara 5: Bot(-Y), +X, +Z   normal = (-0.5774,  0.5774, -0.5774)
         0.0f, -0.5f,  0.0f,   0.751f, 0.980f,   -0.5774f,  0.5774f, -0.5774f,
         0.0f,  0.0f,  0.5f,   0.520f, 0.761f,   -0.5774f,  0.5774f, -0.5774f,
         0.5f,  0.0f,  0.0f,   0.980f, 0.761f,   -0.5774f,  0.5774f, -0.5774f,
        // Cara 6: Bot(-Y), -X, +Z   normal = ( 0.5774,  0.5774, -0.5774)
         0.0f, -0.5f,  0.0f,   0.740f, 0.500f,    0.5774f,  0.5774f, -0.5774f,
        -0.5f,  0.0f,  0.0f,   0.500f, 0.257f,    0.5774f,  0.5774f, -0.5774f,
         0.0f,  0.0f,  0.5f,   0.985f, 0.257f,    0.5774f,  0.5774f, -0.5774f,
        // Cara 7: Bot(-Y), -X, -Z   normal = ( 0.5774,  0.5774,  0.5774)
         0.0f, -0.5f,  0.0f,   0.750f, 0.500f,    0.5774f,  0.5774f,  0.5774f,
         0.0f,  0.0f, -0.5f,   0.510f, 0.740f,    0.5774f,  0.5774f,  0.5774f,
        -0.5f,  0.0f,  0.0f,   0.980f, 0.740f,    0.5774f,  0.5774f,  0.5774f,
        // Cara 8: Bot(-Y), +X, -Z   normal = (-0.5774,  0.5774,  0.5774)
         0.0f, -0.5f,  0.0f,   0.740f, 0.010f,   -0.5774f,  0.5774f,  0.5774f,
         0.5f,  0.0f,  0.0f,   0.500f, 0.249f,   -0.5774f,  0.5774f,  0.5774f,
         0.0f,  0.0f, -0.5f,   0.980f, 0.249f,   -0.5774f,  0.5774f,  0.5774f,
    };

    unsigned int oct_indices[] = {
         0,  1,  2,
         3,  4,  5,
         6,  7,  8,
         9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
    };

    Mesh* dado8 = new Mesh();
    dado8->CreateMesh(oct_vertices, oct_indices, 192, 24);
    meshList.push_back(dado8);
}

void CrearPacera()
{
	// Prisma rectangular: 7 de largo (X), 4 de ancho (Z), 5 de alto (Y)
	// Centrado en el origen local. Mitades: X=3.5, Y=2.5, Z=2.0
	// Mismo formato que cubo_vertices: x, y, z, S, T, NX, NY, NZ
	// 6 caras x 4 vertices = 24 vertices, 6 caras x 2 triangulos x 3 indices = 36 indices

	unsigned int pacera_indices[] = {
		// front  (+Z)
		 0,  1,  2,   2,  3,  0,
		// back   (-Z)
		 4,  5,  6,   6,  7,  4,
		// left   (-X)
		 8,  9, 10,  10, 11,  8,
		// right  (+X)
		12, 13, 14,  14, 15, 12,
		// bottom (-Y)
		16, 17, 18,  18, 19, 16,
		// top    (+Y)
		20, 21, 22,  22, 23, 20,
	};

	GLfloat pacera_vertices[] = {
		// front (+Z)  normal apunta hacia adentro = (0, 0, -1)
		//x      y       z       S     T      NX    NY     NZ
		-3.5f, -2.5f,  2.0f,   0.0f, 0.0f,  0.0f, 0.0f, -1.0f, // 0 ab Izq
		 3.5f, -2.5f,  2.0f,   1.0f, 0.0f,  0.0f, 0.0f, -1.0f, // 1 ab Der
		 3.5f,  2.5f,  2.0f,   1.0f, 1.0f,  0.0f, 0.0f, -1.0f, // 2 ar Der
		-3.5f,  2.5f,  2.0f,   0.0f, 1.0f,  0.0f, 0.0f, -1.0f, // 3 ar Izq

		// back (-Z)  normal apunta hacia adentro = (0, 0, 1)
		 3.5f, -2.5f, -2.0f,   0.0f, 0.0f,  0.0f, 0.0f,  1.0f, // 4 ab Izq
		-3.5f, -2.5f, -2.0f,   1.0f, 0.0f,  0.0f, 0.0f,  1.0f, // 5 ab Der
		-3.5f,  2.5f, -2.0f,   1.0f, 1.0f,  0.0f, 0.0f,  1.0f, // 6 ar Der
		 3.5f,  2.5f, -2.0f,   0.0f, 1.0f,  0.0f, 0.0f,  1.0f, // 7 ar Izq

		// left (-X)  normal apunta hacia adentro = (1, 0, 0)
		-3.5f, -2.5f, -2.0f,   0.0f, 0.0f,  1.0f, 0.0f,  0.0f, // 8  ab Izq
		-3.5f, -2.5f,  2.0f,   1.0f, 0.0f,  1.0f, 0.0f,  0.0f, // 9  ab Der
		-3.5f,  2.5f,  2.0f,   1.0f, 1.0f,  1.0f, 0.0f,  0.0f, // 10 ar Der
		-3.5f,  2.5f, -2.0f,   0.0f, 1.0f,  1.0f, 0.0f,  0.0f, // 11 ar Izq

		// right (+X)  normal apunta hacia adentro = (-1, 0, 0)
		 3.5f, -2.5f,  2.0f,   0.0f, 0.0f, -1.0f, 0.0f,  0.0f, // 12 ab Izq
		 3.5f, -2.5f, -2.0f,   1.0f, 0.0f, -1.0f, 0.0f,  0.0f, // 13 ab Der
		 3.5f,  2.5f, -2.0f,   1.0f, 1.0f, -1.0f, 0.0f,  0.0f, // 14 ar Der
		 3.5f,  2.5f,  2.0f,   0.0f, 1.0f, -1.0f, 0.0f,  0.0f, // 15 ar Izq

		// bottom (-Y)  normal apunta hacia adentro = (0, 1, 0)
		-3.5f, -2.5f, -2.0f,   0.0f, 0.0f,  0.0f, 1.0f,  0.0f, // 16 ab Izq
		 3.5f, -2.5f, -2.0f,   1.0f, 0.0f,  0.0f, 1.0f,  0.0f, // 17 ab Der
		 3.5f, -2.5f,  2.0f,   1.0f, 1.0f,  0.0f, 1.0f,  0.0f, // 18 ar Der
		-3.5f, -2.5f,  2.0f,   0.0f, 1.0f,  0.0f, 1.0f,  0.0f, // 19 ar Izq

		// top (+Y)  normal apunta hacia adentro = (0, -1, 0)
		-3.5f,  2.5f,  2.0f,   0.0f, 0.0f,  0.0f,-1.0f,  0.0f, // 20 ab Izq
		 3.5f,  2.5f,  2.0f,   1.0f, 0.0f,  0.0f,-1.0f,  0.0f, // 21 ab Der
		 3.5f,  2.5f, -2.0f,   1.0f, 1.0f,  0.0f,-1.0f,  0.0f, // 22 ar Der
		-3.5f,  2.5f, -2.0f,   0.0f, 1.0f,  0.0f,-1.0f,  0.0f, // 23 ar Izq
	};

	// 24 vertices * 8 floats = 192, 36 indices
	Mesh* pacera = new Mesh();
	pacera->CreateMesh(pacera_vertices, pacera_indices, 192, 36);
	meshList.push_back(pacera); // meshList[6]
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CrearDado8Caras();
	CrearPacera();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	dadoTexture = Texture("Textures/dadoEmociones.png");
	dadoTexture.LoadTextureA();
	dado8Texture = Texture("Textures/dado8editado.png");
	dado8Texture.LoadTextureA();

	lamparaTexture = Texture("Textures/1001_Base_Color.png");
	lamparaTexture.LoadTextureA();

	paceraTexture = Texture("Textures/aguaPecera.jpg");
	paceraTexture.LoadTextureA();

	pezTexture = Texture("Textures/pez abisal.png");
	pezTexture.LoadTextureA();

     

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	Nave_M = Model();
	Nave_M.LoadModel("Models/nave.obj");

	Mustang_M = Model();
	Mustang_M.LoadModel("Models/mustangCars.obj");

	Laterne_M = Model();
	Laterne_M.LoadModel("Models/lamppost .obj");

	CuerpoPez_M = Model();
	CuerpoPez_M.LoadModel("Models/cuerpo_pez.obj");
	AntenaPez_M = Model();
	AntenaPez_M.LoadModel("Models/antena_pez.obj");
	FocoPez_M = Model();
	FocoPez_M.LoadModel("Models/foco_pez.obj");
	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(
		1.0f, 1.0f, 1.0f,
		0.5f, 0.3f, //RadiacionComponenteAmbientealExposicion, Intensidad de la luz superior del sol
		0.0f, 0.0f, -1.0f); //VECTOR DE ILUMINACION , se puede poner que el carro se alumnbre desde abajo
	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Declaración de primer luz puntual - roja
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// pointLights[1] es la lampara: se inicializa en el loop con posicion jerarquica
	// y se controla con la tecla Z (encendido/apagado via pointLightCount)
	pointLightCount++;

	// pointLights[2] es el foco del pez abisal: se actualiza cada frame con posicion jerarquica
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,//Posicion
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija verde eliminada - estaba en (5,10,0) e iluminaba independiente del toggle

	// Faro frontal del Mustang (indice 1) - esquina frontal izquierda
	spotLights[1] = SpotLight(
		0.0f, 0.0f, 1.0f,   // Color azul
		0.0f, 3.0f,          // Sin ambiente, difusa fuerte
		0.0f, 0.0f, 0.0f,   // Posicion inicial (se sobreescribe cada frame)
		0.0f, 0.0f, -1.0f,  // Direccion inicial (se sobreescribe cada frame)
		1.0f, 0.0f, 0.0f,   // Atenuacion: solo constante
		20.0f);              // Cono cerrado 20 grados
	spotLightCount++;

	// Faro frontal del Mustang (indice 2) - esquina frontal derecha
	spotLights[2] = SpotLight(
		0.0f, 0.3f, 1.0f,   // Color azul con verde
		0.0f, 3.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	// Reflector amarillo del helicoptero (indice 3) - apunta al piso, se mueve con el helicoptero
	spotLights[3] = SpotLight(
		0.50f, 0.5f, 0.0f,  // Color amarillo intenso
		0.0f, 8.0f,          // difusa muy fuerte
		0.0f, 5.0f, 6.0f,   // Posicion inicial (se sobreescribe cada frame)
		0.0f, -1.0f, 0.0f,  // Apunta hacia el piso
		0.9f, 0.0f, 0.0f,   // Atenuacion minima para que llegue lejos
		23.0f);              // Cono amplio tipo reflector
	spotLightCount++;

	// Spotlight frontal de la nave (indice 4) - cian, se activa al avanzar (V, direccion -X)
	spotLights[4] = SpotLight(
		0.0f, 0.8f, 1.0f,   // Color cian
		0.0f, 5.0f,
		0.0f, 5.0f, 14.0f,  // Posicion inicial (se sobreescribe cada frame)
		-1.0f, -1.0f, 0.0f, // Apunta hacia adelante-abajo (-X, -Y)
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	// Spotlight rojo del pez abisal (indice 5) - rojo, se controla con teclas G/H/T para rotar
	// Intensidad similar al helicoptero, se enciende/apaga con tecla R
	spotLights[5] = SpotLight(
		1.0f, 0.0f, 0.0f,   // Color rojo
		0.0f, 8.0f,          // difusa fuerte (similar al helicoptero)
		0.0f, 0.0f, 0.0f,   // Posicion inicial (se sobreescribe cada frame)
		0.0f, -1.0f, 0.0f,  // Direccion inicial hacia abajo (se sobreescribe cada frame)
		0.9f, 0.0f, 0.0f,   // Atenuacion minima (similar al helicoptero)
		23.0f);              // Cono amplio (similar al helicoptero)
	spotLightCount++;
	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	// Variables del loop declaradas fuera para evitar re-construccion cada frame
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelHeliBase(1.0);
	glm::mat4 modelNaveBase(1.0);
	glm::vec3 color(1.0f, 1.0f, 1.0f);
	glm::vec3 lowerLight;
	glm::vec3 laternePuntaWorld;
	glm::vec3 heliPos;
	glm::vec3 heliDir;
	glm::vec4 faroLocalDir;
	glm::vec4 faroIzqLocalPos;
	glm::vec4 faroDerLocalPos;
	glm::vec3 faroIzqPosWorld;
	glm::vec3 faroDerPosWorld;
	glm::vec3 faroDirWorld;
	unsigned int spotCountFrame = 4;

	// Variables jerarquia del pez abisal
	glm::mat4 modelPezBase(1.0);
	glm::vec3 posBulboPezFinal(0.0f);

	// Limites del pez dentro de la pecera (eje X, largo = 7, mitad = 3.5)
	const float PEZ_X_MIN = -2.0f;
	const float PEZ_X_MAX =  2.0f;
	const float PEZ_AMP   =  0.5f;   // amplitud vertical senoidal (unidades mundo)
	const float PEZ_FREQ  =  0.8f;   // frecuencia senoidal (rad/seg, sobre tiempo real)
	const float PEZ_VEL   =  0.0092f; // velocidad horizontal (unidades/frame)
	float pezPosZ  = mainWindow.getPezPosZ();
	float pezTiempo = mainWindow.getPezTiempo();
	float pezOffsetY = 0.0f;

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// ── Movimiento del pez (teclas O / P) ────────────────────────────────
		{
			int dir = mainWindow.getDireccionPez();
			if (dir != 0)
			{
				// Desplazamiento horizontal uniforme: velocidad fija * tiempo real
				float nuevaX = pezPosZ + dir * PEZ_VEL;
				if (nuevaX < PEZ_X_MIN) nuevaX = PEZ_X_MIN;
				if (nuevaX > PEZ_X_MAX) nuevaX = PEZ_X_MAX;
				pezPosZ = nuevaX;
				// Tiempo senoidal: acumula solo cuando se mueve, a ritmo constante
				pezTiempo += 0.016f * PEZ_FREQ; // ~60fps, incremento fijo
			}
		}
		// Calcular offset Y senoidal para el pez
		pezOffsetY = PEZ_AMP * sinf((float)glfwGetTime() * PEZ_FREQ);
		// ── Fin movimiento pez ────────────────────────────────────────────────

		// Tecla X: ciclar color de faros del Mustang (deteccion de flanco)
		if (mainWindow.getsKeys()[GLFW_KEY_X])
		{
			if (!teclaXPresionada)
			{
				indiceFaro = (indiceFaro + 1) % NUM_COLORES_FARO;
				teclaXPresionada = true;
			}
		}
		else
		{
			teclaXPresionada = false;
		}

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		// Calcular posicion jerarquica de la lampara ANTES de enviar luces al shader
		// para que todos los objetos de la escena reciban su iluminacion
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, -1.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		laternePuntaWorld = glm::vec3(model * glm::vec4(0.0f, 9.6f, 0.0f, 1.0f));
		pointLights[1] = PointLight(
			1.0f, 1.0f, 1.0f,
			0.1f, 2.0f,
			laternePuntaWorld.x, laternePuntaWorld.y, laternePuntaWorld.z,
			0.1f, 0.1f, 0.02f);

		// Calcular posicion jerarquica de los spotlights de la nave ANTES de renderizar
		// para que toda la escena reciba su iluminacion correctamente
		modelNaveBase = glm::mat4(1.0);
		modelNaveBase = glm::translate(modelNaveBase, glm::vec3(mainWindow.getmueveNave(), 5.5f, 14.0f));
		modelNaveBase = glm::scale(modelNaveBase, glm::vec3(1.6f, 1.6f, 1.6f));
		{
			glm::vec3 naveCentroWorld = glm::vec3(modelNaveBase * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			glm::vec3 dirFrontal = glm::normalize(glm::vec3(modelNaveBase * glm::vec4(-1.0f, -1.0f, 0.0f, 0.0f)));
			glm::vec3 dirTrasero = glm::normalize(glm::vec3(modelNaveBase * glm::vec4( 1.0f, -1.0f, 0.0f, 0.0f)));

			int dirNave = mainWindow.getDireccionNave();
			if (dirNave == -1)
			{
				// Avanzando (V): luz cian apuntando hacia adelante-abajo
				// Reconstruir spotlight con color cian en indice 4
				spotLights[4] = SpotLight(0.0f, 0.8f, 1.0f, 0.0f, 5.0f,
					naveCentroWorld.x, naveCentroWorld.y, naveCentroWorld.z,
					dirFrontal.x, dirFrontal.y, dirFrontal.z,
					1.0f, 0.0f, 0.0f, 20.0f);
				spotCountFrame = 5; // shader procesa indices 0-4
			}
			else if (dirNave == 1)
			{
				// Retrocediendo (B): luz naranja apuntando hacia atras-abajo
				// Reconstruir spotlight con color naranja en indice 4
				spotLights[4] = SpotLight(1.0f, 0.4f, 0.0f, 0.0f, 5.0f,
					naveCentroWorld.x, naveCentroWorld.y, naveCentroWorld.z,
					dirTrasero.x, dirTrasero.y, dirTrasero.z,
					1.0f, 0.0f, 0.0f, 20.0f);
				spotCountFrame = 5; // shader procesa indices 0-4
			}
			else
			{
				// Quieta: excluir luz de nave — shader solo procesa indices 0-3
				spotCountFrame = 4;
			}
		}

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		
		// pointLights se envía justo antes de renderizar el pez (después de calcular su posición jerárquica)
		// para que la luz se mueva con el modelo, igual que el helicóptero
		
		shaderList[0].SetSpotLights(spotLights, spotCountFrame);




		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//Instancia del coche Kitt
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 0.5f, -3.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Mustang paralelo al Kitt
		model = glm::mat4(1.0);				// izq , arriba , profundidad
		model = glm::translate(model, glm::vec3(mainWindow.getmueveMustang(), 0.5f, 10.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		// Faro frontal: Se calcula la posicion y direccion en espacio mundo
		// usando la matriz model del Mustang (modelado jerarquico)
		// Faros frontales: posicion y direccion en espacio mundo via modelado jerarquico.
		// El Mustang tiene rotacion -180 en Y, por lo que su frente local apunta en +X.
		// Y = altura, Z = separacion lateral izquierda/derecha. 
		faroLocalDir = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);

		// Faro izquierdo
		faroIzqLocalPos = glm::vec4(1.4f, 1.5f, -3.5f, 1.0f);
		faroIzqPosWorld = glm::vec3(model * faroIzqLocalPos);
		faroDirWorld    = glm::normalize(glm::vec3(model * faroLocalDir));

		// Aplicar color del ciclo actual a ambos faros (tecla X cicla entre 6 colores)
		const ColorFaro& cf = cicloFaros[indiceFaro];
		spotLights[1] = SpotLight(cf.r, cf.g, cf.b, 0.0f, 3.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 20.0f);
		spotLights[2] = SpotLight(cf.r, cf.g, cf.b, 0.0f, 3.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 20.0f);

		spotLights[1].SetFlash(faroIzqPosWorld, faroDirWorld);

		// Faro derecho: offset Z positivo
		faroDerLocalPos = glm::vec4(1.4f, 1.5f, 3.5f, 1.0f);
		faroDerPosWorld = glm::vec3(model * faroDerLocalPos);
		spotLights[2].SetFlash(faroDerPosWorld, faroDirWorld);

		// Actualizar spotlights al shader con posiciones jerarquicas de faros
		shaderList[0].SetSpotLights(spotLights, spotCountFrame);

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Mustang_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
	

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(mainWindow.getmueveHelicoptero(), 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		// Reflector del helicoptero: posicion derivada jerarquicamente de la matriz model
		modelHeliBase = glm::mat4(1.0);
		modelHeliBase = glm::translate(modelHeliBase, glm::vec3(mainWindow.getmueveHelicoptero(), 5.0f, 6.0f));
		heliPos = glm::vec3(modelHeliBase * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		heliDir = glm::vec3(0.0f, -1.0f, 0.0f);
		spotLights[3].SetFlash(heliPos, heliDir);

		// Actualizar spotlights al shader con posicion jerarquica del helicoptero
		shaderList[0].SetSpotLights(spotLights, spotCountFrame);

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		// Nave paralela al helicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(mainWindow.getmueveNave(), 5.5f, 14.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Nave_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3( 0.0f, 5.0f, 4.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();

		// Dado de 8 caras (octaedro)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, 5.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado8Texture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		// Lamppost - posicion y luz ya calculadas al inicio del frame
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, -1.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lamparaTexture.UseTexture();
		Laterne_M.RenderModel();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		// ── PEZ ABISAL (jerarquia raiz = pecera) ──────────────────────────────
		// Se renderiza ANTES de la pecera (opaco antes que transparente).
		// glDisable(GL_CULL_FACE) permite verlo desde cualquier lado.

		// Calcular matriz base del pez (similar al helicoptero)
		// Matriz base del pez: parte desde el centro de la pecera
		// pezPosZ controla la posicion en X local de la pecera (largo = 7)
		// pezOffsetY es la oscilacion senoidal en Y
		modelPezBase = glm::mat4(1.0f);
		modelPezBase = glm::translate(modelPezBase, glm::vec3(
			posPecera.x + pezPosZ,   // movimiento sobre X
			posPecera.y + pezOffsetY, // oscilacion senoidal en Y suave
			posPecera.z));            // Z fijo
		modelPezBase = glm::rotate(modelPezBase, 90.0f * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		modelPezBase = glm::scale(modelPezBase, glm::vec3(0.09f, 0.09f, 0.09f));

		// Calcular posicion jerarquica del foco del pez (igual que el helicoptero)
		// Misma traslacion post-scale que se usa al renderizar el foco
		{
			glm::mat4 modelFocoPos = modelPezBase;
			modelFocoPos = glm::translate(modelFocoPos, glm::vec3(-0.04f, 7.85f, 8.84f));
			posBulboPezFinal = glm::vec3(modelFocoPos * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		}

		// Actualizar pointLights[2] con la posicion jerarquica correcta del foco
		pointLights[2] = PointLight(
			0.0f, 1.0f, 2.0f,  // Color azul bioluminiscente el doble de intenso
			1.6f, 10.0f,       // Ambiente y difusa el doble de intensas
			posBulboPezFinal.x, posBulboPezFinal.y, posBulboPezFinal.z,
			0.6f, 0.4f, 0.2f); // Atenuación igual para mantener el alcance corto

		// ── Spotlight rojo del pez (tecla R para encender/apagar, G/H/T para rotar) ──
		// La posición siempre es el foco del pez (jerárquica)
		// Las teclas G/H/T controlan hacia dónde apunta la luz
		{
			// Direccion base: hacia adelante en el espacio local del pez
			// Como el pez está rotado 90° en Y, su "adelante" local es diferente
			glm::vec3 spotDirBase(0.0f, 0.0f, 1.0f); // Adelante en Z local
			
			// Aplicar rotaciones controladas por el usuario (G/H/T)
			// Estas rotaciones son independientes de la orientación del pez
			glm::mat4 rotMat(1.0f);
			rotMat = glm::rotate(rotMat, mainWindow.getSpotlightRojoPezRotX() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); // G: pitch (arriba/abajo)
			rotMat = glm::rotate(rotMat, mainWindow.getSpotlightRojoPezRotY() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // H: yaw (izquierda/derecha)
			rotMat = glm::rotate(rotMat, mainWindow.getSpotlightRojoPezRotZ() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); // T: roll (rotar sobre eje)
			
			// Calcular la direccion final en espacio mundo
			glm::vec3 spotDirWorld = glm::normalize(glm::vec3(rotMat * glm::vec4(spotDirBase, 0.0f)));
			
			// Actualizar spotlight rojo del pez
			// Posición: siempre en el foco del pez (jerárquica, se mueve con el pez)
			// Dirección: controlada por las teclas G/H/T
			spotLights[5] = SpotLight(
				1.0f, 0.0f, 0.0f,   // Color rojo
				0.0f, 8.0f,          // difusa fuerte (similar al helicoptero)
				posBulboPezFinal.x, posBulboPezFinal.y, posBulboPezFinal.z, // Posición jerárquica
				spotDirWorld.x, spotDirWorld.y, spotDirWorld.z,              // Dirección controlable
				0.9f, 0.0f, 0.0f,   // Atenuacion minima
				23.0f);              // Cono amplio
		}
		// ── Fin spotlight rojo del pez ────────────────────────────────────────────

		// Enviar pointLights al shader (igual que el helicoptero envia spotlights)
		// pointLights[0] = luz roja fija (siempre encendida)
		// pointLights[1] = lampara (toggle Z)
		// pointLights[2] = foco del pez (toggle F)
		{
			// Guardar la lámpara original por si necesitamos restaurarla
			PointLight lamparaOriginal = pointLights[1];
			
			unsigned int nPL = 1; // Siempre incluir la luz roja fija (pointLights[0])
			
			// Si ambas están encendidas, enviar las 3 luces
			if (mainWindow.getLamparaEncendida() && mainWindow.getFocoPezEncendido()) {
				nPL = 3;
			}
			// Si solo la lámpara está encendida
			else if (mainWindow.getLamparaEncendida()) {
				nPL = 2;
			}
			// Si solo el foco del pez está encendido, necesitamos enviar luz[0] y luz[2]
			// pero el shader espera luces consecutivas, así que copiamos luz[2] a luz[1] temporalmente
			else if (mainWindow.getFocoPezEncendido()) {
				pointLights[1] = pointLights[2]; // Copiar foco del pez a posición 1
				nPL = 2;
			}
			
			shaderList[0].SetPointLights(pointLights, nPL);
			
			// Restaurar la lámpara original
			pointLights[1] = lamparaOriginal;
		}

		// Enviar spotlights al shader incluyendo el spotlight rojo del pez si está encendido
		{
			unsigned int nSL = spotCountFrame; // Incluye linterna, faros, helicoptero, nave
			if (mainWindow.getSpotlightRojoPezEncendido()) {
				nSL = 6; // Incluir spotlight rojo del pez (indice 5)
			}
			shaderList[0].SetSpotLights(spotLights, nSL);
		}

		// Culling OFF: el pez se ve desde dentro y desde fuera de la pecera
		glDisable(GL_CULL_FACE);
		pezTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);

		// Cuerpo del pez
		color = glm::vec3(1.0f, 3.0f, 1.0f); // tinte verdoso bioluminiscente
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPezBase));
		CuerpoPez_M.RenderModel();

		// Antena - hereda modelPezBase (jerarquia directa)
		AntenaPez_M.RenderModel();

		// Foco del pez - traslacion post-scale hacia la punta de la antena
		{
			glm::mat4 modelFoco = modelPezBase;
			modelFoco = glm::translate(modelFoco, glm::vec3(-0.04f, 7.85f, 8.84f));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelFoco));
			FocoPez_M.RenderModel();
		}

		glEnable(GL_CULL_FACE);
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		// ── FIN PEZ ABISAL ────────────────────────────────────────────────────

		// ── PECERA (transparente, se renderiza DESPUES del pez) ───────────────
		// glDepthMask(GL_FALSE): no escribe en el depth buffer durante el blend,
		// asi el pez (ya en el buffer) se ve a traves del vidrio desde afuera.
		// glDisable(GL_CULL_FACE): la pecera se ve desde dentro y desde fuera.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.5f, 4.5f, -4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		paceraTexture.UseTexture();

		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendColor(0.0f, 0.0f, 0.0f, 0.3f);
		glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
		meshList[6]->RenderMesh();
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		// ── FIN PECERA ────────────────────────────────────────────────────────

		//Agave �qu� sucede si lo renderizan antes del coche y el helic�ptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
