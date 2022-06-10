import socket
import time
import datetime as dt

start_time = dt.datetime.today().timestamp()
i = 0


HOST = "localhost"  # The server's hostname or IP address
PORT = 6969  # The port used by the server

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
for i in range(50000):
    s.send(f'ADD jon{i} {i};;'.encode())
    data = s.recv(1024)
    s.send(f'GET jon{i};;'.encode())
    data = s.recv(1024)
    time_diff = dt.datetime.today().timestamp() - start_time
    i += 1
    print(i / time_diff)
s.close()
