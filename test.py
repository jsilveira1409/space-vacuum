import socket
import struct

# MKR server IP and port
server_ip = "192.168.1.181"  # Replace with your MKR's IP address
server_port = 50054

# Create a client socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((server_ip, server_port))

try:
    while True:
        # Send an integer command in big-endian format
        command = int(input("Enter an integer command to send: "))
        client_socket.send(struct.pack('>i', command))  # '>i' for big-endian 4-byte integer

        # Receive response in big-endian format
        response = client_socket.recv(4)  # Receive 4 bytes
        if not response:
            break
        response_value = struct.unpack('>i', response)[0]  # '>i' for big-endian
        print(f"Received response: {response_value}")
except KeyboardInterrupt:
    print("Client shutting down.")
finally:
    client_socket.close()
