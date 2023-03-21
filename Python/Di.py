string = input("염기 서열을 입력해주세요: ")

counta = string.lower().count('a')
countt = string.lower().count('t')
countg = string.lower().count('g')
countc = string.lower().count('c')

print("a의 개수: ",counta)
print("t의 개수: ",countt)
print("g의 개수: ",countg)
print("c의 개수: ",countc)