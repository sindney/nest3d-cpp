#include <fstream>
#include <malloc.h>

#include "camera3d.h"
#include "vector4.h"

#include "utils.h"

using namespace std;
using namespace nest;

bool printLog(const char *log)
{
	fstream logfile;
	logfile.open("log.txt", ios::out | ios::app);

	if(!logfile) return false;

	logfile << log;
	logfile.close();
}

char* readTextFile(const char *fileName) 
{
	char* text;
	if (fileName != NULL) {
		FILE *file = fopen(fileName, "rt");
		if (file != NULL) {
			fseek(file, 0, SEEK_END);
			int count = ftell(file);
			rewind(file);
			if (count > 0) {
				text = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(text, sizeof(char), count, file);
				text[count] = '\0';
			}
			fclose(file);
		}
	}
	return text;
}

bool keys[256];

void onKeyStateChange(int id, int state)
{
	keys[id] = state == 1;
}

extern camera3d *camera0;

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
	camera0->localMatrix.translate(pos * camera0->localMatrix);
	camera0->recompose();
}

bool moving = false;

float DEGREE_90 = 3.1416 / 2;

int oldX = 0, oldY = 0;

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

    	camera0->localMatrix.rotate(vector4(rotX, rotY, 0.0f, 0.0f));
    	camera0->recompose();

		oldX = x;
		oldY = y;
	}
}

void onMouseButtonStateChange(int id, int state)
{
	if(id == 0)
	{
		if(state == 1)
		{
			int x, y;
			getMousePos(&x, &y);
			moving = true;
			oldX = x;
			oldY = y;
		}
		else if(state == 0)
		{
			moving = false;
		}
	}
}