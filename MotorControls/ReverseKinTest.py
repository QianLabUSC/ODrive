import matplotlib.pyplot as plt
import numpy as np
from MotorControl import *

# Data for plotting
r = np.arange(17,27,0.5)
phi = 1.74533 #approx 100 degrees

theta_1 = []
theta_2 = []

robot = MotorControl(10, 20, 20, 10, 1)
for i in range(len(r)):
    temp = robot.PolartoTheta(r[i], phi)
    theta_1.append(temp[0])
    theta_2.append(temp[1])


t = theta_1
s = theta_2

fig, ax = plt.subplots()
ax.plot(t, s)

ax.set(xlabel='theta_1 (radians)', ylabel='theta_2 (radians)',
       title='Motor Angles When Adjusting Foot Location')
ax.grid()

fig.savefig("Polar to Theta.png")
plt.show()

resultR = []
resultPhi = []

for j in range(len(theta_1)):
    temp = robot.ThetatoPolar(theta_1[j], theta_2[j])
    resultR.append(temp[0])
    resultPhi.append(temp[1])
    
 

x = resultR
y = resultPhi

fig, ax = plt.subplots()
ax.plot(x, y)

ax.set(xlabel='r (cm)', ylabel='phi (radians)',
       title='Foot Location When Adjusting Motor Angles')
ax.grid()

fig.savefig("ThetatoPolar.png")
plt.show()