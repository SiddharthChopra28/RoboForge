import pygame.draw

from comps import *
from socks import Sock
from ardparser import ardparser
from PyQt5.QtWidgets import QApplication, QFileDialog
import json
from threading import Thread


def open_file_dialog():
    app = QApplication([])
    file_dialog = QFileDialog()
    file_path, _ = file_dialog.getOpenFileName()
    app.exit()
    return file_path

def process_input(sc):
    while True:
        data = sc.receive_data()
        print(data)
        # do some things with this data


def make_circuit_dict():
    d = {'COMPS': [], 'CONNECTIONS':[]}
    for comp in components:
        d['COMPS'].append(comp.id)
    for w in wires:
        e = ((w.node1.parent_id, w.node1.id), (w.node2.parent_id, w.node2.id))
        # if there are muliple objects of same component then this will cause issue. fix it
        # a possible solution: maintain a counter for each clss initializr usin somethinglike static pf cpp but in python
        d['CONNECTIONS'].append(e)

    return d



WIDTH = 1400
HEIGHT = 800
SIDEBAR_WIDTH = 320

COMP = 0
CODE = 1
COND = 2

WHITE = (255,255,255)
GREY = (144, 147, 150)
BLACK = (0,0,0)
RED = (255,0,0)
EDITORBG = (161, 203, 209)
SIDEBARBG = (40, 50, 51)

pygame.init()
pygame.font.init()

window = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("RoboForge")



# ard_img = pygame.transform.scale(ard_img, (WIDTH*scaling_factor, HEIGHT*scaling_factor))
# use to scale
code_file = None

wires = []
components = []
ard_comp = make_comp_ard()
components.append(ard_comp)


font_ausa = pygame.font.Font("./assets/PlaywriteAUSA-Regular.ttf", 30)
font_rob = pygame.font.Font("./assets/Roboto-Regular.ttf", 20)
font_lat = pygame.font.Font("./assets/Lato-Regular.ttf", 20)

active_pane = COMP

#render text & buttons
titletext = font_ausa.render("RoboForge", True, WHITE)

comp_t = font_lat.render("Components", True, (WHITE if active_pane == COMP else GREY))
comp_t_rect = comp_t.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+17, HEIGHT//25+75))

code_t = font_lat.render("Code", True, (WHITE if active_pane == CODE else GREY))
code_t_rect = code_t.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+144, HEIGHT//25+75))

cond_t = font_lat.render("Conditions", True, (WHITE if active_pane == COND else GREY))
cond_t_rect = cond_t.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+207, HEIGHT//25+75))

spawn_ir_button = font_rob.render("+ IR Sensor", True, WHITE)
spawn_ir_button_rect = spawn_ir_button.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+144, HEIGHT//5))

upload_code_button = font_rob.render("Upload Code", True, WHITE)
upload_code_button_rect = upload_code_button.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+144, HEIGHT//4))

run_code_button = font_rob.render("Run Code", True, GREY)
run_code_button_rect = run_code_button.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+144, HEIGHT//3))

code_status_text = font_rob.render("Upload code file", True, WHITE)

editor_area = pygame.Rect(0,0,WIDTH-SIDEBAR_WIDTH, HEIGHT)


wire_node1 = None

running = True
socket_connected = False
# sock = None
sock = Sock()


global_button_rects = [comp_t_rect, code_t_rect, cond_t_rect]
comp_button_rects = [spawn_ir_button_rect]
code_button_rects = [upload_code_button_rect]
cond_button_rects = []
visible_button_rects = global_button_rects+comp_button_rects

socket_thread = None

