res = i = 0
while i < 1e2:
    if i % 2 == 0:
        res += i
    else:
        res -= i
    i += 1
print(res, end='')
