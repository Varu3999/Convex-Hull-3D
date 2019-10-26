#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <float.h>
#include <iomanip>

using namespace std;

string inputfile = "CONVEX.IN";
string outputfile = "CONVEX.OUT";

// struct to represent a vector in 3D
struct vector3 {
    // x.y and z coordinates
    float x, y, z;
    
    // Constructors
    vector3(float a , float b , float c){
        x = a;
        y = b;
        z = c;
    }
    vector3(){
        x = -1;
        y = -1;
        z = -1;
    }
    
    // Cross product
    vector3 cross(vector3 v){
        return vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    
    // Subtraction
    vector3 sub(vector3 v){
        return vector3(x - v.x, y - v.y, z - v.z);
    }
    
    // Dot product
    float dot(vector3 v){
        return x * v.x + y * v.y + z * v.z;
    }
    
};

// Contains all points
vector<vector3> all_points;


// struct to represent a face of hull
struct face{
    
    // normal vector of the plane
    vector3 normal;
    
    
    float dfo;
    
    // corner point index
    int c1 , c2 , c3;
    
    void setNormal(vector3 nor){
        normal = nor;
    }
    
    // Constructor
    face(int p1, int p2, int p3, int mid){
        c1 = p1;
        c2 = p2;
        c3 = p3;
        
        
        // finding normal using (P2-P1) X (P3-P1)
        setNormal(all_points[p2].sub(all_points[p1]).cross(all_points[p3].sub(all_points[p1])));
        dfo = normal.dot(all_points[p1]);
        
        // Correcting the direction of normal
        if(normal.dot(all_points[mid]) > dfo){
            normal.x *= -1;
            normal.y *= -1;
            normal.z *= -1;
        }
    }
    face(){
        int a = 0;
    }
};

// Contains all the faces of the hull
vector<face> hull_faces;

// struct to store a pair of index
struct pair_index{
    
    int i1, i2;
    
    // Constructor
    pair_index(){
        i1 = -1;
        i2 = -1;
    }
    
    // Inserting a new index
    void insert(int a){
        if(i1 == -1){
            i1 = a;
        }else{
            i2 = a;
        }
    }
    
    // Removing an Index
    void remove(int a){
        if(i1 == a){
            i1 = -1;
        }
        else if(i2 == a){
            i2 = -1;
        }
    }
    
    // Number of valid index
    int num_ver(){
        int a = 0;
        if(i1 != -1){
            a++;
        }
        if(i2 != -1){
            a++;
        }
        return a;
    }
};

// edge_to_face[i][j] store the vertices to form the plane on hull using edge i, j
pair_index edge_to_face[10001][10001];

// add a new face to convex hull and updating edge_to_face
face add_face(int p1, int p2, int p3, int mid){
    edge_to_face[p1][p2].insert(p3);
    edge_to_face[p2][p1].insert(p3);
    edge_to_face[p2][p3].insert(p1);
    edge_to_face[p3][p2].insert(p1);
    edge_to_face[p1][p3].insert(p2);
    edge_to_face[p3][p1].insert(p2);
    
    face newf(p1, p2, p3, mid);
    return newf;
}


// Reads the input file and initializes the all_points vector with the initial points, returns the number of points
int getPoints(string file_name){
    fstream file;
    file.open(file_name);
    string word;
    
    file >> word;
    int n = stoi(word);
    
    for(int i = 0 ; i < n ; i ++){
        float a, b, c;
        
        file >> word;
        a = stof(word);
        
        file >> word;
        b = stof(word);
        
        file >> word;
        c = stof(word);
        
        vector3 point(a,b,c);
        all_points.push_back(point);
    }
    file.close();
    return n;
}

// create an initial hull
void hullInitial(int n){
    memset(edge_to_face, -1, sizeof(edge_to_face));

    // hull from first 4 points
    for(int i = 0 ; i < 4 ; i++){
        for(int j = i + 1 ; j < 4 ; j++){
            for(int k = j + 1 ; k < 4 ; k++){
                hull_faces.push_back(add_face(i, j, k, 6 - i - j - k));
            }
        }
    }
    
}

int main(){
    // get the points from the inputfile
    int n = getPoints(inputfile);
    
    // Contruct initial hull
    hullInitial(n);

    
    
    // increment the hull by including one point at a time
    for(int i = 4 ; i < n ; i ++){
        
        // Delete the face which lie inside the updated hull
        for(int j = 0 ; j < hull_faces.size() ; j++){
            face f = hull_faces[j];
            
            if(f.normal.dot(all_points[i]) > f.dfo){
                edge_to_face[f.c1][f.c2].remove(f.c3);
                edge_to_face[f.c2][f.c1].remove(f.c3);
                edge_to_face[f.c1][f.c3].remove(f.c2);
                edge_to_face[f.c3][f.c1].remove(f.c2);
                edge_to_face[f.c3][f.c2].remove(f.c1);
                edge_to_face[f.c2][f.c3].remove(f.c1);
                hull_faces[j--] = hull_faces.back();
                hull_faces.resize(hull_faces.size() - 1);
                
            }
        }
        
        // make new faces by using the naked edges
        int num_f = hull_faces.size();
        for(int j = 0 ; j < num_f ; j++){
            face f = hull_faces[j];
            if(edge_to_face[f.c1][f.c2].num_ver() != 2){
                hull_faces.push_back(add_face(f.c1, f.c2, i, f.c3));
                
            }
            if(edge_to_face[f.c1][f.c3].num_ver() != 2){
                hull_faces.push_back(add_face(f.c1, f.c3, i, f.c2));
                
            }
            if(edge_to_face[f.c2][f.c3].num_ver() != 2){
                hull_faces.push_back(add_face(f.c2, f.c3, i, f.c1));
                
            }
        }
    }

    // Contains the final hull points
    vector<vector3> hull_points;
    
    // Itterating through each point and checking if it lies on any face of the hull
    for(int i = 0; i < n; i++) {
        vector3 v;
        v = all_points[i];
        for(int j = 0; j < hull_faces.size(); j++) {
            float d = hull_faces[j].dfo - hull_faces[j].normal.dot(v);
            if(d == 0){
                hull_points.push_back(all_points[i]);
                break;
            }
        }
    }
    
    // Contains the final hull size
    int hull_size = hull_points.size();
   
    // writing to the output file
    fstream file;
    
    file.open(outputfile,ios::out);
    file << hull_size << "\n";
    for(int i = 0 ; i < hull_size ; i ++){
        file << (float)hull_points[i].x << " " << hull_points[i].y << " " << hull_points[i].z << " " << "\n";
    }
    return 0;
}
