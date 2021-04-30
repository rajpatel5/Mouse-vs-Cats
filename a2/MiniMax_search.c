/*
	CSC D84 - Unit 2 - MiniMax search and adversarial games

	This file contains stubs for implementing a MiniMax search
        procedure with alpha-beta pruning. Please read the assignment
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

	Starter code: F.J.E. Sep. 15
*/

#include "MiniMax_search.h"

int queue[graph_size];
int front_index = 0;
int size = 0;
bool deadPaths[graph_size];
double board[graph_size][4];

double MiniMax(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), int agentId, int depth, int maxDepth, double alpha, double beta)
{
	/*
	   This function is the interface between your solution for the assignment and the driver code. The driver code
	   in MiniMax_search_core_GL will call this function once per frame, and provide the following data
	   
	   Board and game layout:

		Exactly the same as for Assignment 1 - have a look at your code if you need a reminder of how the adjacency
		list and agent positions are stored.	

		Note that in this case, the path will contain a single move - at the top level, this function will provide
		an agent with the 'optimal' mini-max move given the game state.


		IMPORTANT NOTE: Mini-max is a recursive procedure. This function will need to fill-in the mini-max values for 
			   all game states down to the maximum search depth specified by the user. In order to do that,
			   the function needs to be called with the correct state at each specific node in the mini-max
			   search tree.

			   The game state is composed of:

				* Mouse, cat, and cheese positions (and number of cats and cheeses)
				
			   At the top level (when this function is called by the mini-max driver code), the game state
			   correspond to the current situation of the game. But once you start recursively calling
			   this function for lower levels of the search tree the positions of agents will have changed.
			   
			   Therefore, you will need to define local variables to keep the game state at each node of the
			   mini-max search tree, and you will need to update this state when calling recursively so that
			   the search does the right thing.

			   This function *must check* whether:
				* A candidate move results in a terminal configuration (cat eats mouse, mouse eats cheese)
				  at which point it calls the utility function to get a value
				* Maximum search depth has been reached (depth==maxDepth), at which point it will also call
				  the utility function to get a value
				* Otherwise, call recursively using the candidate configuration to find out what happens
				  deeper into the mini-max tree.

		Arguments:
			gr[graph_size][4]   		- This is an adjacency list for the maze
			path[1][2] 			- Your function will return the optimal mini-max move in this array.
			minmax_cost[size_X][size_Y]	- An array in which your code will store the
							  minimax value for maze locations expanded by
							  the search *when called for the mouse, not
							  for the cats!*

							  This array will be used to provide a visual 
							  display of minimax values during the game.

			cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
						 but there can be fewer). Only valid cat locations are 0 to (cats-1)
			cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
							 but possibly fewer). Valid locations are 0 to (cheeses-1)
			mouse_loc[1][2] - Mouse location - there can be only one!
			mode - Search mode selection:
						mode = 0 	- No alpha-beta pruning
						mode = 1	- Alpha-beta pruning

			(*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]);
					- This is a pointer to the utility function which returns a value for a specific game configuration

					   NOTE: Unlike the search assignment, this utility function also gets access to the graph so you can do any processing 					 that requires knowledge of the maze for computing the utility values.

					  * How to call the utility function from within this function : *
						- Like any other function:
							u = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
							
			agentId: Identifies which agent we are doing MiniMax for. agentId=0 for the mouse, agentId in [1, cats] for cats. Notice that recursive calls
							 to this function should increase the agentId to reflect the fact that the next level down corresponds to the next agent! For a game
							 with two cats and a mouse, the agentIds for the recursion should look like 0, 1, 2, 0, 1, 2, ...
		
			depth: Current search depth - whether this is a MIN or a MAX node depends both on depth and agentId.
			
			maxDepth: maximum desired search depth - once reached, your code should somehow return
				  a minimax utility value for this location.

			alpha. beta: alpha and beta values passed from the parent node to constrain search at this
					 level.

		Return values:
			Your search code will directly update data passed-in as arguments:
			
			- Mini-Max value	: Notice this function returns a double precision number. This is
						  the minimax value at this level of the tree. It will be used 
						  as the recursion backtracks filling-in the mini-max values back
						  from the leaves to the root of the search tree. 

			- path[1][2]		: Your MiniMax function will return the location for the agent's 
						  next location (i.e. the optimal move for the agent). 
			- minmax_cost[size_X][size_Y] 	:  Your search code will update this array to contain the
							   minimax value for locations that were expanded during
							   the search. This must be done *only* for the mouse.

							   Values in this array will be in the range returned by
							   your utility function.

			* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
				  or the location of the mouse - if you try, the driver code will know it *
				
			That's that, now, implement your solution!
	 */

	 /********************************************************************************************************
	 * 
	 * 	TO DO:	Implement code to perform a MiniMax search. This will involve a limited-depth BFS-like
	 *              expansion. Once nodes below return values, your function will propagate minimax utilities
	 *		as per the minimax algorithm.
	 *	
	 *		Note that if alpha-beta pruning is specified, you must keep track of alphas and betas
	 *		along the path.
	 *
	 *		You can use helper functions if it seems reasonable. Add them to the MiniMax_search.h
	 *		file and explain in your code why they are needed and how they are used.
	 *
	 *		Recursion should appear somewhere.
	 *
	 *		MiniMax cost: If the agentId=0 (Mouse), then once you have a MiniMax value for a location
	 *		in the maze, you must update minmax_cost[][] for that location.
	 *
	 *		How you design your solution is up to you. But:
	 *
	 *		- Document your implementation by adding concise and clear comments in this file
	 *		- Document your design (how you implemented the solution, and why) in the report
	 *
	 ********************************************************************************************************/

	// Stub so that the code compiles/runs - This will be removed and replaced by your code!
	
	for (int i = 0; i < graph_size; i++) {
		for (int j = 0; j < 4; j++){
			board[i][j] = gr[i][j];
		}
	}
	MiniMaxAlgorithm(path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, mouse_loc, mode, agentId, depth, maxDepth, alpha, beta);
}

