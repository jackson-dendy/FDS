import matplotlib.pyplot as plt

def heat_plot(mesh):
    plt.imshow(mesh, cmap='turbo', interpolation = 'bilinear')
    plt.colorbar(label='Temperature (°C)')
    plt.title("2D SS Heat Transfer")
    plt.xlabel("X Position")
    plt.ylabel("Y Position")
    plt.show()