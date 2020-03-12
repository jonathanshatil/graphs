import random
import math
import matplotlib.pyplot as plt
import pygame
import sys

WHITE=(255,255,255)
GRAY=(150,150,150)
CREAM=(250,250,200)
BLACK=(0,0,0)
RED=(255,0,0)
GREEN=(0,255,0)
BLUE=(0,0,255)


PRED=(255,150,150)
PGREEN=(150,255,150)
PBLUE=(150,150,255)
pi = math.pi


class Graph:
    def __init__(self,verts=[],edges=[],is_weighted=True,is_directed=True):
        self.verts=verts
        self.edges=edges
        self.is_weighted=is_weighted
        self.is_directed=is_directed

    def make_directed(self):
        new_edges=[[] for i in self.verts]
        if not self.is_weighted:
            for u in range(len(self.verts)):
                for v in range(len(self.edges[u])):
                    if u not in new_edges[v] and u != v:
                        new_edges[v].append(u)
                    if v not in new_edges[u] and u != v:
                        new_edges[u].append(v)
        self.edges=new_edges
        self.is_directed=False

    def add_vert(self,val):
        self.verts.append(val)
        self.edges.append([])

    def from_str(self,type_str,vert_str,edge_str):
        self.is_directed = True
        self.is_weighted = True
        if type_str[0] == "i":
            self.is_directed = False
        if type_str[1] == "n":
            self.is_weighted = False
        self.verts = [i for i in vert_str.split(",")]
        if self.is_weighted:
            self.edges = list(map(lambda x:(int(x.split(",")[0]),float(x.split(",")[1])),[i.split(";") for i in edge_str[1:].replace("}","").split(",{")]))
        else:
            self.edges = [list(map(lambda x:int(x),i.split(";"))) for i in edge_str[1:].replace("}","").split(",{")]

    def __str__(self):
        string=""
        if self.is_directed:
            string+="d"
        else:
            string+="i"
        if self.is_weighted:
            string+="w"
        else:
            string+="n"
        string+="\n"
        for i in self.verts:
            string+=str(i)+","
        string=string[:-1]+"\n"
        for adj_list in self.edges:
            string += "{"
            temp = ""
            for v in adj_list:
                if self.is_weighted:
                    temp = temp + str(v[0])+","+str(v[1])+";"
                else:
                    temp = temp +str(v) + ";"
            string+=temp[:-1]+"},"
        string=string[:-1]
        return string

def gui_graph():
    pass


def add_vert(g,vert_locations,location):
    g.add_vert(location)
    vert_locations.append(location)

def add_edge(g,a,b):
    if b not in g.edges[a]:
        g.edges[a].append(b)


def draw_point(pos,DISPLAY):
    pygame.draw.circle(DISPLAY,BLUE,pos,30)

def draw_arrow(pos_a,pos_b,display):
    m=(pos_b[1]-pos_a[1])/(pos_b[0]-pos_a[0])
    b=pos_a[1]-m*pos_a[0]
    A=m**2+1
    B=2*(pos_b[0]+m*b-m*pos_b[1])
    C=pos_b[1]**2+pos_b[0]**2-2*pos_b[1]*b+b**2-100
    delta=math.sqrt(23**2 - b**2 + 2*b*pos_b[1] - pos_b[1]**2 - 2*pos_b[0]*b*m + 2*pos_b[0]*pos_b[1]*m + 23**2*m**2 - pos_b[0]**2*m**2)
    x_c=(pos_b[0] - b*m + pos_b[1]*m -delta )/(1 + m**2)
    if (x_c<pos_b[0] and x_c<pos_a[0]) or (x_c>pos_b[0] and x_c>pos_a[0]):
        x_c = (pos_b[0] - b*m + pos_b[1]*m +delta )/(1 + m**2)
    pos_c=(int(x_c),int(x_c*m+b))
    print(pos_a,pos_b,pos_c)
    pygame.draw.line(display, BLACK, pos_a, pos_c)
    pygame.draw.circle(DISPLAY, BLACK,pos_c, 7)

def text_objects(text,font):
    textSurf=font.render(text,True,BLACK)
    return textSurf,textSurf.get_rect()


def save_graph(g,filename):
    open(filename,"a").write("//////\n"+str(g)+"\n\n")


