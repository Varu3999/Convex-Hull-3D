# Convex Hull 3D (Incremental algorithum) ![java:v8](https://img.shields.io/badge/C++-14-brightgreen.svg)

This contains a C++ code which computes the points of 3D Convex Hull using incremental convex hull algorithm.

# How to Use

**Download the project in the directory you want**

```sh
$ git clone https://github.com/Varu3999/Convex-Hull-3D
$ cd Convex-Hull-3D
$ g++ Solution.cpp
```

**Edit the Input file**
Open CONVEX.IN using any text editor and give the input in the form:
1) The first line should contain the total number of points(n)
2) Then each subsequent n lines should contain coordinates of the points separated by spaces.

**Run**

```sh
$ ./a.out
```
You can check the output in the CONVEX.OUT file

# Algorithm
**Incremental convex hull**
The basic idea of the algorithm is to first make a convex hull using any number of points such that it can be easily made. Then iterate through rest of the points one by one and update the hull to accommodate them.

# Computation
We have defined a structure vector3 which can represent any type of 3D vector and perform cross product, dot product and subtraction. Another structure face is used to store a face in the form of 3 corner coordinates and the normal of the plane containing the face. We have created one more structure pair_index to store a map between an edge and pair of points that form faces with that edge on the hull.
We are storing the faces of the convex hull formed by points that have been considered 

1) The first step was to form a simple convex hull using the first 4 points and storing all the 4 faces in the hull faces.
2) We go to the next point.
3) We then iterate through all the faces and check if the point is visible from that plane.
4) A point is visible from a plane if the point does not lie on the same side of the plane as the other points in the hull lie.
5) If the point is visible then that face is deleted and the information of that face is also deleted from the edge map.
6) Now after deleting all the faces from which the point was visible we have some edges which have exactly one faces connected to them in the convex hull.
7) But as we know for a complete convex hull all the edges of the hull must have 2 faces connected to them.
8) So we iterate through all the edges of the faces contained in hull faces and if an edge map to only one face that means it is a naked edge and so we create a new face using the new point and that edge and add it to the hull faces
9) We repeat step 2 - 8 for all the point and finally, all the faces of the hull are stored in hull faces.
10) We then iterate through all the points and if the point lies on any of the hull faces then it is included in the answer.
