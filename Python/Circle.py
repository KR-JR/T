import turtle

t = turtle.Turtle()
t.shape("turtle")
t.pensize("8")

t.penup()
t.goto(-250, 0)
t.pendown()


for i in range(3):
    if i == 0:
        t.pencolor("blue")
    elif i ==1:
        t.pencolor("black")
    elif i ==2:
        t.pencolor("red")
        t.circle(100)
        break
    t.circle(100)
    t.penup()
    t.forward(250)
    t.pendown()


t.penup()
t.goto(-125, -75)
t.pendown()


for i in range(2):
    if i == 0:
        t.pencolor("yellow")
    elif i ==1:
        t.pencolor("green")
        t.circle(100)
        break
    t.circle(100)
    t.penup()
    t.forward(250)
    t.pendown()








turtle.done()
