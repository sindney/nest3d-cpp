#ifndef UTILS_H
#define UTILS_H

extern bool printLog(const char *log);

extern char* readTextFile(const char *fileName);

extern float sensitive;

extern float speed;

extern void onKeyStateChange(int id, int state);

extern void handleKeyInput();

extern void onMouseMove(int x, int y);

extern void onMouseButtonStateChange(int id, int state);

extern void getMousePos(int *x, int *y);

#endif