double MiniMaxAlgorithm(int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int agentId, int depth, int maxDepth, double alpha, double beta)
{
	/* MiniMax Algorithm with enhanced utility values */
	
    //If maximum depth has been reached, or if the node is a terminal node, calculate and return a utility value.
    if(checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses) == 1 || depth == maxDepth) {
        return utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, board);
    }
	
	int optimal_location[2];
	int current_mouse_location[1][2];
	int current_cat_location[10][2];
	int current_cat = agentId - 1;
	int index = -1;
	int i = 0;
	double minCost = 10000;
    double maxCost = -10000;
	
    if(agentId != 0) {		
		// Check all moves the current cat can make
        while (i < 4) {
			index = (size_X * (cat_loc[current_cat][1])) + cat_loc[current_cat][0];
			
            if(board[index][i]) {	
				int j = 0;
                while (j < cats) {
                    current_cat_location[j][0] = cat_loc[j][0];
                    current_cat_location[j][1] = cat_loc[j][1];
					j++;
                }
				
                updateLocation(current_mouse_location, current_cat_location, current_cat, i, 1);
				
                //If all cats have made moves, then reset agentId to mouse's move.
                if(cats == agentId) {
                    agentId = -1;
                }
				
                // Find cost
                double cost = MiniMaxAlgorithm(path, minmax_cost, current_cat_location, cats, cheese_loc, cheeses, mouse_loc, mode, agentId + 1, depth + 1, maxDepth, alpha, beta);
                
				if(cost < minCost) {
                    minCost = cost;
                }
				
                // Alpha-beta pruning
                if(mode && prune(mode, &alpha, &beta, &cost, 1)) {
					// Prune the search tree
                    break;
                }
            }
			
			i++;
        }
		
        return minCost;
    } else {
		i = 0;
		
		// Check all possible moves the mouse can make
        while (i < 4) {
			index = ((size_X * mouse_loc[0][1])) + mouse_loc[0][0];
			
            if(board[index][i]) {
                current_mouse_location[0][0] = mouse_loc[0][0];
                current_mouse_location[0][1] = mouse_loc[0][1];
				
				// Update location and find cost
				updateLocation(current_mouse_location, current_cat_location, current_cat, i, 0);
                double cost = MiniMaxAlgorithm(path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, current_mouse_location, mode, agentId + 1, depth + 1, maxDepth, alpha, beta);
                
				// Replace the value of maxCost with cost.
                if(cost > maxCost) {
                    maxCost = cost;
                    optimal_location[0] = current_mouse_location[0][0];
                    optimal_location[1] = current_mouse_location[0][1];
                }
				
                // Alpha-beta pruning
                if(mode && prune(mode, &alpha, &beta, &cost, 0)) {
					// Prune the search tree
                    break;
                }
				
				// Store cost
                minmax_cost[current_mouse_location[0][0]][current_mouse_location[0][1]] = cost;
            }
			
			i++;
        }
		
        // Set next position once all possible paths have been considered
        if(depth == 0) {
            path[0][0] = optimal_location[0];
            path[0][1] = optimal_location[1];
        }
		
        return maxCost;
    }
}

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4])
{
	 /*
		This function computes and returns the utility value for a given game configuration.
		As discussed in lecture, this should return a positive value for configurations that are 'good'
		for the mouse, and a negative value for locations that are 'bad' for the mouse.
		How to define 'good' and 'bad' is up to you. Note that you can write a utility function
		that favours your mouse or favours the cats, but that would be a bad idea... (why?)
		Input arguments:
			cat_loc - Cat locations
			cheese_loc - Cheese locations
			mouse_loc - Mouse location
			cats - # of cats
			cheeses - # of cheeses
			depth - current search depth
			gr - The graph's adjacency list for the maze
			These arguments are as described in A1. Do have a look at your solution!
	 */
	
	// Initialize positions and distances, also utility value
	int mouse_pos = (size_X * mouse_loc[0][1]) + mouse_loc[0][0];
	int enhance_utility = 2;									// Used to make utility better
	double dist_to_cells[graph_size] = { 10000 };			// Mouse's distance to each cell in graph
	double closest_cat[2] = { 10000, 10000 };				// store index and distance of closest cat
	double closest_cheese[2] = { 10000, 10000 };			// store index and distance of closest cheese
	double utility_value = 0;
	
	// Initialize the mouse to cells distance
	int i = 0;
    while(i < graph_size) {
        dist_to_cells[i] = 10000;
		i++;
    }
	
	// Run BFS
	breadthFirstSearch(dist_to_cells, cheese_loc, cat_loc, cats, cheeses, mouse_pos);
	
	// Find closest cat and cheese relative to the mouse
	findClosest(cat_loc, dist_to_cells, closest_cat, closest_cat[1], cats, false);
	findClosest(cheese_loc, dist_to_cells, closest_cheese, closest_cheese[1], cheeses, true);
	
	// Try using heuristics to enhance utility
	H_cost(0, 0, cat_loc, cheese_loc, mouse_loc, cats, cheeses);
	
	// Determine initial utility value depending on the difference of the distance
	// between mouse and cheese, and mouse and cat
	if(closest_cat[1] != 0 && closest_cheese[1] != 0) {
		// Farther the cheese => increase utility a little and closer the cat => decrease utility a bit
        utility_value = enhance_utility * ((1 / closest_cheese[1]) * 25) - (closest_cat[1] * 0.03);
		updateUtility(closest_cat[1] - closest_cheese[1], &utility_value);
		
        // Increase utility if mouse is within 6 cells from closest cheese
        if (closest_cheese[1] < 6) {
            utility_value += enhance_utility * 10;
        }
    } else {
		// Cat eats mouse => set utility value to significantly low value.
		if(closest_cat[1] == 0) {
            utility_value = enhance_utility * -10000;
        }
		
		// Mouse eats a cheese and cat cannot land on cheese either => increase utility significantly
		// OR last cheese remaining => set utility value to significantly high value
		if((cheeses == 1 || closest_cat[1] > 2) && closest_cheese[1] == 0) {
            utility_value = enhance_utility * 10000;
        }
    }
	
	deadPaths[mouse_pos] = 1;		// Set to true
	handleDeadEnds(deadPaths, cheeses, cheese_loc, closest_cheese[0], closest_cat[1], mouse_pos, &utility_value);
	
	// Depth is closer to the root node => increase depth factor.
	double depth_factor = 3.0/(1 + depth);
	
	// Utility value doesn't favour mouse => depth factor should be negative.
	utility_value += depth_factor;		// Update utlility value with depth factor
	if(utility_value < 0) {
        utility_value -= enhance_utility * 2 * depth_factor;		// Update utility value
    }

    return utility_value;
}

