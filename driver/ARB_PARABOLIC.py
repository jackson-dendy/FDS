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
equations = selection.ARB_PARABOLIC

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




mesh_instance.d3_structure_mesh_gen([0, 10], [0, 8], [0, 70], bound_cond)

test_instance = Mesh_Solver(mesh_instance, True)


print("\nBegin Solver Phase\n")
h = 10
dt = 0.01
alpha = 289

r = alpha * dt / h**2

values = [-1, -r, -r, 1+4*r , -r, -r, 1]

# Solve it
Mesh = test_instance.solve(equations, values)

Mesh = Mesh.get_mesh_3d()

print("Solution Found")

animated_heat_plot(Mesh, 100)