import numpy as np
import cv2
import os
import math
import re

title = 'Draw Shapes'
image_path = "C:/Code/T/Private/Python/vidioprocessing/image.jpg"
image = cv2.imread(image_path)

# 이미지가 로드되지 않았다면, 예외 처리
if image is None:
    raise Exception("영상파일 읽기 에러")

# 이미지를 640x480으로 리사이즈
image = cv2.resize(image, (640, 480))
original_image = np.copy(image)

mode = 'NULL'  # 시작 모드
points = []  # 다각형을 이루는 점들

# 저장 디렉토리 구현
save_directory = "saved_images"
if not os.path.exists(save_directory):
    os.makedirs(save_directory)

# 이름을 순서대로 저장 (같은 이름이 존재할 경우 다음 순으로 저장)
def get_next_filename():
    files = os.listdir(save_directory)
    max_index = 0
    for file in files:
        match = re.match(r"ljh(\d+).jpg", file)
        if match:
            index = int(match.group(1))
            if index > max_index:
                max_index = index
    return f"ljh{max_index + 1:04d}.jpg"

# ROI 이미지를 저장 함수
def save_image(roi):
    filename = get_next_filename()
    filepath = os.path.join(save_directory, filename)
    cv2.imwrite(filepath, roi)
    print(f"이미지 저장됨: {filepath}")

# 주어진 좌표에 따라 타원 ROI를 추출하고 저장
def extract_and_save_ellipse_roi(center, size):
    mask = np.zeros(original_image.shape[:2], dtype=np.uint8)
    cv2.ellipse(mask, center, size, 0, 0, 360, 255, -1)
    roi = cv2.bitwise_and(original_image, original_image, mask=mask)
    x, y = center[0] - size[0], center[1] - size[1]
    roi_cropped = roi[y:y+2*size[1], x:x+2*size[0]]
    save_image(roi_cropped)


# 타원 모드 핸들
def handle_ellipse_mode(event, x, y, flags, param):
    global image, points

    if event == cv2.EVENT_LBUTTONDOWN:
        points = [(x, y)]
        
    elif event == cv2.EVENT_MOUSEMOVE and flags == cv2.EVENT_FLAG_LBUTTON and points:
        temp_image = np.copy(original_image)
        cv2.ellipse(temp_image, points[0], (abs(x - points[0][0]), abs(y - points[0][1])), 0, 0, 360, (255, 255, 0), 2)
        display_mode(temp_image, mode)
        
    elif event == cv2.EVENT_LBUTTONUP and points:
        dx, dy = points[0][0] - x, points[0][1] - y
        size = (abs(dx), abs(dy))
        cv2.ellipse(original_image, points[0], size, 0, 0, 360, (255, 255, 0), 2)
        extract_and_save_ellipse_roi(points[0], size)
        print("Ellipse created", points)
        display_mode(original_image, mode)
        points = []



# 다각형 각도 계산 함수
def calculate_angle(p1, p2, p3):
    # 세 점의 각을 p2를 꼭짓점을 기준으로 각도를 계산
    v1 = np.array([p1[0] - p2[0], p1[1] - p2[1]])
    v2 = np.array([p3[0] - p2[0], p3[1] - p2[1]])
    dot = np.dot(v1, v2) # 내적 계산
    det = np.linalg.det([v1, v2]) # 행렬식 적용
    angle = np.abs(np.degrees(np.arctan2(det, dot)))
    return angle, det

def handle_polygon_mode(event, x, y, flags, param):
    global points, original_image, mode

    if event == cv2.EVENT_LBUTTONDOWN:
        new_point = (x, y)

        if len(points) >= 2:
            angle, det = calculate_angle(points[-2], points[-1], new_point)
            if angle > 180 or det < 0:  # 내각이 180 이상이거나 시계 방향이 아니면 그리지 않음
                print("잘못된 각도입니다.")
                return
            
        # 다각형 닫기
        if len(points) > 0 and np.linalg.norm(np.array(points[0]) - np.array(new_point)) < 20:
            if len(points) >= 3:
                cv2.line(original_image, points[-1], points[0], (0, 0, 255), 2)
                points.append(points[0])
                # roi 추출 및 저장
                mask = np.zeros(original_image.shape[:2], dtype=np.uint8)
                cv2.fillPoly(mask, [np.array(points, np.int32)], 255)
                roi = cv2.bitwise_and(original_image, original_image, mask=mask)
                save_image(roi)
                print("다각형을 닫습니다.", points)
                display_mode(original_image, mode)
                points = []
            else:
                print("다각형을 구성하기에 충분하지 않습니다.")
                
        # 선 그리기
        else: 
            points.append(new_point)
            if len(points) > 1:
                cv2.line(original_image, points[-1], points[-2], (0, 0, 255), 2)
            print(f"추가된 포인트 {new_point}, 총 포인트: {len(points)}")
            display_mode(original_image, mode)            




# 모드를 이미지에 표시하는 함수
def display_mode(image, mode):
    overlay = image.copy()
    cv2.putText(overlay, f"Mode: {mode}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2, cv2.LINE_AA)
    cv2.addWeighted(overlay, 0.4, image, 0.6, 0, image)
    cv2.imshow(title, image)

# 마우스 처리 함수
def onMouse(event, x, y, flags, param):
    global image, points, mode

    if mode == 'Ellipse':
        handle_ellipse_mode(event, x, y, flags, param)
    elif mode == 'Polygon':
        handle_polygon_mode(event, x, y, flags, param)

# 키보드 이벤트 처리
def onChange(key):
    global mode, original_image, points
    # 키 1을 입력했을 때 (타원)
    if key == ord('1'):
        if mode == 'Ellipse':
            mode = 'NULL'
        else:
            mode = 'Ellipse'
            points = []
    # 키 2를 입력했을 때(다각형) 
    elif key == ord('2'):
        if mode == 'Polygon':
            mode = 'NULL'
        else:
            mode = 'Polygon'
            points = []
    original_image = np.copy(image)
    display_mode(original_image, mode)

cv2.namedWindow(title, cv2.WINDOW_NORMAL)
cv2.resizeWindow(title, 640, 480)
display_mode(original_image, mode)
cv2.setMouseCallback(title, onMouse)

while True:
    key = cv2.waitKey(1) & 0xFF
    if key == 27:  # ESC to quit
        break
    elif key in [ord('1'), ord('2')]:
        onChange(key)

cv2.destroyAllWindows()