int checkForTerminal(int mouse_loc[1][2],int cat_loc[10][2],int cheese_loc[10][2],int cats,int cheeses) {
    /*
      This function determines whether a given configuration constitutes a terminal node.
      Terminal nodes are those for which:
        - A cat eats the mouse
        or
        - The mouse eats a cheese
      If the node is a terminal, the function returns 1, else it returns 0
    */

	// Check for cats having lunch
    for (int i=0; i<cats; i++)
        if (mouse_loc[0][0]==cat_loc[i][0]&&mouse_loc[0][1]==cat_loc[i][1]) return(1);

	// Check for mouse having lunch
    for (int i=0; i<cheeses; i++)
        if (mouse_loc[0][0]==cheese_loc[i][0]&&mouse_loc[0][1]==cheese_loc[i][1]) return(1);

    return(0);

}

int pop() {
    /* Pop first value from queue. */
    int location = queue[front_index];
    front_index = front_index + 1;
    size = size - 1;
	
    return location;
}

bool foundDistances(int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, double distances[graph_size]) {
	/* Checks if all distances have been found */
	
	bool found = 1;	// Set to true
	int i = 0;		// initialize
  
	// Check distances found to all cheeses
	while(found && i < cheeses) {
		found = !(distances[(size_X * cheese_loc[i][1]) + cheese_loc[i][0]] == 10000);	
		i++;
	}
  
	i = 0;
	// Check distances found to all cats
	while(found && i < cats) {
		found = !(distances[(size_X * cat_loc[i][1]) + cat_loc[i][0]] == 10000);
		i++;
	}
  
	// All distances found => empty queue
	if(found) {
		size = 0;
		front_index = 0;
	}
  
	return found;
}

