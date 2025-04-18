import matplotlib.pyplot as plt
import matplotlib.animation as animation

def animated_heat_plot(mesh, frame_rate):
    # Set up figure and axis
    fig, ax = plt.subplots()

    # Initial data for heatmap (10x10 grid)
    heatmap = ax.imshow(mesh[0], cmap='turbo', interpolation='bilinear')


    # Animation update function
    def update(frame):
        
        heatmap.set_data(mesh[frame])
        
        return [heatmap]

    # Create animation
    ani = animation.FuncAnimation(fig, update, frames=len(mesh), interval=frame_rate)

    plt.title("Animated Heatmap")
    plt.colorbar(heatmap)
    plt.show()
