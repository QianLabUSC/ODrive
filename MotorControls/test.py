import matplotlib.pyplot as plt
import numpy as np
from MotorControl import *

# Data for plotting
theta_1 = np.arange(0,math.pi/2,0.1)
theta_2 = np.arange(math.pi/2, math.pi, 0.1)


t = theta_1
s = theta_2

fig, ax = plt.subplots()
ax.scatter(t, s)

ax.set(xlabel='theta_1 (radians)', ylabel='theta_2 (radians)',
       title='Desired Motor Angles')
ax.grid()

fig.savefig("DesiredAngles.png")
plt.show()


x = []
y = []

robot = MotorControl(10, 20, 20, 10, 1)
for i in range(len(theta_2)):
    temp = robot.ThetatoXY(theta_1[i], theta_2[i])
    x.append(temp[0])
    y.append(temp[1])


t = x
s = y

fig, ax = plt.subplots()
ax.scatter(t, s)
plt.gca().invert_yaxis()

ax.set(xlabel='x (cm)', ylabel='y (cm)',
       title='Location of Foot When Adjusting Motor Angle')
ax.grid()

fig.savefig("AnglesToCartesian.png")
plt.show()

resultTheta_1 = []
resultTheta_2 = []

for i in range(len(theta_2)):
    temp = robot.XYtoTheta(x[i], y[i])
    resultTheta_1.append(temp[0])
    resultTheta_2.append(temp[1])

t = resultTheta_1
s = resultTheta_2

fig, ax = plt.subplots()
ax.scatter(t, s)

ax.set(xlabel='theta_1 (radians)', ylabel='theta_2 (radians)',
       title='Resulting Angles')
ax.grid()

fig.savefig("ResultingAngles.png")
plt.show()