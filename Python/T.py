file = open('C:/Code/Python/num.txt', 'w')

i = 10000

while True:
    num = i
    file.write(str(num))
    file.write(" ")
    i -= 1
    if i == 1:
        break

