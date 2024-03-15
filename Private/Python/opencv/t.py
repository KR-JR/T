import cv2

# 노트북의 카메라를 시작합니다.
cap = cv2.VideoCapture(0) # 0은 노트북의 기본 카메라를 의미합니다.

while True:
    # 현재 프레임을 캡쳐합니다.
    ret, frame = cap.read()
    
    # 캡쳐한 프레임을 화면에 표시합니다.
    cv2.imshow('Video', frame)
    
    # 'q' 키를 누르면 루프에서 나옵니다.
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 모든 작업을 마쳤다면, 캡처를 종료하고 모든 창을 닫습니다.
cap.release()
cv2.destroyAllWindows()


