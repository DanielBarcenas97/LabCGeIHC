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
Cylinder cylinder1(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Box box1;
Box box2;
Box box3;

bool exitApp = false;
int lastMousePosX, offsetx = 0;
int lastMousePosY, offsety = 0;

double deltaTime;

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
	glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

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
	sphere2.setColor(glm::vec4(255,255,255, 1.0));

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

	camera->setPosition(glm::vec3(2.0, 0.0, 4.0));
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	//Destruye los VAO,EBO,VBO
	sphere1.destroy();
	cylinder1.destroy();
	box1.destroy();

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
	offsetx = xpos - lastMousePosX;
	offsety = ypos - lastMousePosY;
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

	//Mover la esfera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetx, offsety, 0.01);
	offsetx = 0;
	offsety = 0;


	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		shader.turnOn();

		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		glm::mat4 model = glm::mat4(1.0f);

		//Visualizar con lineas
		//sphere1.enableWireMode();
		//Dibujado de la geometria y recibe la matriz de transformacion 
		//sphere1.render(model);


		/*cylinder1.render(model);
		cylinder1.enableWireMode();*/

		//box1.enableWireMode();
		box1.render(glm::scale(model, glm::vec3(1.0, 1.0, 0.1)));

		//Articulacion
		glm::mat4 j1 = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j1, glm::vec3(0.1, 0.1, 0.1)));

		//Hueso
		glm::mat4 l1 = glm::translate(j1, glm::vec3(0.25f, 0.0f, 0.0f));
		l1 = glm::rotate(l1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l1, glm::vec3(0.1, 0.5, 0.1)));

		//Articulacion2
		glm::mat4 j2 = glm::translate(j1, glm::vec3(0.5f, 0.0f, 0.0f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j2, glm::vec3(0.1, 0.1, 0.1)));

		//Hueso2
		glm::mat4 l2 = glm::translate(j2, glm::vec3(0.215, 0.1223, 0.0f));
		l2 = glm::rotate(l2, glm::radians(120.0f), glm::vec3(0, 0, 1.0));
		cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l2, glm::vec3(0.1, 0.5, 0.1)));

		//Articulacion3
		glm::mat4 j3 = glm::translate(j2, glm::vec3(0.215, 0.1223, 0.0f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j3, glm::vec3(0.1, 0.1, 0.1)));

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

		//hueso Izq
		glm::mat4 h1 = glm::translate(a1, glm::vec3(-0.25f, 0.0f, 0.0f));
		h1 = glm::rotate(h1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder1.enableWireMode();
		cylinder1.render(glm::scale(h1, glm::vec3(0.1, 0.5, 0.1)));

		//articulacion2 Izq
		glm::mat4 a2 = glm::translate(a1, glm::vec3(-0.5f, 0.0f, 0.0f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(a2, glm::vec3(0.1, 0.1, 0.1)));

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







		shader.turnOff();

		glfwSwapBuffers(window);
	}
}

int main(int argc, char** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}