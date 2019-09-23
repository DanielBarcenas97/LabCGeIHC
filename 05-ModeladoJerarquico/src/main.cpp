//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int screenWidth;
int screenHeight;

GLFWwindow* window;

Shader shader;
std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());


Sphere sphere1(20, 20);
Sphere sphere2(20, 20);
Sphere sphere3(20, 20);
Cylinder cylinder1(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Box box1;
Box box2;
Box box3;

Cylinder torsoR2D2(20, 20, 0.5, 0.5);//se declara el torso de nuevo modelo
Sphere cabezaR2D2(20, 20);//se declara la cabeza del modelo
Sphere articulacionR2D2(20, 20);//se declara la articulacion del modelo
Cylinder brazoR2D2(20, 20, 0.5, 0.5);//se declara el brazo del modelo
Box pieR2D2;//se declara los pies de soporte del modelo

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

float rot1 = 0.0, rot2 = 0.0;///1
float rot3 = 0.0, rot4 = 0.0;///2

float rota1 = 0.0, rota2 = 0.0;///1
float rota3 = 0.0, rota4 = 0.0;///2

bool sentido = true;

float d11 = 0, d12 = 0, d13 = 0, d14 = 0;   //Movimiento de los dedos
float r21 = 0, r22 = 0, r23 = 0, r24 = 0;

float desplazamiento = 0.0f;
float rotacionTotal = 0.0f;
float brazoDerecho = 0.0f, brazoIzquierdo = 0.0f;

float rot0 = 0;
float dz = 0;

float rota0 = 0;
float dza = 0;

float rotr20 = 0;
float dzd2 = 0;
double deltaTime;///1

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}
	screenWidth = width;
	screenHeight = height;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);
	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}
	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	//Inicializacion de VAO,VBO,EBO
	sphere1.init();
	//Es un metodo0 setter que coloca el apuntador al sheder
	sphere1.setShader(&shader);
	//setter para poner el color de la geometria
	sphere1.setColor(glm::vec4(0.0, 0.0, 0.0, 1.0));
	sphere2.init();
	//Es un metodo0 setter que coloca el apuntador al sheder
	sphere2.setShader(&shader);
	//setter para poner el color de la geometria
	sphere2.setColor(glm::vec4(255, 255, 255, 1.0));
	sphere3.init();
	sphere3.setShader(&shader);
	sphere3.setColor(glm::vec4(255, 0.0, 0.0, 0.0));

	cylinder1.init();
	cylinder1.setShader(&shader);
	cylinder1.setColor(glm::vec4(255, 255, 0.0, 1.0));
	cylinder2.init();
	cylinder2.setShader(&shader);
	cylinder2.setColor(glm::vec4(0.803, 0.521, 0.247, 1.0));
	box1.init();
	box1.setShader(&shader);
	box1.setColor(glm::vec4(255, 255, 0.0, 1.0));

	box2.init();
	box2.setShader(&shader);
	box2.setColor(glm::vec4(0.803, 0.521, 0.247, 1.0));

	box3.init();
	box3.setShader(&shader);
	box3.setColor(glm::vec4(0.0, 0.0, 0.0, 1.0));


	//////////////////////////////////
	//se inicializan los objetos para el modelo de R2D2

	torsoR2D2.init();
	torsoR2D2.setShader(&shader);
	torsoR2D2.setColor(glm::vec4(255, 255, 255, 255));

	cabezaR2D2.init();
	cabezaR2D2.setShader(&shader);
	cabezaR2D2.setColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	articulacionR2D2.init();
	articulacionR2D2.setShader(&shader);
	articulacionR2D2.setColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	brazoR2D2.init();
	brazoR2D2.setShader(&shader);
	brazoR2D2.setColor(glm::vec4(255,255,255, 255));

	pieR2D2.init();
	pieR2D2.setShader(&shader);
	pieR2D2.setColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
	////////////////////////////////////

	camera->setPosition(glm::vec3(2.0, 0.0, 4.0));
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.
	// Destrucción de los VAO, EBO, VBO
	sphere1.destroy();
	cylinder1.destroy();
	box1.destroy();
	//se declaran la funcion destroy() para cuando se termine de ejecutar el programa

	torsoR2D2.destroy();
	sphere3.destroy();
	cabezaR2D2.destroy();
	articulacionR2D2.destroy();
	brazoR2D2.destroy();
	pieR2D2.destroy();
	shader.destroy();
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	TimeManager::Instance().CalculateFrameRate(false);
	deltaTime = TimeManager::Instance().DeltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, 0.01);
	offsetX = 0;
	offsetY = 0;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)///3
		sentido = false;///1
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && sentido)
		rot1 += 0.1;///1
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !sentido)
		rot1 -= 0.1;///3
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && sentido)
		rot2 += 0.1;///1
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && sentido)
		rot3 += 0.1;///2
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && sentido)
		rot4 += 0.1;///2
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)///3
		sentido = true;///1
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && sentido)
		rota1 += 0.1;///1
	else if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && !sentido)
		rota1 -= 0.1;///3
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && sentido)
		rota2 += 0.1;///1
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && sentido)
		rota3 += 0.1;///2
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && sentido)
		rota4 += 0.1;///2

	//Movimiento de los dedos:
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && sentido)
		d11 += 0.01;///2
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && sentido)
		d12 -= 0.01;///2
	//Se mueva completo:
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)///3
		rot0 = 0.01;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)///3
		rot0 = -0.01;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)///3
		dz = 0.01;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)///3
		dz = -0.01;
	//R2D2
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)///3
		rota0 = 0.01;
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)///3
		rota0 = -0.01;
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)///3
		dza = 0.01;
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)///3
		dza = -0.01;
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 modelR2D2 = glm::mat4(1.0f);//referencia para el nuevo modelo
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));
		model = glm::translate(model, glm::vec3(0, 0, dz));
		model = glm::rotate(model, rot0, glm::vec3(0, 1, 0));
		modelR2D2 = glm::translate(modelR2D2, glm::vec3(0.0f, 0.0f, dza));
		modelR2D2 = glm::rotate(modelR2D2, rota0, glm::vec3(0.0f, 1.0f, 0.0f));
		//box1.enableWireMode();
		box1.render(glm::scale(model, glm::vec3(1.0, 1.0, 0.1)));

		//Articulacion
		glm::mat4 j1 = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j1, glm::vec3(0.1, 0.1, 0.1)));
		j1 = glm::rotate(j1, rot1, glm::vec3(0, 0, 1));///1
		j1 = glm::rotate(j1, rot2, glm::vec3(0, 1, 0));///1


		//Hueso
		glm::mat4 l1 = glm::translate(j1, glm::vec3(0.25f, 0.0f, 0.0f));
		l1 = glm::rotate(l1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l1, glm::vec3(0.1, 0.5, 0.1)));

		//Articulacion2
		glm::mat4 j2 = glm::translate(j1, glm::vec3(0.5f, 0.0f, 0.0f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j2, glm::vec3(0.1, 0.1, 0.1)));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j2, glm::vec3(0.1, 0.1, 0.1)));

		//Hueso2
		glm::mat4 l2 = glm::translate(j2, glm::vec3(0.215, 0.1223, 0.0f));
		l2 = glm::rotate(l2, glm::radians(120.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l2, glm::vec3(0.1, 0.5, 0.1)));

		//Articulacion3
		glm::mat4 j3 = glm::translate(j2, glm::vec3(0.45, 0.2523, 0.0f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j3, glm::vec3(0.1, 0.1, 0.1)));
		j3 = glm::rotate(j3, d11, glm::vec3(1, 0, 0));///1
		//j3 = glm::rotate(j3, d11, glm::vec3(0, 1, 0));///1

		//dedo 1
		glm::mat4 d1 = glm::translate(j3, glm::vec3(0.1, 0.1, 0.0f));
		d1 = glm::rotate(d1, glm::radians(120.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(d1, glm::vec3(0.04, 0.2, 0.1)));
		

		//dedo 2
		glm::mat4 d2 = glm::translate(j3, glm::vec3(0.12, 0.05, 0.0f));
		d2 = glm::rotate(d2, glm::radians(100.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(d2, glm::vec3(0.04, 0.2, 0.1)));

		//dedo 3
		glm::mat4 d3 = glm::translate(j3, glm::vec3(0.14, 0.00, 0.0f));
		d3 = glm::rotate(d3, glm::radians(80.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(d3, glm::vec3(0.04, 0.2, 0.1)));

		//dedo 4
		glm::mat4 d4 = glm::translate(j3, glm::vec3(0.16, -0.05, 0.0f));
		d4 = glm::rotate(d4, glm::radians(60.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(d4, glm::vec3(0.04, 0.2, 0.1)));

		//Pupila 
		glm::mat4 pupila1 = glm::translate(model, glm::vec3(0.25, 0.25, 0.065f));
		//sphere1.enableWireMode();						
		sphere1.render(glm::scale(pupila1, glm::vec3(0.1, 0.1, 0.1)));
		//Pupila 2
		glm::mat4 pupila2 = glm::translate(model, glm::vec3(-0.25, 0.25, 0.065f));
		//sphere1.enableWireMode();					
		sphere1.render(glm::scale(pupila2, glm::vec3(0.1, 0.1, 0.1)));


		//OjoDer
		glm::mat4 ojo = glm::translate(model, glm::vec3(0.25, 0.25, 0.05f));
		sphere2.render(glm::scale(ojo, glm::vec3(0.2, 0.2, 0.1)));
		//OjoIzq
		glm::mat4 ojo2 = glm::translate(model, glm::vec3(-0.25, 0.25, 0.05f));
		sphere2.render(glm::scale(ojo2, glm::vec3(0.2, 0.2, 0.1)));

		//articulacion Izq
		glm::mat4 a1 = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(a1, glm::vec3(0.1, 0.1, 0.1)));
		a1 = glm::rotate(a1, rot3, glm::vec3(0, 0, 1));///1
		a1 = glm::rotate(a1, rot3, glm::vec3(0, 1, 0));///1
	
		//hueso Izq
		glm::mat4 h1 = glm::translate(a1, glm::vec3(-0.25f, 0.0f, 0.0f));
		h1 = glm::rotate(h1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder1.enableWireMode();
		cylinder1.render(glm::scale(h1, glm::vec3(0.1, 0.5, 0.1)));

		//articulacion2 Izq
		glm::mat4 a2 = glm::translate(a1, glm::vec3(-0.5f, 0.0f, 0.0f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(a2, glm::vec3(0.1, 0.1, 0.1)));
		a2 = glm::rotate(a2, rot4, glm::vec3(1, 0, 0));///1
		a2 = glm::rotate(a2, rot4, glm::vec3(0, 1, 0));///1

		//hueso2 Izq 
		glm::mat4 h2 = glm::translate(a2, glm::vec3(-0.215, -0.1223, 0.0f));
		h2 = glm::rotate(h2, glm::radians(120.0f), glm::vec3(0, 0, 1.0));
		cylinder1.enableWireMode();
		cylinder1.render(glm::scale(h2, glm::vec3(0.1, 0.5, 0.1)));

		//Pantalon
		glm::mat4 p = glm::translate(model, glm::vec3(0.0f, -0.625f, 0.0f));
		//box2.enableWireMode();
		box2.render(glm::scale(p, glm::vec3(1.0, 0.25, 0.1)));

		glm::mat4 p2 = glm::translate(p, glm::vec3(-0.25f, -0.25f, 0.0f));
		//cylinder2.enableWireMode();
		cylinder2.render(glm::scale(p2, glm::vec3(0.25, 0.25, 0.1)));

		glm::mat4 p3 = glm::translate(p, glm::vec3(0.25f, -0.25f, 0.0f));
		//cylinder2.enableWireMode();
		cylinder2.render(glm::scale(p3, glm::vec3(0.25, 0.25, 0.1)));

		//Pies

		glm::mat4 pie = glm::translate(p2, glm::vec3(0.0f, -0.375f, 0.0f));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(pie, glm::vec3(0.1, 0.5, 0.1)));

		glm::mat4 pie2 = glm::translate(p3, glm::vec3(0.0f, -0.375f, 0.0f));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(pie2, glm::vec3(0.1, 0.5, 0.1)));


		//Pie IZQ
		glm::mat4 p6 = glm::translate(pie, glm::vec3(0.0f, -0.275f, 0.15f));
		p6 = glm::rotate(p6, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		cylinder2.enableWireMode();
		cylinder2.render(glm::scale(p6, glm::vec3(0.1f, 0.25f, 0.1f)));

		//Pie DER
		glm::mat4 p7 = glm::translate(pie2, glm::vec3(0.0f, -0.275f, 0.15f));
		p7 = glm::rotate(p7, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		cylinder2.enableWireMode();
		cylinder2.render(glm::scale(p7, glm::vec3(0.1f, 0.25f, 0.1f)));


		//BOCA
		glm::mat4 b = glm::translate(model, glm::vec3(0.0f, -0.30f, 0.05f));
		//box3.enableWireMode();
		box3.render(glm::scale(b, glm::vec3(0.50, 0.15, 0.1)));


		/////////////////////////////////////////////////////////////////////////////////////////
		//apartir de aqui se modelara a R2D2

		glm::mat4 torso = glm::translate(modelR2D2, glm::vec3(5.0f, 0.0f, 0.0f));
		torso = glm::rotate(torso, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//torsoR2D2.enableWireMode();
		torsoR2D2.render(glm::scale(torso, glm::vec3(1.0f, 1.0f, 1.0f)));

		glm::mat4 cabeza = glm::translate(torso, glm::vec3(0.0f, 0.5f, 0.0f));
		//cabezaR2D2.enableWireMode();
		cabezaR2D2.render(glm::scale(cabeza, glm::vec3(1.0f, 1.0f, 1.0f)));

		//
		///ojo
		glm::mat4 oj1 = glm::translate(cabeza, glm::vec3(0.0f, 0.2f, 0.4f));
		//sphere1.enableWireMode();
		sphere3.render(glm::scale(oj1, glm::vec3(0.15, 0.15, 0.15)));

		glm::mat4 articulacioDerecha = glm::translate(torso, glm::vec3(0.55f, 0.35f, 0.0f));
		//articulacionR2D2.enableWireMode();
		articulacionR2D2.render(glm::scale(articulacioDerecha, glm::vec3(0.15f, 0.15f, 0.15f)));
		articulacioDerecha = glm::rotate(articulacioDerecha, rota1, glm::vec3(0, 0, 1));///1
		articulacioDerecha = glm::rotate(articulacioDerecha, rota1, glm::vec3(0, 1, 0));///1

		glm::mat4 brazoDer = glm::translate(articulacioDerecha, glm::vec3(0.05f, -0.4f, -0.13f));
		brazoDer = glm::rotate(brazoDer, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//brazoR2D2.enableWireMode();
		brazoR2D2.render(glm::scale(brazoDer, glm::vec3(0.15f, 1.0f, 0.1f)));

		glm::mat4 munecaDerecha = glm::translate(brazoDer, glm::vec3(0.0f, -0.5f, 0.0f));;
		//articulacionR2D2.enableWireMode();
		articulacionR2D2.render(glm::scale(munecaDerecha, glm::vec3(0.15f, 0.15f, 0.15f)));

		glm::mat4 pataDerecha = glm::translate(munecaDerecha, glm::vec3(0.0f, -0.1f, 0.0f));
		//pieR2D2.enableWireMode();
		pieR2D2.render(glm::scale(pataDerecha, glm::vec3(0.2f, 0.2f, 0.2f)));

		glm::mat4 articulacioIzquierda = glm::translate(torso, glm::vec3(-0.55f, 0.35f, 0.0f));
		//articulacionR2D2.enableWireMode();
		articulacionR2D2.render(glm::scale(articulacioIzquierda, glm::vec3(0.15f, 0.15f, 0.15f)));
		articulacioIzquierda = glm::rotate(articulacioIzquierda, rota2, glm::vec3(1, 0, 0));///1

		glm::mat4 brazoIzq = glm::translate(articulacioIzquierda, glm::vec3(-0.05f, -0.4f, -0.13f));
		brazoIzq = glm::rotate(brazoIzq, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//brazoR2D2.enableWireMode();
		brazoR2D2.render(glm::scale(brazoIzq, glm::vec3(0.15f, 1.0f, 0.1f)));

		glm::mat4 munecaIzquierda = glm::translate(brazoIzq, glm::vec3(0.0f, -0.5f, 0.0f));;
		articulacionR2D2.enableWireMode();
		articulacionR2D2.render(glm::scale(munecaIzquierda, glm::vec3(0.15f, 0.15f, 0.15f)));

		glm::mat4 pataIzquierda = glm::translate(munecaIzquierda, glm::vec3(0.0f, -0.1f, 0.0f));
		//pieR2D2.enableWireMode();
		pieR2D2.render(glm::scale(pataIzquierda, glm::vec3(0.2f, 0.2f, 0.2f)));

		glm::mat4 coxis = glm::translate(torso, glm::vec3(0.0f, -0.15f, 0.0f));
		//cabezaR2D2.enableWireMode();
		cabezaR2D2.render(glm::scale(coxis, glm::vec3(1.0f, 1.0f, 1.0f)));

		glm::mat4 artiulacionCentral = glm::translate(coxis, glm::vec3(0.0f, -0.55f, 0.0f));
		//articulacionR2D2.enableWireMode();
		articulacionR2D2.render(glm::scale(artiulacionCentral, glm::vec3(0.15f, 0.15f, 0.15f)));

		glm::mat4 pataCentral = glm::translate(artiulacionCentral, glm::vec3(0.0f, -0.1f, 0.0f));
		pataCentral = glm::rotate(pataCentral, glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//pieR2D2.enableWireMode();
		pieR2D2.render(glm::scale(pataCentral, glm::vec3(0.2f, 0.2f, 0.2f)));
		/////////////////////////////////////////////////////////////////////////////////////////
		shader.turnOff();

		dz = 0;
		dza = 0;
		rot0 = 0;
		rota0 = 0;
		glfwSwapBuffers(window);
	}
}

int main(int argc, char** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}