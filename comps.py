import pygame
from ids import *

WIDTH = 1400
HEIGHT = 800

class Component:
    def __init__(self, img):
        self.image = img
        self.rect = img.get_rect()
        self.nodes = []
        self.id = None

        self.being_dragged = False
        self.drag_mouse_coord = None
        self.drag_og_coord = None

    def add_node(self, offset_x, offset_y, width, height, node_id):
        self.nodes.append(Node(self.rect, offset_x, offset_y, width, height, node_id, self.id))

    def move_nodes(self):
        for node in self.nodes:
            node.update_coords()


class Node:
    """parent rect, offsets from center of parent rect"""
    def __init__(self, parent_rect, offset_x, offset_y, width, height, node_id, parent_id):
        self.rect = pygame.Rect((0,0,width, height))
        self.rect.center = (parent_rect.centerx+offset_x, parent_rect.centery+offset_y)
        self.parent_rect = parent_rect
        self.offset_x = offset_x
        self.offset_y = offset_y
        self.wire = None
        self.parent_id = parent_id
        self.id = node_id


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
    ard_comp.id = ARDUINO

    x = -76
    pins = [(13, PIN_SCL),(13, PIN_SDA), (13,PIN_AREF), (13,PIN_R_GND), (13,PIN_D13), (14,PIN_D12), (13,PIN_D11), (14,PIN_D10), (13,PIN_D9), (13,PIN_D8), (23,PIN_D7), (14,PIN_D6), (13,PIN_D5), (13,PIN_D4), (13,PIN_D3), (14,PIN_D2), (13,PIN_D1), (13,PIN_D0)]

    for increment, node_id in pins:
        x += increment
        ard_comp.add_node(x,-127,8,8, node_id)

    x= -15
    pins = [(13, PIN_IOREF), (13, PIN_RESET), (14, PIN_3V3), (13, PIN_5V), (13, PIN_L_GND_2), (13, PIN_L_GND_1), (14, PIN_VIN), (27, PIN_A0), (14, PIN_A1), (13, PIN_A2), (13, PIN_A3), (13, PIN_A4), (13, PIN_A5)]
    for increment, node_id in pins:
        x += increment
        ard_comp.add_node(x,127,8,8, node_id)

    return ard_comp

def make_comp_ir():
    ir_img = pygame.image.load("./assets/ir.png").convert_alpha()
    ir_comp = Component(ir_img)
    ir_comp.rect.center = (WIDTH//2, HEIGHT//2)
    ir_comp.id = IR
    y=-15
    pins = [(0, PIN_IRSENSOR_VIN), (12, PIN_IRSENSOR_GND), (12, PIN_IRSENSOR_OUT)]

    for increment, node_id in pins:
        y += increment
        ir_comp.add_node(87,y,10,5, node_id)


    return ir_comp