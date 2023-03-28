import string


#(1)줄 개수 출력
with open('news.txt','r') as file:
    num_of_linee = len(file.readlin())

print(f"파일의 줄 수: {num_of_line}")

#(2) 모르겠습니다..
new_line = []

with open('news.txt', 'r'):
    line = 


#(3) 출현 횟루를 음절 순으로 정렬
char_count = {}
for line in new_line:
    for char in line:
        if char == ' ':
            continue

        if char in char_count: #있는지 확인
            char_count[char] += 1
        else:
            char_count[char] =1




#(4) 잘 안됩니다..