import sys
from Queue import Queue
import adjacency_list


class Tree(object):
    def __init__(self, data, direction, parent):
        self.data = data
        self.direct = direction
        self.children = []
        self.parent = parent

    def add_child(self, node):
        self.children.append(node)


class BreadthSearch:
    def __init__(self, adj_map, current):
        self.Map = adj_map
        self.path = []
        self.current_node = current

    def set_blocked(self, j):
        for i in range(len(self.Map.nodeList)):
            if j == self.Map.nodeList[i].get_id():
                self.Map.nodeList[i].set_blocked()
                break

    def get_neighbors(self, tree, queue, queue2):
        for i in range(0, 4):
            if tree.data.connectedTo[i].id != 'null':
                if tree.data.connectedTo[i].id not in queue2:
                    if i == 0:
                        node = Tree(tree.data.connectedTo[i], "up", tree)
                        queue.put(node)
                    elif i == 1:
                        node = Tree(tree.data.connectedTo[i], "right", tree)
                        queue.put(node)
                    elif i == 2:
                        node = Tree(tree.data.connectedTo[i], "down", tree)
                        queue.put(node)
                    elif i == 3:
                        node = Tree(tree.data.connectedTo[i], "left", tree)
                        queue.put(node)

    def get_adjacent(self, tree, queue):
        for i in range(0, 4):
            if tree.data.connectedTo[i].id != 'null':
                self.get_neighbors(tree.data.connectedTo[i], queue)

    def is_goal(self, goal):
        if goal == "J10":
            return True
        else:
            return False


    #def make_path(self, temp):
        # do stuff here

    def search(self):
        tree = Tree(self.current_node, "null", None)
        opened = Queue()
        closed = {}
        opened.put(tree)
        help = opened.get()
        print help.data
        opened.put(help)
        self.get_neighbors(tree, opened, closed)
        opened.get()
        tree = opened.get()
        opened.put(tree)
        while not opened.empty():
            help = opened.get()
            closed[help.data.id] = help
            print help.data
            self.get_neighbors(tree, opened, closed)
#        while not opened.empty():
#           if self.is_goal("J10"):
#                #self.make_path()
#               break
#            self.get_adjacent(tree, opened)
#            print opened


def main(argv):
    amap = adjacency_list.make_map()
    bs = BreadthSearch(amap, amap.get_node('A1'))
    bs.search()
    pass


if __name__ == '__main__':
    main(sys.argv)



