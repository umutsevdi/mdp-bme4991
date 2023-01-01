import socket
import cv2
import math
import time
from enum import Enum

sock = None

class DIRECTION(Enum):
    END = -1
    NONE = 0
    LEFT = 1
    DOWN = 2
    UP = 3
    RIGHT = 4

# Function to detect the eyes in the frame and determine their direction
def detect_eye_direction(frame) -> DIRECTION:
    # Convert the frame to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # Load the Haar cascade for detecting eyes
    eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')
    
    # Detect eyes in the frame
    eyes = eye_cascade.detectMultiScale(gray, 1.1, 3)
    
    # Check if eyes were detected
    if len(eyes) == 0:
        return DIRECTION.NONE
    
    # Initialize variables to store the coordinates of the left and right eyes
    left_eye = None
    right_eye = None
    
    # Iterate over the detected eyes and determine which is the left eye and which is the right eye
    for (x,y,w,h) in eyes:
        if x < frame.shape[1] / 2:
            left_eye = (x,y,w,h)
        else:
            right_eye = (x,y,w,h)
    
    # Check if both eyes were detected
    if left_eye is None or right_eye is None:
        return DIRECTION.NONE
    
    # Calculate the center of the left and right eyes
    left_eye_center = (left_eye[0] + left_eye[2] // 2, left_eye[1] + left_eye[3] // 2)
    right_eye_center = (right_eye[0] + right_eye[2] // 2, right_eye[1] + right_eye[3] // 2)
    
    # Calculate the angle between the eyes
    dx = right_eye_center[0] - left_eye_center[0]
    dy = right_eye_center[1] - left_eye_center[1]
    angle = math.atan2(dy, dx) * 180.0 / math.pi
    
    # Check the direction based on the angle
    if angle > 45:
        return DIRECTION.RIGHT
    elif angle < -45:
        return DIRECTION.LEFT
    elif dy > 0:
        return DIRECTION.DOWN
    else:
        return DIRECTION.UP

def stream_direction():
    # Create a VideoCapture object
    cap = cv2.VideoCapture(0)

    while True:
        # Capture frame-by-frame
        ret, frame = cap.read()
        # Determine the eye direction
        direction = detect_eye_direction(frame)
        # Print the current eye direction
        print(direction)
        # Break the loop if the user presses the 'q' key
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        time.sleep(0.3)
        send(sock,direction)

    # Release the VideoCapture object
    cap.release()
    cv2.destroyAllWindows()

def set_udp(hostname: str, port: int):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    addr = socket.gethostbyname(hostname)
    sock.connect((addr, port))
    return sock


def send(sock, direction : DIRECTION):
    message = str(direction.value).encode()
    print(message, direction, direction.value)
    #sock.send(message)

def main():
    #    set_udp("pi.local",8080)
    stream_direction()




if __name__ == "__main__":
    main()
