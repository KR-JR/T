while True:
    num1 = input("1st = ")
    try:
        num1 = float(num1)
    except ValueError:
        print("invalid Input")
        continue

    num2 = input("2nd = ")
    try:
        num2 = float(num2)
    except ValueError:
        print("invalid Input")
        continue
    


    exp  = input("exp(+-*/) = ")
    
    if exp in "+-*/":
        result = eval(f"{num1} {exp} {num2}")
        print("result = ",result)

    else: 
        print("wrong answer")
        continue

    while True:
        C = input("Q or C ")
        if C.lower() == "c":
            break
        elif C.lower() == "q":
            print("exiting..")
            exit()
        else:
            print("wrong answer")