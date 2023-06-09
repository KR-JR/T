<<<<<<< HEAD
file = open('C:/Code/Python/num.txt', 'w')

i = 10000

while True:
    num = i
    file.write(str(num))
    file.write(" ")
    i -= 1
    if i == 1:
        break

=======
import csv

file = open('C:/Code/Python/average.csv', "r")
myData = csv.reader(file)

sum_score = 0

for line in myData:
    sum_score += int(line[1])
    
average_score = sum_score / 5

print(average_score)
>>>>>>> origin/master
