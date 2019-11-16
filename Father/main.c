
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define TIMEOUT_IN_MILLISECONDS 5000
#define BRUTAL_TERMINATION_CODE 0x55

BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);
int CreateProcessSimpleMain(void);

int main(int argc, char** argv)
{
	int braket_stack[256];
	char expression_first_part[256];
	char expression_last_part[256];
	char result_string[256];
	char sub_solution[256];
	char* expression = argv[1];
	
	int i = 0;
	int result = 0;
	int stack_index = -1;
	int expression_start = 0;
	int expression_end = 0; 
	char* child_expression = NULL;
	int child_expression_size=0;
	//opening file and earasing previous content if needed
	openFile("result_file.txt");

	while (argv[i] != '\0')
	{
		if (argv[i] == '(')
		{
			stack_index++;
			braket_stack[stack_index] = i;
			i++;
		}
		else if (argv[i] == ')')
		{
			expression_start = braket_stack[stack_index];
			stack_index--;
			expression_end = i;
			child_expression_size = expression_end - expression_start;
			child_expression = (char*)malloc(child_expression_size * sizeof(char));
			strncpy_s(child_expression,child_expression_size, 
				expression[expression_start+1], child_expression_size - 1);
			
			//result = CreateProcessSimpleMain(child_expression); - the function doesn't return int
			result = 15; //check

			printf("%d\n", result);

			//create string with sub expression
			strncpy(expression_first_part, expression, expression_start);
			strcat(sub_solution, expression_first_part);
			sprintf(result_string, "%d", result);
			strcat(sub_solution, result_string);
			strncpy(expression_last_part, expression + expression_end, strlen(expression) - expression_end - 1);
			strcat(sub_solution, expression_last_part);
			strcat(sub_solution, "\n");
			//append to file
			appendToFile("result_file.txt", sub_solution);
		}

	}

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
