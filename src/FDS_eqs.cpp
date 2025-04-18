#include "FDS.hpp"

FDS_eqs::FDS_eqs(int selection){
    // Constructor for FDS_eqs

    this -> selection = selection;
};

void FDS_eqs::FDS_call(Matrix<double>* a, Mesh &mesh, std::vector<int> ind, std::vector<double> vals){
    // Used to call the FDE and edit the coefficient Matrix
    
    switch(selection){

        case 0:
            
            d2_ss_heat_equations(a, mesh, ind);
            break;

        case 1:
            d2_trans_heat_equations(a, mesh, ind, vals);
            break;
            
        case 2:
            arb_stencil(a, mesh, ind, vals);
            break;
    };
};

void FDS_eqs::d2_ss_heat_equations(Matrix<double>* a, Mesh &mesh, std::vector<int> ind){

    // 2d steady-state heat equations:       [     1    ]
    //                                delH = [ 1  -4  1 ]
    //                                       [     1    ]
    // 0 = u(x+h, y) + u(x-h, y) + u(x, y+h) + u(x, y-h) - 4(x, y)

    int i = ind.at(0);

    int size_mesh = mesh.get_size();
    int mat_index = i*size_mesh + i;

    a->values[mat_index] = -4; // u(x,y)
    a->values[mat_index+1] = 1; // u(x+h,y)
    a->values[mat_index -1 ] = 1; // u(x-h,y)
    a->values[mat_index+mesh.cols] = 1; // u(x, y+h)
    a->values[mat_index-mesh.cols] = 1; // u(x, y-h)
    
    
    };

    void FDS_eqs::d2_trans_heat_equations(Matrix<double>* a, Mesh &mesh, std::vector<int> ind, std::vector<double> vals){
    // 2d transient heat equations:         [     ] [     -r      ] [     ]
    //                                delH =[  1  ] [ -r  2-4r -r ] [  0  ]
    //                                      [     ] [     -r      ] [     ]
    // r = 2*delt*alpha/h^2
    //
    // 0 = u(x, y, t+h) + u(x, y, t-h) + (2-r)*u(x, y, t) -r(u(x, y+h, t) + u(x, y-h, t) + u(x+h, y, t) + u(x-h, y, t))

    double h = vals.at(0);
    double delt = vals.at(1);
    double alpha = vals.at(2);

    double r = delt*alpha/pow(h,2);

    int i = ind.at(0);
    int size_mesh = mesh.get_size();
    int mat_index = i*size_mesh + i;
    
    
    a->values[mat_index] = 1+4*r; // u(x,y,t)
    a->values[mat_index-mesh.cols*mesh.rows] = -1; // u(x,y,t-h)
    a->values[mat_index+1] = -r; // u(x+h,y,t)
    a->values[mat_index-1] = -r; // u(x-h,y,t)
    a->values[mat_index+mesh.cols] = -r; // u(x,y+h,t)
    a->values[mat_index-mesh.cols] = -r; // u(x,y-h,t)

    }

    void FDS_eqs::arb_stencil(Matrix<double>* a, Mesh &mesh, std::vector<int> ind, std::vector<double> vals){
        // Intake Arbitary 3d Finite difference stencils indexs a 1d vector in this way BTCS
        //                                      [           ] [          vals[1]         ] [     ]
        //                                delH =[  vals[0]  ] [ vals[2]  vals[3] vals[4] ] [  0  ]
        //                                      [           ] [          vals[5]         ] [     ]

    
        int i = ind.at(0);
        int size_mesh = mesh.get_size();
        int mat_index = i*size_mesh + i;
        
        
        a->values[mat_index] = vals.at(3); // u(x,y,t)
        a->values[mat_index-mesh.cols*mesh.rows] = vals.at(0); // u(x,y,t-h)
        a->values[mat_index+1] = vals.at(4); // u(x+h,y,t)
        a->values[mat_index-1] = vals.at(2); // u(x-h,y,t)
        a->values[mat_index+mesh.cols] = vals.at(1); // u(x,y+h,t)
        a->values[mat_index-mesh.cols] = vals.at(5); // u(x,y-h,t)
    
        }