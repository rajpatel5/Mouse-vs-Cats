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
% 1) Student Name:	Raj Patel	
% 2) Student Name:	Harshit Patel
%
% 1) Student number: 1005240721
% 2) Student number: 1004779215
% 
% 1) UtorID: patel732
% 2) UtorID: patel564
% 
% We hereby certify that the work contained here is our own
%
% __Raj Patel_________             ___Harshit Patel____
% (sign with your name)            (sign with your name)
***********************************************************************/

#include "AI_search.h"

/* Insert item to the front of the data structure */
void push(Queue *queue, int data) {
  Data *entry = (struct Data*)calloc(1, sizeof(struct Data));
  entry->data = data;
  entry->next = NULL;

  // Update head (with new entry) and tail
  if (queue->tail != NULL || queue->head != NULL) {
    queue->tail->next = entry;
    queue->tail = entry;
  } else {
    queue->tail = entry;
    queue->head = queue->tail;
  }
}

/* Append item to the end of the data structure */
void append(Queue *queue, int data) {
  Data *entry = (struct Data*)calloc(1, sizeof(struct Data));
  entry->data = data;
  entry->next = NULL;

  // Update head and tail (with new entry)
  if (queue->tail != NULL || queue->head != NULL) {
    entry->next = queue->head;
    queue->head = entry;
  } else {
    queue->tail = entry;
    queue->head = queue->tail;
  }
}

/* Select method of insert depending on mode */
void insert(Queue *queue, int data, int mode) {
  if (mode < 2) {
    if (mode == 0){
      push(queue, data);      // For BFS
    } else {
      append(queue, data);    // For DFS
    }
  }
}

/* Return the removed front item in the data structure */
int pop(Queue *queue) {
  Data *temp = queue->head;
  int front = queue->head->data;

  // Update head and tail
  if (queue->tail != queue->head) {
    queue->head = queue->head->next;
  } else {
    queue->tail = NULL;
    queue->head = queue->tail;
  }

  free(temp);
  return front;
}

/* Return true if queue is empty, else false */
int is_empty(Queue *queue) {
  return (queue->tail == NULL && queue->head == NULL);
}

/* Find the index given the location */
int find_idx(int loc[2]) {
  return loc[0] + (loc[1] * size_X);    // Given from assignment documentation
}

/* Return true if current in locs, else false */
int is_cheese_or_cat(int current, int locs[10][2], int total) {
  int i = 0;
  while (i < total){
    if (current == find_idx(locs[i])){
      return 1;     // True
    }

    i++;
  }

  return 0;     // False
}

/* Get the path */
void get_path(int current, int visited[graph_size], int path[graph_size][2]) {
  int i = -1;
  int temp = current;

  // Get value of i
  while (temp >= 0 && temp != -1){
    i = i + 1;
    temp = visited[temp];   // Update value
  }

  temp = current;
  while (i >= 0){
    // Set path
    path[i][0] = temp % size_X;
    path[i][1] = temp / size_Y;
    temp = visited[temp];     // Update value
    i--;
  }
}

