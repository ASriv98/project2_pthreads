import numpy as np
import itertools
import networkx as nx
import matplotlib.pyplot as plt


# Tested on Python 3.5, use with Python 3+
class UndirectedErdosRenyi:
    def __init__(self, num_vertices, p_edge, rand_low, rand_high):
        self.num_vertices = num_vertices
        self.adjacency_matrix = [ [ None for j in range(num_vertices) ] for i in range(num_vertices) ]
        random_matrix = np.random.rand(num_vertices, num_vertices)
        for row_number, row in enumerate(random_matrix):
            for column_number, rand_matrix_entry in enumerate(row):
                if column_number == row_number:
                    self.adjacency_matrix[row_number][column_number] = 0
                elif rand_matrix_entry < p_edge:
                    rand_int = np.random.randint(rand_low, rand_high)
                    self.adjacency_matrix[row_number][column_number] = rand_int
                    self.adjacency_matrix[column_number][row_number] = rand_int
                else:
                    self.adjacency_matrix[row_number][column_number] = 0
                    self.adjacency_matrix[column_number][row_number] = 0

        self.connected_bool = nx.is_connected(nx.from_numpy_matrix(np.array(self.adjacency_matrix)))

    def draw_graph(self):
        nx_matrix = nx.from_numpy_matrix(np.array(self.adjacency_matrix))
        all_weights = []
        pos = nx.circular_layout(nx_matrix)
        for (node1, node2, data) in nx_matrix.edges(data=True):
            all_weights.append(data['weight'])
        unique_weights = list(set(all_weights))
        for weight in unique_weights:
            weighted_edges = [(node1, node2) for (node1, node2, edge_attr) in nx_matrix.edges(data=True) if
                              edge_attr['weight'] == weight]
            width = weight
            nx.draw_networkx_edges(nx_matrix, pos, edgelist=weighted_edges, width=width)
        nx.draw_networkx(nx_matrix, pos, cmap=plt.get_cmap('jet'), node_color=range(self.num_vertices))
        plt.axis('off')
        plt.title('Connected graph with {:d} nodes'.format(self.num_vertices))
        plt.show()
        if self.connected_bool:
            T = nx.algorithms.minimum_spanning_tree(nx_matrix)
            all_weights = []
            for (node1, node2, data) in T.edges(data=True):
                all_weights.append(data['weight'])
            unique_weights = list(set(all_weights))
            for weight in unique_weights:
                weighted_edges = [(node1, node2) for (node1, node2, edge_attr) in T.edges(data=True) if
                                  edge_attr['weight'] == weight]
                width = weight
                nx.draw_networkx_edges(T, pos, edgelist=weighted_edges, width=width)
            nx.draw_networkx(T, pos, cmap=plt.get_cmap('jet'), node_color=range(self.num_vertices))
            plt.axis('off')
            plt.title("Minimum Spanning Tree")
            plt.show()


# https://stackoverflow.com/questions/2429098/how-to-treat-the-last-element-in-list-differently-in-python
def notlast(itr):
    itr = iter(itr)  # ensure we have an iterator
    prev = itr.__next__()
    for item in itr:
        yield prev
        prev = item


def main():
    # Erdos Renyi graph will be connected almost surely if p_edge > ln(num_vertices)/num_vertices
    num_vertices = 10
    p_edge = .5  # prob of an edge 0 <= p_edge <= 1
    rand_low = 1  # random weight for edge low bbound
    rand_high = 10  # random weight for edge high bound
    draw_graph = True
    print_to_file = False

    if print_to_file:
        f1 = open('graph_output.txt', 'w+')  # Change to None for not output file
    else:
        f1 = None

    while True:
        random_graph = UndirectedErdosRenyi(num_vertices, p_edge, rand_low, rand_high)
        if random_graph.connected_bool:
            print('int adj_matrix[{:d}][{:d}] = {{'.format(num_vertices, num_vertices), file=f1)
            for row in random_graph.adjacency_matrix:
                print("{", end='', file=f1)
                for item in notlast(row):
                    print("{:d}".format(item), end=", ", file=f1)
                print("{:d}".format(row[-1]), end="}, ", file=f1)
                print("", file=f1)
            print('};', file=f1)
            break

    if draw_graph:
        random_graph.draw_graph()


if __name__ == "__main__":
    main()
