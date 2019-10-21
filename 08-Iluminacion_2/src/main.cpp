#define _USE_MATH_DEFINES
#include <cmath>
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

//Descomentar
#include "Headers/Texture.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

GLFWwindow* window;

Shader shader;
//Shader de texturizado
Shader shaderTexture;
//Shader con iluminacion solo color
Shader shaderColorLighting;
//Shader con iluminacion y textura
Shader shaderTextureLighting;
// Descomentar
//Shader con materiales
Shader shaderMaterialLighting;
// Descomentar
//Shader con skybox
Shader shaderSkybox;
//shader con multiples luces
Shader shaderMulLighting;


std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere sphere1(20, 20);
Sphere sphere2(20, 20);
Sphere sphere3(20, 20);
Sphere sphereLamp(20, 20);
// Descomentar
Sphere skyboxSphere(20, 20);
Cylinder cylinder1(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Cylinder cylinder3(20, 20, 0.5, 0.5);
// Descomentar
Cylinder cylinderMaterials(20, 20, 0.5, 0.5);
Box box1;
Box box;
Box box2;
Box box3;
Box box8, box9, box10, box4, box5, box6, box7;
Box boxMaterials;

GLuint textureID1, textureID2, textureID3, textureID4,
textureID5, textureID6, textureID7, textureID8,
textureID9, textureID10, textureID11, textureID12,
textureID13, textureID14, textureID15,
textureID16, textureID17, textureID18,
textureID19, textureID20, textureID21,
textureID22, textureID23, textureID24,
textureID25, textureID26, textureID27, textureID28, textureCubeTexture, textureID30;
// Descomentar
GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/ely_darkcity/darkcity_ft.tga",
		"../Textures/ely_darkcity/darkcity_bk.tga",
		"../Textures/ely_darkcity/darkcity_up.tga",
		"../Textures/ely_darkcity/darkcity_dn.tga",
		"../Textures/ely_darkcity/darkcity_rt.tga",
		"../Textures/ely_darkcity/darkcity_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

float rot0 = 0.0, dz = 0.0;

float rot1 = 0.0, rot2 = 0.0, rot3 = 0.0, rot4 = 0.0;
bool sentido = true;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action,
	int mode);
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
	shaderTexture.initialize("../Shaders/texturizado_res.vs",
		"../Shaders/texturizado_res.fs");
	shaderColorLighting.initialize("../Shaders/iluminacion_color_res.vs",
		"../Shaders/iluminacion_color_res.fs");
	shaderTextureLighting.initialize("../Shaders/iluminacion_texture_res.vs",
		"../Shaders/iluminacion_texture_res.fs");
	// Descomentar
	shaderMaterialLighting.initialize("../Shaders/iluminacion_material.vs",
		"../Shaders/iluminacion_material.fs");
	// Descomentar
	shaderSkybox.initialize("../Shaders/cubeTexture.vs",
		"../Shaders/cubeTexture.fs");
	shaderMulLighting.initialize("../Shaders/Iluminacion_texture_res.vs",
		"../Shaders/multipleLights.fs");

	// Inicializar los buffers VAO, VBO, EBO
	sphere1.init();
	// Método setter que colocar el apuntador al shader
	sphere1.setShader(&shaderColorLighting);
	//Setter para poner el color de la geometria
	sphere1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphere2.init();
	// Método setter que colocar el apuntador al shader
	sphere2.setShader(&shaderColorLighting);
	//Setter para poner el color de la geometria
	sphere2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphereLamp.init();
	// Método setter que colocar el apuntador al shader
	sphereLamp.setShader(&shader);
	//Setter para poner el color de la geometria
	sphereLamp.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	cylinder1.init();
	cylinder1.setShader(&shaderColorLighting);
	cylinder1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	cylinder2.init();
	cylinder2.setShader(&shaderTextureLighting);

	cylinder3.init();
	cylinder3.setShader(&shaderTextureLighting);

	// Descomentar
	cylinderMaterials.init();
	cylinderMaterials.setShader(&shaderMaterialLighting);
	cylinderMaterials.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	boxMaterials.init();
	boxMaterials.setShader(&shaderMaterialLighting);
	// Inicialización de la esfera del skybox
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	box.init();
	box.setShader(&shaderTextureLighting);

	box1.init();
	// Settea el shader a utilizar
	box1.setShader(&shaderTextureLighting);
	box1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	box2.init();
	box2.setShader(&shaderTextureLighting);

	sphere3.init();
	sphere3.setShader(&shaderTextureLighting);

	box3.init();
	box3.setShader(&shaderTextureLighting);

	box8.init();
	box8.setShader(&shaderTextureLighting);
	box9.init();
	box9.setShader(&shaderTextureLighting);
	box10.init();
	box10.setShader(&shaderTextureLighting);
	box4.init();
	box4.setShader(&shaderTextureLighting);
	box5.init();
	box5.setShader(&shaderTextureLighting);
	box6.init();
	box6.setShader(&shaderTextureLighting);
	box7.init();
	box7.setShader(&shaderTextureLighting);

	camera->setPosition(glm::vec3(0.0, 0.0, 4.0));



	// Descomentar
	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	// Definiendo la textura a utilizar
	Texture texture1("../Textures/sponge.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	FIBITMAP* bitmap = texture1.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	unsigned char* data = texture1.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID1);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture1.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture2("../Textures/water.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = texture2.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture2.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID2);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture2.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture3("../Textures/goku.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture3.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture3.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID3);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture3.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture4("../Textures/texturaLadrillos.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture4.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture4.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID4);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID4);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture4.freeImage(bitmap);

	/*-----------------------------------------------------------------------------------------------------------*/
	Texture texture6("../Textures/pared.jpg");
	bitmap = texture6.loadImage(true);
	data = texture6.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID6);
	glBindTexture(GL_TEXTURE_2D, textureID6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture6.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture7("../Textures/piso.jpg");
	bitmap = texture7.loadImage(true);
	data = texture7.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7);
	glBindTexture(GL_TEXTURE_2D, textureID7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture7.freeImage(bitmap);

	Texture texture8("../Textures/piel.jpg");
	bitmap = texture8.loadImage(true);
	data = texture8.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID8);
	glBindTexture(GL_TEXTURE_2D, textureID8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture8.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture9("../Textures/mesa.jpg");
	bitmap = texture9.loadImage(true);
	data = texture9.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID9);
	glBindTexture(GL_TEXTURE_2D, textureID9);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture9.freeImage(bitmap);

	Texture texture11("../Textures/espejo.jpg");
	bitmap = texture11.loadImage(true);
	data = texture11.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID11);
	glBindTexture(GL_TEXTURE_2D, textureID11);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture11.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture12("../Textures/maceta.jpg");
	bitmap = texture12.loadImage(true);
	data = texture12.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID12);
	glBindTexture(GL_TEXTURE_2D, textureID12);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture12.freeImage(bitmap);
	//Texture texture13("../Textures/ventana2.png");
	// Definiendo la textura a utilizar

	Texture texture13("../Textures/ventana.png");
	bitmap = texture13.loadImage(true);
	data = texture13.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID13);
	glBindTexture(GL_TEXTURE_2D, textureID13);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture13.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture14("../Textures/cesped.jpg");
	bitmap = texture14.loadImage(true);
	data = texture14.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID14);
	glBindTexture(GL_TEXTURE_2D, textureID14);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture14.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture15("../Textures/seda.jpg");
	bitmap = texture15.loadImage(true);
	data = texture15.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID15);
	glBindTexture(GL_TEXTURE_2D, textureID15);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture15.freeImage(bitmap);



	// Definiendo la textura a utilizar
	Texture texture16("../Textures/almohada.jpg");
	bitmap = texture16.loadImage(true);
	data = texture16.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID16);
	glBindTexture(GL_TEXTURE_2D, textureID16);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture16.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture17("../Textures/ropero.jpg");
	bitmap = texture17.loadImage(true);
	data = texture17.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID17);
	glBindTexture(GL_TEXTURE_2D, textureID17);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture17.freeImage(bitmap);

	Texture texture18("../Textures/metal.jpg");
	bitmap = texture18.loadImage(true);
	data = texture18.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID18);
	glBindTexture(GL_TEXTURE_2D, textureID18);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture18.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture19("../Textures/compu.jpg");
	bitmap = texture19.loadImage(true);
	data = texture19.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID19);
	glBindTexture(GL_TEXTURE_2D, textureID19);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture19.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture20("../Textures/fondo.jpg");
	bitmap = texture20.loadImage(true);
	data = texture20.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID20);
	glBindTexture(GL_TEXTURE_2D, textureID20);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture20.freeImage(bitmap);

	Texture texture24("../Textures/banco.jpg");
	bitmap = texture24.loadImage(true);
	data = texture24.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID24);
	glBindTexture(GL_TEXTURE_2D, textureID24);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture24.freeImage(bitmap);

	Texture texture25("../Textures/teclado.jpg");
	bitmap = texture25.loadImage(true);
	data = texture25.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID25);
	glBindTexture(GL_TEXTURE_2D, textureID25);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture25.freeImage(bitmap);

	Texture texture23("../Textures/barandal.png");
	bitmap = texture23.loadImage(true);
	data = texture23.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID23);
	glBindTexture(GL_TEXTURE_2D, textureID23);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture23.freeImage(bitmap);

	Texture texture22("../Textures/puerta.png");
	bitmap = texture22.loadImage(true);
	data = texture22.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID22);
	glBindTexture(GL_TEXTURE_2D, textureID22);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture22.freeImage(bitmap);

	Texture texture30("../Textures/puertaP.png");
	bitmap = texture30.loadImage(true);
	data = texture30.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID30);
	glBindTexture(GL_TEXTURE_2D, textureID30);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture30.freeImage(bitmap);





	/*------------------------------------------------------------------------------------------------------------*/

	
	// Descomentar
	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	//tipo de textura CUBE_MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//Suavizado a la imagen gl_linear
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP* bitmap = skyboxTexture.loadImage(true);
		unsigned char* data = skyboxTexture.convertToData(bitmap, imageWidth,
			imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
				GL_BGRA, GL_UNSIGNED_BYTE, data);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

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
	box.destroy();
	box8.destroy();
	box9.destroy();
	box10.destroy();
	box2.destroy();
	box3.destroy();
	box4.destroy();
	box5.destroy();
	box6.destroy();
	box7.destroy();

	shader.destroy();
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action,
	int mode) {
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

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		sentido = false;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && sentido)
		rot1 += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !sentido)
		rot1 -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		rot2 += 0.01;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		rot2 -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && sentido)
		rot3 += 0.01;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && sentido)
		rot4 += 0.01;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot0 = 0.001;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot0 = -0.001;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		dz = 0.001;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		dz = -0.001;

	sentido = true;

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	glm::mat4 model = glm::mat4(1.0f);
	float offX = 0.0;
	float angle = 0.0;
	float ratio = 5.0;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con solo textura
		shaderTexture.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con iluminacion solo color
		shaderColorLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderColorLighting.setMatrix4("view", 1, false, glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con iluminacion con textura
		shaderTextureLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderTextureLighting.setMatrix4("view", 1, false,
			glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con iluminacion con material
		shaderMaterialLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderMaterialLighting.setMatrix4("view", 1, false,
			glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
			glm::value_ptr(glm::mat4(glm::mat3(view))));
		//settea la matriz de vist y proyeccion al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
			glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Propiedades de la luz para objetos con color
		shaderColorLighting.setVectorFloat3("viewPos",
			glm::value_ptr(camera->getPosition()));
		shaderColorLighting.setVectorFloat3("light.ambient",
			glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.diffuse",
			glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.specular",
			glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		// Propiedades de la luz para objetos con textura
		shaderTextureLighting.setVectorFloat3("viewPos",
			glm::value_ptr(camera->getPosition()));
		shaderTextureLighting.setVectorFloat3("light.ambient",
			glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.diffuse",
			glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.specular",
			glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		// Propiedades de la luz para objetos con textura
		shaderMaterialLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMaterialLighting.setVectorFloat3("light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMaterialLighting.setVectorFloat3("light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMaterialLighting.setVectorFloat3("light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));

		//propiedades de la liuz para objetos con multiples luces
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(0.0, 0.0, 1.0)));

		glm::mat4 lightModelmatrix = glm::rotate(glm::mat4(1.0f), angle,
			glm::vec3(1.0f, 0.0f, 0.0f));
		lightModelmatrix = glm::translate(lightModelmatrix,
			glm::vec3(0.0f, 0.0f, -ratio));
		// Posicion luz para objetos con color
		shaderColorLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(
					lightModelmatrix
					* glm::vec4(0.0, 0.0, 0.0, 1.0))));

		// Posicion luz para objetos con textura
		shaderTextureLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(
					lightModelmatrix
					* glm::vec4(0.0, 0.0, 0.0, 1.0))));

		// Posicion luz para objetos con materiales
		shaderMaterialLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(
					lightModelmatrix
					* glm::vec4(0.0, 0.0, 0.0, 1.0))));
		sphereLamp.render(lightModelmatrix);

		/*

		model = glm::translate(model, glm::vec3(0, 0, dz));
		model = glm::rotate(model, rot0, glm::vec3(0, 1, 0));
		//box1.enableWireMode();
		//Descomentar
		// Usamos la textura ID 1
		glBindTexture(GL_TEXTURE_2D, textureID1);
		box1.render(glm::scale(model, glm::vec3(1.0, 1.0, 0.1)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		// Articulacion 1
		glm::mat4 j1 = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		j1 = glm::rotate(j1, rot1, glm::vec3(0, 0, 1));
		j1 = glm::rotate(j1, rot2, glm::vec3(0, 1, 0));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j1, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 1
		glm::mat4 l1 = glm::translate(j1, glm::vec3(0.25f, 0.0, 0.0));
		l1 = glm::rotate(l1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l1, glm::vec3(0.1, 0.5, 0.1)));

		// Articulacion 2
		glm::mat4 j2 = glm::translate(j1, glm::vec3(0.5, 0.0f, 0.0f));
		j2 = glm::rotate(j2, rot3, glm::vec3(0.0, 0.0, 1.0));
		j2 = glm::rotate(j2, rot4, glm::vec3(1.0, 0.0, 0.0));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j2, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 2
		glm::mat4 l2 = glm::translate(j2, glm::vec3(0.25, 0.0, 0.0));
		l2 = glm::rotate(l2, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l2, glm::vec3(0.1, 0.5, 0.1)));

		// Ojo
		glm::mat4 ojo = glm::translate(model, glm::vec3(0.25, 0.25, 0.05));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(ojo, glm::vec3(0.2, 0.2, 0.1)));

		glm::mat4 ojo2 = glm::translate(model, glm::vec3(-0.25, 0.25, 0.05));
		//sphere2.enableWireMode();
		sphere2.render(glm::scale(ojo2, glm::vec3(0.2, 0.2, 0.1)));

		glm::mat4 modelAgua = glm::mat4(1.0);
		modelAgua = glm::translate(modelAgua, glm::vec3(0.0, -3.0, 0.0));
		modelAgua = glm::scale(modelAgua, glm::vec3(5.0, 0.01, 5.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID2);
		shaderTexture.setFloat("offsetX", offX);
		box2.render(modelAgua);
		glBindTexture(GL_TEXTURE_2D, 0);
		shaderTexture.setFloat("offsetX", 0);

		*/

		/*------------------------------------------------------------------Inicio casa---------------------------------------------------------------------------*/

		/*Estructura de la casa*/

		//Pared 1 Frente
		///////////glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		////////////box.setPosition(glm::vec3(15.0, 0.0, -3.0));
		///////////box.setScale(glm::vec3(60.0, 30.0, 2.0));
		///////////box.render();

		/// PARED ATRAS ----------------------------Dos pisos completa
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(15.0, 0.0, 36.5));
		box.setScale(glm::vec3(60.0, 30.0, 2.0));
		box.render();

		//Piso
		glBindTexture(GL_TEXTURE_2D, textureID7);
		//box1.setShader(&shaderSpotLight);
		//box1.setProjectionMatrix(projection);
		//box1.setViewMatrix(view);
		box8.setPosition(glm::vec3(15.0, -15.0, 0.0));
		box8.setScale(glm::vec3(60.0, 2.0, 75.0));
		box8.render();

		//aTecho
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box1.setShader(&shaderSpotLight);
		//box1.setProjectionMatrix(projection);
		//box1.setViewMatrix(view);
		box8.setPosition(glm::vec3(15.0, 15.0, 0.0));
		box8.setScale(glm::vec3(61.0, 2.0, 90.0));
		box8.render();

		//Pared lateral 1 Derecha AbajoD
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(-15.0, -7.5, 0.0));
		box9.setScale(glm::vec3(1.0, 17.0, 75.0));
		box9.render();

		//Pared lateral 1 Derecha Arriba
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(-15.0, 8.0, 5.00));
		box9.setScale(glm::vec3(1.0, 16.0, 65.0));
		box9.render();

		//Pared lateral 2 Izquierda
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box3.setShader(&shaderSpotLight);
		//box3.setProjectionMatrix(projection);
		//box3.setViewMatrix(view);
		box10.setPosition(glm::vec3(45.0, 0.0, 0.0));
		box10.setScale(glm::vec3(1.0, 32.0, 75.0));
		box10.render();


		//Pared medio por escaleras
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(24.75, 7.5, -18.00));
		box9.setScale(glm::vec3(0.5, 13.0, 39.0));
		box9.render();

		//Piso intermedio
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box1.setShader(&shaderSpotLight);
		//box1.setProjectionMatrix(projection);
		//box1.setViewMatrix(view);
		box8.setPosition(glm::vec3(5.0, 0.0, 0.0));
		box8.setScale(glm::vec3(40.0, 2.0, 90.0));
		box8.render();

		//Pasto
		glBindTexture(GL_TEXTURE_2D, textureID14);
		//box1.setShader(&shaderSpotLight);
		//box1.setProjectionMatrix(projection);
		//box1.setViewMatrix(view);
		box8.setPosition(glm::vec3(15.0, -14.4, 0.0));
		box8.setScale(glm::vec3(200.0, 0.5, 150.0));
		box8.render();

		//Piedra izquierda 1
		glBindTexture(GL_TEXTURE_2D, textureID4);
		//box1.setShader(&shaderSpotLight);
		//box1.setProjectionMatrix(projection);
		//box1.setViewMatrix(view);
		box8.setPosition(glm::vec3(30.0, -14.20, -50.0));
		box8.setScale(glm::vec3(5.0, 0.5, 10.0));
		box8.render();

		//Piedra izquierda 2
		glBindTexture(GL_TEXTURE_2D, textureID4);
		//box1.setShader(&shaderSpotLight);
		//box1.setProjectionMatrix(projection);
		//box1.setViewMatrix(view);
		box8.setPosition(glm::vec3(40.0, -14.20, -50.0));
		box8.setScale(glm::vec3(5.0, 0.5, 10.0));
		box8.render();

		/*Fin de la Estructura de la casa*/


		/* ------------ Escaleras --------------------*/

		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(26.0, -0.5, 33.0));
		box9.setScale(glm::vec3(3.0, 1.0, 5.0));
		box9.render();

		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(28.0, -2.5, 33.0));
		box9.setScale(glm::vec3(3.0, 1.0, 5.0));
		box9.render();

		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(30.0, -4.5, 33.0));
		box9.setScale(glm::vec3(3.0, 1.0, 5.0));
		box9.render();

		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(32.0, -6.5, 33.0));
		box9.setScale(glm::vec3(3.0, 1.0, 5.0));
		box9.render();

		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(34.0, -8.5, 33.0));
		box9.setScale(glm::vec3(3.0, 1.0, 5.0));
		box9.render();

		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(36.0, -10.5, 33.0));
		box9.setScale(glm::vec3(3.0, 1.0, 5.0));
		box9.render();

		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(38.0, -12.5, 33.0));
		box9.setScale(glm::vec3(3.0, 1.0, 5.0));
		box9.render();

		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(40.0, -14.5, 33.0));
		box9.setScale(glm::vec3(3.0, 1.0, 5.0));
		box9.render();

		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(42.0, -16.5, 33.0));
		box9.setScale(glm::vec3(3.0, 1.0, 5.0));
		box9.render();
		/* ------------ Fin de Escaleras --------------------*/

		/* ----------------- Barandal -----------------------*/
		//Esfera
		glBindTexture(GL_TEXTURE_2D, textureID13);
		//sphere2.setShader(&shaderSpotLight);
		//sphere2.setProjectionMatrix(projection);
		//sphere2.setViewMatrix(view);
		sphere2.setPosition(glm::vec3(-15.0, 6.0, -44.5));
		sphere2.setScale(glm::vec3(1.0, 1.0, 1.0));
		sphere2.render();

		//Murito 1
		glBindTexture(GL_TEXTURE_2D, textureID23);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(-5.0, 3.5, -44.5));
		box.setScale(glm::vec3(20.0, 5.0, 1.0));
		box.render();

		//Murito 2
		glBindTexture(GL_TEXTURE_2D, textureID23);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(-15.0, 3.5, -36.250));
		box.setScale(glm::vec3(1.0, 5.0, 17.5));
		box.render();

		//Esfera
		glBindTexture(GL_TEXTURE_2D, textureID13);
		//sphere2.setShader(&shaderSpotLight);
		//sphere2.setProjectionMatrix(projection);
		//sphere2.setViewMatrix(view);
		sphere2.setPosition(glm::vec3(-15.0, 6.0, -28.0));
		sphere2.setScale(glm::vec3(1.0, 1.0, 1.0));
		sphere2.render();

		//Esfera
		glBindTexture(GL_TEXTURE_2D, textureID13);
		//sphere2.setShader(&shaderSpotLight);
		//sphere2.setProjectionMatrix(projection);
		//sphere2.setViewMatrix(view);
		sphere2.setPosition(glm::vec3(5.0, 6.0, -44.5));
		sphere2.setScale(glm::vec3(1.0, 1.0, 1.0));
		sphere2.render();

		//Murito 3
		glBindTexture(GL_TEXTURE_2D, textureID23);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(15.0, 3.5, -44.5));
		box.setScale(glm::vec3(20.0, 5.0, 1.0));
		box.render();



		/*------------ Fin Barandal --------------------------*/

		/* ----------------- Ventana Superior con puerta DERECHA --------------------*/
		

		//Venatha entrada
		glBindTexture(GL_TEXTURE_2D, textureID13);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(-8.0, 9.0, -27.0));
		box.setScale(glm::vec3(11, 6.0, 1.0));
		box.render();

		//Murito arriba  izquierda y derecha
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(4.5, 13.0, -27.0));
		box.setScale(glm::vec3(38.0, 2.0, 1.0));
		box.render();

		//Murito 
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(-7.5, 3.0, -27.0));
		box.setScale(glm::vec3(14.0, 6.0, 1.0));
		box.render();

		//Murito CUADRITO 1
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(-1.5, 9.0, -27.0));
		box.setScale(glm::vec3(2.0, 6.0, 1.0));
		box.render();

		//Murito CUADRITO 2 
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(-14.0, 9.0, -27.0));
		box.setScale(glm::vec3(1.0, 6.0, 1.0));
		box.render();

		//Puerta
		glBindTexture(GL_TEXTURE_2D, textureID22);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(2.0, 6.5, -27.0));
		box.setScale(glm::vec3(5.0, 11.0, 1.0));
		box.render();

		/* ---------- Fin de Ventana Superior con puerta ---------------------*/


		/* ---------- Ventana Superior con puerta izquierda   ---------------------*/

		//Venatha entrada
		glBindTexture(GL_TEXTURE_2D, textureID13);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(18.25, 9.0, -27.0));
		box.setScale(glm::vec3(10.5, 6.0, 1.0));
		box.render();
		//Murito 
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(18.0, 3.0, -27.0));
		box.setScale(glm::vec3(14.0, 6.0, 1.0));
		box.render();

		//Murito CUADRITO 1
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(12.0, 9.0, -27.0));
		box.setScale(glm::vec3(2.0, 6.0, 1.0));
		box.render();

		//Murito CUADRITO 2 
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(24.0, 10.0, -27.0));
		box.setScale(glm::vec3(1.0, 8.0, 1.0));
		box.render();

		//Puerta
		glBindTexture(GL_TEXTURE_2D, textureID22);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(8.25, 6.5, -27.0));
		box.setScale(glm::vec3(5.5, 11.0, 1.0));
		box.render();

		/* ---------- Fin de Ventana Superior con puerta izquierda ---------------------*/

		// Division de Cuartos Arriba 

		//Pared lateral superior que divide cuartos 
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(5.0, 7.0, -13.5));
		box9.setScale(glm::vec3(1.0, 15.0, 28.0)); //60
		box9.render();

		/* ---------- Cuarto Derecho Entrada ---------------------*/

		//Murito derecheco de frente
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(-7.5, 6.0, 1.0));
		box.setScale(glm::vec3(14.0, 12.00, 1.0));
		box.render();

		//Murito arriba  izquierda y derecha dentro casa
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(4.5, 13.0, 1.0));
		box.setScale(glm::vec3(40.0, 2.0, 1.0));
		box.render();

		//mURO 
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(5.0, 6.0, 1.0));
		box.setScale(glm::vec3(1.0, 12.0, 1.0));
		box.render();

		//Puerta
		glBindTexture(GL_TEXTURE_2D, textureID22);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(2.0, 6.5, 1.0));
		box.setScale(glm::vec3(5.0, 11.0, 1.0));
		box.render();

		/* ---------- Fin Cuarto Derecho Entrada ---------------------*/


		//Murito izuierdo de frente
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(17.5, 6.0, 1.0));
		box.setScale(glm::vec3(14.0, 12.00, 1.0));
		box.render();

		//Puerta --
		glBindTexture(GL_TEXTURE_2D, textureID22);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(8.00, 6.5, 1.0));
		box.setScale(glm::vec3(5.0, 11.0, 1.0));
		box.render();

		/* ---------- Cuarto izquierdo Entrada ---------------------*/

		/*----------------------Baño ------------------------------*/

		//Pared frente de escaleras banio
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(5.0, 7.0, 25.25));
		box9.setScale(glm::vec3(1.0, 15.0, 20.5));
		box9.render();

		//Murito Baño entrada
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(-7.5, 6.0, 15.25));
		box.setScale(glm::vec3(14.0, 12.00, 0.5));
		box.render();

		//Murito arriba  izquierda y derecha dentro banio
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(-5.0, 13.0, 15.25));
		box.setScale(glm::vec3(19.0, 2.0, 0.5));
		box.render();

		//Puerta BANIO
		glBindTexture(GL_TEXTURE_2D, textureID22);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(2.00, 6.5, 15.25));
		box.setScale(glm::vec3(5.0, 11.0, 0.5));
		box.render();

		/*---------------------- Fin Baño -----------------------*/

		/*----------------Barandal sala pelis--------------------*/

		//Esfera
		glBindTexture(GL_TEXTURE_2D, textureID13);
		//sphere2.setShader(&shaderSpotLight);
		//sphere2.setProjectionMatrix(projection);
		//sphere2.setViewMatrix(view);
		sphere2.setPosition(glm::vec3(45.0, 6.0, -38.0));
		sphere2.setScale(glm::vec3(1.0, 1.0, 1.0));
		sphere2.render();

		//Esfera
		glBindTexture(GL_TEXTURE_2D, textureID13);
		//sphere2.setShader(&shaderSpotLight);
		//sphere2.setProjectionMatrix(projection);
		//sphere2.setViewMatrix(view);
		sphere2.setPosition(glm::vec3(25.0, 6.0, -44.5));
		sphere2.setScale(glm::vec3(1.0, 1.0, 1.0));
		sphere2.render();

		//Esfera
		glBindTexture(GL_TEXTURE_2D, textureID13);
		//sphere2.setShader(&shaderSpotLight);
		//sphere2.setProjectionMatrix(projection);
		//sphere2.setViewMatrix(view);
		sphere2.setPosition(glm::vec3(45.0, 6.0, -44.5));
		sphere2.setScale(glm::vec3(1.0, 1.0, 1.0));
		sphere2.render();

		//Murito 1
		glBindTexture(GL_TEXTURE_2D, textureID23);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(35.0, 3.5, -44.5));
		box.setScale(glm::vec3(20.0, 5.0, 1.0));
		box.render();

		glBindTexture(GL_TEXTURE_2D, textureID23);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(45.0, 3.5, -41.25));
		box.setScale(glm::vec3(1.0, 5.0, 7.0));
		box.render();

		/*----------------Fin Barandal sala  pelis--------------------*/

		/*----------------Ventana sala pelis--------------------*/

		//Venatha entrada
		glBindTexture(GL_TEXTURE_2D, textureID13);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(32.5, 9.0, -37.0));
		box.setScale(glm::vec3(11, 6.0, 1.0));
		box.render();

		//Murito 2 enfrente del brandal
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(32.5, 3.0, -37.0));
		box.setScale(glm::vec3(15.0, 6.0, 1.0));
		box.render();

		//Murito 2 enfrente del brandal arriba
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(32.5, 13.0, -37.0));
		box.setScale(glm::vec3(15.0, 2.0, 1.0));
		box.render();

		//Murito CUADRITO 1
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(26.00, 9.0, -37.0));
		box.setScale(glm::vec3(2.0, 6.0, 1.0));
		box.render();

		//Murito CUADRITO 2 
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(39.00, 9.0, -37.0));
		box.setScale(glm::vec3(2.0, 6.0, 1.0));
		box.render();

		//Murito CUADRITO Arriba de puerta
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(42.25, 13.0, -37.0));
		box.setScale(glm::vec3(4.5, 2.0, 1.0));
		box.render();

		//Puerta PELIS
		glBindTexture(GL_TEXTURE_2D, textureID22);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(42.25, 6.5, -37.0));
		box.setScale(glm::vec3(4.5, 11.0, 1.0));
		box.render();

		/*---------------- Fin Ventana sala pelis--------------------*/

		/*--------------------Sala de Pelis--------------------- */

		//Piso intermedio pelis
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box1.setShader(&shaderSpotLight);
		//box1.setProjectionMatrix(projection);
		//box1.setViewMatrix(view);
		box8.setPosition(glm::vec3(35.0, 0.0, -15.0));
		box8.setScale(glm::vec3(21.0, 2.0, 60.0));
		box8.render();

		//Murito 2 enfrente de las escaleras
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(34.75, 7.5, 14.75));
		box.setScale(glm::vec3(19.50, 13.0, 0.5));
		box.render();

		//Barandal por Escaleras
		//Pared frente de escaleras banio
		glBindTexture(GL_TEXTURE_2D, textureID23);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(24.75, 3.5, 22.75));
		box9.setScale(glm::vec3(0.5, 5.0, 15.5));
		box9.render();

		//Entrada cuarto cine
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(24.75, 6.5, 5.50));
		box9.setScale(glm::vec3(0.5, 15.0, 8));
		box9.render();

		//Entrada cuarto cine cuadro arriba puerta
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(24.75, 13.0, 12.250));
		box9.setScale(glm::vec3(0.5, 2.0, 5.5));
		box9.render();

		//Entrada cuarto cine barrita
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(24.75, 6.5, 14.75));
		box9.setScale(glm::vec3(0.5, 11.0, 0.5));
		box9.render();

		/*------------------Fin sala Pelis----------------------*/

		//// -------------  Frente  ------------------------- 

		//Muro ABAJO IZQUIERDA Funciona como pared del baño
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(17.0, -8.5, -37.0));
		box.setScale(glm::vec3(14, 15.0, 1.0));
		box.render();

		//Murito CUADRITO 2 ABAJO Derecha
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(-7.0, -11.5, -37.0));
		box.setScale(glm::vec3(13, 5.0, 1.0));
		box.render();

		//Murito CUADRITO 2 arriba de parte baja
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(-2.25, -2.0, -37.0));
		box.setScale(glm::vec3(24.5, 2.0, 1.0));
		box.render();

		//cASTILLO puerta 2M
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(0.50, -9.0, -37.0));
		box.setScale(glm::vec3(2.0, 12.0, 1.0));
		box.render();


		//cASTILLO Ventana 1M
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(-14.0, -9.0, -37.0));
		box.setScale(glm::vec3(1.0, 12.0, 1.0));
		box.render();

		//Puerta p
		glBindTexture(GL_TEXTURE_2D, textureID30);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(5.75, -8.5, -37.0));
		box.setScale(glm::vec3(8.5, 11.0, 1.0));
		box.render();

		//Venatha entrada
		glBindTexture(GL_TEXTURE_2D, textureID13);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(-7.0, -6.0, -37.0));
		box.setScale(glm::vec3(13, 6.0, 1.0));
		box.render();

		///--------------------------------------Cocina-----------------------------------------------////
		//Murito CUADRITO 2 ABAJO IZQUIERDA
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(17.0, -8.5, 18.0));
		box.setScale(glm::vec3(14, 15.0, 1.0));
		box.render();

		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(17.0, -8.5, -8.0));
		box.setScale(glm::vec3(14, 15.0, 1.0));
		box.render();

		//  pared  cocina completa der
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(10.5, -7.5, 14.75));
		box9.setScale(glm::vec3(1.0, 14.0, 5.5));
		box9.render();

		//  pared mitad cocina
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(10.5, -11.5, 6.75));
		box9.setScale(glm::vec3(1.0, 5.0, 10.5));
		box9.render();

		//  pared mitad cocina arriba
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(10.5, -2.0, 6.75));
		box9.setScale(glm::vec3(1.0, 2.0, 10.5));
		box9.render();

		//  pared  cocina completa izq
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(10.5, -7.5, 0.5));
		box9.setScale(glm::vec3(1.0, 14.0, 2.0));
		box9.render();

		////-------------------------Arriba 

		//  pared mitad cocina arriba puierta
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(10.5, -2.0, -4.00));
		box9.setScale(glm::vec3(1.0, 2.0, 7.0));
		box9.render();


		//  pared mitad cocina arriba
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(10.5, -2.0, -11.50));
		box9.setScale(glm::vec3(1.0, 2.0, 6.0));
		box9.render();


		// -------------------Cochera -----------------------//
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box.setShader(&shaderSpotLight);
		//box.setProjectionMatrix(projection);
		//box.setViewMatrix(view);
		box.setPosition(glm::vec3(34.75, -8.5, -37.0));
		box.setScale(glm::vec3(19.5, 15.0, 1.0));
		box.render();


		/* -----------------------Inician Accesorios ----------------------------------*/
		//SILLON 1
				// sillon respaldo 
		glBindTexture(GL_TEXTURE_2D, textureID8);
		//box4.setShader(&shaderSpotLight);
		//box4.setProjectionMatrix(projection);
		//box4.setViewMatrix(view);
		box4.setPosition(glm::vec3(0.0, -11.0, 0.0));
		box4.setScale(glm::vec3(6.0, 4.0, 4.0));
		box4.render();
		// sillon asiento
		glBindTexture(GL_TEXTURE_2D, textureID8);
		//box5.setShader(&shaderSpotLight);
		//box5.setProjectionMatrix(projection);
		//box5.setViewMatrix(view);
		box5.setPosition(glm::vec3(0.0, -13.0, 0.0));
		box5.setScale(glm::vec3(6.0, 2.0, 8.0));
		box5.render();
		// sillon codo izquierdo 
		glBindTexture(GL_TEXTURE_2D, textureID8);
		//box6.setShader(&shaderSpotLight);
		//box6.setProjectionMatrix(projection);
		//box6.setViewMatrix(view);
		box6.setPosition(glm::vec3(-2.5, -12.0, 0.0));
		box6.setScale(glm::vec3(1.0, 1.5, 8.0));
		box6.render();
		// sillon codo derecho
		glBindTexture(GL_TEXTURE_2D, textureID8);
		//box7.setShader(&shaderSpotLight);
		//box7.setProjectionMatrix(projection);
		//box7.setViewMatrix(view);
		box7.setPosition(glm::vec3(2.5, -12.0, 0.0));
		box7.setScale(glm::vec3(1.0, 1.5, 8.0));
		box7.render();



		//_---------------------------------------
		//SILLON 2

		// sillon respaldo 
		glBindTexture(GL_TEXTURE_2D, textureID8);
		//box4.setShader(&shaderSpotLight);
		//box4.setProjectionMatrix(projection);
		//box4.setViewMatrix(view);
		box4.setPosition(glm::vec3(-8.0, -11.0, 8.0));
		box4.setScale(glm::vec3(1.0, 2.0, 8.0));
		box4.render();
		// sillon asiento
		glBindTexture(GL_TEXTURE_2D, textureID8);
		//box5.setShader(&shaderSpotLight);
		//box5.setProjectionMatrix(projection);
		//box5.setViewMatrix(view);
		box5.setPosition(glm::vec3(-7.0, -13.0, 8.0));
		box5.setScale(glm::vec3(3.0, 2.0, 8.0));
		box5.render();
		// sillon codo izquierdo 
		glBindTexture(GL_TEXTURE_2D, textureID8);
		//box6.setShader(&shaderSpotLight);
		//box6.setProjectionMatrix(projection);
		//box6.setViewMatrix(view);
		box6.setPosition(glm::vec3(-7.0, -11.0, 11.5));
		box6.setScale(glm::vec3(3.0, 1.0, 1.0));
		box6.render();

		//_---------------------------------------
		//SILLON 3

		// sillon respaldo 
		glBindTexture(GL_TEXTURE_2D, textureID8);
		//box4.setShader(&shaderSpotLight);
		//box4.setProjectionMatrix(projection);
		//box4.setViewMatrix(view);
		box4.setPosition(glm::vec3(8.0, -11.0, 8.0));
		box4.setScale(glm::vec3(1.0, 2.0, 8.0));
		box4.render();
		// sillon asiento
		glBindTexture(GL_TEXTURE_2D, textureID8);
		//box5.setShader(&shaderSpotLight);
		//box5.setProjectionMatrix(projection);
		//box5.setViewMatrix(view);
		box5.setPosition(glm::vec3(7.0, -13.0, 8.0));
		box5.setScale(glm::vec3(3.0, 2.0, 8.0));
		box5.render();
		// sillon codo izquierdo 
		glBindTexture(GL_TEXTURE_2D, textureID8);
		//box6.setShader(&shaderSpotLight);
		//box6.setProjectionMatrix(projection);
		//box6.setViewMatrix(view);
		box6.setPosition(glm::vec3(7.0, -11.0, 11.5));
		box6.setScale(glm::vec3(3.0, 1.0, 1.0));
		box6.render();

		// sillon codo derecho
		glBindTexture(GL_TEXTURE_2D, textureID8);
		//box7.setShader(&shaderSpotLight);
		//box7.setProjectionMatrix(projection);
		//box7.setViewMatrix(view);
		box7.setPosition(glm::vec3(7.0, -11.0, 5.0));
		box7.setScale(glm::vec3(3.0, 1.0, 1.0));
		box7.render();


		//--------------------------- centro de mesa
		//cilindro
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//cylinder3.setShader(&shaderPointLight);
		//cylinder3.setProjectionMatrix(projection);
		//cylinder3.setViewMatrix(view);
		cylinder3.setPosition(glm::vec3(0.0, -13.0, 8.0));
		cylinder3.setScale(glm::vec3(3.0, 3.0, 3.0));
		cylinder3.render();

		//-----------------------espejo
		glBindTexture(GL_TEXTURE_2D, textureID11);
		//box4.setShader(&shaderSpotLight);
		//box4.setProjectionMatrix(projection);
		//box4.setViewMatrix(view);
		box4.setPosition(glm::vec3(0.0, -5.0, 0.0));
		box4.setScale(glm::vec3(2.0, 5.0, 0.5));
		box4.render();


		//----------------------- maceta 
		//cilindro
		glBindTexture(GL_TEXTURE_2D, textureID12);
		//cylinder3.setShader(&shaderPointLight);
		//cylinder3.setProjectionMatrix(projection);
		//cylinder3.setViewMatrix(view);
		cylinder3.setPosition(glm::vec3(6.0, -12.0, 14.0));
		cylinder3.setScale(glm::vec3(1.0, 3.0, 1.0));
		cylinder3.render();

		//cactus
		glBindTexture(GL_TEXTURE_2D, textureID13);
		//sphere2.setShader(&shaderSpotLight);
		//sphere2.setProjectionMatrix(projection);
		//sphere2.setViewMatrix(view);
		sphere2.setPosition(glm::vec3(6.0, -10.0, 14.0));
		sphere2.setScale(glm::vec3(1.0, 1.0, 1.0));
		sphere2.render();

		//_________________cama
		glBindTexture(GL_TEXTURE_2D, textureID15);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(-5.0, 3.0, -20.0));
		box9.setScale(glm::vec3(8.0, 2.0, 5.0));
		box9.render();

		//_________________base cama
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(-5.0, 2.0, -20.0));
		box9.setScale(glm::vec3(8.0, 2.0, 5.0));
		box9.render();

		//_________________cabecera cama
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(-9.0, 4.0, -20.0));
		box9.setScale(glm::vec3(0.5, 2.0, 5.0));
		box9.render();
		//-------almohada
		glBindTexture(GL_TEXTURE_2D, textureID16);
		//cylinder3.setShader(&shaderPointLight);
		//cylinder3.setProjectionMatrix(projection);
		//cylinder3.setViewMatrix(view);
		cylinder3.setPosition(glm::vec3(-7.0, 4.0, -20.0));
		cylinder3.setScale(glm::vec3(1.0, 0.5, 3.0));
		cylinder3.render();

		//-------almohada
		glBindTexture(GL_TEXTURE_2D, textureID16);
		//cylinder3.setShader(&shaderPointLight);
		//cylinder3.setProjectionMatrix(projection);
		//cylinder3.setViewMatrix(view);
		cylinder3.setPosition(glm::vec3(-6.0, 4.0, -19.0));
		cylinder3.setScale(glm::vec3(1.0, 0.5, 3.0));
		cylinder3.render();

		//-----------------ropero
		glBindTexture(GL_TEXTURE_2D, textureID17);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(2.0, 5.0, -15.0));
		box9.setScale(glm::vec3(4.0, 6.0, 5.0));
		box9.render();

		//-----------------mesa 
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(-12.0, 5.0, -14.0));
		box9.setScale(glm::vec3(4.0, 1.0, 5.0));
		box9.render();
		//base mesa 
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//cylinder3.setShader(&shaderPointLight);
		//cylinder3.setProjectionMatrix(projection);
		//cylinder3.setViewMatrix(view);
		cylinder3.setPosition(glm::vec3(-12.0, 2.0, -14.0));
		cylinder3.setScale(glm::vec3(1.0, 5.0, 3.0));
		cylinder3.render();

		//----- base computadora
		glBindTexture(GL_TEXTURE_2D, textureID18);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(-12.0, 5.5, -14.0));
		box9.setScale(glm::vec3(1.0, 0.5, 2.0));
		box9.render();
		//----- tapa computadora
		glBindTexture(GL_TEXTURE_2D, textureID19);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(-12.4, 5.4, -14.0));
		box9.setScale(glm::vec3(0.2, 4.0, 2.0));
		box9.render();

		//----- fondo computadora
		glBindTexture(GL_TEXTURE_2D, textureID20);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(-12.3, 5.5, -14.0));
		box9.setScale(glm::vec3(0.1, 3.6, 1.8));
		box9.render();

		//----- teclado computadora
		glBindTexture(GL_TEXTURE_2D, textureID25);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(-12.0, 5.8, -14.0));
		box9.setScale(glm::vec3(0.9, 0.1, 1.8));
		box9.render();

		//banco para computadora
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//cylinder3.setShader(&shaderPointLight);
		//cylinder3.setProjectionMatrix(projection);
		//cylinder3.setViewMatrix(view);
		cylinder3.setPosition(glm::vec3(-10.0, 2.0, -14.0));
		cylinder3.setScale(glm::vec3(3.0, 3.0, 3.0));
		cylinder3.render();

	
		//  pared separa cochera 
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(24.5, -8.5, -9.5));
		box9.setScale(glm::vec3(1.0, 15.0, 56.0));
		box9.render();


		//  pared separa sala y pared cochera
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//box2.setShader(&shaderSpotLight);
		//box2.setProjectionMatrix(projection);
		//box2.setViewMatrix(view);
		box9.setPosition(glm::vec3(10.5, -8.5, -25.5));
		box9.setScale(glm::vec3(1.0, 15.0, 22.0));
		box9.render();

		
		
		



		/*------------------------------------------------------------Fin--------------------------w---------------------------------*/
		/*
		glm::mat4 modelSphere = glm::mat4(1.0);
		modelSphere = glm::translate(modelSphere, glm::vec3(3.0, 0.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID3);
		sphere3.render(modelSphere);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelCylinder = glm::mat4(1.0);
		modelCylinder = glm::translate(modelCylinder,
			glm::vec3(-3.0, 0.0, 0.0));
		// Envolvente desde el indice 0, el tamanio es 20 * 20 * 6
		// Se usa la textura 1 ( Bon sponja)
		glBindTexture(GL_TEXTURE_2D, textureID1);
		cylinder2.render(0, cylinder2.getSlices() * cylinder2.getStacks() * 6,
			modelCylinder);
		// Tapa Superior desde el indice : 20 * 20 * 6, el tamanio de indices es 20 * 3
		// Se usa la textura 2 ( Agua )
		glBindTexture(GL_TEXTURE_2D, textureID2);
		cylinder2.render(cylinder2.getSlices() * cylinder2.getStacks() * 6,
			cylinder2.getSlices() * 3, modelCylinder);
		// Tapa inferior desde el indice : 20 * 20 * 6 + 20 * 3, el tamanio de indices es 20 * 3
		// Se usa la textura 3 ( Goku )
		glBindTexture(GL_TEXTURE_2D, textureID3);
		cylinder2.render(
			cylinder2.getSlices() * cylinder2.getStacks() * 6
			+ cylinder2.getSlices() * 3, cylinder2.getSlices() * 3,
			modelCylinder);
		glBindTexture(GL_TEXTURE_2D, 0);

		

		// Render del cubo con textura de ladrillos y con repeticion en x
		glm::mat4 cubeTextureModel = glm::mat4(1.0);
		cubeTextureModel = glm::translate(cubeTextureModel,
			glm::vec3(3.0, 2.0, 3.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(cubeTextureModel);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Render del cyindro con materiales
		// Descomentar
		glm::mat4 cylinderMaterialModel = glm::mat4(1.0);
		cylinderMaterialModel = glm::translate(cylinderMaterialModel, glm::vec3(3.0, 2.0, -3.0));
		shaderMaterialLighting.setVectorFloat3("material.ambient", glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
		shaderMaterialLighting.setVectorFloat3("material.diffuse", glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.0f)));
		shaderMaterialLighting.setVectorFloat3("material.specular", glm::value_ptr(glm::vec3(0.60f, 0.60f, 0.50f)));
		shaderMaterialLighting.setFloat("material.shininess", 32.0f);
		cylinderMaterials.render(cylinderMaterialModel);

		//Crear matriz de transformacion
		glm::mat4 boxMaterialModel = glm::mat4(1.0f);
		boxMaterialModel = glm::translate(boxMaterialModel, glm::vec3(-3.0, 2.0, -3.0));
		shaderMaterialLighting.setVectorFloat3("material.ambient", glm::value_ptr(glm::vec3(0.0f, 0.05f, 0.05f)));
		shaderMaterialLighting.setVectorFloat3("material.diffuse", glm::value_ptr(glm::vec3(0.4f, 0.5f, 0.5f)));
		shaderMaterialLighting.setVectorFloat3("material.specular", glm::value_ptr(glm::vec3(0.04f, 0.7f, 0.7f)));
		shaderMaterialLighting.setFloat("material.shininess", 32.0f);
		boxMaterials.render(boxMaterialModel);

		*/

		if (angle > 2 * M_PI)
			angle = 0.0;
		else
			angle += 0.0001;

		// Descomentar
		// Se Dibuja el Skybox
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		//se deshabilita el recorde de caras ocultas para ver la esfera dese
		//adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

		dz = 0;
		rot0 = 0;
		offX += 0.001;

		glfwSwapBuffers(window);
	}
}

int main(int argc, char** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}