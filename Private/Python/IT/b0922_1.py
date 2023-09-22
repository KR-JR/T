a = 1; b = 2

if a > b:
    print("a is bigger")
elif a < b:
    print("b is bigger")
else:
    print("same")
    
max_1 = a if a > b else b
print(max_1)

#리스트
nameList = ["쯔위", "나나", "모모"]
for index, item in enumerate(nameList):
    print(index, item)
    
nameTule = (1, 2, 4, 6)
for i in nameTule:
    print(i)
    
Testset = {1,2,3,1,2,3,1,1,2,3,1,1,1}
for i in Testset:
    print(i)
print(Testset)


#딕셔너리
ExDict = {'a':100, 'B':90, 'c':75, 'd':30}
for i in ExDict:
    print(i)

for i in ExDict:
    print(ExDict[i])
    
for k, v in ExDict.items():
    print(k, v)


try:
    a = 3
    b = 0
    print(a/b)
    
except ZeroDivisionError:
    print("0으로 나누면 안되요.")    
    
except Exception as e:
    print(f"예외 : {e}")

finally:
    print("항상 실행되는 명령줄")



def f1(a, *args, **kwargs):
    print(a)
    print(args)
    print(type(args))
    print(type(kwargs))
    
    
f1(1,2,3,10,20,30, aa=11,bb=12,cc=13)

a = 1; b = 11; c = 111
tuple_1 = (a,b,c) #packing

(x, y, z) = tuple_1 #unpacking

x = 3; y = 5
print(x,y)
x,y = y,x
print(x,y)




