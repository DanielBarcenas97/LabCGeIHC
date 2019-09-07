//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

int screenWidth;
int screenHeight;

GLFWwindow * window;

bool exitApp = false;
int lastMousePosX;
int lastMousePosY;

double deltaTime;
int figure_code;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

GLint vertexShaderID, fragmentShaderID, shaderProgramID;
GLuint VAO, VBO;
GLuint VAO2, VBO2;

typedef struct _Vertex {
	float m_Pos[3];
	float m_Color[3];
} Vertex;
// Codigo de los shaders, por ahora se crean en una cadena de texto
// Shader de vertices
const GLchar * vertexShaderSource = "#version 330 core\n"
		"layout (location=0) in vec3 in_position;\n"
		"layout (location=1) in vec3 in_color;\n"
		"out vec3 our_color;\n"
		"void main(){\n"
		"gl_Position = vec4(in_position, 1.0);\n"
		"our_color = in_color;\n"
		"}\0";
// Shader de fragmento
const GLchar * fragmentShaderSource = "#version 330 core\n"
		"out vec4 color;\n"
		"in vec3 our_color;"
		"void main(){\n"
		"color = vec4(our_color, 1.0);\n"
		"}\0";

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

	// Compilacion de los shaders

	// Se crea el id del Vertex Shader
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	// Se agrega el codigo fuente al ID
	glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
	// Compilación de Vertex Shader
	glCompileShader(vertexShaderID);
	GLint success;
	GLchar infoLog[512];
	// Se obtiene el estatus de la compilacion del vertex shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		// En caso de error se obtiene el error y lanza mensaje con error
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		std::cout << "Error al compilar el VERTEX_SHADER." << infoLog << std::endl;
	}

	// Se crea el id del Fragment Shader
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	// Se agrega el codigo fuente al ID
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
	// Compilacion de Fragment Shader
	glCompileShader(fragmentShaderID);
	// Se obtiene el estatus de la compilacion del Fragment shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		//En caso de error se obtiene el error y lanza mensaje con error
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		std::cout << "Error al compilar el FRAGMENT_SHADER." << infoLog << std::endl;
	}

	// Programa con los shaders
	shaderProgramID = glCreateProgram();
	// Se agregan el vertex y fragment shader al program
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	// Proceso de linkeo
	glLinkProgram(shaderProgramID);
	// Revision de error de linkeo del programa
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		std::cout << "ERROR al linkear el programa." << infoLog << std::endl;
	}

	// Se definen los vertices de la geometria a dibujar
	//GLfloat vertices[] = {-0.5, -0.5, 0.0, 0.5, -0.5, 0.0, 0.0, 0.5, 0.0};
	// Se definen los vertices de la geometria a dibujar
	/*Vertex vertices[] =
	{
		{ {-0.5f, -0.5f, 0.0f}, { 1.0f, 0.0f, 0.0f }},
		{ { 0.5f, -0.5f, 0.0f}, { 0.0f, 1.0f, 0.0f }},
		{ { 0.5f, 0.5f, 0.0f}, { 0.0f, 0.0f, 1.0f }},
		{ {-0.5f, -0.5f, 0.0f}, { 1.0f, 0.0f, 0.0f }},
		{ { 0.5f, 0.5f, 0.0f}, { 0.0f, 0.0f, 1.0f }},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 1.0f}}
	};*/

	Vertex vertices_estrella[] =
	{
		{ {0.0, 0.0, 0.0}, { 0.2, 0.3, 0.5 }},
		{ {0.2, 0.7, 0.0}, { 0.5, 0.3, 0.7 }},
		{ {0.0, 0.7, 0.0}, { 0.2, 0.4, 0.1 }},
		{ {0.0, 0.7, 0.0}, { 0.6, 0.6, 0.45 }},
		{ {0.2, 0.7, 0.0}, { 0.5, 0.3, 0.7 }},
		{ {0.0, 0.85, 0.0}, { 0.4, 0.1, 0.8 }},
		{ {0.0, 0.7, 0.0}, { 0.6, 0.6, 0.45 }},
		{ {-0.2, 0.7, 0.0}, { 0.5, 0.3, 0.7 }},
		{ {0.0, 0.85, 0.0}, { 0.4, 0.1, 0.8 }},
		{ {-0.2, 0.7, 0.0}, { 0.5, 0.3, 0.7 }},
		{ {0.0, 0.7, 0.0}, { 0.6, 0.6, 0.45 }},
		{ {0.0, 0.0, 0.0}, { 0.4, 0.1, 0.8 }},
		{ {0.0, 0.0, 0.0}, { 0.2, 0.3, 0.5 }},
		{ {0.7, 0.2, 0.0}, { 0.5, 0.3, 0.7 }},
		{ {0.7, 0.0, 0.0}, { 0.2, 0.4, 0.1 }},
		{ {0.7, 0.0, 0.0}, { 0.2, 0.4, 0.1 }},
		{ {0.7, 0.2, 0.0}, { 0.5, 0.3, 0.7 }},
		{ {0.85, 0.0, 0.0}, { 0.4, 0.1, 0.8 }},
		{ {0.7, 0.0, 0.0}, { 0.2, 0.4, 0.1 }},
		{ {0.7, -0.2, 0.0}, { 0.5, 0.3, 0.7 }},
		{ {0.85, 0.0, 0.0}, { 0.4, 0.1, 0.8 }},
		{ {0.7, -0.2, 0.0}, { 0.5, 0.3, 0.7 }},
		{ {0.7, 0.0, 0.0}, { 0.2, 0.4, 0.1 }},
		{ {0.0, 0.0, 0.0}, { 0.4, 0.1, 0.8 }},
		{ { 0.0,0.0,0.0}, { 0.2, 0.3, 0.5 }},
		{ { -0.7,0.2, 0.0}, { 0.5, 0.3, 0.7 }},
		{ { -0.7,0.0, 0.0}, { 0.2, 0.4, 0.1 }},
		{ { -0.7,0.0, 0.0}, { 0.6, 0.6, 0.45 }},
		{ { -0.85,0.0, 0.0}, { 0.4, 0.1, 0.8 }},
		{ { -0.7,0.2, 0.0}, { 0.5, 0.3, 0.7 }},
		{ { -0.7,0.0, 0.0}, { 0.6, 0.6, 0.45 }},
		{ { -0.7,-0.2, 0.0}, { 0.5, 0.3, 0.7 }},
		{ { -0.85,0.0, 0.0}, { 0.4, 0.1, 0.8 }},
		{ { -0.7,-0.2, 0.0}, { 0.5, 0.3, 0.7 }},
		{ { -0.7,0.0, 0.0}, { 0.6, 0.6, 0.45 }},
		{ { -0.0,0.0, 0.0}, { 0.4, 0.1, 0.8 }},
		{ {0.0, -0.0, 0.0}, { 0.2, 0.3, 0.5 }},
		{ {0.2, -0.7, 0.0}, { 0.5, 0.3, 0.7 }},
		{ {0.0, -0.7, 0.0}, { 0.2, 0.4, 0.1 }},
		{ {0.0, -0.7, 0.0}, { 0.2, 0.4, 0.1 }},
		{ {0.2,	-0.7, 0.0}, { 0.5, 0.3, 0.7 }},
		{ {0.0, -0.85, 0.0}, { 0.4, 0.1, 0.8 }},
		{ {0.0, -0.7, 0.0}, { 0.2, 0.4, 0.1 }},
		{ {-0.2,-0.7,  0.0}, { 0.5, 0.3, 0.7 }},
		{ {0.0,	-0.85, 0.0}, { 0.4, 0.1, 0.8 }},
		{ {-0.2,-0.7,  0.0}, { 0.5, 0.3, 0.7 }},
		{ {0.0, -0.7, 0.0}, { 0.2, 0.4, 0.1 }},
		{ {0.0, -0.0, 0.0}, { 0.4, 0.1, 0.8 }}
	};

	Vertex vertices_casa[] =
	{
		{ {-0.5, -0.65, 0.0}, { 1.0, 1.0, 1.0 }},//cuarto
		{ {0.5, -0.65, 0.0}, { 1.0, 1.0, 1.0 }},
		{ {0.5, 0.25, 0.0}, { 1.0, 1.0, 1.0 }},
		{ {0.5, 0.25, 0.0}, { 1.0, 1.0, 1.0 }},
		{ {-0.5, 0.25, 0.0}, { 1.0, 1.0, 1.0 }},
		{ {-0.5,-0.65, 0.0}, { 1.0, 1.0, 1.0 }},
		{ {-0.4,-0.65, 0.0}, { 0.1, 0.0, 0.0 }},//puertagrande
		{ {-0.05,-0.65, 0.0}, { 0.1, 0.0, 0.0 }},
		{ {-0.05, 0.0, 0.0}, { 0.1, 0.0, 0.0 }},
		{ {-0.05, 0.0, 0.0}, { 0.1, 0.0, 0.0 }},
		{ {-0.4, 0.0, 0.0}, { 0.1, 0.0, 0.0 }},
		{ {-0.4,-0.65, 0.0}, { 0.1, 0.0, 0.0 }},
		{ {-0.35,-0.6, 0.0}, { 1.0, 0.5, 0.0 }},//puertachica
		{ {-0.1,-0.6, 0.0}, { 1.0, 0.5, 0.0 }},
		{ {-0.1, -0.05, 0.0}, { 1.0, 0.5, 0.0 }},
		{ {-0.1, -0.05, 0.0}, { 1.0, 0.5, 0.0 }},
		{ {-0.35, -0.05, 0.0}, { 1.0, 0.5, 0.0 }},
		{ {-0.35, -0.6, 0.0}, { 1.0, 0.5, 0.0 }},
		{ {0.05, -0.25, 0.0}, { 0.0, 0.7, 0.1 }},//ventanagrande
		{ {0.4, -0.25, 0.0}, {  0.0, 0.7, 0.1}},
		{ {0.4, 0.15, 0.0}, {  0.0, 0.7, 0.1 }},
		{ {0.4, 0.15, 0.0}, {  0.0, 0.7, 0.1 }},
		{ {0.05, 0.15, 0.0}, {  0.0, 0.7, 0.1 }},
		{ {0.05, -0.25, 0.0}, {  0.0, 0.7, 0.1 }},
		{ {0.1, 0.0, 0.0},  { 0.0, 0.5, 0.7 }},//ven1
		{ {0.35, 0.0, 0.0}, { 0.0, 0.5, 0.7 }},
		{ {0.35, 0.1, 0.0}, { 0.0, 0.5, 0.7 }},
		{ {0.35, 0.1, 0.0}, { 0.0, 0.5, 0.7}},
		{ {0.1, 0.1, 0.0}, {  0.0, 0.5, 0.7}},
		{ {0.1, 0.0, 0.0}, {  0.0, 0.5, 0.7}},
		{ {0.1, -0.2, 0.0}, { 0.0, 0.5, 0.7}},//ven2
		{ {0.2, -0.2, 0.0}, { 0.0, 0.5, 0.7}},
		{ {0.2, -0.05, 0.0}, {0.0, 0.5, 0.7 }},
		{ {0.2, -0.05, 0.0}, {0.0, 0.5, 0.7 }},
		{ {0.1, -0.05, 0.0}, {0.0, 0.5, 0.7 }},
		{ {0.1, -0.2, 0.0}, { 0.0, 0.5, 0.7 }},
		{ {0.25, -0.2, 0.0}, { 0.0, 0.5, 0.7}},//ven3
		{ {0.35, -0.2, 0.0}, { 0.0, 0.5, 0.7}},
		{ {0.35, -0.05, 0.0}, {0.0, 0.5, 0.7 }},
		{ {0.35, -0.05, 0.0}, {0.0, 0.5, 0.7 }},
		{ {0.25, -0.05, 0.0}, {0.0, 0.5, 0.7 }},
		{ {0.25, -0.2, 0.0}, { 0.0, 0.5, 0.7}},
		{ {-0.5, 0.25, 0.0}, { 0.8, 0.4, 0.0 }},//cuarto
		{ {0.5, 0.25, 0.0},	 { 0.8, 0.4, 0.0 }},
		{ {0.5, 0.35, 0.0},  { 0.8, 0.4, 0.0 }},
		{ {0.5, 0.35, 0.0},  { 0.8, 0.4, 0.0 }},
		{ {-0.5, 0.35, 0.0}, { 0.8, 0.4, 0.0 }},
		{ {-0.5, 0.25, 0.0}, { 0.8, 0.4, 0.0 }},
		{ {-0.1, 0.5, 0.0}, { 0.0, 0.0, 1.0 }},//venp
		{ {0.1, 0.5, 0.0}, { 0.0, 0.0, 1.0 }},
		{ {0.1, 0.7, 0.0}, { 0.0, 0.0, 1.0 }},
		{ {0.1, 0.7, 0.0}, { 0.0, 0.0, 1.0 }},
		{ {-0.1, .7, 0.0}, { 0.0, 0.0, 1.0 }},
		{ {-0.1, 0.55, 0.0}, { 0.0, 0.0, 1.0 }},
		{ {0.5, 0.35, 0.0}, { 1.0, 0.6, 0.0 }},//pico
		{ {-0.5, 0.35, 0.0}, {1.0, 0.6, 0.0 }},
		{ {0.0, 0.75, 0.0}, { 1.0, 0.6, 0.0 }},
		{ {0.5, 0.35, 0.0}, { 1.0, 0.7, 0.0 }},//chim
		{ {0.5, 0.75, 0.0}, { 1.0, 0.7, 0.0 }},
		{ {0.2, 0.63, 0.0}, { 1.0, 0.7, 0.0 }},
		{ {0.2, 0.63, 0.0}, { 1.0, 0.7, 0.0 }},
		{ {0.5, .75, 0.0}, { 1.0, 0.7, 0.0 }},
		{ {0.2, 0.75, 0.0}, { 1.0, 0.7, 0.0 }},
		{ {0.55, 0.75, 0.0}, { 1.0, 0.5, 0.0 }},//chimc
		{ {0.15, 0.75, 0.0}, { 1.0, 0.5, 0.0}},
		{ {0.15, 0.8, 0.0}, { 1.0, 0.5, 0.0 }},
		{ {0.15, 0.8, 0.0}, { 1.0, 0.5, 0.0 }},
		{ {0.55, .8, 0.0}, { 1.0, 0.5, 0.0 }},
		{ {0.55, 0.75, 0.0}, { 1.0, 0.5, 0.0 }},
		{ {-0.58, 0.35, 0.0}, { 1.0, 0.5, 0.0 }},//pico
		{ {-0.5, .35, 0.0}, { 1.0, 0.5, 0.0 }},
		{ {0.0, 0.85, 0.0}, { 1.0, 0.5, 0.0}},
		{ {0.0, 0.85, 0.0}, { 1.0, 0.5, 0.0 }},
		{ {0.0, 0.75, 0.0}, { 1.0, 0.5, 0.0 }},
		{ {-0.5, 0.35, 0.0}, { 1.0, 0.5, 0.0 }},
		{ {0.58, 0.35, 0.0}, { 1.0, 0.5, 0.0 }},//pico
		{ {0.5, .35, 0.0}, { 1.0, 0.5, 0.0 }},
		{ {0.0, 0.85, 0.0}, { 1.0, 0.5, 0.0}},
		{ {0.0, 0.85, 0.0}, { 1.0, 0.5, 0.0 }},
		{ {0.0, 0.75, 0.0}, { 1.0, 0.5, 0.0 }},
		{ {0.5, 0.35, 0.0}, { 1.0, 0.5, 0.0 }},

	};
	size_t bufferSize = sizeof(vertices_estrella);
	size_t vertexSize = sizeof(vertices_estrella[0]);
	size_t rgbOffset = sizeof(vertices_estrella[0].m_Pos);

	std::cout << "Vertices:" << std::endl;
	std::cout << "bufferSize:" << bufferSize << std::endl;
	std::cout << "vertexSize:" << vertexSize << std::endl;
	std::cout << "rgbOffset:" << rgbOffset << std::endl;
	// Se crea el ID del VAO
	/*
	El VAO es un objeto que nos permite almacenar la estructura de nuestros datos,
	Esto es de gran utilidad debido a que solo se configura la estructura una vez
	y se puede utilizar en el loop de renderizado
	*/
	glGenVertexArrays(1, &VAO);
	// Cambiamos el estado para indicar que usaremos el id del VAO
	glBindVertexArray(VAO);
	// Se crea el VBO (buffer de datos) asociado al VAO
	glGenBuffers(1, &VBO);

	// Cambiamos el estado para indicar que usaremos el id del VBO como Arreglo de vertices (GL_ARRAY_BUFFER)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copiamos los datos de los vertices a memoria del procesador grafico
	//           TIPO DE BUFFER     TAMANIO          DATOS    MODO (No cambian los datos)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_estrella), vertices_estrella, GL_STATIC_DRAW);
	

	// Se crea un indice para el atributo del vertice posicion, debe corresponder al location del atributo del shader
	// indice del atributo, Cantidad de datos, Tipo de dato, Normalizacion, Tamanio del bloque (Stride), offset
	//vertexSize = 6 * sizeof(float)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)0);
	// Se habilita el atributo del vertice con indice 0 (posicion)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)rgbOffset);
	glEnableVertexAttribArray(1);
	// Ya que se configuro, se regresa al estado original
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_casa), vertices_casa, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)rgbOffset);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	glUseProgram(0);
	glDetachShader(shaderProgramID, vertexShaderID);
	glDetachShader(shaderProgramID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glDeleteProgram(shaderProgramID);

	glBindVertexArray(VAO);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glDeleteBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);

	glBindVertexArray(VAO2);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glDeleteBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO2);
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
		case GLFW_KEY_E:
			figure_code = 0;
			break;
		case GLFW_KEY_C:
			figure_code = 1;
			break;

		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
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

bool processInput(bool continueApplication){
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	deltaTime = 1 / TimeManager::Instance().CalculateFrameRate(false);
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT);
		 glUseProgram(shaderProgramID);
		 if (figure_code == 1) {
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 48);
			glBindVertexArray(0);

		 }
		 if(figure_code == 0){
			 glBindVertexArray(VAO2);
			 glDrawArrays(GL_TRIANGLES, 0, 81);
			 glBindVertexArray(0);

		 }
		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