def button(msg,rect,ic,ac,g,DISPLAY):
    mouse = pygame.mouse.get_pos()
    ret=False
    if rect.x+rect.w > mouse[0] > rect.x and rect.y+rect.h > mouse[1] > rect.y:
        save_graph(g,"gui graphs.txt")
        DISPLAY.fill(WHITE)
        pygame.draw.rect(DISPLAY, ac, rect)
        ret= True
    else:
        pygame.draw.rect(DISPLAY, ic,rect)

    smallText = pygame.font.Font("freesansbold.ttf",20)
    textSurf, textRect = text_objects(msg, smallText)
    textRect.center = rect.center
    DISPLAY.blit(textSurf, textRect)
    return ret


def genrateRandomGraph(size,is_directed=True,is_weighted=False,min_w=0,max_w=1):
    verts=[chr(random.randint(ord("a"), ord("z"))) for i in range(size)]
    edges=[[] for i in range(size)]
    p=[[j for j in range(size) if j!=i ] for i in range(size)]
    if is_directed:
        for i in range(size):
            adj_num = random.randint(0, size - 1)
            for j in range(adj_num):
                a = p[i][random.randint(0, len(p[i]) - 1)]
                if is_weighted:
                    w = round(random.uniform(min_w, max_w), 5)
                    edges[i].append([a, w])
                else:
                    edges[i].append(a)
                p[i].remove(a)
    else:
        edge_num=random.randint(0,size*(size-1)/2)
        p_edges=[[a,b] for a in range(size) for b in range(size) if a != b]
        while edge_num > 0:
            edge=p_edges[random.randint(0,len(p_edges)-1)]
            p_edges.remove(edge)
            p_edges.remove([edge[1],edge[0]])
            if is_weighted:
                w = round(random.uniform(min_w, max_w), 5)
                edges[edge[0]].append([edge[1], w])
                edges[edge[1]].append([edge[0], w])
            else:
                edges[edge[0]].append(edge[1])
                edges[edge[1]].append(edge[0])
            edge_num -= 1
    return Graph(verts,edges,is_weigted,is_directed)

def draw_graph(g):
    n=len(g.verts)
    x = [math.sin(pi-(2*pi*i/n)) for i in range(n)]
    y = [math.cos(pi - (2 * pi * i / n)) for i in range(n)]
    for i in range(n):
        for v in g.edges[i]:
            if g.is_directed:
                if g.is_weighted:
                    plt.arrow(x[i],y[i],x[v[0]]-x[i],y[v[0]]-y[i],color="black",head_width=0.02,length_includes_head=True)
                else:
                    plt.arrow(x[i],y[i],x[v]-x[i],y[v]-y[i],color="black",head_width=0.02,length_includes_head=True)
            else:
                if g.is_weighted:
                    plt.plot([x[i],x[v[0]]],[y[i],y[v[0]]],color="black")
                else:
                    plt.plot([x[i],x[v]],[y[i],y[v]],color="black")
    plt.scatter(x, y,s=400)

'''draw_graph(genrateRandomGraph(5,False,False))
plt.show()
draw_graph(genrateRandomGraph(5,True,False))
plt.show()
draw_graph(genrateRandomGraph(5,True,True))
plt.show()
f=open("graphs.txt","r")
data=f.read().split("\n")
type_str=data[1]
print(type_str)
vert_str=data[2]
edge_str=data[3]
g=Graph()
g.from_str(type_str,vert_str,edge_str)
draw_graph(g)
plt.show()'''
if __name__ == "__main__":
    pygame.init()
    vert_locations = []
    DISPLAY = pygame.display.set_mode((800, 650), 0, 32)
    DISPLAY.fill(WHITE)
    mode="v"
    g=Graph(is_weighted=False)
    curr_vert=None
    submit_rec = pygame.Rect(20, 530, 200, 100)
    while True:
        flag = True
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                if button("save graph", submit_rec, CREAM, CREAM, g, DISPLAY):
                    g.edges=[]
                    g.verts=[]
                    vert_locations=[]
                if pygame.mouse.get_pos()[1] <= 500:
                    for i in range(len(vert_locations)):
                        if abs(vert_locations[i][0]-pygame.mouse.get_pos()[0])<30 and abs(vert_locations[i][1]-pygame.mouse.get_pos()[1])<30:
                            if mode=='v':
                                mode='e'
                                curr_vert=i
                            else:
                                if i!=curr_vert:
                                    add_edge(g,curr_vert,i)
                                    draw_arrow(vert_locations[curr_vert],vert_locations[i],DISPLAY)
                                curr_vert=None
                                mode='v'
                                flag=False
                    if mode=='v' and flag:
                        add_vert(g,vert_locations,pygame.mouse.get_pos())
                        draw_point(pygame.mouse.get_pos(),DISPLAY)
        pygame.display.update()
        pygame.time.Clock().tick(15)