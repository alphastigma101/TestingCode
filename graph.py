import matplotlib.pyplot as plt
import os
import numpy as np

def bursty(data:dict, filename='bursty_graph.png'):
    # Plot points on a graph
    for key, value in data.items():
        x_values, y_values = zip(*value)
        x_values = np.array(x_values)
        y_values = np.array(y_values)
    # Plot the data using NumPy arrays
    plt.plot(x_values, y_values)
    plt.xlabel('X Axis Uptime')
    plt.ylabel('Y Axis Ticks')
    plt.savefig(filename)  # Save the plot as a .png file




def waste(data:dict, filename='Waste'):
    for key, value in data.items():
        x_values, y_values = zip(*value)
        x_values = np.array(x_values)
        y_values = np.array(y_values)
    # Plot the data using NumPy arrays
    plt.plot(x_values, y_values)
    plt.xlabel('X Axis Uptime')
    plt.ylabel('Y Axis Ticks')
    plt.savefig(filename)  # Save the plot as a .png file

def robin(data:dict, filename='Round-Robin'):
    for key, value in data.items():
        x_values, y_values = zip(*value)
        x_values = np.array(x_values)
        y_values = np.array(y_values)
    # Plot the data using NumPy arrays
    plt.plot(x_values, y_values)
    plt.xlabel('X Axis Uptime')
    plt.ylabel('Y Axis Ticks')
    plt.savefig(filename)  # Save the plot as a .png file


# Driver code
if __name__ == '__main__':
    # Bursty graph
    data = {}
    directory = 'graph/bursty/'
    points = []
    for filename in os.listdir('./graph/bursty/'):
        if filename.endswith('.txt'):
            filepath = os.path.join('./graph/bursty/', filename)
            with open(filepath, 'r') as file:
                points = [(int(item.split(': ')[1].split(', ')[0]), int(item.split(', ')[1])) for item in file]
                data[filename] = points
    bursty(data)
    ###############################################
    # waste graph
    data.clear()
    points.clear()
    directory = 'graph/waste/'
    for filename in os.listdir(directory):
        if filename.endswith('.txt'):
            with open(os.path.join(directory, filename), 'r') as file:
                points = [(int(item.split(': ')[1].split(', ')[0]), int(item.split(', ')[1])) for item in file]
                data[filename] = points
    waste(data)

    ###############################################
    # Round Robin
    data.clear()
    points.clear()
    directory = 'graph/robin/'
    for filename in os.listdir(directory):
        if filename.endswith('.txt'):
            with open(os.path.join(directory, filename), 'r') as file:
                points = [(int(item.split(': ')[1].split(', ')[0]), int(item.split(', ')[1])) for item in file]
                data[filename] = points
    robin(data)

