#include <fstream>
#include <malloc.h>

#include "utils.h"

namespace nest
{
	using namespace std;
	
	bool utils::printLog(const char *log)
	{
		fstream logfile;
		logfile.open("log.txt", ios::out | ios::app);

		if(!logfile) return false;

		logfile << log;
		logfile.close();
	}

	char* utils::readTextFile(const char *fileName) 
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
}