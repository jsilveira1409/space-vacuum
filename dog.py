import cv2
import socket
import struct
import time
import RPi.GPIO as GPIO
from time import sleep
from ultralytics import YOLO
import subprocess
import threading

# Servo configuration
PIN = 16
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(PIN, GPIO.OUT)

pwm = GPIO.PWM(PIN, 50)
pwm.start(0)


# Load the YOLOv5 model (adjust the model path as needed)
model = YOLO("yolo11n.pt")

# Start libcamera feed
subprocess.Popen(["libcamera-vid", "-n", "-t", "0", "--inline", "--listen", "-o", "tcp://0.0.0.0:8888", "--width", "1280", "--height", "720"])

time.sleep(5)

# Open TCP stream
tcp_address = 'tcp://0.0.0.0:8888'
cap = cv2.VideoCapture(tcp_address)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)  # Set width to 1280 (wide)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)  # Set height to 720 (HD)

if not cap.isOpened():
    print(f"Error: Unable to open video stream at {tcp_address}")
    exit()

# TCP client configuration for control
SERVER_IP = '0.0.0.0'  # Replace with the server's IP address
SERVER_PORT = 50054     # Replace with the server's port
BUFFER_SIZE = 1         # Receive 1 byte (U8)

# Shared variables
latest_frame = None
frame_lock = threading.Lock()

def set_servo_angle(pwm, angle):
    duty_cycle = 5 + (angle / 180) * 5
    pwm.ChangeDutyCycle(duty_cycle)
    sleep(0.5)
    pwm.ChangeDutyCycle(0)


def capture_frames():
    global latest_frame
    while True:
        ret, frame = cap.read()
        if ret:
            # Capture the most recent frame
            with frame_lock:
                latest_frame = frame
        else:
            print("Error: Failed to capture frame")

def run_inference():
    global latest_frame
    while True:
        if latest_frame is not None:
            # Run YOLO inference only on the latest frame
            with frame_lock:
                frame = latest_frame.copy()  # Create a copy to avoid overwriting in capture thread

            # Run YOLO inference on the captured frame
            results = model(frame)

            # Process results
            for result in results:
                boxes = result.boxes

                # Handle case when no detections are made
                if len(boxes.xyxy) == 0:
                    print("No objects detected in this frame.")
                    continue

                # Process each detection
                for i, box in enumerate(boxes):
                    confidence = box.conf.item()  # Confidence score for this detection
                    class_id = int(box.cls.item())  # Class ID (e.g., 'dog', 'person')

                    # Map the class ID to class name
                    label = result.names[class_id]

                    print(f"Detected {label} with confidence {confidence}")

                    # Example logic to control servo based on detection
                    if label.lower() == 'dog' and confidence > 0.3:
                        print("Dog detected! Opening lid...")
                        set_servo_angle(pwm, 200)  # Open lid
                        sleep(0.5)  # Wait for the lid to open
                        set_servo_angle(pwm, 175)  # Close lid
                        sleep(0.5)  # Wait for the lid to open
                        set_servo_angle(pwm, 200)  # Open lid
                        sleep(0.5)  # Wait for the lid to open
                        set_servo_angle(pwm, 175)  # Close lid
                        print("Lid closed.")
                    # Optionally, you can save or display the image with bounding boxes
                    img = result.plot()  # Annotated image with bounding boxes
                    cv2.imwrite("detected.jpg", img)  # Save the image to a file



def connect_to_server():
    while True:
        try:
            print(f"Attempting to connect to server at {SERVER_IP}:{SERVER_PORT}...")
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client_socket.connect((SERVER_IP, SERVER_PORT))
            print("Connected to the server.")
            return client_socket
        except socket.error as e:
            print(f"Socket error: {e}. Retrying in 5 seconds...")
            time.sleep(5)
        except Exception as e:
            print(f"Connection failed: {e}. Retrying in 5 seconds...")
            time.sleep(5)

def handle_socket_commands(client_socket):
    while True:
        try:
            data = client_socket.recv(BUFFER_SIZE)
            if not data:
                print("Connection dropped by the server.")
                break  # Exit to reconnect

            command = struct.unpack("B", data)[0]
            
            if command == 250:  # Open
                print("Received command: OPEN. Setting angle to 250°.")
                set_servo_angle(pwm, 200)
                set_servo_angle(pwm, 175)
                
            elif command == 80:  # Close
                print("Received command: CLOSE. Setting angle to 80°.")
                set_servo_angle(pwm, 175)
            else:
                print(f"Unknown command received: {command}")
        except socket.timeout:
            continue

try:
    # Start threads for capturing frames and running inference
    capture_thread = threading.Thread(target=capture_frames, daemon=True)
    inference_thread = threading.Thread(target=run_inference, daemon=True)

    capture_thread.start()
    inference_thread.start()

    # Establish connection to the server
    client_socket = connect_to_server()
    client_socket.settimeout(0.1)

    # Handle socket commands in the main thread
    handle_socket_commands(client_socket)

except KeyboardInterrupt:
    print("Script terminated by user.")

finally:
    # Cleanup resources
    print("Cleaning up GPIO and stopping PWM...")
    pwm.stop()
    GPIO.cleanup()
    print("Client script terminated.")


