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

int main(int argc, char** argv)
{
	char* expression = argv[1];
	
	solveExpression(expression);
	
	return 0;
}

void solveExpression(char* expression)
{
	node_t* bracket_index_stack = NULL;

	node_t* values_stack = NULL;
	node_t* operator_index_stack = NULL;

	int i = 0;
	int expression_length = strlen(expression);

	int number_start_index = 0;
	int number_length = 0;
	char* number_string = NULL;
	int value = 0;

	char operator = ' ';
	int first_operand = 0;
	int second_operand = 0;

	int child_expression_result = 0;

	int expression_start = 0;
	int expression_end = 0;
	char* child_expression = NULL;
	int child_expression_size = 0;

	while (expression[i] != '\0')
	{
		if (expression[i] >= '0' && expression[i] <= '9')
		{
			number_start_index = i;
			while (i < expression_length && expression[i] >= '0' && expression[i] <= '9')
			{
				i++;
			}
			number_length = i - number_start_index;
			number_string = (char*)malloc(number_length * sizeof(char));
			strncpy_s(number_string, number_length, 
				expression + number_start_index, number_length - 1);
			value = (int)strtol(number_string, (char **)NULL, 10);
			free(number_string);
			push(&values_stack, value);
		}
		else if (expression[i] == '+' || expression[i] == '*')
		{
			push(&operator_index_stack, i);
			i++;
		}
		else if (expression[i] == '(')
		{
			push(&operator_index_stack, i);
			i++;
		}
		else if (expression[i] == ')')
		{
			// Closing brace encountered, solve entire brace
			while (expression[peek(operator_index_stack)] != '(')
			{
				operator = expression[pop(operator_index_stack)];
				first_operand = pop(values_stack);
				second_operand = pop(values_stack);
				// Solve simple expression

				push(values_stack, child_expression_result);
			}
		}
	}
}

char* solveSimpleExpression(char* expression)
{

}

int CreateProcessSimpleMain(char* expression)
{
	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				exitcode;
	BOOL				retVal;
	CHAR				process_name[] = ("Son.exe ");
	int command_length = strlen(expression) + strlen(process_name) + 1;
	char*				command = (char*)malloc(sizeof(char)*command_length);
	strcpy_s(command, command_length, process_name);
	strcat_s(command, command_length, expression);
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
