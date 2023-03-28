num = 0

for i in range(1, 1000):
    num_str = str(i)
    if '3' not in num_str and '6' not in num_str and '9' not in num_str:
        num += i

print(num)
    