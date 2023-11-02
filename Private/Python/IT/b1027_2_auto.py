import pyautogui
import pyperclip
import time

# # ex0. 사용가능한 키
# key = pyautogui.KEYBOARD_KEYS #사용가능한 특수키 확인
# print(key)

# # ex1. 마우스 좌표 가져오기
# for _ in range(20):
#     print(pyautogui.position())
#     time.sleep(0.5)

#좌표 좌측 상단이 0,0이다.

#shift + F10 마우스 우클릭

#pyautogui.typewrite("ABC")

#서울 날씨 검색하는 코드 selenium으로 작성

날씨 = ["서울 날씨","시흥 날씨","청주 날씨","부산 날씨","강원도 날씨"]

addr_x = 1145
addr_y = 53
start_x = 992
start_y = 220
end_x = 1656
end_y = 635

for 지역날씨 in 날씨:
    pyautogui.moveTo(addr_x,addr_y,1)
    time.sleep(0.2)
    pyautogui.click()
    time.sleep(0.2)
    pyautogui.write("www.naver.com",interval=0.1)
    pyautogui.write(["enter"])
    time.sleep(1)

    pyperclip.copy(지역날씨)
    pyautogui.hotkey("ctrl", "v")
    time.sleep(0.5)
    pyautogui.write(["enter"])
    time.sleep(1)
    저장경로 = '10. 오토마우스를 활용한 웹페이지 자동화\\' + 지역날씨 +'.png'
    pyautogui.screenshot(저장경로, region=(start_x,start_y,end_x-start_x,end_y-start_y))

# 과제 프로젝트 11 오토마우스를 활용한 PC카카오톡 자동화
# 다음주 수업전까지 교탁에 제출
# 껍데기 x
# pipinstall schedule
# selenium + pyautogui를 같이 사용해야함
# 마우스 올렸을때 마우스 클릭했을때 아무것도 안했을때 다 상태가 다르다 이걸 찾아 사용
# png파일을 만든후 사진으로 좌표 추출
# picPosition = pyautogui.locationOnScreen('plc.png')
# 내가 보낼 문자열을 리스트로 만든후 보내고 보내는시간 time.sleep을 랜덤하게 보내기 하기
# 한 페이지는 소스코드를 넣고 코드 배경을 흰색으로 바꿔서 하기.
# 프린트해서 제출.