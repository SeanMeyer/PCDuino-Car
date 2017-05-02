import sys


class Node:
    def __init__(self, key):
        self.id = key
        self.blocked = False
        self.chckpnt = False
        self.connectedTo = []
        # connectedTo is set up as {up,right,down,left}

    def add_neighbor(self, neighbor):
        self.connectedTo.append(neighbor)

    def __str__(self):
        return str(self.id) + ' connected to: ' + str([x.id for x in self.connectedTo])

    def get_connections(self):
        return self.connectedTo

    def get_id(self):
        return self.id

    def set_blocked(self):
        self.blocked = True

    def get_blocked(self):
        return self.blocked

    def set_checkpoint(self):
        self.chckpnt = True

    def get_checkpoint(self):
        return self.chckpnt


class Map:
    def __init__(self):
        self.nodeList = {}
        self.numNodes = 0
        self.dict = {}

    def add_nodes(self, key):
        if key not in self.dict:
            self.dict[key] = key
            self.numNodes = self.numNodes + 1
            new_node = Node(key)
            self.nodeList[key] = new_node
            return new_node

    def get_node(self, i):
        if i in self.nodeList:
            return self.nodeList[i]
        else:
            return None

    def __contains__(self, i):
        return i in self.nodeList

    def add_road(self, i, j):
        if i not in self.dict:
            self.add_nodes(i)
        if j not in self.dict:
            self.add_nodes(j)
        self.nodeList[i].add_neighbor(self.nodeList[j])

    def get_nodes(self):
        return self.nodeList.keys()

    def __iter__(self):
        return iter(self.nodeList.values())


def make_map():
    graph = Map()
    # Using the relative file path to access the adjacency map file
    with open("Embedded_Map.txt", "r") as mapp:
        for line in mapp:
            data_list = line.split(';')
            node_main = str(data_list[0])
            graph.add_nodes(node_main)
            for i in range(0, 4):
                node_sub = str(data_list[i + 1])
                graph.add_road(node_main, node_sub)
    return graph


#Thanks to http://interactivepython.org/courselib/static/pythonds/Graphs/Implementation.html for implementation of Adjacency Graph in python.