bool prune(int mode, double *alpha, double *beta, double *cost, bool isCats) {
	/* Does Alpha-beta pruning */
	
	// Make sure alpha-beta pruning in ON
	if (mode){
		// Prune
		if(*alpha >= *beta) {
			return 1;
		}
		
		if (isCats){	
			// Update Beta
			if(*beta > *cost) {
				*beta = *cost;
			}
		}
		else {
			// Update alpha
			if(*alpha < *cost) {
				*alpha = *cost;
			}
		}
	}
	
	return 0;
}

void push(int location) {
    /* Add location node to the queue. */
    queue[front_index + size] = location;
    size = size + 1;
}


void updateLocation(int current_mouse_location[1][2], int current_cat_location[10][2], int current_cat, int index, bool isCats) {
	/* Update the location of the current cat or the location of the mouse */
	
	if (isCats){
		// Update current cat location
		if(index % 2 != 0) {
			current_cat_location[current_cat][0] = current_cat_location[current_cat][0] - (index - 2);
		} else {
			current_cat_location[current_cat][1] = current_cat_location[current_cat][1] + (index - 1);
		}
	}
	else {
		// Update current mouse location
		if(index % 2 != 0) {
			current_mouse_location[0][0] = current_mouse_location[0][0] - (index - 2);
		} else {
			current_mouse_location[0][1] = current_mouse_location[0][1] + (index - 1);
		}
	}
}

void updateUtility(double minimized_distance_diff, double *utility_val) {
	/* If a cat is closer to a cheese than the mouse, then add to the utility
    depending on the difference between the mouse and cat. Otherwise subtract 
	from the utility depending on how close the cat is to the mouse. */
	
	// Increase utlity if cheese is closer
	if (minimized_distance_diff >= 0 && minimized_distance_diff < 4) {
		*utility_val += 2;
	} else if (minimized_distance_diff >= 4 && minimized_distance_diff < 8) {
		*utility_val += 8;
	} else if (minimized_distance_diff >= 8) {
		*utility_val += 18;
	} 
	
	// Decrease utlity if cat is closer
	if (minimized_distance_diff <= -2 && minimized_distance_diff > -6) {
		*utility_val -= 4;
	} else if (minimized_distance_diff <= -6 && minimized_distance_diff > -11) {
		*utility_val -= 12;
	} else if (minimized_distance_diff <= -11) {
		*utility_val -= 24;
	}
}

