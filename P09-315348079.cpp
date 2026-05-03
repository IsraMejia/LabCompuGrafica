/*
Animaci�n:
- Simple o b�sica:Por banderas y condicionales (m�s de 1 transformaci�n geom�trica se ve modificada)
-Compleja: Por medio de funciones y algoritmos. 
-Textura Animada
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

//variables para animaci�n
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0f;
float angulovaria = 0.0f;

// Variables para el sistema de fuego y Aeolipile
bool sistemaActivo = false;
float tiempoSistema = 0.0f;
float duracionMaximaFuego = 600.0f;
float rotacionAeolipile = 0.0f;
float velocidadRotacion = 3.0f;

// Variables para animaci�n de n�meros y fuego
float contadorNumero = 0.0f;
float intervaloNumero = 10.0f;
float animacionFuegoU = 0.0f;
float animacionFuegoV = 0.0f;

// Variables para animacion del humo
float escalaHumo = 1.0f;
float transparenciaHumo = 0.5f;
float duracionAnimacionHumo = 10.0f;

// Variables para animacion de la catapulta
bool animacionCatapultaActiva = false;
bool catapultaDisparadaAutomaticamente = false; // Bandera para evitar disparos múltiples
float tiempoCatapulta = 0.0f;
float duracionLanzamiento = 50.0f; // 50 unidades de tiempo para lanzar (~5 segundos reales) - doble de rápido
float duracionParabolica = 200.0f; // 200 unidades de tiempo para el tiro parabólico (~20 segundos reales)
float duracionRegreso = 50.0f; // 50 unidades de tiempo para regresar (~5 segundos reales) - doble de rápido
float anguloInicioCatapulta = -20.0f;
float anguloFinCatapulta = 50.0f;
float anguloActualCatapulta = -20.0f;
int faseCatapulta = 0; // 0=lanzamiento, 1=tiro parabólico, 2=regreso
// Traslación de la cuchara
glm::vec3 traslacionInicioCatapulta = glm::vec3(0.0f, 0.5f, -0.15f);
glm::vec3 traslacionFinCatapulta = glm::vec3(0.0f, 0.37f, 0.6f);
glm::vec3 traslacionActualCatapulta = glm::vec3(0.0f, 0.5f, -0.15f);
// Variables para el tiro parabólico de la esfera
glm::vec3 posicionInicialEsfera = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 posicionActualEsfera = glm::vec3(0.0f, 0.0f, 0.0f);
bool esferaEnVuelo = false;
float velocidadInicialX = -0.25f; // Velocidad horizontal hacia -X (hacia adelante) - mitad de distancia
float velocidadInicialY = 0.3f; // Velocidad vertical inicial
float gravedad = 0.01f; // Gravedad reducida para trayectoria más visible
// Variables para rebotes
int numeroRebote = 0; // 0=lanzamiento inicial, 1-4=rebotes
float factorReduccion = 0.75f; // Cada rebote tiene 75% de la velocidad anterior
float velocidadReboteX = 0.0f;
float velocidadReboteY = 0.0f;
float tiempoRebote = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
Texture FuegoTexture;
Texture HumoTexture;



Model Kitt_M;
Model Llanta_M;
Model Pista_M;
Model Aeolipile_base_M;
Model Aeolipile_M;
Model Catapulta_base_M;
Model Catapulta_cuchara_M;
Model Catapulta_esfera_M;

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



//c�lculo del promedio de las normales para sombreado de Phong
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
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

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

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los n�meros

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

	// Mesh triangular para el fuego
	unsigned int fuegoIndices[] = {
	   0, 1, 2,
	   3, 4, 5,
	   6, 7, 8,
	   9, 10, 11
	};

	GLfloat fuegoVertices[] = {
		// Triangulo 1
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 1.0f,

		// Triangulo 2
		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,		0.5f, 1.0f,		1.0f, 0.0f, 0.0f,

		// Triangulo 3
		-0.35f, -0.5f, -0.35f,	0.0f, 0.0f,		0.7f, 0.0f, 0.7f,
		0.35f, -0.5f, 0.35f,	1.0f, 0.0f,		0.7f, 0.0f, 0.7f,
		0.0f, 0.5f, 0.0f,		0.5f, 1.0f,		0.7f, 0.0f, 0.7f,

		// Triangulo 4
		0.35f, -0.5f, -0.35f,	0.0f, 0.0f,		-0.7f, 0.0f, 0.7f,
		-0.35f, -0.5f, 0.35f,	1.0f, 0.0f,		-0.7f, 0.0f, 0.7f,
		0.0f, 0.5f, 0.0f,		0.5f, 1.0f,		-0.7f, 0.0f, 0.7f,
	};

	Mesh* obj8 = new Mesh();
	obj8->CreateMesh(fuegoVertices, fuegoIndices, 96, 12);
	meshList.push_back(obj8);

	// Plano para el humo (10x10 unidades)
	unsigned int humoIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat humoVertices[] = {
		-5.0f, -5.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		5.0f, -5.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		5.0f, 5.0f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
		-5.0f, 5.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	};

	Mesh* obj9 = new Mesh();
	obj9->CreateMesh(humoVertices, humoIndices, 32, 6);
	meshList.push_back(obj9);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(-15.0f, 3.0f, 1.5f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.5f, 0.5f);

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
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA(); 
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();
	FuegoTexture = Texture("Textures/fuego.png");
	FuegoTexture.LoadTextureA();
	HumoTexture = Texture("Textures/humo.png");
	HumoTexture.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Pista_M = Model();
	Pista_M.LoadModel("Models/pista.obj");
	Aeolipile_base_M = Model();
	Aeolipile_base_M.LoadModel("Models/Aeolipile_base.obj");
	Aeolipile_M = Model();
	Aeolipile_M.LoadModel("Models/Aeolipile.obj");
	Catapulta_base_M = Model();
	Catapulta_base_M.LoadModel("Models/base.obj");
	Catapulta_cuchara_M = Model();
	Catapulta_cuchara_M.LoadModel("Models/cuchara.obj");
	Catapulta_esfera_M = Model();
	Catapulta_esfera_M.LoadModel("Models/esfera.obj");


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
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.5f,
		0.0f, -1.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.01f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;

	glm::vec3 lowerLight(0.0f,0.0f,0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f*deltaTime;
		movCoche -= movOffset * deltaTime;
		rotllanta += rotllantaOffset * deltaTime;

		// Animación de flechas
		toffsetflechau += 0.01f * deltaTime;
		if (toffsetflechau > 1.0f) {
			toffsetflechau = 0.0f;
		}

		// PUNTO 1: Animación del número cambiante
		contadorNumero += deltaTime;
		if (contadorNumero >= intervaloNumero) {
			toffsetnumerocambiau += 0.25f;
			if (toffsetnumerocambiau >= 1.0f) {
				toffsetnumerocambiau = 0.0f;
			}
			contadorNumero = 0.0f;
		}

		// PUNTO 2 y 3: Sistema de fuego y rotación del Aeolipile
		bool* teclas = mainWindow.getsKeys();
		
		if (teclas[GLFW_KEY_F] && !sistemaActivo) {
			sistemaActivo = true;
			tiempoSistema = 0.0f;
			catapultaDisparadaAutomaticamente = false; // Resetear bandera al iniciar nuevo ciclo
		}

		if (sistemaActivo) {
			tiempoSistema += deltaTime;

			if (tiempoSistema >= duracionMaximaFuego) {
				sistemaActivo = false;
				catapultaDisparadaAutomaticamente = false; // Resetear bandera al terminar ciclo
				rotacionAeolipile = 0.0f;
			}
			else {
				rotacionAeolipile += velocidadRotacion * deltaTime;

				animacionFuegoU += 0.01f * deltaTime;
				animacionFuegoV = sin(tiempoSistema * 3.0f) * 0.05f;
				
				if (animacionFuegoU > 1.0f) {
					animacionFuegoU = 0.0f;
				}

				// Animacion del humo con escalado gradual
				// Escala base crece continuamente
				escalaHumo = 1.0f + (tiempoSistema * 0.5f);
				
				// Transparencia: de 0.5 a 0.0 en los primeros 10 segundos
				float progreso = tiempoSistema / duracionAnimacionHumo;
				if (progreso > 1.0f) progreso = 1.0f;
				transparenciaHumo = 0.5f * (1.0f - progreso);
				
				// Disparador automático: activar catapulta 3 segundos antes de que termine el humo
				if (tiempoSistema >= 570.0f && !animacionCatapultaActiva && !catapultaDisparadaAutomaticamente) {
					animacionCatapultaActiva = true;
					catapultaDisparadaAutomaticamente = true; // Marcar que ya se disparó
					tiempoCatapulta = 0.0f;
					faseCatapulta = 0;
					anguloActualCatapulta = anguloInicioCatapulta;
					traslacionActualCatapulta = traslacionInicioCatapulta;
					esferaEnVuelo = false;
					numeroRebote = 0;
				}
			}
		}

		// Animación de la catapulta (tecla C) - Duración total: ~30 segundos reales
		if (teclas[GLFW_KEY_C] && !animacionCatapultaActiva) {
			animacionCatapultaActiva = true;
			tiempoCatapulta = 0.0f;
			faseCatapulta = 0;
			esferaEnVuelo = false;
		}

		if (animacionCatapultaActiva) {
			tiempoCatapulta += deltaTime;

			if (faseCatapulta == 0) {
				// FASE 1: Lanzamiento de la cuchara (~10 segundos reales)
				float progreso = tiempoCatapulta / duracionLanzamiento;
				if (progreso >= 1.0f) {
					progreso = 1.0f;
					faseCatapulta = 1;
					tiempoCatapulta = 0.0f;
					esferaEnVuelo = true;
					
					// Calcular la posición real de la esfera en el espacio mundial
					// La esfera está en la cuchara con las siguientes transformaciones:
					// Base: translate(-6, -2, 1.5) + rotate(-90°Y) + scale(3)
					// Cuchara: rotate(50°X) + rotate(-180°Y) + translate(0, 0.37, 0.6)
					// Esfera: rotate(90°Y) + translate(-0.4, 0, 0)
					
					// Aplicar transformaciones manualmente para obtener posición mundial
					glm::mat4 matrizBase = glm::mat4(1.0);
					matrizBase = glm::translate(matrizBase, glm::vec3(-6.0f, -2.0f, 1.5f));
					matrizBase = glm::rotate(matrizBase, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
					matrizBase = glm::scale(matrizBase, glm::vec3(3.0f, 3.0f, 3.0f));
					
					glm::mat4 matrizCuchara = matrizBase;
					matrizCuchara = glm::rotate(matrizCuchara, anguloFinCatapulta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
					matrizCuchara = glm::rotate(matrizCuchara, -180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
					matrizCuchara = glm::translate(matrizCuchara, traslacionFinCatapulta);
					
					glm::mat4 matrizEsfera = matrizCuchara;
					matrizEsfera = glm::rotate(matrizEsfera, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
					matrizEsfera = glm::translate(matrizEsfera, glm::vec3(-0.40f, 0.0f, 0.0f));
					
					// Extraer la posición de la matriz (columna 3)
					glm::vec3 posicionMundial = glm::vec3(matrizEsfera[3]);
					
					// Guardar como posición inicial relativa a la base de la catapulta
					posicionInicialEsfera = posicionMundial - glm::vec3(-6.0f, -2.0f, 1.5f);
				}
				// Interpolación lineal para rotación
				anguloActualCatapulta = anguloInicioCatapulta + (anguloFinCatapulta - anguloInicioCatapulta) * progreso;
				// Interpolación lineal para traslación
				traslacionActualCatapulta = traslacionInicioCatapulta + (traslacionFinCatapulta - traslacionInicioCatapulta) * progreso;
			}
			else if (faseCatapulta == 1) {
				// FASE 2: Tiro parabólico de la esfera Y regreso simultáneo de la cuchara (~20 segundos reales)
				float t = tiempoCatapulta;
				
				// Tiro parabólico de la esfera
				if (t >= duracionParabolica) {
					t = duracionParabolica;
					animacionCatapultaActiva = false;
					esferaEnVuelo = false;
					numeroRebote = 0; // Resetear rebotes
					// Asegurar que la cuchara esté en posición inicial
					anguloActualCatapulta = anguloInicioCatapulta;
					traslacionActualCatapulta = traslacionInicioCatapulta;
				}
				else {
					// Determinar qué velocidades usar según el rebote actual
					float velX = (numeroRebote == 0) ? velocidadInicialX : velocidadReboteX;
					float velY = (numeroRebote == 0) ? velocidadInicialY : velocidadReboteY;
					float tActual = (numeroRebote == 0) ? t : tiempoRebote;
					
					// Ecuaciones de movimiento parabólico en el sistema de coordenadas del mundo
					// La catapulta está en X=-6.0, lanza hacia -X (adelante, hacia la izquierda)
					posicionActualEsfera.x = posicionInicialEsfera.x + velX * tActual; // Movimiento hacia -X
					posicionActualEsfera.y = posicionInicialEsfera.y + velY * tActual - 0.5f * gravedad * tActual * tActual;
					posicionActualEsfera.z = posicionInicialEsfera.z; // Sin movimiento en Z
					
					// Detectar colisión con el suelo 
					if (posicionActualEsfera.y <= 0.20f) {
						posicionActualEsfera.y = 0.20f;
						
						// Incrementar contador de rebotes
						numeroRebote++;
						
						// Si aún quedan rebotes (máximo 4 rebotes después del lanzamiento inicial)
						if (numeroRebote <= 4) {
							// Guardar la posición actual como nueva posición inicial para el rebote
							posicionInicialEsfera = posicionActualEsfera;
							
							// Calcular nuevas velocidades: 75% de las velocidades anteriores
							velocidadReboteX = velX * factorReduccion;
							velocidadReboteY = velY * factorReduccion;
							
							// Resetear el tiempo del rebote
							tiempoRebote = 0.0f;
						}
						else {
							// Ya se completaron los 4 rebotes, terminar animación
							animacionCatapultaActiva = false;
							esferaEnVuelo = false;
							numeroRebote = 0;
							anguloActualCatapulta = anguloInicioCatapulta;
							traslacionActualCatapulta = traslacionInicioCatapulta;
						}
					}
					
					// Incrementar tiempo del rebote si estamos en un rebote
					if (numeroRebote > 0) {
						tiempoRebote += deltaTime;
					}
					
					// Regreso simultáneo de la cuchara a posición inicial
					// Usar duracionRegreso para el regreso, pero sincronizado con duracionParabolica
					float progresoRegreso = t / duracionRegreso;
					if (progresoRegreso > 1.0f) {
						progresoRegreso = 1.0f;
						anguloActualCatapulta = anguloInicioCatapulta;
						traslacionActualCatapulta = traslacionInicioCatapulta;
					}
					else {
						// Interpolación lineal inversa para rotación
						anguloActualCatapulta = anguloFinCatapulta - (anguloFinCatapulta - anguloInicioCatapulta) * progresoRegreso;
						// Interpolación lineal inversa para traslación
						traslacionActualCatapulta = traslacionFinCatapulta - (traslacionFinCatapulta - traslacionInicioCatapulta) * progresoRegreso;
					}
				}
			}
		}

	

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
 
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
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		model= glm::mat4(1.0);
		modelaux= glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//Pista
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.1f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pista_M.RenderModel();

		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche-50.0f, 0.5f, -2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();


		//AEOLIPILE - Modelado jerárquico
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Aeolipile_base_M.RenderModel();

		// Tanque giratorio
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(0.0f, 5.0f, 0.0f));
		modelaux = glm::rotate(modelaux, rotacionAeolipile * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		Aeolipile_M.RenderModel();

		// Humo (jerarquico al tanque giratorio) - Renderizar 2 instancias
		if (sistemaActivo) {
			// Posiciones de los dos humos
			glm::vec3 posicionesHumo[2] = {
				glm::vec3(0.72f, 0.0f, 0.3f),
				glm::vec3(-0.82f, 0.0f, -0.3f)
			};
			
			// Calcular divisor de escala gradual basado en el tiempo (una sola vez)
			float divisorEscala;
			
			if (tiempoSistema <= 6.0f) {
				// De segundo 0 a 6: interpolar de 200.0 a 150.0
				float t = tiempoSistema / 6.0f;
				divisorEscala = 2700.0f - (t * 50.0f);
			}
			else if (tiempoSistema <= 12.0f) {
				// De segundo 6 a 12: interpolar de 150.0 a 90.0
				float t = (tiempoSistema - 6.0f) / 6.0f;
				divisorEscala = 1300.0f - (t * 60.0f);
			}
			else {
				// Despues del segundo 12: continuar reduciendo gradualmente
				float t = (tiempoSistema - 12.0f) / (duracionMaximaFuego - 12.0f);
				divisorEscala = 390.0f - (t * 60.0f);
				if (divisorEscala < 30.0f) divisorEscala = 30.0f;
			}
			
			float escalaGeometria = escalaHumo / divisorEscala;
			
			// Configurar blending y material una sola vez
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			HumoTexture.UseTexture();
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			
			// Renderizar ambos humos
			for (int i = 0; i < 2; i++) {
				glm::mat4 modelHumo = modelaux;
				modelHumo = glm::translate(modelHumo, posicionesHumo[i]);
				modelHumo = glm::scale(modelHumo, glm::vec3(escalaGeometria, escalaGeometria, 1.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelHumo));
				meshList[8]->RenderMesh();
			}
			
			glDisable(GL_BLEND);
		}

		//CATAPULTA - Modelado jerárquico
		// Base de la catapulta (elemento padre)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, -2.0f, 1.5f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Catapulta_base_M.RenderModel();

		// Cuchara (hijo de la base) - hereda transformaciones de la base
		modelaux = model;
		modelaux = glm::rotate(modelaux, anguloActualCatapulta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = glm::rotate(modelaux, -180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = glm::translate(modelaux, traslacionActualCatapulta);
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		Catapulta_cuchara_M.RenderModel();

		// Esfera - renderizado condicional según si está en vuelo o no
		if (esferaEnVuelo) {
			// Esfera en tiro parabólico - posición absoluta en el mundo
			glm::mat4 modelEsfera = glm::mat4(1.0);
			modelEsfera = glm::translate(modelEsfera, glm::vec3(-6.0f, -2.0f, 1.5f)); // Posición base de la catapulta
			modelEsfera = glm::translate(modelEsfera, posicionActualEsfera); // Desplazamiento parabólico
			modelEsfera = glm::scale(modelEsfera, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsfera));
			Catapulta_esfera_M.RenderModel();
		}
		else {
			// Esfera en la cuchara - hereda transformaciones de la cuchara
			glm::mat4 modelEsfera = modelaux;
			modelEsfera = glm::rotate(modelEsfera, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			modelEsfera = glm::translate(modelEsfera, glm::vec3(-0.40f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsfera));
			Catapulta_esfera_M.RenderModel();
		}

		//Agave
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		//Flechas con textura animada
		toffsetflechav = 0.0f;
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//plano con todos los n�meros
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//número 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		for (int i = 1; i < 4; i++)
		{
			//números 2-4
			toffsetnumerou += 0.25;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();

		}

		for (int j = 1; j < 5; j++)
		{
			//números 5-8
			toffsetnumerou += 0.25;
			toffsetnumerov = -0.33;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();
		}


		//número cambiante
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		//cambiar automáticamente entre textura número 1 y número 2
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Numero1Texture.UseTexture();

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		
		glDisable(GL_BLEND);

		// FUEGO ANIMADO
		if (sistemaActivo) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, -1.0f, 1.5f));
			
			// Calcular factor de desvanecimiento
			float tiempoRestante = duracionMaximaFuego - tiempoSistema;
			float factorDesvanecimiento = 1.0f;
			
			if (tiempoRestante <= 100.0f) {
				factorDesvanecimiento = tiempoRestante / 100.0f;
			}
			
			// Variación de escala para efecto de llamas
			float factorEscala = 2.0f + (sin(tiempoSistema * 3.0f) * 0.15f);
			factorEscala *= factorDesvanecimiento;
			model = glm::scale(model, glm::vec3(factorEscala, factorEscala, factorEscala));

			// Offset de textura para movimiento
			toffset = glm::vec2(animacionFuegoU, animacionFuegoV);
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			FuegoTexture.UseTexture();
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[7]->RenderMesh();

			glDisable(GL_BLEND);
		}

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
