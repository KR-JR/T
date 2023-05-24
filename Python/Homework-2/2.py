def filter_list(numbers):
    filtered_numbers = []

    for number in numbers:
        if number > 10:  # 숫자가 10보다 큰 경우만 조건을 만족하도록 설정
            filtered_numbers.append(number)

    return filtered_numbers


# 예시: 리스트에서 10보다 큰 숫자를 필터링하여 반환
input_list = [5, 12, 8, 20, 3, 15]
filtered_list = filter_list(input_list)
print(filtered_list)
