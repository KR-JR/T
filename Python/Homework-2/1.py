def filter_even_numbers(num, even_numbers):
    even_numbers.append(num)

    # 짝수만 필터링하여 결과 리스트에 추가
    filtered_numbers = []
    for number in even_numbers:
        if number % 2 == 0:  # 숫자가 짝수인지 확인
            filtered_numbers.append(number)

    return filtered_numbers

even_numbers = []

# 'c'를 입력할 때까지 숫자를 입력받아 리스트에 저장
while True:
    num = input()
    if str(num.lower()) == 'c':
        break
    else:
        filter_even_numbers(int(num), even_numbers)

filtered_numbers = filter_even_numbers(0, even_numbers)
print("짝수는:", filtered_numbers)
