def to_decimal_to_binary(num): #to_decimal_to_binary 함수
    int_part, de_part = str(num).split('.') 
    int_part = int(int_part) #정수부분
    de_part = float('.' + de_part) #소수부분

    binary_int = '' 
    while int_part > 0: #정수부분 변환
        binary_int = str(int_part % 2) + binary_int
        int_part //= 2

    binary_de = '' 
    while de_part != 0.0: #음수부분 변환
        de_part *= 2

        if de_part >= 1.0:
            binary_de += '1'
            de_part -= 1.0
        else:
            binary_de +='0'

    if binary_de != '': #결과값 확인 후 반환
        return binary_int + '.' + binary_de
    else:
        return binary_int
    
print(to_decimal_to_binary(input("변환할 실수를 입력 해주세요:"))) #input받고 t_d_t_b함수 출력