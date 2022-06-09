import socket

HOST = "localhost"  # The server's hostname or IP address
PORT = 6969  # The port used by the server

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
for i in range(500):
    s.send(f'ADD jon{i} {i}\0'.encode())
    data = s.recv(1024)
    s.send(f'GET jon{i}\0'.encode())
s.close()
