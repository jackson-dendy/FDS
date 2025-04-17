import sys
from pathlib import Path
path_root = Path(__file__).parents[1]
root = str(path_root)
sys.path.insert(1, f'{root}//build//Release')

import random
import matplotlib.pyplot as plt
import matplotlib.animation as animation

from btcs  import Mesh_Solver, Mesh, selection 

print("\nBegin Meshing Phase\n")

# Equation Selection
equations = selection.D2_TRANS_HEAT

# Mesh Generation
mesh_instance = Mesh()
size = 10


bound_cond= [ 
              200, 200, 60, 70, 20, 20, 20, 20, 20, 20, 
              100, 20, 20, 20, 20, 20, 20, 20, 20, 0, 
              80, 20, 20, 20, 20, 20, 20, 20, 20, 0, 
              80, 20, 20, 20, 20, 20, 20, 20, 20, 0, 
              200, 100, 100, 30, 25, 100, 20, 20, 20, 0]




mesh_instance.d3_structure_mesh_gen([0, 10], [0, 5], [0, 20], bound_cond)

test_instance = Mesh_Solver(mesh_instance, True)


print("\nBegin Solver Phase\n")

# Solve it
Mesh = test_instance.solve(equations, 0, 4, 0.001, [0.0001])

Mesh = Mesh.get_mesh_3d()

print("Solution Found")



# Set up figure and axis
fig, ax = plt.subplots()

# Initial data for heatmap (10x10 grid)
heatmap = ax.imshow(Mesh[0], cmap='turbo', interpolation='bilinear')


# Animation update function
def update(frame):
      
    heatmap.set_data(Mesh[frame])
    
    return [heatmap]

# Create animation
ani = animation.FuncAnimation(fig, update, frames=len(Mesh), interval=500)

plt.title("Animated Heatmap")
plt.colorbar(heatmap)
plt.show()
