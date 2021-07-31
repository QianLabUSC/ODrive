import matplotlib.pyplot as plt
import numpy as np
from MotorControl import *

# Data for plotting
theta_1 = np.arange(0,math.pi,0.1)
theta_2 = np.arange(math.pi/2, math.pi, 0.1)

x = []
y = []

robot = MotorControl(10, 20, 20, 10, 1)
for i in range(len(theta_2)):
    temp = robot.ThetatoXY(theta_1[i], theta_2[i])
    x.append(temp[0])
    y.append(temp[1])
    print("x: " + str(temp[0]))
    print("y: " + str(temp[1]))


t = x
s = y

fig, ax = plt.subplots()
ax.plot(t, s)
plt.gca().invert_yaxis()

ax.set(xlabel='x', ylabel='y',
       title='Location of Foot When Adjusting Motor Angle')
ax.grid()

fig.savefig("test.png")
plt.show()