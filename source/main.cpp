#include <cstdlib>
#include <ctime>
#include <iostream>

#include "GL/glew.h"
#include "GL/glfw.h"

#include "geometry.h"
#include "matrix4.h"
#include "mesh.h"
#include "shader3d.h"
#include "vector4.h"
#include "containerprocess.h"

#include "utils.h"

void getMousePos(int *x, int *y) { glfwGetMousePos(x, y); }

using namespace nest;
using namespace std;

float sensitive = 0.01f;

float speed = 0.5f;

camera3d *camera0;

containerprocess *process0;

int main(void)
{
	// Print Time
	time_t rawtime;
	time(&rawtime);
	struct tm *timeinfo = localtime(&rawtime);
	printLog(asctime(timeinfo));
	// Initialize GLFW
	if(!glfwInit()) exit(EXIT_FAILURE);
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	if(!glfwOpenWindow(800, 600, 0, 0, 0, 0, 24, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetWindowTitle("Nest3D C++");
	glfwSwapInterval(1);
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK) exit(EXIT_FAILURE);
	// Initialize Containerprocess
	process0 = new containerprocess(
		new rendertarget(0, 0, 800, 600), 
		new container3d(), 
		new camera3d()
	);
	camera3d::setupCamera(*(process0->camera), 0.7854f, 4 / 3, 1.0f, 1000.0f);
	camera0 = process0->camera;
	// Initialize Shader
	char *vs = readTextFile("shader_vs.glsl");
	char *fs = readTextFile("shader_fs.glsl");
	shader3d *shader = new shader3d();
	shader3d::setupShader(*shader, vs, fs);
	delete [] vs, fs;
	glUseProgram(shader->program);
	glUniform4f(glGetUniformLocation(shader->program, "OutColor"), 0.0f, 0.0f, 1.0f, 1.0f);
	// Initialize Geometry
	GLfloat vertexData[] = {-0.5f,  0.5f, -0.5f,  0.5,   0.5f, -0.5f, 
							 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 
							 0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, 
							-0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f};
	GLuint indexData[] = {0, 3, 2, 0, 2, 1, 
							4, 7, 6, 4, 6, 5, 
							5, 0, 1, 5, 1, 4, 
							7, 2, 3, 7, 3, 6, 
							5, 6, 3, 5, 3, 0, 
							1, 2, 7, 1, 7, 4};
	geometrydata *geomdata = new geometrydata();
	geomdata->vertexData = vertexData;
	geomdata->vertexDataSize = 24;
	geomdata->indexData = indexData;
	geomdata->indexDataSize = 36;
	geometry *geom = new geometry();
	geometry::setupAABB(geom->bound, vertexData, 24);
	geometry::setupGeometry(*geom, *geomdata, true, false, false, false, true);
	delete geomdata;
	// Initialize mesh
	mesh *mesh0;
	int i, j, k, l = 10, m = 50;
	for (i = 0; i < l; i++) 
	{
		for (j = 0; j < l; j++) 
		{
			for (k = 0; k < l; k++) 
			{
				mesh0 = new mesh(geom, shader);
				mesh0->localMatrix.translate(vector4(i * 10 - m, j * 10 - m, k * 10 - m, 0.0f));
				process0->container->addChild(mesh0);
			}
		}
	}
	// Input handling
	glfwSetKeyCallback(onKeyStateChange);
	glfwSetMousePosCallback(onMouseMove);
	glfwSetMouseButtonCallback(onMouseButtonStateChange);
	// Rendering
	bool running = true;
	int delay = 0;
	while(running)
	{
		process0->calculate();
		glfwSwapBuffers();
		handleKeyInput();
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
		if(delay < 100)
		{
			delay++;
			continue;
		}
		delay = 0;
		cout<<process0->numObjects<<endl;
	}
	// Delocate
	glfwSetKeyCallback(NULL);
	glfwSetMousePosCallback(NULL);
	glfwSetMouseButtonCallback(NULL);
	delete geom, shader, process0;
	glfwTerminate();
	exit(EXIT_SUCCESS);
}