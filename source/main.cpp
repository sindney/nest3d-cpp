#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <typeinfo>
#include <vector>

#include "GL/glew.h"
#include "GL/glfw.h"

#include "event.h"
#include "eventdispatcher.h"
#include "geometry.h"
#include "matrix4.h"
#include "mesh.h"
#include "object3d.h"
#include "quaternion.h"
#include "shader3d.h"
#include "utils.h"
#include "vector4.h"
#include "containerprocess.h"

using namespace nest;
using namespace std;

void checkError();

vector<bool> keys(256, false);

void onKeyStateChange(int id, int state);

void handleKeyInput();

void onMouseMove(int x, int y);

void onMouseButtonStateChange(int id, int state);

camera3d *camera;

container3d *container0;

containerprocess *process0;

int main(void)
{
	// Print Time
	time_t rawtime;
	time(&rawtime);
	struct tm *timeinfo = localtime(&rawtime);
	utils::printLog(asctime(timeinfo));
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
	// GL Setup
	glViewport(0.0f, 0.0f, 800.0f, 600.0f);
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK) exit(EXIT_FAILURE);
	// Initialize Camera3d
	camera = new camera3d();
	camera3d::setupCamera(*camera, 0.7854f, 4 / 3, 1.0f, 1000.0f);
	// Initialize Containerprocess
	container0 = new container3d();
	process0 = new containerprocess(container0, camera);
	// Initialize Shader
	shader3d *shader = new shader3d();
	shader3d::setupShader(*shader, utils::readTextFile("shader_vs.glsl"), utils::readTextFile("shader_fs.glsl"));
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
				mesh0->parent = container0;
				container0->objects.push_back(static_cast<object3d*>(mesh0));
				mesh0->recompose();
				mesh0 = NULL;
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
	vector<container3d*> containers;
	container3d *current = container0;
	object3d *object0 = NULL;
	while(true)
	{
		while(current->objects.size() != 0)
		{
			object0 = current->objects.back();
			current->objects.pop_back();
			if(typeid(*object0) == typeid(container3d))
			{
				containers.push_back(static_cast<container3d*>(object0));
			} 
			else if(typeid(*object0) == typeid(mesh))
			{
				mesh0 = static_cast<mesh*>(object0);
				delete mesh0;
			}
		}
		delete current;
		if(containers.size() != 0)
		{
			current = containers.back();
			containers.pop_back();
			continue;
		}
		break;
	}
	mesh0 = NULL;
	current = NULL;
	delete geom, shader, camera;
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void checkError()
{
	GLenum error = glGetError();
  	if(error != GL_NO_ERROR)
  	{
  		switch(error)
  		{
  			case GL_INVALID_ENUM:
  				utils::printLog("GL_INVALID_ENUM\n");
  				break;
  			case GL_INVALID_VALUE:
  				utils::printLog("GL_INVALID_VALUE\n");
  				break;
  			case GL_INVALID_OPERATION:
  				utils::printLog("GL_INVALID_OPERATION\n");
  				break;
  			case GL_INVALID_FRAMEBUFFER_OPERATION:
  				utils::printLog("GL_INVALID_FRAMEBUFFER_OPERATION\n");
  				break;
  			case GL_OUT_OF_MEMORY:
  				utils::printLog("GL_OUT_OF_MEMORY\n");
  				break;
  			case GL_STACK_UNDERFLOW:
  				utils::printLog("GL_STACK_UNDERFLOW\n");
  				break;
  			case GL_STACK_OVERFLOW:
  				utils::printLog("GL_STACK_OVERFLOW\n");
  				break;
  		}
  	}
}

void onKeyStateChange(int id, int state)
{
	keys[id] = state == GLFW_PRESS;
}

float speed = 0.5f;

void handleKeyInput()
{
	vector4 pos;
	if(keys[65])
	{
		pos.x = -speed;
	}
	else if(keys[68])
	{
		pos.x = speed;
	}
	else if(keys[87])
	{
		pos.z = -speed;
	}
	else if(keys[83])
	{
		pos.z = speed;
	}
	camera->localMatrix.translate(pos * camera->localMatrix);
	camera->recompose();
}

const float DEGREE_90 = 3.1416 / 2;

int oldX = 0, oldY = 0;

bool moving = false;

float sensitive = 0.01f;

float rotX = 0.0f, rotY = 0.0f;

void onMouseMove(int x, int y)
{
	if(moving)
	{
		float t = (oldY - y) * sensitive + rotX;
		if(t >= DEGREE_90) 
		{
			t = DEGREE_90;
		} 
		else if(t <= -DEGREE_90)
		{
			t = -DEGREE_90;
		}
		rotX = t;
		rotY += (oldX - x) * sensitive;

    	camera->localMatrix.rotate(vector4(rotX, rotY, 0.0f, 0.0f));
    	camera->recompose();

		oldX = x;
		oldY = y;
	}
}

void onMouseButtonStateChange(int id, int state)
{
	if(id == GLFW_MOUSE_BUTTON_LEFT)
	{
		if(state == GLFW_PRESS)
		{
			int x, y;
			glfwGetMousePos(&x, &y);
			moving = true;
			oldX = x;
			oldY = y;
		}
		else if(state == GLFW_RELEASE)
		{
			moving = false;
		}
	}
}