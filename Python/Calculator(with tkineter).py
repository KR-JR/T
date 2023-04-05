from tkinter import *

def click(key):
    if key == '=':		# “=” 버튼이면 수식을 계산하여 결과를 표시한다.
        try:
            result = eval(entry.get())
            entry.delete(0, END) 
            entry.insert(END, str(result))
        except:
            entry.insert(END, "오류!")

    elif key == 'C': # 수식 전체 삭제 구현
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
'+/-', '0', '.', '=', ]

# 반복문으로 버튼을 생성한다.
i=0
for b in buttons:
    cmd = lambda x=b: click(x)
    b = Button(window,text=b,width=4,relief='ridge',command=cmd)
    b.grid(row=i//4+1,column=i%4)
    i += 1

# 엔트리 위젯은 맨 윗줄의 5개의 셀에 걸쳐서 배치된다. 
entry = Entry(window, width=33, bg="yellow")
entry.grid(row=0, column=0, columnspan=5)

window.mainloop()