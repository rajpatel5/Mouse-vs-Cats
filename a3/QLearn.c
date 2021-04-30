/*
	CSC D84 - Unit 3 - Reinforcement Learning
	
	This file contains stubs for implementing the Q-Learning method
	for reinforcement learning as discussed in lecture. You have to
	complete two versions of Q-Learning.
	
	* Standard Q-Learning, based on a full-state representation and
	  a large Q-Table
	* Feature based Q-Learning to handle problems too big to allow
	  for a full-state representation
	    
	Read the assignment handout carefully, then implement the
	required functions below. Sections where you have to add code
	are marked

	**************
	*** TO DO:
	**************

	If you add any helper functions, make sure you document them
	properly and indicate in the report.txt file what you added.
	
	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Jan. 16
*/

#include "QLearn.h"

//Global Queue variable for breadthFirstSearch
int queue[max_graph_size];
int front_index = 0;
int size = 0;

void QLearn_update(int s, int a, double r, int s_new, double *QTable)
{
 /*
   This function implementes the Q-Learning update as stated in Lecture. It 
   receives as input a <s,a,r,s'> tuple, and updates the Q-table accordingly.
   
   Your work here is to calculate the required update for the Q-table entry
   for state s, and apply it to the Q-table
     
   The update involves two constants, alpha and lambda, which are defined in QLearn.h - you should not 
   have to change their values. Use them as they are.
     
   Details on how states are used for indexing into the QTable are shown
   below, in the comments for QLearn_action. Be sure to read those as well!
 */
 
  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
	
    int i;
    double current;
    double curr_val;
    double max_next = -100000;
    
	// Consider optimal action to take
    i = 0;
	while (i < 4) {
      current = *(QTable + (4 * s_new) + i);
		
      if (max_next < current) {
        max_next = current;
      }
      
      i++;
	}
	
	// Get current value of QTable
	curr_val = *(QTable + (4 * s) + a);
	
	// Update QSA value for current state
	*(QTable + (4 * s) + a) = (alpha * (r + (max_next * lambda) - curr_val)) + curr_val;
}

int QLearn_action(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, double *QTable, int size_X, int graph_size)
{
  /*
     This function decides the action the mouse will take. It receives as inputs
     - The graph - so you can check for walls! The mouse must never move through walls
     - The mouse position
     - The cat position
     - The chees position
     - A 'pct' value in [0,1] indicating the amount of time the mouse uses the QTable to decide its action,
       for example, if pct=.25, then 25% of the time the mouse uses the QTable to choose its action,
       the remaining 75% of the time it chooses randomly among the available actions.
       
     Remember that the training process involves random exploration initially, but as training
     proceeds we use the QTable more and more, in order to improve our QTable values around promising
     actions.
     
     The value of pct is controlled by QLearn_core_GL, and increases with each round of training.
     
     This function *must return* an action index in [0,3] where
        0 - move up
        1 - move right
        2 - move down
        3 - move left
        
     QLearn_core_GL will print a warning if your action makes the mouse cross a wall, or if it makes
     the mouse leave the map - this should not happen. If you see a warning, fix the code in this
     function!
     
   The Q-table has been pre-allocated and initialized to 0. The Q-table has
   a size of
   
        graph_size^3 x 4
        
   This is because the table requires one entry for each possible state, and
   the state is comprised of the position of the mouse, cat, and cheese. 
   Since each of these agents can be in one of graph_size positions, all
   possible combinations yield graph_size^3 states.
   
   Now, for each state, the mouse has up to 4 possible moves (up, right,
   down, and left). We ignore here the fact that some moves are not possible
   from some states (due to walls) - it is up to the QLearn_action() function
   to make sure the mouse never crosses a wall. 
   
   So all in all, you have a big table.
        
   For example, on an 8x8 maze, the Q-table will have a size of
   
       64^3 x 4  entries
       
       with 
       
       size_X = 8		<--- size of one side of the maze
       graph_size = 64		<--- Total number of nodes in the graph
       
   Indexing within the Q-table works as follows:
   
     say the mouse is at   i,j
         the cat is at     k,l
         the cheese is at  m,n
         
     state = (i+(j*size_X)) + ((k+(l*size_X))*graph_size) + ((m+(n*size_X))*graph_size*graph_size)
     ** Make sure you undestand the state encoding above!
     
     Entries in the Q-table for this state are

     *(QTable+(4*state)+a)      <-- here a is the action in [0,3]
     
     (yes, it's a linear array, no shorcuts with brackets!)
     
     NOTE: There is only one cat and once cheese, so you only need to use cats[0][:] and cheeses[0][:]
   */
  
  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/  
  
  // TODO: Move most of this into a helper function since feat_QLearn_action has similar code
  
  // Declare and Initialize variables and arrays
  int initial_moves[4];
  int i;
  int random_chance = (int) 100 * pct;
  int roll = (rand() % 100) + 1;
  int move = 0;
  int counter = 0;
  int total_counter = 0;
  int curr_mouse_index = (size_X * mouse_pos[0][1]) + mouse_pos[0][0];
  int state = ((size_X * mouse_pos[0][1]) + mouse_pos[0][0]) + (graph_size * ((size_X * cats[0][1]) + cats[0][0])) + (graph_size * graph_size * ((size_X * cheeses[0][1]) + cheeses[0][0]));
  double curr_val;

  if (roll > random_chance) {
    // Get value of counter
    counter = initalizeMoves(gr, initial_moves, curr_mouse_index);
    
    // Declare available moves array
    int available_moves[counter];
    
    // Store all possible available moves
    i = 0;
    while (i < 4) {
      if (initial_moves[i] != -1) {
        available_moves[total_counter] = initial_moves[i];
        total_counter++;
      }
      
      i++;
    }
    
    move = available_moves[rand() % counter];
  } else {
    move = findOptimalAction(gr, QTable, state, curr_mouse_index);
  }
  
  return move; // <--- of course, you will change this!
}

