#1. input
#val1 = input("입력하세요: ")
#print("입력값: ", type(val1))

#2. 복소수
#comp1 = 3 + 4j
#comp2 = complex(3, 4)
#print(abs(comp1))
#print(abs(comp2))

# 실습 3.
#sum = 0
#for i in range(1, 101):
#    sum += i
    
#print(sum)

# 실습 3-1
#result = sum(list(range(101)))
#print(result)

# 실습 3-2
#sum2 = sum([i For i in range(101)])

# 실습 4. 100까지 홀수와 짝수의 합을 각각 출력하는 프로그램을 작성하시오

#odd = sum(list(range(1, 101, 2)))
#even = sum(list(range(2, 101, 2)))

#print("홀수합: ", odd,"\n작수합: ", even)

# 4-1
#odd = 0; even = 0
#for i in range(101):
#    if(i % 2 == 0):
#        even += i
#    else:
#        odd += i
#print("홀수합: ", odd,"\n작수합: ", even)

# 4-2
#even = 0; odd = 0
#even = sum([i for i in range(101) if i % 2 == 0])
#odd = sum([i for i in range(101) if i % 2 == 1])

# 실습5 리스트 정렬, sort vs sorted
#exlist = [1, 3, 9, 2, 5]
#relist1 = exlist.sort()
#print(relist1)

#exlist = [1, 3, 9, 2, 5]
#relist2 = sorted(exlist)
#print(relist2)

#l1 = [1, 2, 3]
#l2 = l1
#l2[2] = 100
#print(l1)

#l3 = list(l1)
#l3[2] = 1000
#print(l1)
#print(l2)


#a = [1, 2, 3]
#print(a[::-1]) # 연순으로 출력
#print(a[:-1]) # 제일 마지막 item 제외하고 출력

#prices = [135, -534, 922, 356, -992, 217]
#mprices = [i if i > 0 else 0 for i in prices]
#print(mprices)

origin = [10, 20, 30, 40, 50]
new = [sum(origin[0:i+1]) for i in range(len(origin))]
print("origin: ", origin)
print('new: ', new)
