import turtle

t = turtle.Turtle()
t.shape("turtle")
t.pensize("5")

#초기 위치설정
t.penup()
t.goto(-100, 100)
t.pendown()
t.color("yellow")


i = 0

#5번 선 긋기 모양:STAR
while True:
    if i < 6:
        t.forward(200)
        t.right(144)
        i = i+1
    else:
        break


turtle.done()
