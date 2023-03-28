x = int(input("x= "))
y = int(input("y= "))
z = int(input("z= "))

if x > y:
    if x > z:
        print("x가 가장 큽니다", x)
    else:
        print("z가 가장 큽니다", z)
else:
    if y > z:
        print("y가 가장 큽니다", y)
    else:
        print("z가 가장 큽니다", z)
