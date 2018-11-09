import socket

# UDP_IP = "127.0.0.1"
# UDP_IP = "192.168.1.7"
UDP_IP = "0.0.0.0"

# UDP_PORT = 1234
UDP_PORT = 9000

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM, socket.IPPROTO_UDP) # UDP
# sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind((UDP_IP, UDP_PORT))
sock.settimeout(3)

while True:
    data, addr = sock.recvfrom(2048) # buffer size is 1024 bytes
    print("received message len:", len(data))
