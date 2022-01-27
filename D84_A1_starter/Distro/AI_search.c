/*
	CSC D84 - Unit 1 - Search

	This file contains stubs for implementing the different search
	algorithms covered in the course. Please read the assignment
	handout carefully - it describes the game, the data you will
	have to handle, and the search functions you must provide.

	Once you have read the handout carefully, implement your search
	code in the sections below marked with

	**************
	*** TO DO:
	**************

	Make sure to add it to your report.txt file - it will be marked!

	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E., Jul. 15
	Updated: F.J.E., Jan. 18
*/

/**********************************************************************
% COMPLETE THIS TEXT BOX:
%
% 1) Student Name: Nazmus Saqeeb		
% 2) Student Name: Mutasem Kharsa
%
% 1) Student number: 1006206007
% 2) Student number:
% 
% 1) UtorID: saqeebna
% 2) UtorID
% 
% We hereby certify that the work contained here is our own
%
%  Nazmus Saqeeb					
% (sign with your name)            (sign with your name)
***********************************************************************/

#include "AI_search.h"

void search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]))
{
 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in AI_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:
	
	The game takes place on a grid of size 32x32, the file board_layout.h specifies the size and defines two
	constants 'size_X' and 'size_Y' for the horizontal and vertical size of the board, respectively. For our
	purposes, the grid of locations is represented by a graph with one node per grid location, so given
	the 32x32 cells, the graph has 1024 nodes. 

	To create a maze, we connect cell locations in the grid in such a way that a) there is a path from any
	grid location to any other grid location (i.e. there are no disconnected subsets of nodes in the graph),
	and b) there are loops.

	Since each node represents a grid location, each node can be connected to up to 4 neighbours in the
	top, right, bottom, and left directions respectively:

			node at (i,j-1)
				^
				|
	(node at i-1, j) <- node at (i,j) -> node at (i+1, j)
				|
				v
			node at (i,j+1)

	The graph is theredore stored as an adjacency list with size 1024 x 4, with one row per node in the
	graph, and 4 columns corresponding to the weight of an edge linking the node with each of its 4
	possible neighbours in the order towp, right, bottom, left (clockwise from top).

	Since all we care is whether nodes are connected. Weights will be either 0 or 1, if the weight is
	1, then the neighbouring nodes are connected, if the weight is 0, they are not. For example, if

	graph[i][0] = 0 
	graph[i][1] = 1
	graph[i][2] = 0
	graph[i][3] = 1

	then node i is connected to the right and left neighbours, but not to top or bottom.

	The index in the graph for the node corresponding to grid location (x,y) is

	index = x + (y*size_X) 		or in this case		index = x + (y*32)

	Conversely, if you have the index and want to figure out the grid location,

	x = index % size_X		or in this case		x = index % 32
	y = index / size_Y		or in this case		y = index / 32

	(all of the above are *integer* operations!)

	A path is a sequence of (x,y) grid locations. We store it using an array of size	
	1024 x 2 (since there are 1024 locations, this is the maximum length of any
	path that visits locations only once).

	Agent locations are coordinate pairs (x,y)	

   Arguments:
		gr[graph_size][4]   - This is an adjacency list for the maze
		path[graph_size][2] - An initially empty path for your code to fill.
				      In this case, empty means all entries are initially -1
		visit_order[size_X][size_Y]	- An array in which your code will store the
						  *order* in which grid locations were 
						  visited during search. For example, while
						  doing BFS, the initial location is the
						  start location, it's visit order is 1.
						  Then the search would expand the immediate
						  neighbours of the initial node in some order,
						  these would get a visit order of 2, 3, 4, and
						  5 respectively, and so on.
					
						  This array will be used to display the search
						  pattern generated by each search method.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- BFS
					mode = 1	- DFS
					mode = 2	- A*

		(*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[10][2], int cats, int cheeses)
				- This is a pointer to one of the heuristic functions you will implement, either H_cost()
				  or H_cost_nokitty(). The driver in AI_search_core_GL will pass the appropriate pointer
				  depending on what search the user wants to run.
			
				  If the mode is 0 or 1, this pointer is NULL

				  * How to call the heuristic function from within this function : *
					- Like any other function:
						h = heuristic( x, y, cat_loc, cheese_loc, mouse_loc, cats, cheeses);

   Return values:
		Your search code will directly update data passed-in as arguments:

		- path[graph_size][2]	: Your search code will update this array to contain the path from
					  the mouse to one of the cheese chunks. The order matters, so
					  path[0][:] must be the mouse's current location, path[1][:]
					  is the next move for the mouse. Each successive row will contain
					  the next move toward the cheese, and the path ends at a location
					  whose coordinates correspond to one of the cheese chunks.
					  Any entries beyond that must remain set to -1
		- visit_order[size_X][size_Y] 	:  Your search code will update this array to contain the
						   order in which each location in the grid was expanded
						   during search. This means, when that particular location
						   was checked for being a goal, and if not a goal, had its
						   neighbours added as candidates for future expansion (in
						   whatever order is dictated by the search mode).

						   Note that since there are 1024 locations, the values in 
						   this array must always be in [0, 1023]. The driver code
						   will then display search order as a yellow-colored
						   brightness map where nodes expanded earlier will look
						   brighter.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *
			
		That's that, now, implement your solution!
 */

 /********************************************************************************************************
 * 
 * TO DO:	Implement code to carry out the different types of search depending on the
 *		mode.
 *
 *		You can do this by writing code within this single function (and being clever about it!)
 *		Or, you can use this function as a wrapper that calls a different search function
 *		 (BFS, DFS, A*) depending on the mode. Note that in the latter case, you will have
 *	    	  to inform your A* function somehow of what heuristic it's supposed to use.
 *
 *		Visiting Order: When adding the neighbours of a node to your list of candidates for
 *				expansion, do so in the order top, right, bottom, left.
 *
 *		NOTE: Your search functions should be smart enough to not choose a path that goes
 *		      through a cat! this is easily done without any heuristics.
 * 
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/

 // Stub so that the code compiles/runs - The code below will be removed and replaced by your code!
 //BFS
 if (mode == 0) {
	queue* q = initQueue();
	int start = mouse_loc[0][0] + mouse_loc[0][1]*size_X;
	int num_visited = 1;
	visit_order[mouse_loc[0][0]][mouse_loc[0][1]] = num_visited;
	int pred[graph_size];
	int found_x = -1, found_y = -1;

	for (int i = 0; i < graph_size; i++) {
		pred[i] = -1;
	}

	enqueue(q, start);

	while(!emptyQueue(q)) {
		// printf("here\n");

		int cur = dequeue(q);

		//printQueue(q);

		int cur_x = cur % size_X;
		int cur_y = cur/size_Y;

		for (int i = 0; i < cheeses; i++) {
			if (cur_x == cheese_loc[i][0] && cur_y == cheese_loc[i][1]) {
				found_x = cur_x;
				found_y = cur_y;
				break;
			}
		}

		if (found_x != -1 && found_y != -1) {
			break;
		}

		if (gr[cur][0] != 0 && !in(visit_order, cur-size_X) && !isCat(cat_loc, cats, cur-size_X)) {
			enqueue(q, cur-size_X);
			num_visited++;
			put(visit_order, cur-size_X, num_visited);
			pred[cur-size_X] = cur;
		}
		if (gr[cur][1] != 0 && !in(visit_order, cur+1) && !isCat(cat_loc, cats, cur+1)) {
			enqueue(q, cur+1);
			num_visited++;
			put(visit_order, cur+1, num_visited);
			pred[cur+1] = cur;
		}
		if (gr[cur][2] != 0 && !in(visit_order, cur+size_X) && !isCat(cat_loc, cats, cur+size_X)) {
			enqueue(q, cur+size_X);
			num_visited++;
			put(visit_order, cur+size_X, num_visited);
			pred[cur+size_X] = cur;
		}
		if (gr[cur][3] != 0 && !in(visit_order, cur-1) && !isCat(cat_loc, cats, cur-1)) {
			enqueue(q, cur-1);
			num_visited++;
			put(visit_order, cur-1, num_visited);
			pred[cur-1] = cur;
		}
	}
	
	freeQueue(q);

	int reversed_path[graph_size][2];
	int counter = 0;
	int temp_x = found_x;
	int temp_y = found_y;
	int temp_mid; //BETTE NAME?

	//THIS IS TEMP FOR PATH NOT FOUND
	if(temp_x == -1 || temp_y == -1){
		return;
	}

	while (pred[temp_x + temp_y*size_X] != -1) {
		reversed_path[counter][0] = temp_x;
		reversed_path[counter][1] = temp_y;
		temp_mid = pred[temp_x + temp_y*size_X] % size_X;
		temp_y = pred[temp_x + temp_y*size_X] / size_Y;
		temp_x = temp_mid;
		counter++;
	}

	int total_nodes = counter;
	counter--;

	for (int i = 0; i < total_nodes; i++) {
		path[i][0] = reversed_path[counter][0];
		path[i][1] = reversed_path[counter][1];
		counter--;
	}
	path[total_nodes][0] = path[total_nodes-1][0];
	path[total_nodes][1] = path[total_nodes-1][1];
	// printf("cheese shoukd be at %d %d \n", path[total_nodes-1][0], path[total_nodes-1][1]);
 //DFS
 } else if (mode == 1) {
	stack* s = initStack();
	int start = mouse_loc[0][0] + mouse_loc[0][1]*size_X;
	int num_visited = 1;
	visit_order[mouse_loc[0][0]][mouse_loc[0][1]] = num_visited;
	int pred[graph_size];
	int found_x = -1, found_y = -1;

	for (int i = 0; i < graph_size; i++) {
		pred[i] = -1;
	}

	push(s, start);
	
	while(!emptyStack(s)) {
		int cur = pop(s);

		int cur_x = cur % size_X;
		int cur_y = cur/size_Y;

		for (int i = 0; i < cheeses; i++) {
			if (cur_x == cheese_loc[i][0] && cur_y == cheese_loc[i][1]) {
				found_x = cur_x;
				found_y = cur_y;
				break;
			}
		}

		if (found_x != -1 && found_y != -1) {
			break;
		}

		if (gr[cur][0] != 0 && !in(visit_order, cur-size_X) && !isCat(cat_loc, cats, cur-size_X)) {
			push(s, cur-size_X);
			num_visited++;
			put(visit_order, cur-size_X, num_visited);
			pred[cur-size_X] = cur;
		}
		if (gr[cur][1] != 0 && !in(visit_order, cur+1) && !isCat(cat_loc, cats, cur+1)) {
			push(s, cur+1);
			num_visited++;
			put(visit_order, cur+1, num_visited);
			pred[cur+1] = cur;
		}
		if (gr[cur][2] != 0 && !in(visit_order, cur+size_X) && !isCat(cat_loc, cats, cur+size_X)) {
			push(s, cur+size_X);
			num_visited++;
			put(visit_order, cur+size_X, num_visited);
			pred[cur+size_X] = cur;
		}
		if (gr[cur][3] != 0 && !in(visit_order, cur-1) && !isCat(cat_loc, cats, cur-1)) {
			push(s, cur-1);
			num_visited++;
			put(visit_order, cur-1, num_visited);
			pred[cur-1] = cur;
		}
	}
	freeStack(s);

	int reversed_path[graph_size][2];
	int counter = 0;
	int temp_x = found_x;
	int temp_y = found_y;
	int temp_mid; //BETTE NAME?

	//THIS IS TEMP
	if(temp_x == -1 || temp_y == -1){
		return;
	}

	while (pred[temp_x + temp_y*size_X] != -1) {
		reversed_path[counter][0] = temp_x;
		reversed_path[counter][1] = temp_y;
		temp_mid = pred[temp_x + temp_y*size_X] % size_X;
		temp_y = pred[temp_x + temp_y*size_X] / size_Y;
		temp_x = temp_mid;
		counter++;
	}

	int total_nodes = counter;
	counter--;

	for (int i = 0; i < total_nodes; i++) {
		path[i][0] = reversed_path[counter][0];
		path[i][1] = reversed_path[counter][1];
		counter--;
	}
	path[total_nodes][0] = path[total_nodes-1][0];
	path[total_nodes][1] = path[total_nodes-1][1];

 //A*
 } else if (mode == 2) {
	int cost_array[graph_size];
	int p_queue[graph_size][2];
	int start = mouse_loc[0][0] + mouse_loc[0][1]*size_X;
	int pred[graph_size];
	int found_x = -1, found_y = -1;
	int num_visited = 0;
	for (int i = 0; i < graph_size; i++) {
		pred[i] = -1;
		cost_array[i] = -1;
	}
	cost_array[start] = 0;

	insert(p_queue, 0, start);

	int cur;
	int cur_x;
	int cur_y;
	int cur_cost;

	while(p_queue_size > 0){

		cur = p_queue[0][1];
		cur_cost = p_queue[0][0];
		cur_x = cur % size_X;
		cur_y = cur/size_Y;
		deleteRoot(p_queue, cur);

		for (int i = 0; i < cheeses; i++){
			if (cur_x == cheese_loc[i][0] && cur_y == cheese_loc[i][1]){
				found_x = cur_x;
				found_y = cur_y;
				break;
			}
		}

		if (found_x != -1 && found_y != -1){
			break;
		}
		
		int difference;
		int real_cost;
		int f_cost;
		int new_node;
		for (int i = 0; i<4; i++){
			if(i==0) difference = -size_X;
			if(i==1) difference = 1;
			if(i==2) difference = size_X;
			if(i==3) difference = -1;
			new_node = cur+difference;

			if (gr[cur][i] != 0 && !in(visit_order, new_node) && !isCat(cat_loc, cats, new_node)) {
				real_cost = cur_cost + gr[cur][i];
				f_cost = real_cost + heuristic(new_node%size_X, new_node/size_Y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr);


				if(in_queue(p_queue,new_node)){
					if(real_cost < cost_array[new_node]){
						pred[new_node] = cur;
						cost_array[new_node] = real_cost;
						deleteRoot(p_queue, new_node);
						insert(p_queue, f_cost, new_node);
					}
				}
				else{
					pred[new_node] = cur;
					cost_array[new_node] = real_cost;
					insert(p_queue, f_cost, new_node);
				}
			}
		}
		num_visited++;
		put(visit_order, cur, num_visited);
		visit_order[mouse_loc[0][0]][mouse_loc[0][1]] = num_visited;
	}

	int reversed_path[graph_size][2];
	int counter = 0;
	int temp_x = found_x;
	int temp_y = found_y;
	int temp_mid; //BETTE NAME?

	//THIS IS TEMP FOR PATH NOT FOUND
	if(temp_x == -1 || temp_y == -1){
		return;
	}

	while (pred[temp_x + temp_y*size_X] != -1) {
		reversed_path[counter][0] = temp_x;
		reversed_path[counter][1] = temp_y;
		temp_mid = pred[temp_x + temp_y*size_X] % size_X;
		temp_y = pred[temp_x + temp_y*size_X] / size_Y;
		temp_x = temp_mid;
		counter++;
	}

	int total_nodes = counter;
	counter--;

	for (int i = 0; i < total_nodes; i++) {
		path[i][0] = reversed_path[counter][0];
		path[i][1] = reversed_path[counter][1];
		counter--;
	}
	path[total_nodes][0] = path[total_nodes-1][0];
	path[total_nodes][1] = path[total_nodes-1][1];
	printf("cheese shoukd be at %d %d \n", path[total_nodes-1][0], path[total_nodes-1][1]);
 }

 return;
}

int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
 /*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
	The goal is cheese. Which cheese is up to you.
	Whatever you code here, your heuristic must be admissible.

	Input arguments:

		x,y - Location for which this function will compute a heuristic search cost
		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		gr - The graph's adjacency list for the maze

		These arguments are as described in the search() function above
 */

 return(0);		// <-- Evidently you will need to update this.
}