double QLearn_reward(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function computes and returns a reward for the state represented by the input mouse, cat, and
    cheese position. 
    
    You can make this function as simple or as complex as you like. But it should return positive values
    for states that are favorable to the mouse, and negative values for states that are bad for the 
    mouse.
    
    I am providing you with the graph, in case you want to do some processing on the maze in order to
    decide the reward. 
    
    This function should return a maximim/minimum reward when the mouse eats/gets eaten respectively.      
   */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/ 

  // Run BFS to return length of optimal path to cat or cheese
  int i, j;
  int walls = 0;
  int enhace_reward = 2;
  int mouse_index = (size_X * mouse_pos[0][1]) + mouse_pos[0][0];
  double distances_from_mouse[32][32];
  double cheese_distance;
  double cat_distance;
  double reward = 0;
  
  // Initalize distance
  i = 0;
  while (i < size_X) {
      j = 0;
    while (j < size_X) {
      distances_from_mouse[i][j] = 100000;
      j++;
    }
    
    i++;
  }
  
  distances_from_mouse[mouse_pos[0][0]][mouse_pos[0][1]] = 0;
  
  // Run BFS
  breadthFirstSearch(gr, 1, 1, mouse_index, size_X, cats, cheeses, distances_from_mouse);
  
  // Initialize distances
  cheese_distance = distances_from_mouse[cheeses[0][0]][cheeses[0][1]];
  cat_distance = distances_from_mouse[cats[0][0]][cats[0][1]];
  
  // Mouse gets eaten by cat => negative reward. mouse eats cheese => positive reward
  if (cheese_distance == 0) {
    reward += 50;
  } else if (cat_distance == 0) {
    reward -= 50;
  } 
  
  if (cheese_distance != 0 && cat_distance != 0){
    // Initialize reward
    reward = ((1 / cheese_distance) * 20) - (cat_distance * 0.80);
    updateReward(cat_distance - cheese_distance, &reward, 1, 0);
    
    // Determine if mouse is in dead end.
    i = 0;
    while (i < 4) {
      if (gr[mouse_index][i] == 0) {
        walls++;        // Update count of walls
      }
      
      // Mouse in a cell with atleast 3 walls around it => decrease the reward
      if (walls > 2) {
        reward -= 50;
      }
    
      i++;
    }
    
    // Mouse is within 4 steps of a cheese, so add more to the reward.
    if (cheese_distance < 4) {
      reward += 10;
    }
    
    if (reward == 0) {
      reward = 1;
    }
  }
  
  return reward * (size_X / 2);  
}

