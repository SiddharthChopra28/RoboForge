import pygame
from ids import *

WIDTH = 1400
HEIGHT = 800


class Node:
    """parent rect, offsets from center of parent rect"""
    def __init__(self, parent_rect, offset_x, offset_y, width, height, node_id, parent_comp):
        self.rect = pygame.Rect((0,0,width, height))
        self.rect.center = (parent_rect.centerx+offset_x, parent_rect.centery+offset_y)
        self.parent_rect = parent_rect
        self.parent_comp = parent_comp
        self.id = node_id
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

class Component:
    def __init__(self, off_img, on_img):
        self.image = off_img
        self.off_img = off_img
        self.on_img = on_img
        self.rect = self.image.get_rect()
        self.nodes = []
        self.id = None
        self.state:int = 0

        self.being_dragged = False
        self.drag_mouse_coord = None
        self.drag_og_coord = None

    def add_node(self, offset_x, offset_y, width, height, node_id):
        self.nodes.append(Node(self.rect, offset_x, offset_y, width, height, node_id, self))

    def move_nodes(self):
        for node in self.nodes:
            node.update_coords()



class Arduino(Component):
    def __init__(self):
        ard_img_led_off = pygame.image.load("./assets/arduino_led_off.png").convert_alpha()
        ard_img_led_on = pygame.image.load("./assets/arduino_led_on.png").convert_alpha()

        super().__init__(ard_img_led_off, ard_img_led_on)

        self.rect.center = (WIDTH//2, HEIGHT//2)
        self.id = ARDUINO

        x = -76
        pins = [(13, PIN_SCL),(13, PIN_SDA), (13,PIN_AREF), (13,PIN_R_GND), (13,PIN_D13), (14,PIN_D12), (13,PIN_D11), (14,PIN_D10), (13,PIN_D9), (13,PIN_D8), (23,PIN_D7), (14,PIN_D6), (13,PIN_D5), (13,PIN_D4), (13,PIN_D3), (14,PIN_D2), (13,PIN_D1), (13,PIN_D0)]

        for increment, node_id in pins:
            x += increment
            self.add_node(x,-127,8,8, node_id)

        x= -15
        pins = [(13, PIN_IOREF), (13, PIN_RESET), (14, PIN_3V3), (13, PIN_5V), (13, PIN_L_GND_2), (13, PIN_L_GND_1), (14, PIN_VIN), (27, PIN_A0), (14, PIN_A1), (13, PIN_A2), (13, PIN_A3), (13, PIN_A4), (13, PIN_A5)]
        for increment, node_id in pins:
            x += increment
            self.add_node(x,127,8,8, node_id)

    def set_state(self, state:bool):
        # False means off and True means on
        if state:
            self.image = self.on_img
        else:
            self.image = self.off_img

        self.state = state
        center_coords = self.rect.center
        self.rect = self.image.get_rect()
        self.rect.center = center_coords

    def get_state(self):
        return self.state

    def handle_scroll(self, y):
        # do nothing
        return None


class IRSensor(Component):
    def __init__(self):
        ir_img = pygame.image.load("./assets/ir.png").convert_alpha()
        super().__init__(ir_img, ir_img)

        self.rect.center = (WIDTH//2, HEIGHT//2)
        self.id = IR

        y=-15
        pins = [(0, PIN_IRSENSOR_VIN), (12, PIN_IRSENSOR_GND), (12, PIN_IRSENSOR_OUT)]

        for increment, node_id in pins:
            y += increment
            self.add_node(87,y,10,5, node_id)

    def set_state(self, state:bool):
        pass

    def get_state(self):
        return self.state

    def handle_scroll(self, y):
        self.state = not self.state
        return ["IR Sensor", "ON" if self.state else "OFF"]

class Led(Component):
    def __init__(self):
        led_img_on = pygame.image.load("./assets/led_on.png").convert_alpha()
        led_img_off = pygame.image.load("./assets/led_off.png").convert_alpha()

        super().__init__(led_img_off, led_img_on)

        self.rect.center = (WIDTH//2, HEIGHT//2)
        self.id = LED
        self.add_node(-5, 56, 8, 8, PIN_LED_POS)
        self.add_node(13, 49, 8, 8, PIN_LED_NEG)

    def set_state(self, state:bool):
        if state:
            self.image = self.on_img
        else:
            self.image = self.off_img

        self.state = state


    def get_state(self):
        return self.state

    def handle_scroll(self, y):
        # do nothing
        return None


class TempSensor(Component):
    def __init__(self):
        temp_sens_img = pygame.image.load("./assets/tempsensor.png").convert_alpha()
        super().__init__(temp_sens_img, temp_sens_img)
        self.rect.center = (WIDTH//2, HEIGHT//2)
        self.id = TEMPSENS
        self.temp = 25 # default temp = room temp
        
        self.add_node(-28, 80, 8, 8, PIN_TEMPSENS_VCC)
        self.add_node(0, 80, 8, 8, PIN_TEMPSENS_OUT)
        self.add_node(29, 80, 8, 8, PIN_TEMPSENS_GND)

    # self.add_node()

    def set_state(self, state:int):
        pass

    def get_state(self):
        return self.temp

    def handle_scroll(self, y):
        if y == 1 and self.temp<100:
            self.temp += 0.5
        elif y == -1 and self.temp>0:
            self.temp -= 0.5

        return ["Temp. Sen.", str(self.temp)]