int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
 /*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
	The goal is cheese. 

	However - this time you want your heuristic function to help the mouse avoid being eaten.
	Therefore: You have to somehow incorporate knowledge of the cats' locations into your
	heuristic cost estimate. How well you do this will determine how well your mouse behaves
	and how good it is at escaping kitties.

	This heuristic *does not have to* be admissible.

	Input arguments have the same meaning as in the H_cost() function above.
 */

 return(1);		// <-- Evidently you will need to update this.
}

queue* initQueue() {
	queue* q = (queue*)malloc(sizeof(queue));
	q->head=-1;
	q->tail=-1;
	return q;
}
void enqueue(queue* q, int x) {
	if (q->tail!=graph_size-1) {
		if (q->head==-1) {
			q->head = 0;
		}
		q->tail++;
		q->items[q->tail] = x;
	}
}
int dequeue(queue* q) {
	int item;
	if (!emptyQueue(q)) {
		item = q->items[q->head];
		q->head++;
		if (q->head > q->tail) {
			q->head = -1;
			q->tail = -1;
		}
	} else {
		return -1;
	}
	return item;
}
int emptyQueue(queue* q) {
	if (q->tail == -1) {
		return 1;
	} else {
		return 0;
	}
}

void printQueue(queue* q) {
	int temp_head = q->head;
	while (temp_head != q->tail + 1) {
		printf(" %d",q->items[temp_head]);
		temp_head++;
	}
	printf("\n");
}

