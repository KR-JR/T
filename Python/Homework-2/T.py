def Addlist(num, numbers):
    numbers.append(num)

numbers = []  # 입력을 저장할 리스트

for i in range(3):
    num = input()
    Addlist(num, numbers)
    print(numbers)
