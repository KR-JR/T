num = int(input("정수를 입력하세요: "))

if num < 0: #음수인지 확인
    ab_num = -num
else: 
    ab_num = num

digit_num = 0

for digit in str(ab_num): #문자열로 전환
    digit_num += int(digit)

if num < 0: #음수일때 - 곱하고 출력
    print("각 자리의 합은: ", -1*digit_num)
else:
    print("각 자리의 합은: ", digit_num)