void feat_QLearn_update(double gr[max_graph_size][4],double weights[25], double reward, int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function performs the Q-learning adjustment to all the weights associated with your
    features. Unlike standard Q-learning, you don't receive a <s,a,r,s'> tuple, instead,
    you receive the current state (mouse, cats, and cheese potisions), and the reward 
    associated with this action (this is called immediately after the mouse makes a move,
    so implicit in this is the mouse having selected some action)
    
    Your code must then evaluate the update and apply it to the weights in the weight array.    
   */
  
   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/        
  
  // Declare and initalize variables
  int b = 0;
  int i = 0;
  int *max_a = &b;
  double features[25];
  double definite_Q_value;
  double curr_Q_val;
  double temporal_diff_target;
  double action_a = 0;
  double *max_Q_value = &action_a;
  
  maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, max_Q_value, max_a);
  definite_Q_value = *max_Q_value;
  evaluateFeatures(gr, features, mouse_pos, cats, cheeses, size_X, graph_size);
  curr_Q_val = Qsa(weights, features);
  temporal_diff_target = (reward + (definite_Q_value * lambda)) - curr_Q_val;

  while (i < numFeatures) {
    weights[i] = (alpha * features[i] * temporal_diff_target) + weights[i];        // Adjust weight value
    i++;
  }
}

int feat_QLearn_action(double gr[max_graph_size][4],double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, int size_X, int graph_size)
{
  /*
    Similar to its counterpart for standard Q-learning, this function returns the index of the next
    action to be taken by the mouse.
    
    Once more, the 'pct' value controls the percent of time that the function chooses an optimal
    action given the current policy.
    
    E.g. if 'pct' is .15, then 15% of the time the function uses the current weights and chooses
    the optimal action. The remaining 85% of the time, a random action is chosen.
    
    As before, the mouse must never select an action that causes it to walk through walls or leave
    the maze.    
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/        
  
  // TODO: Move most of this into a helper function since QLearn_action has similar code
  
  // Declare and initialize variables and arrays
  int initial_moves[4];
  int random_chance = (int) 100 * pct;
  int roll = rand() % 100 + 1;
  int move = 0;
  int i = 0;
  int curr_mouse_index = (size_X * mouse_pos[0][1]) + mouse_pos[0][0];
  int bm = 0;
  int *optimal_move = &bm;
  int counter = 0;
  int total_counter = 0;
  double osv = 0;
  double *optimal_state_val = &osv;

  if (roll > random_chance) {  
    // Get value of counter
    counter = initalizeMoves(gr, initial_moves, curr_mouse_index);
    
    // Declare available moves array
    int available_moves[counter];
    
    // Assign all possible available moves
    i = 0;
    while (i < 4) {
      if (initial_moves[i] != -1) {
        available_moves[total_counter] = initial_moves[i];
        total_counter++;
      }
      
      i++;
    }
    
    // Random move given random index
    move = available_moves[rand() % counter];
  } else {
    maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, optimal_state_val, optimal_move);
    move = *optimal_move;
  }
  
  return move;
}

void evaluateFeatures(double gr[max_graph_size][4],double features[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
   This function evaluates all the features you defined for the game configuration given by the input
   mouse, cats, and cheese positions. You are free to define up to 25 features. This function will
   evaluate each, and return all the feature values in the features[] array.
   
   Take some time to think about what features would be useful to have, the better your features, the
   smarter your mouse!
   
   Note that instead of passing down the number of cats and the number of cheese chunks (too many parms!)
   the arrays themselves will tell you what are valid cat/cheese locations.
   
   You can have up to 5 cats and up to 5 cheese chunks, and array entries for the remaining cats/cheese
   will have a value of -1 - check this when evaluating your features!
  */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/      
  
  int i;
  int j;
  int num_of_cheese = 0;
  int num_of_cats = 0;
  int walls = 0;
  int mouse_index = (size_X * mouse_pos[0][1]) + mouse_pos[0][0];
  double distances_from_mouse[32][32];
  double difference_in_distance;
  double min_dist_cheese = 100000;
  double min_dist_cat = 100000;
  double avg_num_of_cats = 0;
  double difference_in_distance_reward = 0;

  // Check cells and determine total number of cheeses and cats
  i = 0;
  while (i < 5) {
    if (cheeses[i][0] != -1) {
      num_of_cheese++;
    }
    
    if (cats[i][0] != -1) {
      num_of_cats++;
    }
    
    i++;
  }

  // Initialize distances
  i = 0;
  while (i < size_X) {
    j = 0;
    while (j < size_X) {
      distances_from_mouse[i][j] = 100000;
      j++;
    }
    
    i++;
  }

  distances_from_mouse[mouse_pos[0][0]][mouse_pos[0][1]] = 0;
  breadthFirstSearch(gr, num_of_cats, num_of_cheese, mouse_index, size_X, cats, cheeses, distances_from_mouse);
  
  // Find minimum distance to cheese
  i = 0;
  while (i < num_of_cheese) {
    if (min_dist_cheese > distances_from_mouse[cheeses[i][0]][cheeses[i][1]]) {
      min_dist_cheese = distances_from_mouse[cheeses[i][0]][cheeses[i][1]];
    }
    
    i++;
  }
  
  // Find minimum distance to cat
  i = 0;
  while (i < num_of_cats) {
    if (min_dist_cat > distances_from_mouse[cats[i][0]][cats[i][1]]) {
      min_dist_cat = distances_from_mouse[cats[i][0]][cats[i][1]];
    }
    
    avg_num_of_cats = distances_from_mouse[cats[i][0]][cats[i][1]] + avg_num_of_cats;
    i++;
  }

  // Average number of cats
  avg_num_of_cats = avg_num_of_cats / (double) num_of_cats;
  
  // Mouse closer to cheese => add to reward depending on distance of cheese
  // Mouse closer to cat => subtract from reward depending on distance to cat
  updateReward(min_dist_cat - min_dist_cheese, &difference_in_distance_reward, (size_X / 2), 1);

  // Check if mouse is in a dead end
  i = 0;
  while (i < 4) {
    if (gr[mouse_index][i] == 0) {
      walls++;
    }
    
    i++;
  }

  // 1st feature: Nearest cheese
  // 2nd feature: Nearest cat
  // 3rd feature: Difference between minimum cheese distance and minimum cat distance
  // 4th feature: Disincentivize dead ends if cheese is not located in a dead end
  // 5th feature: Maximize average distance from mouse to all cats
  
  features[4] = 1 - (1.0 / (avg_num_of_cats + 1));
  if(num_of_cats <= 1) {
    features[4] = 0;
  }
  
  features[3] = (1 / 11);
  if (min_dist_cheese <= 0 || walls != 3) {
    features[3] = 1;
  }
  
  features[2] = 1 - (1 / (difference_in_distance_reward + 1));
  features[1] = 1 - (1 / (min_dist_cat + 1));
  features[0] = 1 / (min_dist_cheese + 1);
}

