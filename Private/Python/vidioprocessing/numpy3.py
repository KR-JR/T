import numpy as np

np.random.seed(10)
a = np.random.rand(2, 3)
b = np.random.randn(2, 3)
c = np.random.rand(6)
d = np.random.randint(1, 100, 6)
c = np.reshape(c, (2,3))
d = d.reshape(2, -1)

print('a 형태:', a.shape, '\n', a)
print('b 형태:', b.shape, '\n', b)
print('c 형태:', c.shape, '\n', c)
print('d 형태:', d.shape, '\n', d)

print('a =', a.flatten())
print('b =', np.ravel(b))
print('c =', np.reshape(c, (-1, )))
print('d =', d.reshape(-1, ))