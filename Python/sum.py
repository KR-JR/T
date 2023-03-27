import random

while True:
    num1 = random.randrange(100)
    num2 = random.randrange(100)
    
    print("문제: ", num1, "+ ", num2)
    
    answer = input("정답(종료는 Q): ")
    try:
        answer = float(answer)
    except:
        print("잘못 입력했습니다.\n")
        continue


    if answer == num1 + num2:
        print("정답입니다.\n")
    elif answer.upper == "Q":
        break
    else: 
        print("틀렸습니다.\n") 