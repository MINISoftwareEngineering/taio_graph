# TAIO-Graph - Graph analysis program

---

## How to run
Download project: git clone <repo-url.git>. 

Open Project folder in Visual Studio, open CMakeLists.txt and press Ctrl + S.
Then select taio_graph.exe project and compile & run it.
After creating exe file you can run it from Visual Studio or you can 
open taio_graph.exe file in taio_graph/out/build/x64-Debug folder.  
  
If you want to move taio_graph.exe file to other location you also need to copy resources folder to the same location where executable file was moved.  

By default project is compiled in a release mode.  
// TODO: remember to change CMakeLists.txt to PRODUCTION when finished

---

## Project intentions
For our group project in the Theory of Algorithms and Computations course, we aim to develop highly optimized algorithms for solutions and reasonably accurate approximations of solutions in heuristic-based algorithms. 
We also focus on maintaining well-structured and well-documented code for improved readability.

---

## Description
// TODO: description for all algorithms  

### Finding smallest graph extention to Hamilton cycle approximation  
This algorithm follows random path to unvisited nods. 
When there is no more out-edges a new one is created. 
That egde will lead to the node with least in-edges. 
Random path creation is continued from newly connected node.  
  
Path creation described above is run `max(graph metric, 1) * retry_factor` times. 
From each try number of added edges is compared and only the smallest graph extention is saved.  
  
In addition, after finding smallest graph extention a number of unique hamilton cycles in extended graph is computed. 
It's also an approximation and cycles are also found by following random paths. 
Paths that have desired lenght and the last node has out-edge to start node are added as unique cycle for the smallest graph extention that was found previously. 
  
### Input format

The input data will be saved in a format common to all teams, ensuring data interchangeability between teams.
These will be text files containing descriptions of one or more graphs, separated by an empty line.
The first line of the file contains the number of graphs in the file. Starting from the second line, the graph descriptions are as follows:  
- The first line of the description contains the number of vertices, recorded on a single line.  
- The following lines contain rows of the adjacency matrix with elements separated by spaces.
- Its recommended to put only 1 graph in 1 file for guaranteed interchangeability between teams.
  
The adjacency matrix records the number of edges between vertices (either 0 or 1).
For undirected graphs, the adjacency matrix will be symmetrical along the main diagonal.

### Example input file (data.txt)
```
3
3
0 1 1
0 0 0
1 0 0

2
0 1
1 0

3
0 1 1
0 0 1
1 0 0
```
---

## Showcase video  
// TODO: video  
![Screenshot 2024-10-29 224418](https://github.com/user-attachments/assets/91ba5ba0-abec-436d-87a4-aecce921dc06)


---

### Source links
Repository social media graphics:  
https://media.architecturaldigest.com/photos/60537a17b05b9ecdd8227ff2/2:1/w_1280%2Cc_limit/Virtual_Networking_FINAL.jpg    