double Qsa(double weights[25], double features[25])
{
  /*
    Compute and return the Qsa value given the input features and current weights
   */

  /***********************************************************************************************
  * TO DO: Complete this function
  ***********************************************************************************************/  
  
  int i = 0;
  double return_val = 0;
  
  while (i < numFeatures) {
    return_val = (features[i] * weights[i]) + return_val;
    i++;
  }
  
  return return_val;
}

void maxQsa(double gr[max_graph_size][4],double weights[25],int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size, double *max_Q_value, int *max_a)
{
 /*
   Given the state represented by the input positions for mouse, cats, and cheese, this function evaluates
   the Q-value at all possible neighbour states and returns the max. The maximum value is returned in max_Q_value
   and the index of the action corresponding to this value is returned in max_a.
   
   You should make sure the function does not evaluate moves that would make the mouse walk through a
   wall. 
  */
 
   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/  
 
  int temp_position[1][2];
  int num_of_cats = 0;
  int mouse_index = (size_X * mouse_pos[0][1]) + mouse_pos[0][0];
  double features[25];
  double current;
  
  *max_Q_value = -100000;
  
  int i = 0;
  while (i < 5) {
    if (cats[i][0] != -1) {
      num_of_cats++;
    }
    
    i++;
  }
  
  i = 0;
  while (i < 4) {
    if (gr[mouse_index][i]) {
      // Find new position of mouse
      temp_position[0][0] = mouse_pos[0][0] - ((i - 2) % 2);
      temp_position[0][1] = mouse_pos[0][1] + ((i - 1) % 2);
      
      // Evaluate the features
      evaluateFeatures(gr, features, temp_position, cats, cheeses, size_X, graph_size);
      
      // Get current value
      current = Qsa(weights, features);
      if (*max_Q_value < current) {
        *max_a = i;
        *max_Q_value = current;
      }
    }
    
    i++;
  }
}

/***************************************************************************************************
 *  Add any functions needed to compute your features below 
 *                 ---->  THIS BOX <-----
 * *************************************************************************************************/

int initalizeMoves(double gr[max_graph_size][4], int initial_moves[4], int mouse_index) {
  /* Initialize moves and find possible moves */
  
  int counter = 0;
  int i = 0;
  while (i < 4) {
    initial_moves[i] = -1;
    if (gr[mouse_index][i]) {
      initial_moves[i] = i;
      counter++;
    }
    
    i++;
  }

  return counter;
}

int findOptimalAction(double gr[max_graph_size][4], double *QTable, int state, int mouse_index) {
  int curr_val;
  int max_index;
  int i = 0;
  double max_value = -100000;

  while (i < 4) {
    curr_val = *(QTable + (4 * state) + i);
    
    if ((curr_val > max_value) && (gr[mouse_index][i])) {
      max_index = i;
      max_value = curr_val;
    }
    
    i++;
  }

  return max_index;
}