/* Set visited */
void set_visited(Queue *queue, int visit_order[size_X][size_Y], int cost[graph_size], int visited[graph_size], int current, int loc, int mode){
  if (mode < 2) {
    insert(queue, loc, mode);

    // Set visited
    if (0 >= visit_order[loc % size_X][loc / size_Y]){
      visited[loc] = current;
    }
  } else {
    // Set cost and visited
    cost[loc] = cost[current] + 1;
    visited[loc] = current;
  }
}

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
			node at (i,location-1)
				^
				|
	(node at i-1, location) <- node at (i,location) -> node at (i+1, location)
				|
				v
			node at (i,location+1)
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
  int cost[graph_size], visited[graph_size]; 
  int visit_counter = 0;
  int current, location, x_idx, y_idx;

  Queue *queue = (struct Queue*)calloc(1, sizeof(struct Queue));
    
  // Initialize data structure
  queue->tail = queue->head = NULL;

  if (mode < 2){
    // BFS and DFS
    insert(queue, find_idx(mouse_loc[0]), mode);

    // Initialize visited nodes
    int i = 0;
    while (i < graph_size){
      visited[i] = -1;
      cost[i] = 0;
      i++;
    }

    // Keep running will we reach a cat or cheese, or till data structure is empty
    while (!(is_cheese_or_cat(current, cheese_loc, cheeses) || is_empty(queue))){
      current = pop(queue);
      x_idx = current % size_X;
      y_idx = current / size_Y;
      int is_valid_current = ((0 <= current) && (graph_size > current));

      if (!is_cheese_or_cat(current, cat_loc, cats) && 
        (0 >= visit_order[x_idx][y_idx]) && is_valid_current){
        
        // Keep count of num of visited nodes
        visit_counter = visit_counter + 1;
        visit_order[x_idx][y_idx] = visit_counter;

        // Locations (top, left, bottom, right)
        int expansion[4] = {current - size_X, current + 1, current + size_X, current - 1};
        
        // Order: top, left, bottom, right
        for (int j = 0; j < 4; j++){
          if (gr[current][j]){
            set_visited(queue, visit_order, cost, visited, current, expansion[j], mode);
          }
        }
      }
    }

    // Get path
    get_path(current, visited, path);

    // Free resources
    while (!is_empty(queue)){
      pop(queue);
    }
    free(queue);
  }
  else {
    // A* search
    int cheese_located, loc_cost;
    int node_cost, parent_node;
    current = find_idx(mouse_loc[0]);

    // Initialize cost and visited
    int i = 0;
    while (i < graph_size) {
      visited[i] = -1;
      cost[i] = INT_MAX;
      i++;
    }

    cost[current] = 0;    // Cost to current node
    visit_counter = 1;    // Since we have a current location
    visit_order[current % size_X][current / size_Y] = visit_counter;

    // Is cheese has been cheese_located
    cheese_located = is_cheese_or_cat(current, cheese_loc, cheeses);

    // Loop until cheese is cheese_located
    while (!cheese_located) {
      visit_counter = visit_counter + 1;
      x_idx = current % size_X;
      y_idx = current / size_Y;

      // Keep count of number of visited nodes
      visit_order[x_idx][y_idx] = visit_counter;
      parent_node = current;

      // Check if it is a cat
      if (!is_cheese_or_cat(current, cat_loc, cats) && current >= 0 && current < graph_size) {
        // Locations (top, left, bottom, right)
        int expansion[4] = {current - size_X, current + 1, current + size_X, current - 1};

        // Order: top, left, bottom, right
        for (int j = 0; j < 4; j++){
          if (gr[current][j] && cost[expansion[j]] > cost[current] + 1){
            set_visited(queue, visit_order, cost, visited, current, expansion[j], mode);
          }
        }
      }

      // find node with least distance that has not been visited
      i = 0;
      location = -1;
      while (i < graph_size) {
        // Find location cost
        loc_cost = INT_MAX;
        if (location != -1){
          if (cost[location] != INT_MAX) {
            loc_cost = cost[location] + heuristic(location % size_X, location / size_Y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr);
          } else {
            loc_cost = cost[location];
          }
        }

        // Find the node cost
        node_cost = cost[i];
        if (cost[i] != INT_MAX) {
          node_cost = cost[i] + heuristic(i % size_X, i / size_Y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr);
        }

        // set new location
        if ((node_cost < loc_cost)  && (visit_order[i % size_X][i / size_Y] <= 0)) {
          location = i;
        }

        i++;    // Move to next potential location
      }

      // Has cheese been located
      current = location;
      cheese_located = is_cheese_or_cat(current, cheese_loc, cheeses) || (parent_node == current);
    }

    // Get path
    get_path(current, visited, path);
  }

  // Set mouse location on path
  if (path[1][0] == -1 || path[1][1] == -1){
    path[1][0] = mouse_loc[0][0];
    path[1][1] = mouse_loc[0][1];
  }

  if (path[0][0] == -1 || path[0][1] == -1){
    path[0][0] = mouse_loc[0][0];
    path[0][1] = mouse_loc[0][1];
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

  int cost = graph_size;
  int x_pos, y_pos;

  for (int i = 0; i < cheeses; i++){

    x_pos = cheese_loc[i][0] - x;
    y_pos = cheese_loc[i][1] - y;

    // Want final cost to be >= 0
    if (x_pos < 0){
      x_pos = x_pos * -1;
    }
    if (y_pos < 0){
      y_pos = y_pos * -1;
    }

    // Manhattan distance
    int new_cost = x_pos + y_pos;

    // Update cost
    if (new_cost < cost){
      cost = new_cost;
    }
  }

  return cost;
}

/* Calculates the heuristic cost of cheese or cats */
int find_cost(int x, int y, int type, int loc[10][2]){
  int x_pos, y_pos, current;
  int nearest = graph_size;    // Might be graph_size for nearest cheeses
  int i = 0, total_cost = 0, cost = 0;

  while (i < type) {
    x_pos = loc[i][0] - x;
    y_pos = loc[i][1] - y;

    // Want final cost to be >= 0
    if (x_pos < 0){
      x_pos = x_pos * -1;
    }
    if (y_pos < 0){
      y_pos = y_pos * -1;
    }

    // Manhattan distance (absolute)
    current = (x_pos + y_pos);
    total_cost += current;

    if (current < nearest) {
      nearest = current;
    }

    i++;
  }

  int divisor;
  if (nearest > 1){
    divisor = nearest;
  } else {
    divisor = 1;
  }

  if ((graph_size / divisor) > nearest){
    cost = (graph_size / divisor) - total_cost;
  } else {
    cost = nearest - total_cost;
  }
  
  return cost;
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
 
  return find_cost(x, y, cats, cat_loc) - find_cost(x, y, cheeses, cheese_loc);
}