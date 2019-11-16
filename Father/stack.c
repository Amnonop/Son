#include <stdlib.h>
#include <limits.h>
#include "stack.h"


node_t* create_node(int data)
{
	node_t* new_node = (node_t*)malloc(sizeof(node_t));
	if (new_node != NULL)
	{
		new_node->data = data;
		new_node->next = NULL;
	}
	return new_node;
}

int is_empty(node_t* stack)
{
	return (stack == NULL);
}

void push(node_t** stack, int data)
{
	node_t* current_head;

	node_t* new_node = create_node(data);

	if (is_empty(stack))
		stack = new_node;

	new_node->next = *stack;
	*stack = new_node;
}

int pop(node_t** stack)
{
	if (is_empty(stack))
		return INT_MIN;

	node_t* temp = stack;
	*stack = (*stack)->next;
	int popped = temp->data;
	free(temp);

	return popped;
}

int peek(node_t* stack)
{
	if (is_empty(stack))
		return INT_MIN;

	return stack->data;
}