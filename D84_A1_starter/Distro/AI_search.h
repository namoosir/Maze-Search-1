/*
	CSC D84 - Unit 1 - Search

	This file contains the API function headers for your assignment.
	Please pay close attention to the function prototypes, and
	understand what the arguments are.

	Stubs for implementing each function are to be found in AI_search.c,
	along with clear ** TO DO markers to let you know where to add code.

	You are free to add helper functions within reason. But you must
	provide a prototype *in this file* as well as the implementation
	in the .c program file.

	Starter by: F.J.E., Jul. 2015
	Updated by: F.J.E., Jan. 2018
*/

#ifndef __AI_search_header

#define __AI_search_header

// Generally needed includes
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>

#include "board_layout.h"

// Function prototypes for D84 - Unit 1 - Search assignment solution

void search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]));
int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]);
int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]);

// If you need to add any function prototypes yourself, you can do so *below* this line.

#define INF graph_size+1
typedef struct queue {
	int items[graph_size];
	int head;
	int tail;
} queue;

queue* initQueue();
void enqueue(queue* q, int x);
void sorted_enqueue(queue* q, int x);
int dequeue(queue* q);
int emptyQueue(queue* q);
void printQueue(queue* q);
void freeQueue(queue* q);

typedef struct stack {
	int items[graph_size];
	int top;
} stack;

stack* initStack();
void push(stack* s, int x);
int pop(stack* s);
int emptyStack(stack* s);
void freeStack(stack* s);

int in(int arr[size_X][size_Y], int search);
void put(int arr[size_X][size_Y], int val, int order);
int isCat(int cat_loc[10][2], int cats, int check);

//This was an edit on code from https://www.programiz.com/dsa/priority-queue
int p_queue_size;
int in_queue(int array[][2], int num);
void deleteRoot(int array[][2], int num);
void insert(int array[][2], int newNum, int node_num);

//This function was an edit on code from https://www.programiz.com/dsa/floyd-warshall-algorithm
void shortest_paths(double gr[graph_size][4]);
int shortest_matrix[graph_size][graph_size];

void bfs(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]));
void a_star(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]));
void dfs(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]));
void calculate_path(int found_x, int found_y, int pred[graph_size], int path[graph_size][2]);
#endif