void handleDeadEnds(bool deadPaths[graph_size], int num_cheese, int cheese_location[10][2], double closest_cheese, double closest_cat, int position, double *utility_val) {
	/* Deals with dead end paths and updates the utility value accordingly */
	
	// Initalize variables
	int i = 0;
	
	// Four cases of dead end paths
	bool only_bottom_open = (board[position][0] == 0 && board[position][1] == 0 && board[position][2] == 0);
	bool only_left_open = (board[position][1] == 0 && board[position][2] == 0 && board[position][3] == 0);
	bool only_top_open = (board[position][2] == 0 && board[position][3] == 0 && board[position][0] == 0);
	bool only_right_open = (board[position][3] == 0 && board[position][0] == 0 && board[position][1] == 0);
	
	// Run only if there's one opening
	if(only_bottom_open || only_left_open || only_top_open || only_right_open) {
		
        // If closest cheese is in a dead end, then check other cheeses
        if(closest_cheese == position) {
			while (i < num_cheese){
				if(!deadPaths[(size_X * cheese_location[i][1]) + cheese_location[i][0]]) {
					break;	// Can exit early reducing time
				}
				
				i++;	// Go to next cheese
			}
        }
		
        // Dead end contains a cheese, but other cheeses exists and have not been considered yet
        // OR if its a dead end, and the cat is close to the mouse, decrease utility to a very low number.
        if(closest_cat < 4 && i != num_cheese) {
            *utility_val -= 6000;
        }
    }
}

void findClosest(int obstacle_location[10][2], double distances[graph_size], double minimum[2], double catDist, int obstacles, bool isCheeses) {
  /* Calculate all min distances and their respective positions for all the obstacles */
	
	// Declare and initialize variables
	int index;
	int i = 0;
	while (i < obstacles) {
        index = (size_X * obstacle_location[i][1]) + obstacle_location[i][0];
		
        // If cheese is at a dead end position, then choose the next closest cheese
        // unless the closest cat is more than 2 cells away.
        if((!isCheeses || catDist > 2 || !deadPaths[index]) && (minimum[1] > distances[index])) {
            minimum[0] = index;
            minimum[1] = distances[index];
        }
		
		i++;
    }
	
    // If all cheeses are at dead ends, choose the minimum distance between them
    if(isCheeses && minimum[1] == 10000) {
		i = 0;
		while(i < obstacles) {
			index = (size_X * obstacle_location[i][1]) + obstacle_location[i][0];
		  
			if(minimum[1] > distances[index]) {
				minimum[0] = index;
				minimum[1] = distances[index];
			}
			
			i++;
		}
    }
}

int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses)
{
 /* Heuristic to make utility function better */

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

void breadthFirstSearch(double distances[graph_size], int cheese_locations[10][2], int cat_locations[10][2], int cats, int cheeses, int start_index) {
    /* Compute BFS distance from mouse to all cats and cheeses.*/
	
	// Declare all variables
	int current;
	int current_x;
	int current_y;
	int child;
	int i;
	
	// Initial values
    distances[start_index] = 0;
    push(start_index);
	
	// Run until all distances are found
    while (!foundDistances(cat_locations, cats, cheese_locations, cheeses, distances)) {
        current = pop();
        current_x = current % size_X;
        current_y = current / size_Y;
		
		i = 0; 
        while(i < 4) {
            // Child is the index of the current child
            child = (size_X * (current_y + ((i - 1) % 2))) + (current_x - ((i - 2) % 2));
			
			// Distance of child hasn't been calculated yet
            if(distances[child] == 10000 && board[current][i] == 1) {
				push(child);
                distances[child] = distances[current] + 1;
            }
			
			i++;
        }
    }
}