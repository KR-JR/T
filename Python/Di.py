class C:
    def __init__(self):
        self.result = 0

    def add(self, num):
        self.result += num
        return self.result

cal1 = C()
cal2 = C()

print(cal1.add(3))
print(cal1.add(3))
print(cal1.add(3))
print(cal1.add(3))
