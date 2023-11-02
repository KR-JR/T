import pyautogui
import pyperclip
import time
import threading
import os
import random  

os.chdir(os.path.dirname(os.path.abspath(__file__)))

# 보낼 메세지 리스트
messages = [
    "첫 번째 ",
    "두 번째 ",
    "세 번째 ",
    "네 번째 "
]

def send_message():
    # 랜덤한 Timer 시간 설정 (예: 5 ~ 15초 사이)
    random_timer = random.randint(5, 15)
    threading.Timer(random_timer, send_message).start()

    picPosition = pyautogui.locateOnScreen('C:\Code\Private\Python\IT\pic.png')
    print(picPosition)

    clickPosition = pyautogui.center(picPosition)
    pyautogui.doubleClick(clickPosition)    

    # 랜덤한 메시지 선택
    random_message = random.choice(messages)
    pyperclip.copy(random_message)
    pyautogui.hotkey("ctrl", "v")
    
    time.sleep(1)  

    pyautogui.write(["enter"])
    time.sleep(1)  

    pyautogui.write(["escape"])
    time.sleep(1)  

send_message()
