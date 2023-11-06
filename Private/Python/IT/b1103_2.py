#pip install pythin-docx #시험
#pip install docxtopdf

#엑셀파일 만들기

# import pandas as pd

# df = pd.DataFrame([["홍길동","1990.01.02","2021-0001"],   #DataFrame 메소드 시험 명령어 대문자 소문자구분필수 ex) DATAFRAME, DataFrame
#                    ["신웅","1991.01.02","2021-0002"],     #대괄호로 묶는다.
#                    ["이이","1992.01.02","2021-0003"],
#                    ["곡률","1993.01.02","2021-0004"]])

# print(df)
# df.to_excel(r'C:/Users/sinsu/Desktop/울산대학교/Code/수료증명단.xlsx',index=False,header=False)


#엑셀파일 불러오기

# from openpyxl import load_workbook

# load_wb = load_workbook(r'C:/Users/sinsu/Desktop/울산대학교/Code/수료증명단.xlsx')
# load_ws = load_wb.active

# name_list=[]
# birthday_list=[]
# ho_list=[]
# 뒤에 못적음


#open type Font과 true type Font 차이 ttf 애플에서 만듦, otf 어도비에서 만듦


#%%
(   )
(   )
(   )
(   )
(   )
(   )

#%%
(   )
(   )

#%%
(   )
(   )
(   )

#%%
(   )
    (   )
    birthday_list.append(load_ws.cell(i, 2).value)
    ho_list.append(load_ws.cell(i, 3).value)

#%%    
print(name_list)
print(birthday_list)
print(ho_list)

for i in range(len(name_list)):
    (   )
    (   )
    (   )
    (   ) # 한글 폰트 적용 코드
    (   )
    
    (   )
    (   )
    (   ) # 공백 14개
    (   )
    (   )
    (   )

    para = doc.add_paragraph()
    run = para.add_run('\n\n')
    run = para.add_run('수  료  증') 
    run.font.name = '나눔고딕'
    run.bold = True
    run._element.rPr.rFonts.set(qn('w:eastAsia'), '나눔고딕')
    run.font.size = docx.shared.Pt(40)
    para.alignment = WD_ALIGN_PARAGRAPH.CENTER

    para = doc.add_paragraph()
    run = para.add_run('\n\n')
    run = para.add_run('        성       명: ' + name_list[i] +'\n') 
    run.font.name = '나눔고딕'
    run._element.rPr.rFonts.set(qn('w:eastAsia'), '나눔고딕')
    run.font.size = docx.shared.Pt(20)
    run = para.add_run('        생 년 월 일: ' + birthday_list[i] +'\n') 
    run.font.name = '나눔고딕'
    run._element.rPr.rFonts.set(qn('w:eastAsia'), '나눔고딕')
    run.font.size = docx.shared.Pt(20)
    run = para.add_run('        교 육 과 정: 파이썬과 40개의 작품들\n')
    run.font.name = '나눔고딕'
    run._element.rPr.rFonts.set(qn('w:eastAsia'), '나눔고딕')
    run.font.size = docx.shared.Pt(20) 
    run = para.add_run('        교 육 날 짜: 2021.08.05~2021.09.09\n') 
    run.font.name = '나눔고딕'
    run._element.rPr.rFonts.set(qn('w:eastAsia'), '나눔고딕')
    run.font.size = docx.shared.Pt(20)

    para = doc.add_paragraph()
    run = para.add_run('\n\n')
    run = para.add_run('        위 사람은 파이썬과 40개의 작품들 교육과정을\n') 
    run.font.name = '나눔고딕'
    run._element.rPr.rFonts.set(qn('w:eastAsia'), '나눔고딕')
    run.font.size = docx.shared.Pt(20)
    run = para.add_run('        이수하였으므로 이 증서를 수여 합니다.\n') 
    run.font.name = '나눔고딕'
    run._element.rPr.rFonts.set(qn('w:eastAsia'), '나눔고딕')
    run.font.size = docx.shared.Pt(20)

    para = doc.add_paragraph()
    run = para.add_run('\n\n\n')
    run = para.add_run('2021.09.19') 
    run.font.name = '나눔고딕'
    run._element.rPr.rFonts.set(qn('w:eastAsia'), '나눔고딕')
    run.font.size = docx.shared.Pt(20)
    para.alignment = WD_ALIGN_PARAGRAPH.CENTER

    para = doc.add_paragraph()
    run = para.add_run('\n\n\n')
    run = para.add_run('파이썬교육기관장') 
    run.font.name = '나눔고딕'
    run.bold = True
    run._element.rPr.rFonts.set(qn('w:eastAsia'), '나눔고딕')
    run.font.size = docx.shared.Pt(20)
    para.alignment = WD_ALIGN_PARAGRAPH.CENTER

#%%
    (    ) # docs 저장
    (    ) # docs를 pdf로 변환