while running:
    #define vars
    pane_switched = False

    # when user starts code running, sock object will be created. when user stops code running, sock is set to none
    if sock is not None:
        if not socket_connected:
            if sock.connect():
                socket_connected = True
                print("socket_connected")
                circ_dict_json = json.dumps(make_circuit_dict())
                sock.send_msg(f"COMMAND:INIT:{circ_dict_json}")
                socket_thread = Thread(target=process_input, args=(sock,), daemon=True)
                socket_thread.start()


    # send data to cpp in following cases:
    # once when program starts about the setup of the circuit
    # if conditions are changed while the code is running
    # change in circuit is not allowed while the code is running


    mouse_pos = pygame.mouse.get_pos()

    for event in pygame.event.get():

        if event.type == pygame.QUIT:
            running = False


        elif event.type == pygame.KEYDOWN:

            if event.key == pygame.K_ESCAPE:
                wire_node1 = None


        elif event.type == pygame.MOUSEBUTTONDOWN:
            print(event.pos)

            if event.button == 1:
                #check for dragging of components
                for component in components.__reversed__():
                    if component.rect.collidepoint(event.pos):
                        component.being_dragged = True
                        component.drag_mouse_coord = event.pos
                        component.drag_og_coord = component.rect.center
                        break

            elif event.button == 3:
                #check for wire attachment/detachment
                for component in components.__reversed__():
                    for node in component.nodes:
                        if node.rect.collidepoint(event.pos):
                            # if node already has a wire connected, can't connect new but can remove existing
                            if node.wire:
                                if wire_node1:
                                    pass
                                else:
                                    wires.remove(node.wire)
                                    wire_node1 = node.wire.node2 if node.wire.node1 == node else node.wire.node1
                                    node.wire = None
                            # if there is no wire on the node, we can connect new wire
                            else:
                                if wire_node1:
                                    if node!=wire_node1:
                                        wires.append(Wire(wire_node1, node))
                                        wire_node1 = None
                                else:
                                    wire_node1 = node
                            break


        elif event.type == pygame.MOUSEBUTTONUP:

            # check for stopping dragging components
            if event.button == 1:
                for component in components:
                    if component.being_dragged:
                        component.being_dragged = False

            # check for global buttons clicked
            if comp_t_rect.collidepoint(event.pos):
                active_pane = COMP
                pane_switched = True
                visible_button_rects = global_button_rects+comp_button_rects


            elif code_t_rect.collidepoint(event.pos):
                active_pane = CODE
                pane_switched = True
                visible_button_rects = global_button_rects+code_button_rects

            elif cond_t_rect.collidepoint(event.pos):
                active_pane = COND
                pane_switched = True
                visible_button_rects = global_button_rects+cond_button_rects

            #change color of buttons when pane is switched
            if pane_switched:
                comp_t = font_lat.render("Components", True, (WHITE if active_pane == COMP else GREY))
                code_t = font_lat.render("Code", True, (WHITE if active_pane == CODE else GREY))
                cond_t = font_lat.render("Conditions", True, (WHITE if active_pane == COND else GREY))


            # check for conditional buttons
            if active_pane == COMP:
                # check for spawn ir button
                if spawn_ir_button_rect.collidepoint(event.pos):
                    ir = make_comp_ir()
                    components.append(ir)

            if active_pane == CODE:
                #check for upload_code button
                if upload_code_button_rect.collidepoint(event.pos):
                    file = open_file_dialog()
                    print(file.split("."))
                    if file == "":
                        code_status_text = font_rob.render("Upload code file", True, WHITE)
                        code_file = None
                        run_code_button = font_rob.render("Run Code", True, GREY)
                        if run_code_button_rect in code_button_rects:
                            code_button_rects.remove(run_code_button_rect)
                        visible_button_rects = global_button_rects+code_button_rects


                    elif file.split(".")[-1] != "ino":
                        code_status_text = font_rob.render("Unsupported file extension", True, WHITE)
                        code_file = None
                        run_code_button = font_rob.render("Run Code", True, GREY)
                        if run_code_button_rect in code_button_rects:
                            code_button_rects.remove(run_code_button_rect)
                        visible_button_rects = global_button_rects+code_button_rects


                    else:
                        code_status_text = font_rob.render("Code uploaded successfully", True, WHITE)
                        code_file = file
                        run_code_button = font_rob.render("Run Code", True, WHITE)
                        if run_code_button_rect not in code_button_rects:
                            code_button_rects.append(run_code_button_rect)
                        visible_button_rects = global_button_rects+code_button_rects


            # check for run_code button
                if run_code_button_rect.collidepoint(event.pos):
                    if code_file:
                        editing_mode = False
                        ardparser(code_file)
                        print("OK")



    # dragging procedure
    for component in components:
        if component.being_dragged:
            component.rect.center = (component.drag_og_coord[0]+mouse_pos[0]-component.drag_mouse_coord[0], component.drag_og_coord[1]+mouse_pos[1]-component.drag_mouse_coord[1])
            component.move_nodes()

    # make mouse pointer while hovering on button
    for rect in visible_button_rects:
        if rect.collidepoint(mouse_pos):
            pygame.mouse.set_cursor(pygame.SYSTEM_CURSOR_HAND)
            break
    else:
        pygame.mouse.set_cursor(pygame.SYSTEM_CURSOR_ARROW)


    # rendering

    window.fill(SIDEBARBG)
    pygame.draw.rect(window, EDITORBG, editor_area)

    window.blit(titletext, (WIDTH-SIDEBAR_WIDTH+80, HEIGHT//25))
    pygame.draw.line(window, WHITE, (WIDTH-SIDEBAR_WIDTH+15, HEIGHT//25+70), (WIDTH-15, HEIGHT//25+70)) # seperator line

    window.blit(comp_t, comp_t_rect.topleft)
    pygame.draw.line(window, WHITE, (WIDTH-SIDEBAR_WIDTH+136, HEIGHT//25+78), (WIDTH-SIDEBAR_WIDTH+136, HEIGHT//25+102)) # seperator line

    window.blit(code_t, code_t_rect.topleft)
    pygame.draw.line(window, WHITE, (WIDTH-SIDEBAR_WIDTH+197, HEIGHT//25+78), (WIDTH-SIDEBAR_WIDTH+197, HEIGHT//25+102)) # seperator line

    window.blit(cond_t, cond_t_rect.topleft)

    pygame.draw.line(window, WHITE, (WIDTH-SIDEBAR_WIDTH+15, HEIGHT//25+110), (WIDTH-15, HEIGHT//25+110)) # seperator line

    # render conditional components:
    if active_pane == COMP:
        window.blit(spawn_ir_button, spawn_ir_button_rect.topleft)

    if active_pane == CODE:
        window.blit(upload_code_button, upload_code_button_rect.topleft)
        window.blit(run_code_button, run_code_button_rect.topleft)
        window.blit(code_status_text, (WIDTH-SIDEBAR_WIDTH, HEIGHT//5))

    for component in components:
        component.rect.clamp_ip(editor_area)

    for component in components:
        window.blit(component.image, component.rect)

    if wire_node1:
        pygame.draw.line(window, RED, wire_node1.rect.center, mouse_pos, width=3)
    for wire in wires:
        pygame.draw.line(window, RED, wire.node1.rect.center, wire.node2.rect.center, width=3)

    pygame.display.flip()

pygame.quit()

