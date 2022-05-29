#maybe fix later: each node is forced to have at least 1 connection to a previous node using this method
#does this even matter? the id of a node doesn't actually carry any meaning, so I dont think this is actually an issue
from random import choice, randint
import turtle
#This script will generate random graphs of connected nodes, and output that information to a file
num_nodes = int(input("Enter the desired number of nodes: "))
min_connections = int(input("Enter the minimum number of connections for each node (must be at least 1): "))
max_connections = int(input("Enter the maximum number of connections for each node: "))

class node:
    def __init__(self, new_id):
        self.ident = new_id
        self.connections = []
        self.distances = []
    def add_connection(self, neighbor, distance):
        self.connections.append(neighbor)
        self.distances.append(distance)
    def get_connections(self):
        return self.connections
    def print(self):
        print(self.ident, end = " ")
        for i in range(len(self.connections)):
            print(self.connections[i], ":", self.distances[i], end = " ", sep = "")
        print()
    def __str__(self):
        output = str(self.ident) 
        for i in range(len(self.connections)):
            output += (" " + str(self.connections[i]) + ":" + str(self.distances[i]))
        return output

node_list = []
for i in range(num_nodes):
    node_list.append(node(i))
    if len(node_list) > 1:
        available_nodes = [x.ident for x in node_list if (len(x.get_connections()) < max_connections)]
        del available_nodes[-1]
        distance = randint(1, 20)
        new_neighbor = choice(available_nodes)
        node_list[i].add_connection(new_neighbor, distance)
        node_list[new_neighbor].add_connection(i, distance)

#By this point, each node in the graph has some sort of connection to the root
#Now, time to add the rest of the connections
low_connections = [x.ident for x in node_list if (len(x.get_connections()) < min_connections)]
while len(low_connections) > 1:
    possible_connections = [x.ident for x in node_list if (len(x.get_connections()) < max_connections)]
    current = possible_connections[0]
    possible_connections = [x for x in possible_connections if x not in node_list[current].connections]
    del possible_connections[0]
    new_neighbor = choice(possible_connections)
    distance = randint(1, 20)
    node_list[current].add_connection(new_neighbor, distance)
    node_list[new_neighbor].add_connection(current, distance)
    low_connections = [x.ident for x in node_list if (len(x.get_connections()) < min_connections)]

xpos = [randint(-800, 800) for x in node_list]
ypos = [randint(-500, 500) for x in node_list]
xpos[0] = 0
ypos[0] = 0

turtle.speed(0)
turtle.hideturtle()
turtle.screensize(1000, 1000)
for i in range(len(node_list)):
    turtle.penup()
    turtle.goto(xpos[i], ypos[i] - 20)
    turtle.write(i)
    turtle.goto(xpos[i], ypos[i] - 5)
    turtle.pendown()
    turtle.circle(radius = 10)

drawn_connections = []
for i in range(len(node_list)):
    full_cons = []
    count = 0
    for connection in node_list[i].connections:
        new_connection = ([i, connection, node_list[i].distances[count]] if (i < connection) else [connection, i, node_list[i].distances[count]]) 
        full_cons.append(new_connection)
        count += 1
    todo_cons = [x for x in full_cons if x not in drawn_connections]
    for connection in todo_cons:
        turtle.penup()
        turtle.goto(xpos[connection[0]], ypos[connection[0]])
        turtle.pendown()
        turtle.goto((xpos[connection[1]]+xpos[connection[0]])/2, (ypos[connection[1]]+ypos[connection[0]])/2)
        turtle.write(connection[2])
        turtle.goto(xpos[connection[1]], ypos[connection[1]])
        drawn_connections.append(connection)
        
file = open("./saved_map", "w")
for i in node_list:
    file.write(str(i))
    file.write("\n")
file.close()
ts = turtle.getscreen()
ts.getcanvas().postscript(file="./map.eps")
