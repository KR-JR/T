import socket

def tcp_time_client(server_ip, server_port):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        client_socket.connect((server_ip, server_port))
        
        client_socket.sendall('time'.encode('utf-8'))
        received = client_socket.recv(1024)
        print(f"서버 시간: {received.decode('utf-8').strip()}")

        client_socket.sendall('current time'.encode('utf-8'))
        received = client_socket.recv(1024)
        print(f"서버 현재 시간: {received.decode('utf-8').strip()}")

        client_socket.sendall('quit'.encode('utf-8'))

tcp_time_client('localhost', 5678)
