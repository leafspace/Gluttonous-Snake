#pragma once
#include <stdio.h>
#define MAXSIZE 1000
typedef  int ElemType;

typedef struct LNode
{
	ElemType data;
	struct LNode *next;
}LNode, *LinkList;

typedef struct Queue
{
	ElemType data[MAXSIZE];
	int front;
}Queue;