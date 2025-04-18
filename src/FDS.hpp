#pragma once
#include <math.h>
#include <iostream>
#include <ctime>
#include <memory>
#include <cmath>
#include <vector>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "utility/Matrix.hpp"
#include "utility/CSRMatrix.hpp"
#include "utility/Matrix.cpp"
#include "utility/CSRMatrix.cpp"
#include "utility/utility.hpp"

#define d3_vector std::vector<std::vector<std::vector<struct node>>>
#define d2_vector std::vector<std::vector<struct node>>
#define d1_vector std::vector<struct node>

class Mesh{

    public:

    // Getters and Setters
    struct node get_node(int index);
    struct node get_node(int index1, int index2);
    struct node get_node(int index1, int index2, int index3);

    void set_mesh(d1_vector &mesh);
    void set_mesh(d2_vector &mesh);
    void set_mesh(d3_vector &mesh);

    void set_node(struct node value, int index1);
    void set_node(struct node value , int index1, int index2);
    void set_node(struct node value, int index1, int index2, int index3);

    std::vector<std::vector<double>> get_mesh_2d();
    std::vector<std::vector<std::vector<double>>> get_mesh_3d();

    // Display Mesh
    void disp_mesh(int diminsion);

    // Size check
    int get_size();

    // 2D mesh generator acts as a constructor
    void d2_structure_mesh_gen(std::vector<int> &x_range, std::vector<int> &y_range, std::vector<double> &boundary_cond);
    void d3_structure_mesh_gen(std::vector<int> &x_range, std::vector<int> &y_range, std::vector<int> &z_range, std::vector<double> &boundary_cond);

    // Mesh Poperties
    bool verbose = true;
    int rows;
    int cols;
    int slices;
    int size;
   

    private:

    // Different Meshs
    d1_vector d1_mesh;
    d2_vector d2_mesh;
    d3_vector d3_mesh;

    // Mesh Converters
    void d3_to_d1();
    void d2_to_d3();
    void d2_to_d1();
    void d1_to_d2();
};

class Mesh_Solver{

    public:

    //Constructor
    Mesh_Solver(Mesh &active_mesh, bool verbose);

    // Solver for Finite Difference Method
    Mesh& glizzinator(int &selection, std::vector<double> values); 

    // Return the active mesh
    Mesh get_sol();

    private:

    Mesh active_mesh;
    bool verbose;
};


struct node{
    // information conatined ata single point in the mesh

    double state;
    std::vector<int> position;
    bool isboundary;

};


enum eq_selection{
    // Enum for which equations to solve
    D2_SS_HEAT,
    D2_TRANS_HEAT,
    ARB
};

class FDS_eqs{
    // Class which contains FDEs

    public:

    // Constructor 
    FDS_eqs(int selection);

    // Calls Equations
    void FDS_call(Matrix<double>* a, Mesh &mesh, std::vector<int> ind, std::vector<double> vals);

    private:

    int selection;
    
    //  FDE Equations 
    void d2_ss_heat_equations(Matrix<double>* a, Mesh &mesh, std::vector<int> ind);  
    void d2_trans_heat_equations(Matrix<double>* a, Mesh &mesh, std::vector<int> ind, std::vector<double> vals);  
    void arb_stencil(Matrix<double>* a, Mesh &mesh, std::vector<int> ind, std::vector<double> vals);
};
