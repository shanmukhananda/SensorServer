import socket
TCP_IP = '127.0.0.1'
TCP_PORT = 1234
BUFFER_SIZE = 1024

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

while True:
    data = s.request.recv()
    print("received data:", data)
