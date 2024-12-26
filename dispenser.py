import socket
import struct
import time
import RPi.GPIO as GPIO
from time import sleep

PIN = 16
# Servo configuration
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(PIN, GPIO.OUT)

pwm = GPIO.PWM(PIN, 50)
pwm.start(0)

def set_servo_angle(pwm, angle):
    """
    Set the servo to the specified angle.

    Parameters:
    - pwm: The PWM object controlling the servo.
    - angle: The desired angle (0 to 250 degrees).
    """
    duty_cycle = 5 + (angle / 180) * 5
    pwm.ChangeDutyCycle(duty_cycle)
    sleep(0.5)

# TCP client configuration
SERVER_IP = '0.0.0.0'  # Replace with the server's IP address
SERVER_PORT = 50054           # Replace with the server's port
BUFFER_SIZE = 1              # Receive 1 byte (U8)

def connect_to_server():
    """
    Attempt to connect to the server, retrying if the connection fails.
    """
    while True:
        try:
            print(f"Attempting to connect to server at {SERVER_IP}:{SERVER_PORT}...")
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client_socket.connect((SERVER_IP, SERVER_PORT))
            print("Connected to the server.")
            return client_socket
        except Exception as e:
            print(f"Connection failed: {e}. Retrying in 5 seconds...")
            time.sleep(5)

try:
    while True:
        # Establish connection to the server
        client_socket = connect_to_server()
        
        try:
            while True:
                # Wait for a command from the server
                data = client_socket.recv(BUFFER_SIZE)
                
                if not data:
                    print("Connection dropped by the server.")
                    break  # Exit to reconnect
                
                # Unpack U8 command
                command = struct.unpack("B", data)[0]
                
                # Process commands
                if command == 250:  # Open
                    print("Received command: OPEN. Setting angle to 250°.")
                    set_servo_angle(pwm, 250)
                elif command == 80:  # Close
                    print("Received command: CLOSE. Setting angle to 80°.")
                    set_servo_angle(pwm, 80)
                else:
                    print(f"Unknown command received: {command}")
        
        except Exception as e:
            print(f"Error during communication: {e}")
        
        finally:
            print("Closing socket and attempting to reconnect...")
            client_socket.close()

except KeyboardInterrupt:
    print("Script terminated by user.")

finally:
    # Cleanup resources
    print("Cleaning up GPIO and stopping PWM...")
    pwm.stop()
    GPIO.cleanup()
    print("Client script terminated.")
