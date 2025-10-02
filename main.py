from json import JSONDecodeError

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

stop_thread = False
def process_input(sc):
    while not stop_thread:
        data = sc.receive_data()
        try:
            data_dict = json.loads(data)
            states = data_dict["STATES"]
            for n, comp in enumerate(components):
                if comp.state != states[n]:
                    comp.set_state(states[n])
        except:
            pass


def make_circuit_dict():
    d = {'COMPS': [], 'CONNECTIONS':[]}
    for comp in components:
        d['COMPS'].append(comp.id)

    for w in wires:
        e = [[components.index(w.node1.parent_comp), w.node1.id], [components.index(w.node2.parent_comp), w.node2.id]]
        d['CONNECTIONS'].append(e)

    return d

def make_cond_list():
    return [comp.get_state() for comp in components]

WIDTH = 1400
HEIGHT = 800
SIDEBAR_WIDTH = 320

COMP = 0
CODE = 1

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
ard_comp = Arduino()
components.append(ard_comp)


font_ausa = pygame.font.Font("./assets/PlaywriteAUSA-Regular.ttf", 30)
font_rob = pygame.font.Font("./assets/Roboto-Regular.ttf", 20)
font_lat = pygame.font.Font("./assets/Lato-Regular.ttf", 20)

active_pane = COMP

#render text & buttons
titletext = font_ausa.render("RoboForge", True, WHITE)

comp_t = font_lat.render("Components", True, (WHITE if active_pane == COMP else GREY))
comp_t_rect = comp_t.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+35, HEIGHT//25+75))

code_t = font_lat.render("Simulation", True, (WHITE if active_pane == CODE else GREY))
code_t_rect = code_t.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+180, HEIGHT//25+75))


reset_button = font_rob.render("Reset", True, WHITE)
reset_button_rect = reset_button.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+144, HEIGHT//5))

spawn_ir_button = font_rob.render("+ IR Sensor", True, WHITE)
spawn_ir_button_rect = spawn_ir_button.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+100, HEIGHT//4))

spawn_led_button = font_rob.render("+ LED", True, WHITE)
spawn_led_button_rect = spawn_led_button.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+100, HEIGHT//4+50))

spawn_tempsens_button = font_rob.render("+ Temperature Sensor", True, WHITE)
spawn_tempsens_button_rect = spawn_tempsens_button.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+100, HEIGHT//4+100))

spawn_breadboard_button = font_rob.render("+ Breadboard", True, WHITE)
spawn_breadboard_button_rect = spawn_breadboard_button.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+100, HEIGHT//4+150))

upload_code_button = font_rob.render("Select File", True, WHITE)
upload_code_button_rect = upload_code_button.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+144, HEIGHT//4))

run_code_button = font_rob.render("Run Code", True, GREY)
run_code_button_rect = run_code_button.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+144, HEIGHT//3))

stop_code_button = font_rob.render("Stop Code", True, WHITE)
stop_code_button_rect = stop_code_button.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+144, HEIGHT//3))

gppnotfound_text = font_rob.render("g++ not found", True, WHITE)

xtermnotfound_text = font_rob.render("xterm not found", True, WHITE)

comp_info_title = ""
comp_info_text = ""
comp_info_title_render = font_rob.render(comp_info_title, True, WHITE)
comp_info_text_render = font_rob.render(comp_info_text, True, WHITE)


code_status_text = font_rob.render("Upload code file", True, WHITE)

editor_area = pygame.Rect(0,0,WIDTH-SIDEBAR_WIDTH, HEIGHT)
comp_info_box = pygame.Rect(0, 0, 100, 50)

wire_node1 = None

show_comp_info_box = 0
SHOW_COMP_LIMIT = 250
running = True
socket_connected = False
editing_mode = True
gppnotfound = False
xtermnotfound = False

sock = None


