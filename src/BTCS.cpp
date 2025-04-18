#include "FDS.hpp"

Mesh_Solver::Mesh_Solver(Mesh &mesh, bool verbose){
    this -> verbose = verbose;
    this -> active_mesh = mesh;
    
    //std::cout << active_mesh.get_size();
};

Mesh& Mesh_Solver::glizzinator(int &selection, std::vector<double> values)
{
    // 3-Var Finite Difference Solver

    // Not needed rn because it does not march through time
    /*std::vector<double> t_array;
    for(double i = t0; i > tf; i += h){
        t_array.push_back(i);
    }*/

    // rearrange mesh into 1D mesh
    int rows = active_mesh.rows;
    int cols = active_mesh.cols;
    int slices = active_mesh.slices;
    int size_mesh = active_mesh.size;
    std::cout << "Slices X Rows X Cols: " << slices <<", "<<rows<<", "<<cols<<"\n";
    FDS_eqs equations = FDS_eqs(selection);

    
    // Display Mesh
    if(verbose){
    std::cout << "Mesh Display: " << rows << " X " << cols << "\n";

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            std::cout << 0 << " ";
        }

        std::cout << "\n";
    }

    }

    // A Must be inverted and multiplied with b
    auto* a = new Matrix<double>(size_mesh, size_mesh, true);
    auto* b = new double[size_mesh];
    auto* sol = new double[size_mesh];

    // Set zeros 
    a->fillZeros();
    
    for(int i = 0; i < size_mesh; i++){
        b[i] = 0;
    }

    if(verbose){
        std::cout << "Matrix Space Allocated" << "\n";
    }

    // Set Boundary Conds
    int active_diag = 0;
    struct node active_node;
    for(int i = 0; i < size_mesh; i++){
        
        active_node = active_mesh.get_node(i);
        
        if(active_node.isboundary){
           
            if (active_node.state == NAN){
                b[i] = 0;
            }
            else{
                b[i] = active_node.state;
            }
            a->values[active_diag*size_mesh+active_diag] = 1;
            
        }
        active_diag++;
    }

    if(verbose){
        std::cout << "Boundary Conditions Set" << "\n";
    }

    // Set unkown Coef - in current position and returns the
    for(int i = 0; i < size_mesh; i++){
        
        if(active_mesh.get_node(i).isboundary){
            continue;
        }

        else{
            std::vector<int> ind = {i};
            equations.FDS_call(a, active_mesh, ind, values);
        }
        }

    if(verbose){
        std::cout << "Unknowns Mapped to Matrix" << "\n";
    }

    
    // Solve PDE or ODE
    if(verbose){
        std::cout << "Solving Mesh" << "\n";
    }
    int nnzs = a->countNonZeros();

    if(nnzs/pow(size_mesh,2) < 0.5){
        
        a->luSolve(b, sol);

    }
    else{
    auto* sparse_a = new CSRMatrix<double>(size_mesh, size_mesh, nnzs, true);
    sparse_a->dense2csr(*a);
    //a->printMatrix();
    sparse_a->SOR(b, sol, 1.2, 0.001);
    }

    if(verbose){
        std::cout << "Mesh Solved" << "\n";
    }
    
    for(int i =0; i<size_mesh;i++){
        node new_node;
        new_node = active_mesh.get_node(i);
        new_node.state = sol[i];
        active_mesh.set_node(new_node, i);
    }
    

    if(verbose){
        std::cout << "Mesh Remapped to 2D" << "\n";  
        
            
    }

    active_mesh.disp_mesh(3);
    
    return active_mesh;

    delete[] b;
    delete a;
    delete[] sol;

};

Mesh Mesh_Solver::get_sol(){
    // Return the Mesh to python side

    return active_mesh;
}

