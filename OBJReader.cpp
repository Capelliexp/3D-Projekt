#define _CRT_SECURE_NO_WARNINGS
#include "OBJReader.h"

//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
//http://www.cplusplus.com/reference/cstdio/fopen/
//http://www.cplusplus.com/forum/beginner/43095/ se botten
//kolla hur man allokerar array:en till heapen...
	//C 9/12 - 14
	//https://en.wikipedia.org/wiki/C_dynamic_memory_allocation
	//int* array = new int[1000000];
	//GLÖM INTE delete[] !!!!
	//A better solution would be to use std::vector<int> and resize it to 1000000 elements...

void readOBJ(OBJFormat faces[], char* FilePath, float scale, float MoveX, float MoveY, float MoveZ){
		
	FILE * file = fopen(FilePath, "r");
	if (file == NULL) {
		std::cout << "file not found" << std::endl;
	}

	vertex* v = new vertex[1000000];
	uv* vt = new uv[1000000];
	normal* vn = new normal[1000000];

	int f_count[9];

	unsigned long long int loopCounter_v = 1;
	unsigned long long int loopCounter_vt = 1;
	unsigned long long int loopCounter_vn = 1;
	unsigned long long int loopCounter_f = 0;

	char lineHeader[128];

	while (666){
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) {							//om filen tog slut, radera arrayerna och sedan break
			delete[] v;
			delete[] vt;
			delete[] vn;
			break;
		}

		if (strcmp(lineHeader, "v") == 0) {			//om vi läser in en vektor
			fscanf(file, "%f %f %f\n", &v[loopCounter_v].x, &v[loopCounter_v].y, &v[loopCounter_v].z);
			v[loopCounter_v].x = v[loopCounter_v].x * scale;
			v[loopCounter_v].y = v[loopCounter_v].y * scale;
			v[loopCounter_v].z = v[loopCounter_v].z * scale;
			loopCounter_v++;
		}

		else if (strcmp(lineHeader, "vt") == 0) {	//om vi läser in en texturkoordinat
			fscanf(file, "%f %f\n", &vt[loopCounter_vt].u, &vt[loopCounter_vt].v);
			loopCounter_vt++;
		}

		else if (strcmp(lineHeader, "vn") == 0) {	//om vi läser in en normal
			fscanf(file, "%f %f %f\n", &vn[loopCounter_vn].x, &vn[loopCounter_vn].y, &vn[loopCounter_vn].z);
			loopCounter_vn++;
		}

		else if (strcmp(lineHeader, "f") == 0) {			//om vi läser in en face
			fscanf(file, "%i/%i/%i %i/%i/%i %i/%i/%i\n",	//ska det vara d istället för i???
				&f_count[0], &f_count[1], &f_count[2], &f_count[3], &f_count[4], &f_count[5], &f_count[6], &f_count[7], &f_count[8]);

			for (short i = 0; i < 3; i++){
				faces[loopCounter_f + (2 - i)].x = v[f_count[3 * i]].x + MoveX;
				faces[loopCounter_f + (2 - i)].y = v[f_count[3 * i]].y + MoveY;
				faces[loopCounter_f + (2 - i)].z = -v[f_count[3 * i]].z + MoveZ;	//negativ

				faces[loopCounter_f + (2 - i)].u = vt[f_count[(3 * i) + 1]].u;
				faces[loopCounter_f + (2 - i)].v = vt[f_count[(3 * i) + 1]].v;

				faces[loopCounter_f + (2 - i)].xn = vn[f_count[(3 * i) + 2]].x;
				faces[loopCounter_f + (2 - i)].yn = vn[f_count[(3 * i) + 2]].y;
				faces[loopCounter_f + (2 - i)].zn = -vn[f_count[(3 * i) + 2]].z;	//negativ
			}
			loopCounter_f = loopCounter_f + 3;
		}
	}
}