import sys
from pathlib import Path
path_root = Path(__file__).parents[1]
root = str(path_root)
sys.path.insert(1, f'{root}//build//Release')
sys.path.insert(2, f'{root}//driver')

from btcs  import Mesh_Solver, Mesh, selection 
from utility import animated_heat_plot

print("\nBegin Meshing Phase\n")

# Equation Selection
equations = selection.ARB

# Mesh Generation
mesh_instance = Mesh()



bound_cond= [ 
              100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
              100, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              100, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
              100, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              100, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              100, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              100, 100, 100, 100, 100, 100, 100, 100, 100, 100]

bound_cond = [3, 3, 3,
              3, 0, 0,
              3, 3, 3]




mesh_instance.d3_structure_mesh_gen([0, 3], [0, 3], [0, 6], bound_cond)

test_instance = Mesh_Solver(mesh_instance, False)


print("\nBegin Solver Phase\n")
h = 10
dt = 0.01
alpha = 289

r = (dt*alpha)/h**2

values = [-1 , -r, -r, 1+4*r, -r, -r, 1]

# Solve it
Mesh = test_instance.solve(equations, values)

Mesh = Mesh.get_mesh_3d()

print("Solution Found")

animated_heat_plot(Mesh, 500)