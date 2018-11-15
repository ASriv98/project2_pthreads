import numpy as np
import itertools
from pprint import pprint

# Use Python3

class UndirectedErdosRenyi:
    def __init__(self, num_vertices, p_edge, rand_low, rand_high):
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
        self.connected_bool = all(x != 0 for x in itertools.chain(*self.adjacency_matrix))


# itr is short for "iterable" and can be any sequence, iterator, or generator
# https://stackoverflow.com/questions/2429098/how-to-treat-the-last-element-in-list-differently-in-python
def notlast(itr):
    itr = iter(itr)  # ensure we have an iterator
    prev = itr.__next__()
    for item in itr:
        yield prev
        prev = item


def main():
    num_vertices = 10
    p_edge = 1 # prob of an edge 0 <= p_edge <= 1
    rand_low = 1 # random weight for edge low bbound
    rand_high = 10 # random weight for edge high bound
    random_graph = UndirectedErdosRenyi(num_vertices, p_edge, rand_low, rand_high)
    print('int adj_matrix[{:d}][{:d}] = {{'.format(num_vertices, num_vertices))
    for row in random_graph.adjacency_matrix:
        print("{", end='')
        for item in notlast(row):
            print("{:d}".format(item), end=", ")
        print("{:d}".format(row[-1]), end="}, ")
        print("")
    print('};')

if __name__ == "__main__":
    main()
