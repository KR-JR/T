def times(input_num):
    result = 0
    i = 1
    for i in range(1, 10):
        result = input_num * i
        print(result)


input_num = int(input("숫자 입력:"))

times(input_num)

