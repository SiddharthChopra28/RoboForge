from comps import *
from socks import Sock

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
wires = []
components = []
ard_comp = make_comp_ard()
components.append(ard_comp)


font_ausa = pygame.font.Font("./assets/PlaywriteAUSA-Regular.ttf", 30)
font_rob = pygame.font.Font("./assets/Roboto-Regular.ttf", 20)
font_lat = pygame.font.Font("./assets/Lato-Regular.ttf", 20)

active_pane = COMP
titletext = font_ausa.render("RoboForge", True, WHITE)
comp_t = font_lat.render("Components", True, (WHITE if active_pane == COMP else GREY))
code_t = font_lat.render("Code", True, (WHITE if active_pane == CODE else GREY))
cond_t = font_lat.render("Conditions", True, (WHITE if active_pane == COND else GREY))
comp_t_rect = comp_t.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+17, HEIGHT//25+75))
code_t_rect = code_t.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+144, HEIGHT//25+75))
cond_t_rect = cond_t.get_rect(topleft=(WIDTH-SIDEBAR_WIDTH+207, HEIGHT//25+75))

editor_area = pygame.Rect(0,0,WIDTH-SIDEBAR_WIDTH, HEIGHT)

clock = pygame.time.Clock()
FPS = 60

wire_node1 = None

running = True
socket_connected = False
# sock = None
sock = Sock()

while running:
    # when user starts code running, sock object will be created. when user stops code running, sock is set to none
    if sock is not None:
        if not socket_connected:
            if sock.connect():
                socket_connected = True
                print("socket_connected")
                sock.send_msg(";this msg carries info about the circuit and conditions;")
            else:
                print("tried real hard but failed :(")
        else:
            # receive info about the states of the circuit
            data = sock.receive_data()
            print(data)

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
            for component in components.__reversed__():
                if event.button == 1:
                    if component.rect.collidepoint(event.pos):
                        component.being_dragged = True
                        component.drag_mouse_coord = event.pos
                        component.drag_og_coord = component.rect.center
                        break
                elif event.button == 3:
                    for node in component.nodes:
                        if node.rect.collidepoint(event.pos):
                            if node.wire:
                                if wire_node1:
                                    pass
                                else:
                                    wires.remove(node.wire)
                                    wire_node1 = node.wire.node2 if node.wire.node1 == node else node.wire.node1
                                    node.wire = None
                            else:
                                if wire_node1:
                                    wires.append(Wire(wire_node1, node))

                                    wire_node1 = None
                                else:
                                    wire_node1 = node
                            break


            print(event.pos)

        elif event.type == pygame.MOUSEBUTTONUP:
            for component in components:
                if event.button == 1:
                    if component.being_dragged:
                        component.being_dragged = False

            # check for buttons clicked
            if comp_t_rect.collidepoint(event.pos):
                active_pane = COMP
                # spawn ir sensor
                ir = make_comp_ir()
                components.append(ir)

            elif code_t_rect.collidepoint(event.pos):
                active_pane = CODE
            elif cond_t_rect.collidepoint(event.pos):
                active_pane = COND

            comp_t = font_lat.render("Components", True, (WHITE if active_pane == COMP else GREY))
            code_t = font_lat.render("Code", True, (WHITE if active_pane == CODE else GREY))
            cond_t = font_lat.render("Conditions", True, (WHITE if active_pane == COND else GREY))

    for component in components:
        if component.being_dragged:
            component.rect.center = (component.drag_og_coord[0]+mouse_pos[0]-component.drag_mouse_coord[0], component.drag_og_coord[1]+mouse_pos[1]-component.drag_mouse_coord[1])
            component.move_nodes()

    if comp_t_rect.collidepoint(mouse_pos) or code_t_rect.collidepoint(mouse_pos) or cond_t_rect.collidepoint(mouse_pos):
        pygame.mouse.set_cursor(pygame.SYSTEM_CURSOR_HAND)
    else:
        pygame.mouse.set_cursor(pygame.SYSTEM_CURSOR_ARROW)


    window.fill(SIDEBARBG)

    pygame.draw.rect(window, EDITORBG, editor_area) # editor area

    window.blit(titletext, (WIDTH-SIDEBAR_WIDTH+80, HEIGHT//25))
    pygame.draw.line(window, WHITE, (WIDTH-SIDEBAR_WIDTH+15, HEIGHT//25+70), (WIDTH-15, HEIGHT//25+70)) # seperator line

    window.blit(comp_t, (WIDTH-SIDEBAR_WIDTH+17, HEIGHT//25+75))
    pygame.draw.line(window, WHITE, (WIDTH-SIDEBAR_WIDTH+136, HEIGHT//25+78), (WIDTH-SIDEBAR_WIDTH+136, HEIGHT//25+102)) # seperator line

    window.blit(code_t, (WIDTH-SIDEBAR_WIDTH+144, HEIGHT//25+75))
    pygame.draw.line(window, WHITE, (WIDTH-SIDEBAR_WIDTH+197, HEIGHT//25+78), (WIDTH-SIDEBAR_WIDTH+197, HEIGHT//25+102)) # seperator line

    window.blit(cond_t, (WIDTH-SIDEBAR_WIDTH+207, HEIGHT//25+75))

    pygame.draw.line(window, WHITE, (WIDTH-SIDEBAR_WIDTH+15, HEIGHT//25+110), (WIDTH-15, HEIGHT//25+110)) # seperator line
    for component in components:
        component.rect.clamp_ip(editor_area)

    for component in components:
        window.blit(component.image, component.rect)

    if wire_node1:
        pygame.draw.line(window, RED, wire_node1.rect.center, mouse_pos, width=3)
    for wire in wires:
        pygame.draw.line(window, RED, wire.node1.rect.center, wire.node2.rect.center, width=3)


    pygame.display.flip()
    clock.tick(FPS)

pygame.quit()

