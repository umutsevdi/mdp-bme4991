import socket
import cv2
import math
import time
import numpy as np
from enum import Enum

sock = None

class DIRECTION(Enum):
    END = -1
    NONE = 0
    LEFT = 1
    DOWN = 2
    UP = 3
    RIGHT = 4

old_direction = DIRECTION.NONE

def detect_eye_frame(frame):
    # Convert the frame to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect faces in the frame
    face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

    # If there is a face in the frame
    if len(faces) > 0:
        (x, y, w, h) = faces[0]

        # Detect eyes in the face region
        roi_gray = gray[y:y+h, x:x+w]
        eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')
        eyes = eye_cascade.detectMultiScale(roi_gray, 1.1, 3)

        # If there are two eyes in the face
        if len(eyes) == 2:
            # Get the coordinates of the two eyes
            (ex1, ey1, ew1, eh1) = eyes[0]
            (ex2, ey2, ew2, eh2) = eyes[1]

            # Draw a rectangle around each eye
            cv2.rectangle(frame, (x+ex1, y+ey1), (x+ex1+ew1, y+ey1+eh1), (255, 0, 0), 2)
            cv2.rectangle(frame, (x+ex2, y+ey2), (x+ex2+ew2, y+ey2+eh2), (255, 0, 0), 2)

            # Calculate the center of each eye
            eye1_center = (x + ex1 + ew1 // 2, y + ey1 + eh1 // 2)
            eye2_center = (x + ex2 + ew2 // 2, y + ey2 + eh2 // 2)

            # Calculate the direction of the eyes
            if eye1_center[0] < eye2_center[0]:
                # Eyes are looking left
                return DIRECTION.LEFT
            elif eye1_center[0] > eye2_center[0]:
                # Eyes are looking right
                return DIRECTION.RIGHT
            elif eye1_center[1] < eye2_center[1]:
                # Eyes are looking up
                return DIRECTION.UP
            elif eye1_center[1] > eye2_center[1]:
                # Eyes are looking down
                return DIRECTION.DOWN
            else:
                # Eyes are not looking in any direction
                return DIRECTION.DOWN
        else:
            # There are not two eyes in the frame
            return DIRECTION.NONE
    else:
        # There is not a face in the frame
        return DIRECTION.NONE

def stream_direction():
    global old_direction, sock
    # Create a VideoCapture object
    cap = cv2.VideoCapture(0)
    # Set the frame width and height
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

    while True:
        # Capture frame-by-frame
        ret, frame = cap.read()
        # Determine the eye direction
        i = 0
        direction = detect_eye_frame(frame)
        # Display the result
        cv2.imshow("Eye Direction", frame)
        cv2.putText(frame, f'Direction: {direction.name}', (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)
        cv2.imshow('Frame with Direction', frame)
        cv2.waitKey(1)
        # Break the loop if the user presses the 'q' key
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        # Print the current eye direction
        if direction.value != old_direction.value:
            old_direction = direction
            send(sock,direction)

    # Release the VideoCapture object
    send(sock,DIRECTION.END)
    cap.release()
    cv2.destroyAllWindows()

def set_udp(hostname: str, port: int):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    addr = socket.gethostbyname(hostname)
    sock.connect((addr, port))
    return sock


def send(sock, direction : DIRECTION):
    message = str(direction.value).encode()
    print(message, direction.name, direction.value)
    #sock.send(message)

def main():
    #    set_udp("pi.local",8080)
    stream_direction()




if __name__ == "__main__":
    main()
