#pip install googletrans==4.0.0-rc1
#시험문제 빨간선 노란색


#설치한 모듈의 버전 확인하기
# cmd에서 pip list
# pip show 모듈명

#예제 1번 googletrans 구글 번역기에 사용되는 언어 확인

# import googletrans

# lang = googletrans.LANGUAGES
# print(lang)

# ex2. 한글 문자열을 영어로, 영어를 한글로

# import googletrans
# translater = googletrans.Translator() ##생성자

# korstr = "저는 여러분 모두 항상 행복하기 바랍니다."
# result1 = translater.translate(korstr,dest = 'en', src = 'ko') ##메소드
# print(result1.text)

#nightly : 거의 매일 업데이트

# ex3.

# from os import linesep
# import googletrans

# tranlater = googletrans.Translator()

# InputFile = 'eng.txt'
# with open(InputFile, 'rt') as f:
#     readLines = f.readlines()

# print(readLines)

# for line in readLines:
#     re1 = tranlater.translate(line, src= 'en', dest = 'ko')
#     print(re1.text)

# ex4.
# from os import linesep
# import googletrans

# tranlater = googletrans.Translator()

# InputFile = 'eng.txt'
# with open(InputFile, 'rt') as f:
#     readLines = f.readlines()

# print(readLines)

# for line in readLines:
#     re1 = tranlater.translate(line, src= 'en', dest = 'ko')
#     #print(re1.text)
#     with open('kor.txt','a',encoding='UTF8') as f:
#         f.write(re1.text+'\n')

# ex5.
from os import linesep
import googletrans

tranlater = googletrans.Translator()

InputFile = 'kor.txt'
with open(InputFile, 'rt') as f:
    readLines = f.readlines()

print(readLines)

for line in readLines:
    re1 = tranlater.translate(line, src= 'ko', dest = 'eng')
    print(re1.text)

#과제. PEP8, PEP20 시험문제

#pip install pypclip?