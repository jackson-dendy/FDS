#include "FDS.hpp"
#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;

// Macro for pybind11 module to translate between Cpp and python
PYBIND11_MODULE(btcs, m) {
    m.doc() = "Finite Difference Solver";

    
    py::class_<struct node>(m, "node")
        .def(py::init<>())
        .def_readwrite("state", &node::state)
        .def_readwrite("position", &node::position)
        .def_readwrite("isboundary", &node::isboundary);
    
    py::class_<Mesh_Solver>(m, "Mesh_Solver")
        .def(py::init<Mesh &, bool>())
        .def("get_sol", &Mesh_Solver::get_sol, py::return_value_policy::reference)
        .def("solve", &Mesh_Solver::glizzinator, py::return_value_policy::reference);

    
    py::class_<Mesh>(m, "Mesh")
        .def(py::init<>())
        .def("size", &Mesh::get_size)
        .def("get_mesh_2d", &Mesh::get_mesh_2d, py::return_value_policy::reference)
        .def("get_mesh_3d", &Mesh::get_mesh_3d, py::return_value_policy::reference)
        .def("d3_structure_mesh_gen", &Mesh::d3_structure_mesh_gen)
        .def("d2_strucutre_mesh_gen", &Mesh::d2_structure_mesh_gen);

    py::enum_<eq_selection>(m, "selection")
        .value("D2_SS_HEAT", eq_selection::D2_SS_HEAT)
        .value("D2_TRANS_HEAT", eq_selection::D2_TRANS_HEAT)
        .value("ARB", eq_selection::ARB);
        

    m.attr("__version__") = "0.0.1";

    
};