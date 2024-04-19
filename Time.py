import time
import winsound

while True:
    # 100초 동안 대기
    time.sleep(100)
    
    # 소리 재생: 440Hz에서 1초 동안 소리를 낸다 (A4 음)
    winsound.Beep(440, 500)
 