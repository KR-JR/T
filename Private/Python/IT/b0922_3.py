try:
    myNum = int(input("입력하세요: "))
    if myNum > 100:
        raise Exception ("100을 초과했습니다.")
    print(myNum)
    
except Exception as e:
    print(f"예외 발생: {e}")