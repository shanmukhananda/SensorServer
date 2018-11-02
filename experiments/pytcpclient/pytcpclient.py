import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('localhost', 1234))

while True:
    data = s.recv(1024)
    print('Received', repr(data))

s.close()


