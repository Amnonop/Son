#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add(int first_operand, int second_operand)
{
    return first_operand + second_operand;
}

int multiply(int first_operand, int second_operand)
{
    return first_operand * second_operand;
}

int calculate(int first_operand, int second_operand, char operator)
{
    if (operator == '+')
    {
        return add(first_operand, second_operand);
    }
    else if (operator == '*')
    {
        return multiply(first_operand, second_operand);
    }
}

int parse_number(char string_to_parse[])
{
    int number;
    number = (int)strtol(string_to_parse, (char **)NULL, 10);

    return number;
}

int get_operator_index(char expression[])
{
    int i = 0;

    while ((expression[i] != '+') && (expression[i] != '*'))
    {
        i++;
    }

    return i;
}

int evaluate_simple_expression(char expression[])
{
    char delimiter[] = "+*";
    int first_operand, second_operand = 0;
    char operator;
	char* next_token = NULL;

    operator = expression[get_operator_index(expression)];

    char *first_operand_string;
    first_operand_string = strtok_s(expression, delimiter, &next_token);
    first_operand = parse_number(first_operand_string);

    char *second_operand_string;
    second_operand_string = strtok_s(NULL, delimiter, &next_token);
    second_operand = parse_number(second_operand_string);

    return calculate(first_operand, second_operand, operator);
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Not enough arguments.");
		return -1;
	}

	char* expression;
    int result;
	expression = argv[1];
    result = evaluate_simple_expression(expression);

    printf("%d\n", result);

    return result;
}