void freeQueue(queue* q) {
	free(q);
}

stack* initStack() {
	stack* s = (stack*)malloc(sizeof(stack));
	s->top = -1;
	return s;
}

void push(stack* s, int x) {
	if (s->top != graph_size-1) {
		s->top++;
		s->items[s->top] = x;
	}
}

int pop(stack* s) {
	int item;
	if (!emptyStack(s)) {
		item = s->items[s->top];
		s->top--;
	} else {
		return 1;
	}
	return item;
}

int emptyStack(stack* s) {
	if (s->top == -1) {
		return 1;
	} else {
		return 0;
	}
}

void freeStack(stack* s) {
	free(s);
}

int in(int arr[size_X][size_Y], int search) {
	int x = search % size_X;
	int y = search/size_Y;

	if (arr[x][y]>0) {
		return 1;
	} else {
		return 0;
	}
}

void put(int arr[size_X][size_Y], int val, int order) {
	int x = val % size_X;
	int y = val/size_Y;

	arr[x][y] = order;
}

int isCat(int cat_loc[10][2], int cats, int check) {
	int x = check % size_X;
	int y = check / size_X;

	for (int i = 0; i < cats; i++) {
		if (cat_loc[i][0] == x && cat_loc[i][1] == y) {
			return 1;
		}
	}
	return 0;
}


