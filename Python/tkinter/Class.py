from tkinter import*


def aver():
    first = float(e1.get())
    second = float(e2.get())
    third = float(e2.get())
    average = (first + second + third) / 3
    la.config(text = average)
    
window = Tk()


l0 = Label(window, text = "20214224 이재훈")
l1 = Label(window, text = "국어")
l2 = Label(window, text = "영어")
l3 = Label(window, text = "수학")
l0.grid(row = 0, column = 1)
l1.grid(row = 1, column = 0)
l2.grid(row = 2, column = 0)
l3.grid(row = 3, column = 0)

la = Label(window)
la.grid(row = 4, column = 1)




e1 = Entry(window)
e2 = Entry(window)
e3 = Entry(window)
e1.grid(row = 1, column = 1)
e2.grid(row = 2, column = 1)
e3.grid(row = 3, column = 1)

b1 = Button(window, text = "평균", command = aver)
b1.grid(row = 4)



window.mainloop()