global_button_rects = [comp_t_rect, code_t_rect]
comp_button_rects = [reset_button_rect, spawn_ir_button_rect, spawn_led_button_rect, spawn_tempsens_button_rect, spawn_breadboard_button_rect]
comp_buttons = [reset_button, spawn_ir_button, spawn_led_button, spawn_tempsens_button, spawn_breadboard_button]
code_button_rects = [upload_code_button_rect]
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
                circ_dict_json = json.dumps(make_circuit_dict())
                sock.send_msg(f"COMMAND:CIRCUIT:{circ_dict_json}")
                cond_dict_json = json.dumps(make_cond_list())
                sock.send_msg(f"COMMAND:CONDITIONS:{cond_dict_json}")
                socket_thread = Thread(target=process_input, args=(sock,), daemon=True)
                socket_thread.start()


    # send data to cpp in following cases:
    # once when program starts about the setup of the circuit
    # if conditions are changed while the code is running
    # change in circuit is not allowed while the code is running

    if show_comp_info_box > 0:
        if show_comp_info_box < SHOW_COMP_LIMIT:
            show_comp_info_box+=1
        elif show_comp_info_box == SHOW_COMP_LIMIT:
            show_comp_info_box = 0



    mouse_pos = pygame.mouse.get_pos()

    for event in pygame.event.get():

        if event.type == pygame.QUIT:
            running = False


        elif event.type == pygame.KEYDOWN:

            if event.key == pygame.K_ESCAPE:
                wire_node1.wire = None
                wire_node1 = None


        elif event.type == pygame.MOUSEBUTTONDOWN:

            if event.button == 1:
                #check for dragging of components
                for component in components.__reversed__():
                    if component.rect.collidepoint(event.pos):
                        component.being_dragged = True
                        component.drag_mouse_coord = event.pos
                        component.drag_og_coord = component.rect.center
                        print("event.pos", event.pos)
                        print("component.rect.center", component.rect.center)
                        break

            elif event.button == 3:
                #check for wire attachment/detachment
                if editing_mode:
                    flag_broken = False
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
                                flag_broken = True
                                break

                        if flag_broken: break


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

            #change color of buttons when pane is switched
                if pane_switched:
                    comp_t = font_lat.render("Components", True, (WHITE if active_pane == COMP else GREY))
                    code_t = font_lat.render("Simulation", True, (WHITE if active_pane == CODE else GREY))


                # check for conditional buttons
                if active_pane == COMP:
                    # check for buttons
                    if editing_mode:

                        if reset_button_rect.collidepoint(event.pos):
                            wires = []
                            components = []
                            ard_comp = Arduino()
                            components.append(ard_comp)

                        elif spawn_ir_button_rect.collidepoint(event.pos):
                            ir = IRSensor()
                            components.append(ir)
                        elif spawn_led_button_rect.collidepoint(event.pos):
                            led = Led()
                            components.append(led)
                        elif spawn_tempsens_button_rect.collidepoint(event.pos):
                            tempsens = TempSensor()
                            components.append(tempsens)
                        elif spawn_breadboard_button_rect.collidepoint(event.pos):
                            breadboard = BreadBoard()
                            components.append(breadboard)

                elif active_pane == CODE:
                    if editing_mode:
                    #check for upload_code button
                        if upload_code_button_rect.collidepoint(event.pos):
                            file = open_file_dialog()
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
                                sock = Sock()
                                res = ardparser(code_file, sock.PORT)
                                if res == 1:
                                    gppnotfound = True
                                elif res == 2:
                                    xtermnotfound = True
                                elif res == 0:
                                    gppnotfound = False
                                    xtermnotfound = False
                                    editing_mode = False
                                    stop_thread = False


                else:
                    if stop_code_button_rect.collidepoint(event.pos):
                        sock.send_msg(f"COMMAND:EXIT:")
                        stop_thread = True
                        sock.end_sock()
                        socket_thread.join()
                        sock = None
                        socket_connected = False
                        socket_thread = None
                        editing_mode = True


        elif event.type == pygame.MOUSEWHEEL:
            showing_data = None # [title, string]
            if event.y == 1 or event.y == -1:
                for n, comp in enumerate(components.__reversed__()):
                    if comp.rect.collidepoint(mouse_pos):
                        showing_data = comp.handle_scroll(event.y)
                        break

                if showing_data:
                    show_comp_info_box = 1
                    comp_info_box.x, comp_info_box.y = mouse_pos
                    comp_info_title = showing_data[0]
                    comp_info_text = showing_data[1]
                    comp_info_title_render = font_lat.render(comp_info_title, True, WHITE)
                    comp_info_text_render = font_lat.render(comp_info_text, True, WHITE)
                    if sock is not None and socket_connected:
                        sock.send_msg(f"COMMAND:CONDITIONS:{json.dumps(make_cond_list())}")


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
    pygame.draw.line(window, WHITE, (WIDTH-SIDEBAR_WIDTH+161, HEIGHT//25+78), (WIDTH-SIDEBAR_WIDTH+161, HEIGHT//25+102)) # seperator line

    window.blit(code_t, code_t_rect.topleft)


    pygame.draw.line(window, WHITE, (WIDTH-SIDEBAR_WIDTH+15, HEIGHT//25+110), (WIDTH-15, HEIGHT//25+110)) # seperator line

    # render conditional components:
    if active_pane == COMP:
        for n, button in enumerate(comp_buttons):
            window.blit(button, comp_button_rects[n])

    elif active_pane == CODE:
        window.blit(upload_code_button, upload_code_button_rect.topleft)

        window.blit(code_status_text, (WIDTH-SIDEBAR_WIDTH+50, HEIGHT//5))
        if editing_mode:
            window.blit(run_code_button, run_code_button_rect.topleft)
        else:
            window.blit(stop_code_button, stop_code_button_rect.topleft)

        if gppnotfound:
            window.blit(gppnotfound_text, (WIDTH-SIDEBAR_WIDTH+120, HEIGHT//3+50))
        if xtermnotfound:
            window.blit(xtermnotfound_text, (WIDTH-SIDEBAR_WIDTH+120, HEIGHT//3+100))


    for component in components:
        component.rect.clamp_ip(editor_area)

    for component in components:
        window.blit(component.image, component.rect)

    if wire_node1:
        pygame.draw.line(window, RED, wire_node1.rect.center, mouse_pos, width=3)
    for wire in wires:
        pygame.draw.line(window, RED, wire.node1.rect.center, wire.node2.rect.center, width=3)

    if show_comp_info_box>0:
        pygame.draw.rect(window, WHITE, comp_info_box, border_radius = 2)
        pygame.draw.rect(window, SIDEBARBG, comp_info_box.inflate(-2, -2), border_radius=2)

        window.blit(comp_info_title_render, (comp_info_box.x+10, comp_info_box.y+6))
        window.blit(comp_info_text_render, (comp_info_box.x+15, comp_info_box.y+25))



    pygame.display.flip()


sock.send_msg(f"COMMAND:EXIT:")
pygame.quit()

