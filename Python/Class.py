class Student:
    def __init__(self, i, p):
        self.sId = i
        self.sNu = p
        
class StudentManage:
    def __init__(self):
        self.students = {}
    
    def addStudent(self, s):
        self.students[s.sId] = s.sNu

    def loginStudent(self, i, p):
        isStudent = i in self.students
        
        print('===== 로그인 결과 =====')
        if isStudent and self.students[i] == p:
            print(i, '님 로그인 성공')
        else:
            print(i, '님 이름과 학번을 다시 확인하세요!')
            
    def removeStudent(self, i, p):
        del self.students[i]
        
    def printStudent(self):
        print('===== 전체 학생 =====')
        for member in self.students.keys():
            print('ID :', member)
            print('PW :', self.students[member])
            print('-------------------------')
            
            
mm = StudentManage()

mm.addStudent(Student('이재훈', 20214224))
mm.addStudent(Student('아무개', 20221234))

mm.printStudent()

mm.loginStudent('이재훈', 20214224)
mm.loginStudent('아무개', 20231233)

mm.printStudent()
