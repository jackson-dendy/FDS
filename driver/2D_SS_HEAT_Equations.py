import sys
from pathlib import Path
path_root = Path(__file__).parents[1]
root = str(path_root)
sys.path.insert(1, f'{root}//build//Release')
sys.path.insert(2, f'{root}//driver')

import random
import matplotlib.pyplot as plt

from btcs  import Mesh_Solver, Mesh, selection 
from utility import heat_plot

print("\nBegin Meshing Phase\n")

# Equation Selection
equations = selection.D2_SS_HEAT

# Mesh Generation
mesh_instance = Mesh()
size = 10

bound_cond= [ 
              100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
              100, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              100, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
              100, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              100, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              100, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              100, 100, 100, 100, 100, 100, 100, 100, 100, 100]

mesh_instance.d2_strucutre_mesh_gen([0, 10], [0, 8] , bound_cond)

test_instance = Mesh_Solver(mesh_instance, True)


print("\nBegin Solver Phase\n")

# Solve it
Mesh = test_instance.solve(equations, [1])

print(Mesh.size())
Mesh = Mesh.get_mesh_2d()

print("Solution Found")


heat_plot(Mesh)