void updateReward(double minimized_distance_diff, double *reward_val, double factor, bool diff_reward) {
  /* If a cat is closer to a cheese than the mouse, then add to the reward
  depending on the difference between the mouse and cat. Otherwise subtract 
  from the reward depending on how close the cat is to the mouse. */
  
  if (diff_reward){
    // Set reward based on difference in distance
    if (minimized_distance_diff >= 14 * factor){
      *reward_val = 150 * factor;
    } else if (minimized_distance_diff >= 6 * factor) {
      *reward_val = 108 * factor;
    } else if (minimized_distance_diff >= 3 * factor) {
      *reward_val = 90 * factor;
    } else if (minimized_distance_diff >= 0 * factor) {
      *reward_val = 72 * factor;
    } else if (minimized_distance_diff <= -11 * factor) {
      *reward_val = 8 * factor;
    } else if (minimized_distance_diff <= -8 * factor) {
      *reward_val = 0 * factor;
    } else if (minimized_distance_diff <= -6 * factor) {
      *reward_val = 18 * factor;
    } else if (minimized_distance_diff < 0 * factor) {
      *reward_val = 54 * factor;
    }
  } else {
    // Increase reward if cheese is closer
    if (minimized_distance_diff >= 0 && minimized_distance_diff < 4) {
      *reward_val += 1;
    } else if (minimized_distance_diff >= 4 && minimized_distance_diff < 8) {
      *reward_val += 4;
    } else if (minimized_distance_diff >= 8) {
      *reward_val += 9;
    } 
    
    // Decrease reward if cat is closer
    if (minimized_distance_diff <= -2 && minimized_distance_diff > -6) {
      *reward_val -= 2;
    } else if (minimized_distance_diff <= -6 && minimized_distance_diff > -11) {
      *reward_val -= 6;
    } else if (minimized_distance_diff <= -11) {
      *reward_val -= 12;
    }
  }
}

bool foundDistances(int cats, int cheeses, int cat_locations[5][2], int cheese_locations[5][2], double distances[32][32]) {
  /* Checks if all distances have been found */
  
  int i;
  bool found = 1;	// Set to true
  
  // Check if distances to all cheeses are found
  i = 0;
  while (i < cheeses) {
    if (distances[cheese_locations[i][0]][cheese_locations[i][1]] == 100000) {
      found = 0;
      break;
    }
    
    i++;
  }
  
  // Check if distances to all cats are found
  i = 0;
  while (i < cats) {
    if (distances[cat_locations[i][0]][cat_locations[i][1]] == 100000) {
      found = 0;
      break;
    }
    
    i++;
  }
  
  // Empty the queue
  if (found) {
    size = 0;
    front_index = 0;
  }
  
  return found;
}

void push(int location) {
  /* Add location node to the queue. */
  
  queue[size + front_index] = location;
  size++;
}

int pop() {
  /* Pop first value from queue. */
  
  int location = queue[front_index];
  front_index++;
  size--;
  return location;
}

int helper(int x, int y, int cat_loc[5][2], int cheese_loc[5][2], int mouse_loc[1][2], int cats, int cheeses, int size_X)
{
 /* Heuristic to make Q-Learning algorithm better */

  int cost = size_X;
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

void breadthFirstSearch(double gr[max_graph_size][4], int num_of_cats, int num_of_cheeses, int source_index, int size_X, int cat_locations[5][2], int cheese_locations[5][2], double distances[32][32]) {
  /* Compute BFS distance from mouse to all cats and cheeses. */
  
  // Declare all variables
  int current;
  int current_x;
  int current_y;
  int child;
  int child_x;
  int child_y;
  int i;
    
  push(source_index);
  
  // Run until all distances are found
  while (!foundDistances(num_of_cats, num_of_cheeses, cat_locations, cheese_locations, distances)) {
    int current = pop();
    int current_x = current % size_X;
    int current_y = current / size_X;
    
    i = 0;
    while (i < 4) {
      child_x = current_x - ((i - 2) % 2);
      child_y = current_y + ((i - 1) % 2);
      
      // Child is the index of the current child
      child = (size_X * child_y) + child_x;
      
      // Distance of child hasn't been calculated yet
      if (distances[child_x][child_y] == 100000 && gr[current][i] == 1) {
        distances[child_x][child_y] = distances[current_x][current_y] + 1;
        push(child);
      }
      
      i++;
    }
  }
}