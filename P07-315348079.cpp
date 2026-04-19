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

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model Mustang_M;
Model Laterne_M;


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




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
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

     

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	Mustang_M = Model();
	Mustang_M.LoadModel("Models/mustangCars.obj");

	Laterne_M = Model();
	Laterne_M.LoadModel("Models/lamppost .obj");
	

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

	// Luz puntual blanca en la punta de la lampara
	// Lampara en (0, -1, 1) escala 0.8, altura modelo ~9.6 -> punta en Y ~ 6.7
	pointLights[1] = PointLight(
		1.0f, 1.0f, 1.0f,   // blanco
		0.1f, 2.0f,          // algo de ambiente, difusa fuerte
		-13.0f, 6.7f, 1.0f,   // posicion: punta de la lampara
		0.1f, 0.1f, 0.02f); // atenuacion baja para que ilumine alrededor
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

	//luz fija verde
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f); //Zona de iluminacion 
	spotLightCount++;

	// Faro frontal azul del Mustang (indice 2) - esquina frontal izquierda
	spotLights[2] = SpotLight(
		0.0f, 0.0f, 1.0f,   // Color azul
		0.0f, 3.0f,          // Sin ambiente, difusa fuerte
		0.0f, 0.0f, 0.0f,   // Posicion inicial (se sobreescribe cada frame)
		0.0f, 0.0f, -1.0f,  // Direccion inicial (se sobreescribe cada frame)
		1.0f, 0.0f, 0.0f,   // Atenuacion: solo constante
		20.0f);              // Cono cerrado 20 grados
	spotLightCount++;

	// Faro frontal azul del Mustang (indice 3) - esquina frontal derecha
	spotLights[3] = SpotLight(
		0.0f, 0.3f, 1.0f,   // Color azul con verde
		0.0f, 3.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	// Reflector amarillo del helicoptero (indice 4) - apunta al piso, se mueve con el helicoptero
	spotLights[4] = SpotLight(
		0.50f, 0.5f, 0.0f,  // Color amarillo intenso
		0.0f, 8.0f,          // difusa muy fuerte
		0.0f, 5.0f, 6.0f,   // Posicion inicial (se sobreescribe cada frame)
		0.0f, -1.0f, 0.0f,  // Apunta hacia el piso
		0.9f, 0.0f, 0.0f,  // Atenuacion minima para que llegue lejos
		23.0f);              // Cono amplio tipo reflector
	spotLightCount++;
	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
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

		// luz ligada a la c�mara de tipo flash
		//sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
			glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		//spotLights[1].SetPos(poscoche + glm::vec(x, y, cofre));

		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
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
		glm::vec4 faroLocalDir = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f); // frente local

		// Faro izquierdo: offset Z negativo
		glm::vec4 faroIzqLocalPos = glm::vec4(1.4f, 1.5f, -3.5f, 1.0f);
		glm::vec3 faroIzqPosWorld = glm::vec3(model * faroIzqLocalPos);
		glm::vec3 faroDirWorld    = glm::normalize(glm::vec3(model * faroLocalDir));
		spotLights[2].SetFlash(faroIzqPosWorld, faroDirWorld);

		// Faro derecho: offset Z positivo
		glm::vec4 faroDerLocalPos = glm::vec4(1.4f, 1.5f, 3.5f, 1.0f);
		glm::vec3 faroDerPosWorld = glm::vec3(model * faroDerLocalPos);
		spotLights[3].SetFlash(faroDerPosWorld, faroDirWorld);

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

		// Reflector del helicoptero: posicion y direccion via modelado jerarquico
		glm::vec3 heliPos = glm::vec3(mainWindow.getmueveHelicoptero(), 5.0f, 6.0f);
		glm::vec3 heliDir = glm::vec3(0.0f, -1.0f, 0.0f); // apunta al piso
		spotLights[4].SetFlash(heliPos, heliDir);

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3( 0.0f, 5.0f, 4.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();

		// Lamppost
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, -1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lamparaTexture.UseTexture();
		Laterne_M.RenderModel();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

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
