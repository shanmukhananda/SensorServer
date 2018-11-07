import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.settimeout(3)
# s.connect(('127.0.0.1', 1234))
s.connect(("192.168.1.7", 9000))

while True:
    data = s.recv(1024)
    if not data:
        break
    print('Received bytes:', len(data))

s.close()
