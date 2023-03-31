cir = [80, 90] #원리의 성적 중간/기말
app = [70, 75] #응용의 성적 중간/기말
data = [85, 80] #자료구조의 성적 중간/기말

#각 과목의 sum값(최초 = 0)
sum_cir = 0
sum_app = 0
sum_data = 0


#cir, app, data 각각의 인덱스를 for구문으로 더함
for i in range(len(cir)): #cir의 인덱스 길이만큼 범위
    sum_cir = sum_cir + cir[i]
aver_cir = sum_cir / len(cir) #cir의 평균값 계산


for i in range(len(cir)): #app의 인덱스 길이만큼 범위
    sum_app = sum_app + app[i]
aver_app = sum_app / len(app) #app의 평균값 계산

for i in range(len(cir)): #data의 인덱스 길이만큼 범위
    sum_data = sum_data + data[i]
aver_data = sum_data / len(data) #data의 평균값 계산

#각각의 result = 학점 계산
if aver_cir >= 60: #60~65 = C0
    if aver_cir >= 65: #65~70 = C+
        if aver_cir >= 70: #70~75 = B0
            if aver_cir >= 75: #75~ 80 = B+
                if aver_cir >= 80: #80~85 = A0
                    if aver_cir >= 85: #85~ = A+
                        result_cir = "A+"
                    else:
                        result_cir = "A0"
                else:
                    result_cir = "B+"
            else:
                result_cir = "B0"
        else:
            result_cir = "C+"
    else:
        result_cir = "C0"
else:
    result_cir = "F"


if aver_app >= 60: #60~65 = C0
    if aver_app >= 65: #65~70 = C+
        if aver_app >= 70: #70~75 = B0
            if aver_app >= 75: #75~ 80 = B+
                if aver_app >= 80: #80~85 = A0
                    if aver_app >= 85: #85~ = A+
                        result_app = "A+"
                    else:
                        result_app = "A0"
                else:
                    result_app = "B+"
            else:
                result_app = "B0"
        else:
            result_app = "C+"
    else:
        result_app = "C0"
else:
    result_app = "F"


if aver_data >= 60: #60~65 = C0
    if aver_data >= 65: #65~70 = C+
        if aver_data >= 70: #70~75 = B0
            if aver_data >= 75: #75~ 80 = B+
                if aver_data >= 80: #80~85 = A0
                    if aver_data >= 85: #85~ = A+
                        result_data = "A+"
                    else:
                        result_data = "A0"
                else:
                    result_data = "B+"
            else:
                result_data = "B0"
        else:
            result_data = "C+"
    else:
        result_data = "C0"
else:
    result_data = "F"


#출력 결과물
print("=============================================")
print("           이재훈의 2023년 1학기 학점          ")
print("---------------------------------------------")
print("교과목            중간    기말    평균    학점 ")
print("---------------------------------------------")
print("프로그래밍 원리    %d      %d      %d     %s" %(cir[0], cir[1], aver_cir, result_cir))
print("프로그래밍 응용    %d      %d      %d     %s" %(app[0], app[1], aver_app, result_app))
print("논리구조 실험      %d      %d      %d     %s" %(data[0], data[1], aver_data, result_data))
print("=============================================")