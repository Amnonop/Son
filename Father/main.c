#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "stack.h"
#include "expressionEvaluation.h"

int main(int argc, char** argv)
{
	char* expression = argv[1];
	
	solveExpression(expression);
	
	return 0;
}


