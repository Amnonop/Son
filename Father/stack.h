#ifndef STACK_H
#define STACK_H

typedef struct node
{
	int data;
	struct node *next;
} node_t;

int is_empty(node_t* stack);
void push(node_t** stack, int data);
int pop(node_t** stack);
int peek(node_t* stack);

#endif // !STACK_H

