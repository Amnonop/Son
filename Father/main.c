#include <stdio.h>
#include <stdlib.h>
#include "expressionEvaluation.h"

int main(int argc, char** argv)
{
	char* expression = argv[1];
	
	solveExpression(expression);

	return 0;
}
