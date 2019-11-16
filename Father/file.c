
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <limits.h>
# include <stdio.h> 
# include <string.h> 
#include "file.h"

void openFile(const char* file_path)
{
	FILE *fp;
	char empty_string = '\0';
	fp = fopen(file_path, "w");
	if (fp == NULL)
		return;
	fclose(fp);

}

void appendToFile(const char* file_path, char* sub_solution)
{
	FILE *fp;
	char empty_string = '\0';
	fp = fopen(file_path, "a");
	if (fp == NULL)
		return;
	fprintf(fp, "%s", sub_solution);
	fclose(fp);
}