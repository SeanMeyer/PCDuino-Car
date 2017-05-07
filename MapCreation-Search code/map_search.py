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
                        tree.add_child(node)
                        queue.put(node)
                    elif i == 1:
                        node = Tree(tree.data.connectedTo[i], "right", tree)
                        tree.add_child(node)
                        queue.put(node)
                    elif i == 2:
                        node = Tree(tree.data.connectedTo[i], "down", tree)
                        tree.add_child(node)
                        queue.put(node)
                    elif i == 3:
                        node = Tree(tree.data.connectedTo[i], "left", tree)
                        tree.add_child(node)
                        queue.put(node)

    def is_goal(self, goal):
        if goal == 'J10':
            return True
        else:
            return False

    def make_path(self, tree):
        p = []
        while tree.parent is not None:
            p.append(tree)
            tree = tree.parent
        p.append(tree)
        path = list(reversed(p))
        return path


    def search(self):
        tree = Tree(self.current_node, "null", None)
        opened = Queue()
        path = []
        closed = {}
        opened.put(tree)
        while not opened.empty():
            help = opened.get()
            closed[help.data.id] = help.data.id
            tree = help
            if self.is_goal(tree.data.id) is True:
                path = self.make_path(tree)
                break
            self.get_neighbors(tree, opened, closed)
        return path

