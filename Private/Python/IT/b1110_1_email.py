# doc string


import re
import requests 

UserAgent = 'User-Agent'
MyAgent = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/118.0.0.0 Whale/3.23.214.10 Safari/537.36'
ContentType = 'Content-Type'
ContentTypeText = 'text/html; charset = utf-8 '

url = 'https://news.v.daum.net/v/20211129144552297'

headers = {
    UserAgent : MyAgent,
    ContentType : ContentTypeText
}

# 셤 문제
res = requests.get(url, headers = headers)
print(res.raise_for_status) # 에러 method
print(res.status_code) # 에러코드

with open('result.html', 'w', encoding = 'utf-8') as f:
    f.write(res.text)
    
results = re.findall(r'[\w\.-]+@[\w\.-]+', res.text)  
results = list(set(results))

print(results)