void Mesh::d2_structure_mesh_gen(std::vector<int> &x_range, std::vector<int> &y_range, std::vector<double> &boundary_cond){
    // Generate a rectangle 2d mesh

    // Find size of struture mesh
    cols = abs(x_range[1] - x_range[0]);
    rows = abs(y_range[1] - y_range[0]);
    slices = 1;
    size = cols*rows;

    if(verbose){
        std::cout << "Constructing Initial " << rows << " X " << cols << " Mesh" << "\n";
    }

    
    // Iterate and create mesh
    struct node active_node;
    d3_mesh.resize(1, d2_vector(rows, d1_vector(cols, active_node)));
    d2_mesh.resize(rows, d1_vector(cols, active_node));
    d1_mesh.resize(size);

    int lazy_val = 0;
    
    // Built from 
    loading_bar l(rows, "Mesh");
    for(int i = 0; i < rows ; i++){
        
        for(int j = 0; j < cols; j++){
            

            // Is the mesh on a boundary
            if (i == 0 || j == 0 || j == cols-1 || i == rows-1){
                d2_mesh.at(i).at(j).isboundary = true;
                
                d2_mesh.at(i).at(j).state = boundary_cond.at(i*cols + j);
            }

            else{
                d2_mesh.at(i).at(j).isboundary = false;
                d2_mesh.at(i).at(j).state = 0;
            }

            d2_mesh.at(i).at(j).position = {i,j};
            d1_mesh.at(lazy_val) = d2_mesh.at(i).at(j);
            lazy_val++;
            
            
            
        }
        
        
        l.step_bar();
    }

    d2_to_d3();

    
    
    
    if(verbose){
        std::cout << "\n" << "Mesh Created" << "\n";
    }
    

    
};

void Mesh::d3_structure_mesh_gen(std::vector<int> &x_range, std::vector<int> &y_range, std::vector<int> &z_range, std::vector<double> &boundary_cond){
    // Generate a rectangular prism 3d mesh (btcs)

    // Find size of struture mesh
    cols = abs(x_range[1] - x_range[0]);
    rows = abs(y_range[1] - y_range[0]);
    slices = abs(z_range[0] - z_range[1]);
    size = cols*rows*slices;

    if(verbose){
        std::cout << "Constructing Initial " << rows << " X " << cols << " X " <<  slices << " Mesh" << "\n";
    }

    
    // Iterate and create mesh
    struct node active_node;
    d3_mesh.resize(slices, d2_vector(rows, d1_vector(cols, active_node)));
    d2_mesh.resize(rows, d1_vector(cols, active_node));
    d1_mesh.resize(size);

    int lazy_val = 0;
    
    // Built from 
    loading_bar l(slices, "Mesh");
    for(int k = 0; k <slices; k++){
        for(int i = 0; i < rows ; i++){
            
            for(int j = 0; j < cols; j++){
                

                // Is the mesh on a boundary
                if (i == 0 || j == 0 || j == cols-1 || i == rows-1 || k == 0){

                    d3_mesh.at(k).at(i).at(j).isboundary = true;
                    
                    d3_mesh.at(k).at(i).at(j).state = boundary_cond.at(i*cols + j);
                    


                }

                else{
                    d3_mesh.at(k).at(i).at(j).isboundary = false;
                    
                    d3_mesh.at(k).at(i).at(j).state = 0;
                }

                d3_mesh.at(k).at(i).at(j).position = {i,j,k};                
            }
            
            
        }

        l.step_bar();
    }

    

    d3_to_d1();
    d1_to_d2();
    
    if(verbose){
        std::cout << "\n" << "Mesh Created" << "\n";
    }
    
};

void Mesh::d3_to_d1(){
    // Update the 2d Mesh to match the 3d Mesh

    for(int k=0; k < slices ; k++){
        for(int i=0; i< rows; i++){
            for(int j =0; j<cols;j++){
                d1_mesh.at(i*cols+j+k*(cols*rows)) = d3_mesh.at(k).at(i).at(j);
                
            }
        }
    }

};

void Mesh::d2_to_d3(){
    // Update the 3d Mesh to match the 2d Mesh

    struct node active_node;
    d2_vector new_slice(rows, d1_vector(cols, active_node));


    for(int k = 0; k < slices ; k++){
        for(int i = 0; i<rows; i++){
            for(int j = 0; j < cols; j++){
                new_slice.at(i).at(j) = d2_mesh.at(i+rows*k).at(j);
            }

        d3_mesh[k] = new_slice;
        
    }
}



};

void Mesh::d2_to_d1(){
    // Update the 1d Mesh to match the 2D Mesh 
    
    
    for(int i = 0; i < rows*slices ; i++){
    
        for(int j = 0; j < cols; j++){
            d1_mesh[cols*i + j] = d2_mesh.at(i).at(j);
        }

        
    }

    
};

void Mesh::d1_to_d2(){
    // Update the d2 mesh to match the d1 mesh

    d2_mesh.resize(slices * rows, d1_vector(cols));

    for (int z = 0; z < slices; ++z) {
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                int flat_row = z * rows + y;
                int index = z * rows * cols + y * cols + x;
                d2_mesh[flat_row][x] = d1_mesh[index];
            }
        }
    }
};

