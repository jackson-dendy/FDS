import sys
from pathlib import Path
path_root = Path(__file__).parents[1]
root = str(path_root)
sys.path.insert(1, f'{root}//build//Release')
sys.path.insert(2, f'{root}//driver')

from btcs  import Mesh_Solver, Mesh, selection 
from utility import wave_plot

print("\nBegin Meshing Phase\n")

# Equation Selection
equations = selection.ARB_HYPERBOLIC

# Mesh Generation
mesh_instance = Mesh()



bound_cond= [ 
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 
              0, 0, 0, 0, 3, 3, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0,]

              



mesh_instance.d3_structure_mesh_gen([0,10], [0, 8], [0, 30], bound_cond, bound_cond)

test_instance = Mesh_Solver(mesh_instance, True)


print("\nBegin Solver Phase\n")
h = 1
dt = 0.01
c = 10

if not(c*dt/h < 1):
    raise ValueError("Current Simulation is Unstable")
    

r = dt**2 * c**2 / h**2

values = [1, -r, -r, 4*r-2, -r, -r, 1]

# Solve it
Mesh = test_instance.solve(equations, values)

Mesh = Mesh.get_mesh_3d()

print("Solution Found")

wave_plot(Mesh, 100)