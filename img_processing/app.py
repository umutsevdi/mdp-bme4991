import socket
from enum import Enum
from math import hypot

import cv2
import dlib
import numpy as np

sock = None
frame = None
gray = None


class DIRECTION(Enum):
    END = -1
    NONE = 0
    LEFT = 1
    DOWN = 2
    UP = 3
    RIGHT = 4


frame_directions: list[DIRECTION] = []


def set_udp(hostname: str, port: int):
    new_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    addr = socket.gethostbyname(hostname)
    new_socket.connect((addr, port))
    return new_socket


def send(direction: DIRECTION):
    global sock
    message = str(direction.value).encode()
    print(message, direction, direction.value)
    # sock.send(message)


def midpoint(p1, p2):
    return int((p1.x + p2.x) / 2), int((p1.y + p2.y) / 2)


font = cv2.FONT_HERSHEY_PLAIN


def get_direction(left_eye, right_eye, gaze_left) -> DIRECTION:
    direction: DIRECTION = DIRECTION.NONE
    if left_eye > 4.5 and right_eye > 4.5:
        if left_eye < 6.5 and right_eye < 6.5:
            direction = DIRECTION.DOWN
    elif gaze_left <= 1.2:
        direction = DIRECTION.RIGHT
    elif gaze_left > 4:
        direction = DIRECTION.LEFT
    else:
        direction = DIRECTION.UP
    return direction


def get_max_direction() -> DIRECTION:
    global frame_directions
    m: dict[DIRECTION, int] = {
        DIRECTION.LEFT: 0,
        DIRECTION.UP: 0,
        DIRECTION.DOWN: 0,
        DIRECTION.RIGHT: 0,
        DIRECTION.NONE: 0,
    }
    for i in frame_directions:
        m[i] += 1
    frame_directions.clear()

    max_dir = DIRECTION.NONE
    for i in m.keys():
        if m[i] > m[max_dir]:
            max_dir = i
    return max_dir


def get_blinking_ratio(eye_points, facial_landmarks):
    global frame
    left_point = (facial_landmarks.part(eye_points[0]).x, facial_landmarks.part(eye_points[0]).y)
    right_point = (facial_landmarks.part(eye_points[3]).x, facial_landmarks.part(eye_points[3]).y)
    center_top = midpoint(facial_landmarks.part(eye_points[1]), facial_landmarks.part(eye_points[2]))
    center_bottom = midpoint(facial_landmarks.part(eye_points[5]), facial_landmarks.part(eye_points[4]))

    hor_line = cv2.line(frame, left_point, right_point, (0, 255, 0), 2)
    ver_line = cv2.line(frame, center_top, center_bottom, (0, 255, 0), 2)

    hor_line_lenght = hypot((left_point[0] - right_point[0]), (left_point[1] - right_point[1]))
    ver_line_lenght = hypot((center_top[0] - center_bottom[0]), (center_top[1] - center_bottom[1]))

    ratio = hor_line_lenght / ver_line_lenght
    return ratio


def get_gaze_ratio(eye_points, facial_landmarks):
    global gray
    left_eye_region = np.array([(facial_landmarks.part(eye_points[0]).x, facial_landmarks.part(eye_points[0]).y),
                                (facial_landmarks.part(eye_points[1]).x, facial_landmarks.part(eye_points[1]).y),
                                (facial_landmarks.part(eye_points[2]).x, facial_landmarks.part(eye_points[2]).y),
                                (facial_landmarks.part(eye_points[3]).x, facial_landmarks.part(eye_points[3]).y),
                                (facial_landmarks.part(eye_points[4]).x, facial_landmarks.part(eye_points[4]).y),
                                (facial_landmarks.part(eye_points[5]).x, facial_landmarks.part(eye_points[5]).y)],
                               np.int32)
    # cv2.polylines(frame, [left_eye_region], True, (0, 0, 255), 2)

    height, width, _ = frame.shape
    mask = np.zeros((height, width), np.uint8)
    cv2.polylines(mask, [left_eye_region], True, 255, 2)
    cv2.fillPoly(mask, [left_eye_region], 255)
    eye = cv2.bitwise_and(gray, gray, mask=mask)

    min_x = np.min(left_eye_region[:, 0])
    max_x = np.max(left_eye_region[:, 0])
    min_y = np.min(left_eye_region[:, 1])
    max_y = np.max(left_eye_region[:, 1])

    gray_eye = eye[min_y: max_y, min_x: max_x]
    _, threshold_eye = cv2.threshold(gray_eye, 70, 255, cv2.THRESH_BINARY)
    if threshold_eye is not None:
        height, width = threshold_eye.shape
        left_side_threshold = threshold_eye[0: height, 0: int(width / 2)]
        left_side_white = cv2.countNonZero(left_side_threshold)

        right_side_threshold = threshold_eye[0: height, int(width / 2): width]
        right_side_white = cv2.countNonZero(right_side_threshold)

        if left_side_white == 0:
            gaze_ratio = 1
        elif right_side_white == 0:
            gaze_ratio = 5
        else:
            gaze_ratio = left_side_white / right_side_white
        return gaze_ratio
    return 0


def stream_frame():
    cap = cv2.VideoCapture(0)

    detector = dlib.get_frontal_face_detector()
    predictor = dlib.shape_predictor("shape_predictor_68_face_landmarks.dat")

    while True:
        global frame, gray, frame_directions
        _, frame = cap.read()
        #  new_frame = np.zeros((500, 500, 3), np.uint8)
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        faces = detector(gray)
        if len(faces) == 0:
            pass
        for face in faces:
            x, y = face.left(), face.top()
            x1, y1 = face.right(), face.bottom()
            cv2.rectangle(frame, (x, y), (x1, y1), (0, 255, 0), 2)

            landmarks = predictor(gray, face)

            # Detect blinking
            left_eye_ratio = get_blinking_ratio([36, 37, 38, 39, 40, 41], landmarks)
            right_eye_ratio = get_blinking_ratio([42, 43, 44, 45, 46, 47], landmarks)
            blinking_ratio = (left_eye_ratio + right_eye_ratio) / 2

            # Gaze detection
            gaze_ratio_left_eye = get_gaze_ratio([36, 37, 38, 39, 40, 41], landmarks)
            gaze_ratio_right_eye = get_gaze_ratio([42, 43, 44, 45, 46, 47], landmarks)
            gaze_ratio = (gaze_ratio_right_eye + gaze_ratio_left_eye) / 2
            direction = get_direction(left_eye_ratio, right_eye_ratio, gaze_ratio_left_eye)
            frame_directions.append(direction)
            if len(frame_directions) >= 10:
                send(get_max_direction())

            cv2.putText(frame, direction.name, (50, 150), font, 4, (255, 0, 0), 2)

        cv2.imshow("Frame", frame)
        # cv2.imshow("New frame", new_frame)
        key = cv2.waitKey(1)
        if key == 27:
            send(DIRECTION.END)
            break

    cap.release()
    cv2.destroyAllWindows()


def main():
    global sock
    # sock = set_udp("pi.local", 8080)
    stream_frame()


if __name__ == "__main__":
    main()
