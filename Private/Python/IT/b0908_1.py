print("LEE")
print('LEE')


print('LEE \
      JAEHUN')

print("""LEE
      JAEHUN""")

print('''LEE
      JAEHUN''')

print('LEE', end = ' ')
print('JAEHUN')

print('Lee', end = '&&&')
print('JAEHUN')

print('LEE', 'JAEHUN', sep = '#')

f = open('dump.txt', 'w')
print('LEE JAEHUN', file = f)
f.close()

import sys
print('LEE JAEHUN', file = sys.stderr)


print('My mother\'shouse')

print('\',\",\\,\a,\t,\n')

i = 123
f = 3.14
s = 'LEE'

print(i, f, s)
