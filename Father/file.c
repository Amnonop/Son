#include <stdlib.h>
#include <limits.h>
# include <stdio.h> 
# include <string.h> 
#include "file.h"
#include <errno.h>

void openFile(const char* file_path)
{
	FILE *fp;
	errno_t error;
	error = fopen_s(&fp, file_path, "w");
	
	if (error != 0)
		printf("An error occured while openning file %s for writing.", file_path);

	if (fp)
	{
		fclose(fp);
	}
}

void appendToFile(char* file_path, char* sub_solution)
{
	FILE *fp;
	fopen_s(&fp, file_path, "a");
	
	if (!fp) 
		return;

	char* new_line;
	int new_line_length;
	new_line_length = strlen(sub_solution) + 2;
	new_line = (char*)malloc(sizeof(char) * new_line_length);
	strcpy_s(new_line, new_line_length, sub_solution);
	strcat_s(new_line, new_line_length, "\n");

	fprintf(fp, "%s", new_line);
	fclose(fp);

	free(new_line);
}