void swap(int a[2], int b[2]) {
  int temp[2];
  temp[0] = b[0];
  temp[1] = b[1];
  b[0] = a[0];
  b[1] = a[1];
  a[0] = temp[0];
  a[1] = temp[1];
}

// Function to heapify the tree
void heapify(int array[][2], int i) {
  if (p_queue_size == 1) {
    printf("Single element in the heap");
  } else {
    // Find the largest among root, left child and right child
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < p_queue_size && array[l][0] < array[largest][0])
      largest = l;
    if (r < p_queue_size && array[r][0] < array[largest][0])
      largest = r;

    // Swap and continue heapifying if root is not largest
    if (largest != i) {
      swap(&array[i][0], &array[largest][0]);
      heapify(array, largest);
    }
  }
}

// Function to insert an element into the tree
void insert(int array[][2], int newNum, int node_num) {
  if (p_queue_size == 0) {
    array[0][0] = newNum;
    array[0][1] = node_num;
    p_queue_size += 1;
  } else {
    array[p_queue_size][0] = newNum;
    array[p_queue_size][1] = node_num;
    p_queue_size += 1;
    for (int i = p_queue_size / 2 - 1; i >= 0; i--) {
      heapify(array, i);
    }
  }
}

// Function to delete an element from the tree
void deleteRoot(int array[][2], int num) {
  int i;
  for (i = 0; i < p_queue_size; i++) {
    if (num == array[i][1])
      break;
  }

  swap(&array[i][0], &array[p_queue_size - 1][0]);
  p_queue_size -= 1;
  for (int i = p_queue_size / 2 - 1; i >= 0; i--) {
    heapify(array, i);
  }
}

bool in_queue(int array[][2], int num) {
	for(int i = 0; i < p_queue_size;i++){
		if (array[i][1] == num) return true;
	}
	return false;
}