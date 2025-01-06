import socket

class Sock:
    def __init__(self):
        self.HOST = '127.0.0.1'
        self.PORT = 8080

        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.bind((self.HOST, self.PORT))
        self.server_socket.listen(1)
        self.server_socket.setblocking(False)

        self.client_socket = None
        self.client_address = None

    def connect(self):
        try:
            self.client_socket, self.client_address = self.server_socket.accept()
            return True
        except BlockingIOError:
            return False


    def receive_data(self):
        # Receive data from the client
        try:
            data = self.client_socket.recv(1024).decode()
            if data:
                return data
        except BlockingIOError:
            pass
            return None

    def send_msg(self, msg):
        try:
            self.client_socket.sendall(msg.encode('utf-8'))
            return True
        except BlockingIOError:
            return False


    def end_sock(self):
        self.client_socket.close()



