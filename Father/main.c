#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "stack.h"

#define TIMEOUT_IN_MILLISECONDS 5000
#define BRUTAL_TERMINATION_CODE 0x55

BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);
int CreateProcessSimpleMain(char* expression);
void solveExpression(char* expression);
int handleSimpleExpression(char* expression, node_t** values_stack, node_t** operator_index_stack);

int main(int argc, char** argv)
{
	char* expression = argv[1];
	
	solveExpression(expression);
	
	return 0;
}

int getNumber(char* expression, int start_index, int end_index)
{
	int number_string_length = end_index - start_index + 1;
	char* number_string = (char*)malloc(number_string_length * sizeof(char));
	strncpy_s(number_string, number_string_length,
		expression + start_index, number_string_length - 1);
	int value = (int)strtol(number_string, (char **)NULL, 10);
	free(number_string);

	return value;
}

void solveExpression(char* expression)
{
	node_t* bracket_index_stack = NULL;

	node_t* values_stack = NULL;
	node_t* operator_index_stack = NULL;

	int i = 0;
	int expression_length = strlen(expression);

	int number_start_index = 0;
	int value = 0;

	char operator = ' ';
	int first_operand = 0;
	int second_operand = 0;

	int child_expression_result = 0;

	while (expression[i] != '\0')
	{
		if (expression[i] >= '0' && expression[i] <= '9')
		{
			number_start_index = i;
			while (i < expression_length && expression[i] >= '0' && expression[i] <= '9')
				i++;
			
			value = getNumber(expression, number_start_index, i);

			push(&values_stack, value);
		}
		else if (expression[i] == '(' || expression[i] == '+' || expression[i] == '*')
		{
			push(&operator_index_stack, i);
			i++;
		}
		else if (expression[i] == ')')
		{
			// Closing brace encountered, solve entire brace
			while (expression[peek(operator_index_stack)] != '(')
			{
				// Solve simple expression
				child_expression_result = handleSimpleExpression(expression, &values_stack, &operator_index_stack);

				push(&values_stack, child_expression_result);
			}
			pop(&operator_index_stack);

			// Print solved expression

			i++;
		}
	}

	// Top of values_stack contains result
	printf("%d", pop(&values_stack));
}

int handleSimpleExpression(char* expression, node_t** values_stack, node_t** operator_index_stack)
{
	char operator = expression[pop(operator_index_stack)];
	int first_operand = pop(values_stack);
	int second_operand = pop(values_stack);

	char operand_string[32];
	sprintf_s(operand_string, 32 ,"%d%c%d", first_operand, operator, second_operand);

	return CreateProcessSimpleMain(operand_string);
}

int CreateProcessSimpleMain(char* expression)
{
	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				exitcode;
	BOOL				retVal;
	CHAR				process_name[] = ("Son.exe ");
	int command_length = strlen(expression) + strlen(process_name) + 2;
	char* command = (char*)malloc(sizeof(char)*command_length);

	sprintf_s(command, command_length, "Son.exe %s", expression);
	
	//strcpy_s(command, command_length, process_name);
	//strcat_s(command, command_length, expression);
	retVal = CreateProcessSimple(command, &procinfo);

	if (retVal == 0)
	{
		printf("Process Creation Failed!\n");
		return;
	}


	waitcode = WaitForSingleObject(
		procinfo.hProcess,
		TIMEOUT_IN_MILLISECONDS); /* Waiting 5 secs for the process to end */

	printf("WaitForSingleObject output: ");
	switch (waitcode)
	{
	case WAIT_TIMEOUT:
		printf("WAIT_TIMEOUT\n"); break;
	case WAIT_OBJECT_0:
		printf("WAIT_OBJECT_0\n"); break;
	default:
		printf("0x%x\n", waitcode);
	}

	if (waitcode == WAIT_TIMEOUT) /* Process is still alive */
	{
		printf("Process was not terminated before timeout!\n"
			"Terminating brutally!\n");
		TerminateProcess(
			procinfo.hProcess,
			BRUTAL_TERMINATION_CODE); /* Terminating process with an exit code of 55h */
		Sleep(10); /* Waiting a few milliseconds for the process to terminate */
	}

	GetExitCodeProcess(procinfo.hProcess, &exitcode);

	printf("The exit code for the process is 0x%x\n", exitcode);


	CloseHandle(procinfo.hProcess); /* Closing the handle to the process */
	CloseHandle(procinfo.hThread); /* Closing the handle to the main thread of the process */

	return exitcode;
}

BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr)
{
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 }; /* <ISP> here we */
															  /* initialize a "Neutral" STARTUPINFO variable. Supplying this to */
															  /* CreateProcess() means we have no special interest in this parameter. */
															  /* This is equivalent to what we are doing by supplying NULL to most other */
															  /* parameters of CreateProcess(). */

	return CreateProcess(NULL, /*  No module name (use command line). */
		CommandLine,			/*  Command line. */
		NULL,					/*  Process handle not inheritable. */
		NULL,					/*  Thread handle not inheritable. */
		FALSE,					/*  Set handle inheritance to FALSE. */
		NORMAL_PRIORITY_CLASS,	/*  creation/priority flags. */
		NULL,					/*  Use parent's environment block. */
		NULL,					/*  Use parent's starting directory. */
		&startinfo,				/*  Pointer to STARTUPINFO structure. */
		ProcessInfoPtr			/*  Pointer to PROCESS_INFORMATION structure. */
	);
}
