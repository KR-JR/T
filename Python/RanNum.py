import random

file = open('C:/Code/Python/num.txt', 'w')

for i in range(10000):
    random_num = random.randrange(1, 10000)
    file.write(str(random_num))
    file.write(" ")