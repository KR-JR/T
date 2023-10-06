#b1006_1_ip.py

import os

#os.system('ipconfig -all')

import socket

#in_addr = socket.gethostbyname(socket.gethostname())

#print(in_addr)

#localhost : 127.0.0.1

#in_addr = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#in_addr.connect(("www.google.co.kr", 443)) # HTTPS: 443, # HTTP: 80
#print(in_addr.getsockname()[0])

import requests
import re

req = requests.get("http://ipconfig.kr")
out_addr = re.search(r'IP Address : (\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})', req.text)[1]
print(out_addr)
print(req.text)
