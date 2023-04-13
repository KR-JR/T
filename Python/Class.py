score = [[50, 70, 50, 30], [20, 50, 70, 90]]
sum = 0
print(score)
print(score[0][0], score[0][1], score[1][1], score[1][3])


for i in range(0, 2):
    for j in range(0, 4):
        sum += score[i][j]

print(sum)