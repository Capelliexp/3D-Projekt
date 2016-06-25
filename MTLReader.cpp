#define _CRT_SECURE_NO_WARNINGS
#include "MTLReader.h"

void readMTL(MTLFormat albedo[], char* FilePath) {

	FILE * file = fopen(FilePath, "r");
	if (file == NULL) {
		std::cout << "file not found" << std::endl;
	}

	int Kd_looper = 0;
	int Ka_looper = 0;
	int Ks_looper = 0;
	int Ns_looper = 0;
	int illum_looper = 0;

	char lineHeader[128];

	while (666) {
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) {							//om filen tog slut, break
			break;
		}

		if (strcmp(lineHeader, "Kd") == 0) {
			fscanf(file, "%f %f %f\n", &albedo[Kd_looper].Kd_r, &albedo[Kd_looper].Kd_g, &albedo[Kd_looper].Kd_b);
			Kd_looper++;
		}

		else if (strcmp(lineHeader, "Ka") == 0) {
			fscanf(file, "%f %f %f\n", &albedo[Ka_looper].Ka_r, &albedo[Ka_looper].Ka_g, &albedo[Ka_looper].Ka_b);
			Ka_looper++;
		}

		else if (strcmp(lineHeader, "Ks") == 0) {
			fscanf(file, "%f %f %f\n", &albedo[Ks_looper].Ks_r, &albedo[Ks_looper].Ks_g, &albedo[Ks_looper].Ks_b);
			Ks_looper++;
		}

		else if (strcmp(lineHeader, "Ns") == 0) {
			fscanf(file, "%f\n", &albedo[Ns_looper].Ns);
			Ns_looper++;
		}

		else if (strcmp(lineHeader, "illum") == 0) {
			fscanf(file, "%i\n", &albedo[illum_looper].illum);
			illum_looper++;
		}
		
	}
}