#!/usr/bin/python3

# matrix II.10.6/y):

N = 12
aij = 1

print("Matrix:");
for i in range(N):
    for j in range(N):
        if (i == j):
            print(1, ", ", end='')
        else:
            print(1/((i+1)*(i+1) + j + 1), ", ", end='')
    print('\n', end='')

print("\nVect:");
for i in range(N):
    print(1/(i + 1), ", ", end='')
print('\n', end='')


