import pygetwindow as gw
from PIL import ImageGrab
import time
import logging

# 로깅 설정
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

def capture_window(window_title, interval=5):
    logging.info("Starting the window capture routine.")
    while True:
        try:
            # 주어진 타이틀과 일치하는 윈도우 찾기
            windows = gw.getWindowsWithTitle(window_title)
            if windows:
                window = windows[0]
                # 윈도우를 전면으로 가져오기
                window.activate()
                time.sleep(1)  # 윈도우 활성화 대기
                logging.info(f"Captured window titled '{window.title}' at position {window.left}, {window.top}, {window.width}, {window.height}.")
                
                # 윈도우의 화면 영역을 스크린샷으로 캡쳐
                bbox = (window.left, window.top, window.right, window.bottom)
                screenshot = ImageGrab.grab(bbox)
                # 저장하거나 표시할 경우 아래 코드 사용
                # screenshot.save("screenshot.png")
                screenshot.show()
            else:
                logging.warning(f"No window with title '{window_title}' found.")
        except Exception as e:
            logging.error(f"An error occurred: {e}")

        # 설정한 간격마다 반복
        time.sleep(interval)

# 함수 실행 예제
capture_window("Maplestory Worlds", interval=10)
