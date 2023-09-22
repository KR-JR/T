class Student():
    def __init__(self, name, age, like):
        self.name = name
        self.age = age
        self.like = like
        
    def studentInfo(self):
        print(f"이름: {self.name}", "나이: {self.age}", "취미: {self.like}")
        
이재훈 = Student('이재훈', 24, '게임')
print(이재훈.name, 이재훈.age, 이재훈.like)