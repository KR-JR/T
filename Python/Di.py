import random


while True:
    myhand = input("나의 결정(나가기 Q): ")
    comhand = random.choice(["가위", "바위", "보"])

    print("컴퓨터의 결정: ", comhand)


    if myhand == comhand:
        print("비겼습니다")

    elif myhand == "가위":
        if comhand == "바위":
            print("졌습니다")
        elif comhand == "보":
            print("이겼습니다")

    elif myhand == "보":
        if comhand == "가위":
            print("졌습니다")
        elif comhand == "바위":
            print("이겼습니다")
    
    elif myhand.lower == "q":
        break
    
    else:
        print("잘못 입력했습니다.")
    