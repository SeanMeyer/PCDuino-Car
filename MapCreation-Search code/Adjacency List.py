import sys


class Node:
    def __init__(self, key):
        self.id = key
        self.connectedTo = {}

    def add_neighbor(self, neighbor):
        self.connectedTo[neighbor] = 1

    def __str__(self):
        return str(self.id) + ' connectedTo: ' + str([x.id for x in self.connectedTo])

    def get_connections(self):
        return self.connectedTo.keys()

    def get_id(self):
        return self.id

    def get_weight(self, neighbor):
        return self.connectedTo[neighbor]


class Map:
    def __init__(self):
        self.nodeList = {}
        self.numNodes = 0

    def add_nodes(self, key):
        self.numNodes = self.numNodes + 1
        new_node = Node(key)
        self.nodeList[key] = new_node
        return new_node

    def get_node(self, i):
        if i in self.nodeList:
            return self.nodeList
        else:
            return None

    def __contains__(self, i):
        return i in self.nodeList

    def add_road(self, i, j):
        if i not in self.nodeList:
            self.add_nodes(i)
        if j not in self.nodeList:
            self.add_nodes(j)
        self.nodeList[i].add_neighbor(self.nodeList[j])

    def get_nodes(self):
        return self.nodeList.keys()

    def __iter__(self):
        return iter(self.nodeList.values())


def main(argv):
    graph = Map()
    with open("insert file path to Embedded_Map.txt here", "r") as mapp:
        for line in mapp:
            data_list = line.split(';')
            node_main = str(data_list[0][0] + data_list[0][1])
            graph.add_nodes(node_main)
            for i in range(len(data_list)-1):
                node_sub = str(data_list[i+1][0] + data_list[i+1][1])
                graph.add_road(node_main, node_sub)
    pass


if __name__ == '__main__':
    main(sys.argv)

#Thanks to http://interactivepython.org/courselib/static/pythonds/Graphs/Implementation.html for implementation of Adjacency List
