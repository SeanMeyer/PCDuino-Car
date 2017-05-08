import sys
import adjacency_list
import map_search


def get_direction(path):
    direct_list = []
    for i in range(len(path)-1):
        if path[i].direct == 'null':
            direct_list.append(path[i+1].direct)
            direct_list.append('forward')
        elif path[i].data.chckpnt is True and path[i - 1].data.id == 'B8':
            direct_list.append('forward')
        elif path[i].data.chckpnt is True:
            if path[i+1].direct != path[i].direct:
                if path[i].direct == 'up':
                    direct_list.append(path[i+1].direct)
                elif path[i].direct == 'down':
                    if path[i+1].direct == 'right':
                        direct_list.append('left')
                    elif path[i+1].direct == 'left':
                        direct_list.append('right')
                elif path[i].direct == 'left':
                    if path[i+1].direct == 'up':
                        direct_list.append('right')
                    elif path[i+1].direct == 'down':
                        direct_list.append('left')
                elif path[i].direct == 'right':
                    if path[i+1].direct == 'up':
                        direct_list.append('left')
                    elif path[i+1].direct == 'down':
                        direct_list.append('right')
            direct_list.append('forward')
    return direct_list


def main(argv):
    amap = adjacency_list.make_map()
    #q = input("Where do we start?")
    bs = map_search.BreadthSearch(amap, amap.get_node('A1'))
    path = bs.search()
    direct = get_direction(path)
    totalpath = ""
    for move in direct:
        totalpath = totalpath + move + ","
    print totalpath[:-1]


if __name__ == '__main__':
    main(sys.argv)