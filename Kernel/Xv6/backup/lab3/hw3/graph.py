import matplotlib.pyplot as plt
import os

def bursty(data:dict, filename='bursty_graph.png'):
    # Plot points on a graph
    for key, value in data.items():
        plt.plot(value)
    plt.xlabel('X Axis Uptime')
    plt.ylabel('Y Axis Ticks')
    plt.show()
    plt.savefig(filename)  # Save the plot as a .png file




def waste(data:dict, filename='Waste'):
    for key, value in data.items():
        plt.plot(value)
    plt.xlabel('X Axis Uptime')
    plt.ylabel('Y Axis Ticks')
    plt.show()
    plt.savefig(filename)  # Save the plot as a .png file





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
    data = {}
    directory = 'graph/waste/'
    for filename in os.listdir(directory):
        if filename.endswith('.txt'):
            with open(os.path.join(directory, filename), 'r') as file:
                points = [(int(item.split(': ')[1].split(', ')[0]), int(item.split(', ')[1])) for item in file]
                data[filename] = points
    waste(data)

