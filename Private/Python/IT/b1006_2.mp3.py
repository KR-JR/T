# b1006_2_mp3.py

from gtts import gTTS
from playsound import playsound

# text = "안녕 여러분들 빡빡이 아저씨야"

# tts = gTTS(text = text, lang = 'ko')
# tts.save("hi.mp3")

# playsound("hi.mp3")

import os

os.chdir(os.path.dirname(os.path.abspath(__file__)))

file_path = '나의텍스트.txt'
with open(file_path, 'rt', encoding = 'UTF8') as f:
    read_file = f.read()
    
tts = gTTS(text = read_file, lang = 'ko')

tts.save("myTest.mp3")

playsound("myTest.mp3")