void Mesh::set_mesh(d1_vector &mesh){
    // Setter Used for 1D mesh

    d1_mesh = mesh;
    d1_to_d2();
    d2_to_d3();

};

void Mesh::set_mesh(d2_vector &mesh){
    // Setter Used for 2D mesh
    d2_mesh = mesh;
    d2_to_d1();
    d2_to_d3();
};

void Mesh::set_mesh(d3_vector &mesh){
    // Setter Used for 3D mesh
    d3_mesh = mesh;
    d3_to_d1();
    d1_to_d2();

};

void Mesh::set_node(struct node value, int index){
    // Setter for a node with 1d indexing

    d1_mesh.at(index) = value; 
    d1_to_d2();
    d2_to_d3();
};

void Mesh::set_node(struct node value, int index1, int index2){
    // Setter for a node with 2d indexing
    d2_mesh.at(index1).at(index2) = value; 
    d2_to_d1();
    d2_to_d3();
};

void Mesh::set_node(struct node value, int index1, int index2, int index3){
    // Setter for a node with 3d indexing
    d3_mesh.at(index3).at(index1).at(index2) = value;
    d3_to_d1();
    d1_to_d2();

};

struct node Mesh::get_node(int index){
    // Getter for a node with 1d indexing

    return d1_mesh.at(index);
};

struct node Mesh::get_node(int index1, int index2){
    // Getter for a node with 2d indexing
    return d2_mesh.at(index1)[index2];
};

struct node Mesh::get_node(int index1, int index2, int index3){
    // Getter for a node with 3d indexing
    return d3_mesh.at(index3).at(index1).at(index2);

}

void Mesh::disp_mesh(int dimension){
    // Display Current Mesh

    switch(dimension){
        case 1:
            std::cout << "\n1D Mesh: \n---------\n";
            for(int i =0; i< size; i++){
                std::cout << get_node(i).state << '\n';
            }
            break;
        case 2:
        std::cout << "\n2D Mesh: \n---------\n";
            for(int i =0; i< rows*slices; i++){
                for(int j = 0; j<cols;j++){
                std::cout << get_node(i, j).state << ' ';
                }
                std::cout<<'\n';
            }

            break;

        case 3:
        std::cout << "\n3D Mesh: \n---------\n";
            for(int k=0; k<slices;k++){
                std::cout << "\nSlice: " << k << "\n";
                for(int i =0; i< rows; i++){

                    for(int j = 0; j<cols;j++){
                    std::cout << get_node(i, j, k).state << ' ';
                    }
                    std::cout << "\n";
                }
            }
            break;
    }

};

int Mesh::get_size(){
    // Get size as in how many nodes are in mesh
    // Also checks that each mesh matchs
    int d1_size;
    int d2_size;
    int d3_size;
    
    d1_size = d1_mesh.size();
    
    d2_size = d2_mesh.size() * d2_mesh.at(0).size();

    d3_size = d3_mesh.size() * d3_mesh.at(0).size() * d3_mesh.at(0).at(0).size();

    if((d1_size != d2_size) || (d1_size != d3_size) || (d2_size != d3_size)){

        std::cout << "Error Array Allocation is Incorrect d1: " << d1_size << ", d2: " << d2_size << ", d3: " << d3_size;
    }
    
    return d1_size;
};

std::vector<std::vector<double>> Mesh::get_mesh_2d(){
    // Returns the Mesh states to python in a list of lists

    std::vector<double> new_row;
    std::vector<std::vector<double>> state;
    new_row.resize(cols, 0);
    state.resize(rows, new_row);
    for(int i = 0; i < rows; i++){
        std::vector<double> new_row;
        for(int j = 0; j < cols;j++){
            
            state.at(i).at(j) = get_node(i, j).state;
        }

        

    }

    return state;
};

std::vector<std::vector<std::vector<double>>> Mesh::get_mesh_3d(){
    // Returns the Mesh states to python in a list of lists of lists
    std::vector<double> new_row;
    std::vector<std::vector<double>> new_slice;
    std::vector<std::vector<std::vector<double>>> state;

    new_row.resize(cols, 0);
    new_slice.resize(rows, new_row);
    state.resize(slices, new_slice);
    for(int k =0; k<slices;k++){
        std::vector<std::vector<double>> new_slice;
        for(int i = 0; i < rows; i++){
            std::vector<double> new_row;
            for(int j = 0; j < cols;j++){
                
                state.at(k).at(i).at(j) = get_node(i, j, k).state;
            }

        }
    }

    return state;
};