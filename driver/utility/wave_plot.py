import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D
from scipy.ndimage import zoom

def smooth_frame(Z, scale=3):
    return zoom(Z, scale, order=3)  # cubic interpolation

def wave_plot(mesh, interval, smooth_scale=3):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    mesh = np.array(mesh)
    # Smooth first frame
    Z0 = smooth_frame(np.array(mesh[0]), scale=smooth_scale)
    rows, cols = Z0.shape
    x = np.linspace(0, 1, cols)
    y = np.linspace(0, 1, rows)
    X, Y = np.meshgrid(x, y)

    # Initial surface
    surf = ax.plot_surface(X, Y, Z0, cmap='coolwarm')

    def update(frame):
        ax.clear()
        Z = smooth_frame(np.array(mesh[frame]), scale=smooth_scale)
        ax.set_zlim(np.min(Z0), np.max(Z0))  # consistent z-axis
        ax.set_title("2D Wave (Smoothed)")
        surf = ax.plot_surface(X, Y, Z, cmap='coolwarm')
        return surf

    ani = animation.FuncAnimation(fig, update, frames=len(mesh), interval=interval, blit=False)
    plt.show()