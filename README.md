# Mouse-vs-Cats
This assignment was assigned by the 309 (Programming on the Web) course instructor at the University of Toronto.
There are 4 assignments displaying the various algorithms and concepts used in artificial intelligence.
The mouse has to get all the pieces of cheese without being caught by the cats with a maze. Cats are manually
set to various levels of smartness; the smarter the cat, the harder to get the cheese. 

**Note**: A4 does not have a visual representation of the mouse and cats moving nor the maze.


## Algorithms/Concepts used in each assignment
| Assignment | Algorithms/Concept |
| ---------- | ------------------ |
| A1 | BFS, DFS, A* Search with heuristics |
| A2 | Minimax Search |
| A3 | Q-learning |
| A3 | Neural Networks |

## Running the code (Windows)
**1.)** Turn on `Windows Subsystem for Linux`\
**2.)** Open windows command prompt or powershell, and run `apt-get update`\
**3.)** Run `apt-get install build-essential freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev`\
**4.)** Download and install **Xming X Server**: https://sourceforge.net/projects/xming/, then run it\
**5.)** Open up the bash terminal in an assignment directory and enter `export DISPLAY=:0`\
**6.)** Enter `./compile.sh`\
**7.)** Enter `./{filename} {parameters}`, ex; `./AI_search 1522 1 1 0 0`

Please see the respective pdf files for the assignment to see what the parameters are and their bounds.
