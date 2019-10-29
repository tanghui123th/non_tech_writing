class C1:
    value = 1

class C2:
    value = 2

class A(C1, C2):
    pass

a = A()
print('a.value = {}'.format(a.value))
