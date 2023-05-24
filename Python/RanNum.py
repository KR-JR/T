import random

file = open('C:/Code/Python/num.txt', 'w')

for i in range(100000):
    random_num = random.randrange(1, 1000000)
    file.write(str(random_num))
    file.write("\n")