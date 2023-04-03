import turtle

t = turtle.Turtle()
t.shape("turtle")
t.pensize("5")

#초기 위치설정
t.penup()
t.goto(-100, 100)
t.pendown()

#5번 선 긋기 모양:STAR
for i in range(5):
    t.forward(200)
    t.right(144)



turtle.done()
