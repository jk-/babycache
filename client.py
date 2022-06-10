import socket
import time
import datetime as dt

start_time = dt.datetime.today().timestamp()

HOST = "localhost"  # The server's hostname or IP address
PORT = 6969  # The port used by the server

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
s.send(f'ADD jon0 0\n'.encode())
data = s.recv(1024)
for i in range(100000):
    s.send(f'GET jon0\n'.encode())
    data = s.recv(1024)
time_diff = dt.datetime.today().timestamp() - start_time
print(time_diff, 100000/time_diff)
s.close()
