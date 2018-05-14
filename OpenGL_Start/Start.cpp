/*
Andy Matthew Guinto
*/

//To draw, create a buffer in GPU memory for the mesh
//Fill the buffer with mesh data (vertices)
//Create shaders for knowing how to draw your mesh
//Define the vertex format for the mesh vertices

#include <iostream>
#include <sstream>


#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

const char* APP_TITLE = "AMG Enterprises";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
bool FullScreen = false;
GLFWwindow* gWindow = NULL;
bool gWireFrame = false;

const GLchar* vertexShaderSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;"
"layout (location = 1) in vec3 color;"
"out vec3 vertex_color;"
"void main()"
"{"
"     vertex_color = color;"
"     gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
"}";

const GLchar* fragmentShaderSrc =
"#version 330 core\n"
"in vec3 vertex_color;"
"out vec4 fragment_color;"
"void main()"
"{"
"     fragment_color = vec4(vertex_color, 1.0f);"
"}";


//Hardware Input
void glfw_keyboard_mouse_input ( GLFWwindow* window, int key, int scan_code, int action, int mode );

//FPS Check.
void showFPS ( GLFWwindow* window );

bool initOpenGL ();

/*
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
Main
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/
int main ()
{	
	if (!initOpenGL ())
	{
		std::cerr << "GLFW initialization failed." << std::endl;
		return -1;
	}

	//Vertices for triangle.
	GLfloat vertex_position[] = {	

		-0.5f,  0.5f, 0.0f,	//Top 
		 0.5f,  0.5f, 0.0f,	//Right
		 0.5f, -0.5f, 0.0f,	//Left
		-0.5f, -0.5f, 0.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	//GLfloat vertex_color[] = {
	//	1.0f, 0.0f, 0.0f, //Top 
	//	0.0f, 1.0f, 0.0f, //Right
	//	0.0f, 0.0f, 1.0f,  //Left
	//
	//	1.0f, 0.0f, 0.0f, //Top 
	//	0.0f, 1.0f, 0.0f, //Right
	//	0.0f, 0.0f, 1.0f  //Left
	//};

	//Create vertex buffers, memory in GPU
	GLuint vbo, ibo, vbo2, vao;
	
	//Position
	glGenBuffers ( 1, &vbo );
	glBindBuffer ( GL_ARRAY_BUFFER, vbo );
	glBufferData ( GL_ARRAY_BUFFER, sizeof ( vertex_position ), vertex_position, GL_STATIC_DRAW );


	////Color
	//glGenBuffers ( 1, &vbo2 );
	//glBindBuffer ( GL_ARRAY_BUFFER, vbo2 );
	//glBufferData ( GL_ARRAY_BUFFER, sizeof ( vertex_color ), vertex_color, GL_STATIC_DRAW );

	glGenVertexArrays ( 1, &vao );
	glBindVertexArray ( vao );

	/*
	-----------------------------------------------------------------------------
							Position/Color of Vertex Buffer
	-----------------------------------------------------------------------------
	*///Make sure to bind the buffer first.
	////Only one buffer can be active at a time.

	//*****Position*****							//0 floats between each position
	glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	//By default, OpenGL disables vertex attribute array.
	glEnableVertexAttribArray ( 0 );


	//*****Indices*****
	glGenBuffers ( 1, &ibo );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, ibo );
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( indices ), indices, GL_STATIC_DRAW );

	//*****Color*****
	/*glBindBuffer ( GL_ARRAY_BUFFER, vbo2 );
	glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray ( 1 );*/
	/*
	-----------------------------------------------------------------------------
							End Position/Color of Vertex Buffer
	-----------------------------------------------------------------------------
	*/

	//Create Shader.
	GLuint vs = glCreateShader ( GL_VERTEX_SHADER );
	glShaderSource ( vs, 1, &vertexShaderSrc, NULL );
	glCompileShader ( vs );

	GLint result;
	GLchar infoLog[512];
	glGetShaderiv ( vs, GL_COMPILE_STATUS, &result );

	//Case if it fails.
	if (!result)
	{
		glGetShaderInfoLog ( vs, sizeof ( infoLog ), NULL, infoLog );
		std::cout << "ERROR: Vertex shader failed to compile. " << infoLog << std::endl;
	}

	GLuint fs = glCreateShader ( GL_FRAGMENT_SHADER );
	glShaderSource ( fs, 1, &fragmentShaderSrc, NULL );
	glCompileShader ( fs );

	glGetShaderiv ( fs, GL_COMPILE_STATUS, &result );
	
	//Case if it fails.
	if (!result)
	{
		glGetShaderInfoLog ( vs, sizeof ( infoLog ), NULL, infoLog );
		std::cout << "ERROR: Fragment shader failed to compile. " << infoLog << std::endl;
	}

	//Program Identifier
	GLuint shaderProgram = glCreateProgram (); 
	glAttachShader ( shaderProgram, vs );
	glAttachShader ( shaderProgram, fs );
	glLinkProgram ( shaderProgram );


	glGetProgramiv ( shaderProgram, GL_LINK_STATUS, &result );

	//Case if it fails.
	if (!result)
	{
		glGetProgramInfoLog ( shaderProgram, sizeof ( infoLog ), NULL, infoLog );
		std::cout << "ERROR: Shader Program Linker Failure. " << infoLog << std::endl;
	}
	//Clean up.
	glDeleteShader ( vs );
	glDeleteShader ( fs );

	//Keep Window Open.
	//Main Loop.
	while (!glfwWindowShouldClose ( gWindow ))
	{	
		showFPS ( gWindow );

		//Query keyboard/mouse events.
		glfwPollEvents ();

		
		glClear (GL_COLOR_BUFFER_BIT);

		//Enable Shader.
		glUseProgram ( shaderProgram );

		//Maken this active.
		glBindVertexArray ( vao );
		
		//glDrawArrays ( GL_TRIANGLES, 0, 6 );
		glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );


		//Unbind vertex since only one can be active at a time.
		glBindVertexArray ( 0 );


		//Double Buffering for the same rendering.
		glfwSwapBuffers ( gWindow );
	}
	glDeleteProgram ( shaderProgram );
	glDeleteVertexArrays ( 1, &vao );
	glDeleteBuffers ( 1, &vbo );
	//glDeleteBuffers ( 1, &vbo2 );
	glDeleteBuffers ( 1, &ibo );
	glfwTerminate ();
	return 0;
}
/*
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
End Main
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/

void glfw_keyboard_mouse_input ( GLFWwindow* window, int key, int scan_code, int action, int mode )
{	
	//Close application.
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose ( window, GL_TRUE );
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		gWireFrame = !gWireFrame;
		if (gWireFrame)
		{
			glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
		}
		else
		{
			glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
		}
	}
	if (key == GLFW_MOUSE_BUTTON_1 && action == GLFW_MOUSE_BUTTON_1)
	{
		glfwSetWindowShouldClose ( window, GL_TRUE );
	}
	
}

void showFPS ( GLFWwindow* window )
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;

	double currentSeconds = glfwGetTime (); //Returns the number of seconds since GLFW started.

	elapsedSeconds = currentSeconds - previousSeconds;

	//Limit text update 4 times per second.
	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		std::ostringstream outs;
		outs.precision ( 3 );
		outs << std::fixed
			<< APP_TITLE
			<< "  "
			<< "FPS: " << fps
			<< "  "
			<< "FrameTime: " << msPerFrame << " (ms)";

		glfwSetWindowTitle ( window, outs.str ().c_str () );

		frameCount = 0;
	}

	frameCount++;
}

bool initOpenGL ()
{
	//Initialize GLFW
	if (!glfwInit ())
	{
		std::cerr << "GLFW initialization failed." << std::endl;
		return false;
	}

	glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint ( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

	GLFWwindow* pWindow = NULL;

	if (FullScreen)
	{
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor ();
		const GLFWvidmode* pVmode = glfwGetVideoMode ( pMonitor );

		if (pVmode != NULL)
		{
			pWindow = glfwCreateWindow ( pVmode->width, pVmode->height, APP_TITLE, pMonitor, NULL );
		}
	}
	else
	{
		//Creates Window
		gWindow = glfwCreateWindow ( gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL );
	}

	if (gWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window." << std::endl;
		glfwTerminate ();
		return false;
	}

	//Make this window current.
	//Need this to draw.
	glfwMakeContextCurrent ( gWindow );

	//Keyboard Input.
	glfwSetKeyCallback ( gWindow, glfw_keyboard_mouse_input );


	//Used to set up GLEW.
	glewExperimental = GL_TRUE;

	//Initialize GLEW.
	if (glewInit () != GLEW_OK)
	{
		std::cerr << "GLEW initialization failed." << std::endl;
		return false;
	}

	//Background Color
	glClearColor ( 0.23f, 0.38f, 0.47f, 1.0f );

	return true;
}
