import sys


class Node:
    def __init__(self, key):
        self.id = key
        self.connectedTo = {}

    def add_neighbor(self, neighbor):
        self.connectedTo[neighbor] = 1

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
            n = self.add_nodes(i)
        if j not in self.nodeList:
            n = self.add_nodes(j)
        self.nodeList[i].addNeighbor(self.nodeList[j])

    def get_nodes(self):
        return self.nodeList.keys()

    def __iter__(self):
        return iter(self.nodeList.values())


def main(argv):
    with open("Embedded_Map.txt", "r") as map:
        for line in map:
            data_list = line,
            print data_list
    pass


if __name__ == '__main__':
    main(sys.argv)


