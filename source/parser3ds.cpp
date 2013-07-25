#include <stdexcept>
#include <string>
#include <fstream>

#include "geometry.h"
#include "parser3ds.h"

namespace nest
{
	using namespace std;

	parser3ds::~parser3ds()
	{
		result.clear();
	}

	void parser3ds::parse(const char *location)
	{
		const unsigned int HEADER = 0x4d4d;
		const unsigned int VERSION = 0x0002;
		const unsigned int EDITOR = 0x3d3d;
		const unsigned int OBJECTS = 0x4000;
		const unsigned int MESH = 0x4100;
		const unsigned int VERTICES = 0x4110;
		const unsigned int INDICES = 0x4120;
		const unsigned int UVS = 0x4140;
		const unsigned int USHORT_SIZE = sizeof(unsigned short);
		const unsigned int UINT_SIZE = sizeof(unsigned int);
		const unsigned int FLOAT_SIZE = sizeof(float);

		result.clear();
		
		ifstream file;
		file.open(location, ios::in | ios::binary);

		if(!file || !file.is_open()) throw runtime_error("Error reading 3DS file: Can't open target file.");

		unsigned short id;
		file.read(reinterpret_cast<char *>(&id), USHORT_SIZE);
		if(id != HEADER) throw runtime_error("Error reading 3DS file: Not a valid 3DS file.");

		file.seekg(0, file.end);
		unsigned long file_size = file.tellg();
		file.seekg(6, file.beg);

		unsigned short ushort0, ushort1;
		float float0;

		int i, j;
		unsigned int length;
		geometry *geom;

		while(file.tellg() < file_size)
		{
			file.read(reinterpret_cast<char *>(&id), USHORT_SIZE);
			file.read(reinterpret_cast<char *>(&length), UINT_SIZE);

			switch(id)
			{
				case VERSION:
					file.read(reinterpret_cast<char *>(&i), UINT_SIZE);
					if(i < 3) throw runtime_error("Error reading 3DS file: Not a valid 3DS file version.");
					break;
				case EDITOR:
					break;
				case OBJECTS:
					geom = new geometry();
					getline(file, geom->name, '\0');
					result.push_back(geom);
					break;
				case MESH:
					break;
				case VERTICES:
					file.read(reinterpret_cast<char *>(&ushort0), USHORT_SIZE);
					geom->numVertices = ushort0;
					geom->vertexData = new GLfloat[ushort0 * 3];
					for(i = 0; i < ushort0; i++)
					{
						j = i * 3;
						file.read(reinterpret_cast<char *>(&float0), FLOAT_SIZE);
						geom->vertexData[j] = float0;
						file.read(reinterpret_cast<char *>(&float0), FLOAT_SIZE);
						geom->vertexData[j + 1] = float0;
						file.read(reinterpret_cast<char *>(&float0), FLOAT_SIZE);
						geom->vertexData[j + 2] = float0;
					}
					break;
				case UVS:
					file.read(reinterpret_cast<char *>(&ushort0), USHORT_SIZE);
					geom->uvData = new GLfloat[ushort0 * 2];
					for(i = 0; i < ushort0; i++)
					{
						j = i * 2;
						file.read(reinterpret_cast<char *>(&float0), FLOAT_SIZE);
						geom->uvData[j] = float0;
						file.read(reinterpret_cast<char *>(&float0), FLOAT_SIZE);
						geom->uvData[j + 1] = 1 - float0;
					}
					break;
				case INDICES:
					file.read(reinterpret_cast<char *>(&ushort0), USHORT_SIZE);
					geom->numTriangles = ushort0;
					geom->indexData = new GLuint[ushort0 * 3];
					for(i = 0; i < ushort0; i++)
					{
						j = i * 3;
						file.read(reinterpret_cast<char *>(&ushort1), USHORT_SIZE);
						geom->indexData[j] = ushort1;
						file.read(reinterpret_cast<char *>(&ushort1), USHORT_SIZE);
						geom->indexData[j + 1] = ushort1;
						file.read(reinterpret_cast<char *>(&ushort1), USHORT_SIZE);
						geom->indexData[j + 2] = ushort1;
						file.seekg(2, file.cur);
					}
					geom = NULL;
					break;
				default:
					file.seekg(length - 6, file.cur);
			}
		}

		file.close();
	}
}