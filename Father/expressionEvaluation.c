#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "stack.h"
#include "expressionEvaluation.h"
#include "file.h"

#define TIMEOUT_IN_MILLISECONDS 5000
#define BRUTAL_TERMINATION_CODE 0x55

BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);
int CreateProcessSimpleMain(char* expression);
void solveExpression(char* expression);
int handleSimpleExpression(char* expression, node_t** values_stack, node_t** operator_index_stack);
char* buildLogString(char* expression, int expression_start, int result, int expression_end);
int isExpressionSolved(char* expression);
char* solveStep(char* expression);

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

void swap(char** source, char** target)
{
	char* temp = *source;
	*target = *source;
}

void solveExpression(char* expression)
{
	FILE *fp;
	char* solved_step;
	char* solution_step = (char*)malloc(sizeof(char) * (strlen(expression) + 1));
	strcpy_s(solution_step, strlen(solution_step), expression);
	
	openFile("result_file.txt", solution_step);
	
	while (!isExpressionSolved(solution_step))
	{
		solved_step = solveStep(solution_step);
		swap(&solved_step, &solution_step);

		fopen_s(&fp, "result_file.txt", "a");
		if (!fp) {
			return 1;
		}
		fprintf(fp, "%s", solution_step);
		fclose(fp);
		//appendToFile("result_file.txt", solution_step);
	}

}

int isExpressionSolved(char* expression)
{
	return expression[0] != '(';
}

char* solveStep(char* expression)
{
	char* solution_step_string;
	int open_brace_index = 0;
	char* simple_expression;
	int simple_expression_length = 0;
	int result = 0;
	int i = 0;
	while (expression[i] != ')')
	{
		if (expression[i] == '(')
			open_brace_index = i;
		i++;
	}
	simple_expression_length = i - open_brace_index;
	simple_expression = (char*)malloc(sizeof(char) * simple_expression_length);
	strncpy_s(simple_expression, simple_expression_length, expression + open_brace_index + 1, simple_expression_length - 1);
	result = CreateProcessSimpleMain(simple_expression);
	free(simple_expression);

	solution_step_string = buildLogString(expression, open_brace_index, result, i + 1);
	return solution_step_string;
}

int handleSimpleExpression(char* expression, node_t** values_stack, node_t** operator_index_stack)
{
	char operator = expression[pop(operator_index_stack)];
	int first_operand = pop(values_stack);
	int second_operand = pop(values_stack);

	char operand_string[32];
	sprintf_s(operand_string, 32, "%d%c%d", first_operand, operator, second_operand);

	return CreateProcessSimpleMain(operand_string);
}

char* buildLogString(char* expression, int expression_start, int result, int expression_end)
{
	char* destination;
	char expression_first_part[256];
	char expression_last_part[256];
	char result_string[256];

	sprintf_s(result_string, 256, "%d", result);
	
	int length = expression_start + strlen(result_string) + (strlen(expression) - expression_end) + 2;
	destination = (char*)malloc(sizeof(char) * length);

	//strncpy_s(expression_first_part, 256, expression, expression_start);
	strncpy_s(destination, length, expression, expression_start);
	
	strcat_s(destination, length, result_string);

	strncpy_s(expression_last_part, 256, expression + expression_end, strlen(expression) - expression_end);
	strcat_s(destination, length, expression_last_part);
	//strcat_s(destination, length, "\n");

	return destination;
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