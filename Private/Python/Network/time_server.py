import socket
from _thread import *
import time

def threaded_client(connection):
    while True:
        data = connection.recv(1024).decode('ascii').strip()
        if not data or data.lower() == 'quit':
            break

        if data.lower() in ['time', 'current time']:
            current_time = time.ctime(time.time()) + "\r\n"
            connection.send(current_time.encode('ascii'))
        else:
            connection.send('잘못된 명령어입니다.\r\n'.encode('utf-8'))

    connection.close()

def tcp_time_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    host = 'localhost'
    port = 5678
    server_socket.bind((host, port))
    server_socket.listen(5)
    
    while True:
        client_socket, addr = server_socket.accept()
        start_new_thread(threaded_client, (client_socket,))


tcp_time_server()
