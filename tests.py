import numpy as np
import matplotlib.pyplot as plt

P = float(input("Enter max pressure value: "))
R = float(input("Enter radius of particle's interaction: "))
print("Calculating normalization factor")
n0 = 100
epsilon = 0.001


def ForDrawFunc(x, P, R):
    return -P * np.sqrt(1 - ((x - R) / R) ** 2) + P

def f(x, P, R):
    return 2 * np.pi * x * ( (-P) * np.sqrt(1 - ((x - R) / R) ** 2) + P )


def RightRect(P, R, n):
    step = R / n
    integral = 0
    for i in range(1, n + 1):
        integral += step * f(i * step, P, R)
    return integral

def LeftRect(P, R, n):
    step = R / n
    integral = 0
    for i in range(0, n):
        integral += step * f(i * step, P, R)
    return integral


n = n0
while(abs(LeftRect(P, R, n) - LeftRect(P, R, 2 * n)) > epsilon):
    n *= 2

print("Left rectangles normalization factor:", LeftRect(P, R, 2 * n))

n = n0
while(abs(RightRect(P, R, n) - RightRect(P, R, 2 * n)) > epsilon):
    n *= 2

print("Right rectangles normalization factor:", RightRect(P, R, 2 * n))

print("Average normalization factor:", 0.5 * (RightRect(P, R, 2 * n) + LeftRect(P, R, 2 * n)))

### ----Plotting---- ###
x = np.linspace(0, R, n0)
plt.figure(figsize=(6, 3))
plt.plot(x, ForDrawFunc(x, P, R), color = "orange")
plt.plot(-x, ForDrawFunc(x, P, R), color = "orange")
#plt.plot(x, f(x, P, R), color = "green")
#plt.plot(-x, f(x, P, R), color = "green")
plt.xlabel('Distance')
plt.ylabel('Pressure')
plt.title('Smoothing Kernel')

plt.grid(which='major', color='purple', linestyle='-', linewidth=0.6)
plt.grid(which='minor', color='blue', linestyle=':', linewidth=0.5)
plt.minorticks_on()

plt.show()