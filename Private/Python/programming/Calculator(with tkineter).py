from tkinter import *
import tkinter.font as font

def click(key):
    if key == '=':		# “=” 버튼이면 수식을 계산하여 결과를 표시한다.
        current = entry.get()
        entry2.insert(END, str(current))

        try:
            result = eval(entry2.get())
            entry.delete(0, END)
            entry2.delete(0, END)
            entry.insert(END, str(result))
        except:
            entry.insert(END, "오류!")

    elif key == '%':
        current = entry2.get()
        remove = current[:-1]   

        try:
            trans = float(remove)
            entry1 = float(entry.get())
            per = trans * (entry1 / 100)
            entry2.insert(END, per)
            entry.delete(0, END)
        except:
            entry.insert(END, "오류!")        

    elif key == 'C': # 수식 전체 삭제 구현
        entry.delete(0, END)
        entry2.delete(0, END)

    elif key == 'CE': # 최근에 입력한 수식 삭제
        entry.delete(0, END)

    elif key == '<-': # 이전 입력했던 항 삭제
        current = entry.get()
        new = current[:-1]
        entry.delete(0, END)
        entry.insert(END, new)

    elif key == '1/x': # 1/x 구현 식 그대로
        try:
            current = float(entry.get())
            reciprocal = 1 / current # 역수(1/x) 계산 한 뒤 변수 할당
            entry.delete(0, END)
            entry.insert(END, str(reciprocal))
        except:
            entry.insert(END, "오류!")

    elif key == 'x^2': # 제곱 구현
        try:
            current = float(entry.get())
            square = current ** 2 # current의 제곱 한 뒤 변수 할당
            entry.delete(0, END)
            entry.insert(END, str(square))
        except:
            entry.insert(END, "오류!")

    elif key == '√x': # 루트 구현
        try:
            current = float(entry.get())
            loot = current ** 0.5 # 0.5 제곱 한 뒤 변수 할당
            entry.delete(0, END)
            entry.insert(END, str(loot))
        except:
            entry.insert(END, "오류!")
    
    elif key == '+/-': # 양수/음수 부호전환 구현
        try:
            current = float(entry.get())
            sign = current * -1 # 곱하기 -1
            entry.delete(0, END)
            entry.insert(END, str(sign))
        except:
            entry.insert(END, "오류!")   

    elif key in ['+', '-', '*', '/']: # operator 입력
        current = entry.get()
        entry2.insert(END, str(current))
        entry2.insert(END, key)
        entry.delete(0, END)
    
    else:
        entry.insert(END, key)
        


window = Tk()
window.title("표준 계산기")

# 버튼 위치 및 종류 정하기(대충)
buttons = [
'%', 'CE', 'C', '<-',
'1/x', 'x^2','√x', '/',
'7', '8', '9', '*', 
'4', '5', '6', '-', 
'1', '2', '3', '+', 
'+/-', '0', '.', '=']

# 반복문으로 버튼을 생성한다.
i=0
button_font = font.Font(size = 40) # 버튼 폰트(사이즈)
for b in buttons:
    cmd = lambda x = b: click(x)
    b = Button(window, text  = b, width = 4, relief = 'ridge', command = cmd, font = button_font) # 버튼 생성
    b.grid(row = (i // 4 + 1) + 1 , column = i % 4) # 버튼 위치 조정
    i += 1

entry_font = font.Font(size = 40) # 엔트리 폰트(사이즈)
# 엔트리 위젯은 맨 윗줄의 5개의 셀에 걸쳐서 배치된다. 
entry = Entry(window, width = 18, bg = "yellow", font = entry_font) # entry 생성
entry.grid(row = 1, column = 0, columnspan = 4) # entry 위치 지정
entry2 = Entry(window, width = 18, bg = "gray", font = entry_font) # entry2 생성
entry2.grid(row = 0, column = 0, columnspan = 4) # entry2 위치 지정




window.mainloop()