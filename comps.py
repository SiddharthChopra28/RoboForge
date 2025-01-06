import pygame

WIDTH = 1400
HEIGHT = 800

class Component:
    def __init__(self, img):
        self.image = img
        self.rect = img.get_rect()
        self.nodes = []

        self.being_dragged = False
        self.drag_mouse_coord = None
        self.drag_og_coord = None

    def add_node(self, offset_x, offset_y, width, height):
        self.nodes.append(Node(self.rect, offset_x, offset_y, width, height))

    def move_nodes(self):
        for node in self.nodes:
            node.update_coords()


class Node:
    """parent rect, offsets from center of parent rect"""
    def __init__(self, parent_rect, offset_x, offset_y, width, height):
        self.rect = pygame.Rect((0,0,width, height))
        self.rect.center = (parent_rect.centerx+offset_x, parent_rect.centery+offset_y)
        self.parent_rect = parent_rect
        self.offset_x = offset_x
        self.offset_y = offset_y
        self.wire = None


    def update_coords(self):
        self.rect.center = (self.parent_rect.centerx+self.offset_x, self.parent_rect.centery+self.offset_y)


class Wire:
    def __init__(self, node1, node2):
        self.node1 = node1
        self.node2 = node2
        node1.wire = self
        node2.wire = self


def make_comp_ard():
    ard_img = pygame.image.load("./assets/arduino.png").convert_alpha()
    ard_comp = Component(ard_img)
    ard_comp.rect.center = (WIDTH//2, HEIGHT//2)

    x = -50
    increments = [13, 13, 13, 14, 13, 14, 13, 13, 23, 14, 13, 13, 13, 14, 13, 13]

    for increment in increments:
        x += increment
        ard_comp.add_node(x,-127,6,6)

    return ard_comp

def make_comp_ir():
    ir_img = pygame.image.load("./assets/ir.png").convert_alpha()
    ir_comp = Component(ir_img)
    ir_comp.rect.center = (WIDTH//2, HEIGHT//2)

    y=-15
    increments = [0,12, 12]

    for increment in increments:
        y += increment
        ir_comp.add_node(87,y,10,5)


    return ir_comp