import math
from decimal import Decimal as D


class MotorControl:
    def __init__(self, leg1, leg2, leg3, leg4, leg6):
        self.leg1 = leg1
        self.leg2 = leg2
        self.leg3 = leg3
        self.leg4 = leg4
        self.leg6 = leg6
    
    def XYtoTheta(self, x, y):
        l16 = math.sqrt(pow(x, 2) + pow(y, 2))
        theta_h = math.acos(-1 * x / l16)
        l46 = self.leg3 + self.leg6
        cVal = (l16 * l16 + self.leg4 * self.leg4 - l46 * l46) / (2 * l16 * self.leg4)
        if(cVal > 1):
            cVal = 1
        if(cVal < -1):
            cVal = -1
        theta_46 = math.acos(cVal)
        theta_1 = theta_h - theta_46
        p4 = (self.leg4 * math.cos(theta_1), self.leg4 * math.sin(theta_1))
        theta_6Num = p4[0] - x
        if(theta_6Num > 1):
            theta_6Num = 1
        if(theta_6Num < -1):
            theta_6Num = -1
        theta_6 = math.acos(theta_6Num/ l46)
        p3 = (x + self.leg3 * math.cos(theta_6), y - self.leg3 * math.sin(theta_6))
        l13 = math.sqrt(pow(p3[0], 2) + pow(p3[1], 2))
        bVal = (self.leg4 * self.leg4 + l13 * l13 - self.leg3 * self.leg3) / (2 * self.leg4 * l13)
        if(bVal > 1):
            bVal = 1
        if(bVal < -1):
            bVal = -1
        theta_b = math.acos(bVal)
        theta_2 = theta_1 - 2 * theta_b
        return (theta_1, theta_2)

    def ThetatoXY(self, theta_1, theta_2):
        if(theta_1 > math.pi):
            theta_1 -= math.pi
        if (theta_2 > math.pi):
            theta_2 = 2 * math.pi - theta_2
        theta_n = math.pi - theta_2
        print("theta_1: " + str(theta_1))
        print("theta_n: " + str(theta_n))
        p2 = (-1 * self.leg1 * math.cos(theta_n), self.leg1 * math.sin(theta_n))
        #if(abs(p2[0]) - 0.01 < 0):
        #    p2 = (0, p2[1])
        #if(abs(p2[1]) - 0.01 < 0):
        #    p2 = (p2[0], 0)
        print("p2: " + str(p2))
        p4 = (self.leg4 * math.cos(theta_1), self.leg4 * math.sin(theta_1))
        #if(abs(p4[0]) - 0.01 < 0):
        #    p4 = (0, p4[1])
        #if(abs(p4[1]) - 0.01 < 0):
        #    p4 = (p4[0], 0)
        print("p4: " + str(p4))
        c = math.sqrt(pow(p4[0] - p2[0], 2) + pow(p4[1] - p2[1], 2))
        print("c: " + str(c))
        theta_3 = math.acos((c * c + self.leg2 * self.leg2 - self.leg3 * self.leg3) / (2 * c * self.leg2))#theta_3 and theta_4 are the same
        print("theta_3: " + str(theta_3))
        theta_4 = math.acos((c * c + self.leg3 * self.leg3 - self.leg2 * self.leg2) / (2 * c * self.leg3))
        print("theta_4: " + str(theta_4))
        theta_h = math.acos((p4[0] - p2[0])/c)
        print("theta_h: " + str(theta_h))
        theta_h3 = theta_h + theta_3
        print("theta_h3 in degrees: " + str(theta_h3*180/math.pi))


        if theta_h3 > math.pi/2:
            theta_h3 = math.pi-theta_h3

        p3xPos = 1
        if math.pi/2 - theta_n + 2 * theta_3 > math.pi:
            p3xPos = -1
            

        print("theta_h3 in degrees after conversion: " + str(theta_h3*180/math.pi))

                
        p6xPos = 1
        if math.pi/2-theta_1 + 2*theta_4 > math.pi:
            p6xPos = -1

        print("p6 Negative?: " + str(p6xPos))
        
        theta_h4 = theta_4 - theta_h

        print("theta_h4 in degrees: " + str(theta_h4*180/math.pi))


        if theta_4 + math.pi/2 - theta_1 > math.pi/2:
            theta_h4 = theta_h + theta_4
        
        print("theta_h4 in degrees after conversion: " + str(theta_h4*180/math.pi))


        p3 = (p2[0] + p3xPos*self.leg2 * math.cos(theta_h3), p2[1] + self.leg2 * math.sin(theta_h3))
        print("p3: " + str(p3))
        p6 = (p3[0] - p6xPos*self.leg6 * math.cos(theta_h4), p3[1] + self.leg6 * math.sin(theta_h4))
        print("p6: " + str(p6))
        print()
        return p6

    def ThetatoPolar(self, theta_1, theta_2):
        angle = (theta_2 + theta_1) / 2
        theta_n = math.pi-theta_2
        p2 = (-1 * self.leg1 * math.cos(theta_n), self.leg1 * math.sin(theta_n))
        p4 = (self.leg4 * math.cos(theta_1), self.leg4 * math.sin(theta_1))
        c = math.sqrt(pow(p4[0] - p2[0], 2) + pow(p4[1] - p2[1], 2))
        theta_h = math.acos((p4[0] - p2[0]) / c)
        print("p4x: " + str(p4[0]))
        print("p2x: " + str(p2[0]))
        print ("c: " + str(c))
        print("p4x - p2x: " + str(p4[0] - p2[0]))
        print("p4x - p2x /c: " + str((p4[0] - p2[0])/c))
        print("theta_h: " + str(theta_h))
        theta_R = 2 * (theta_1 - theta_h)
        if(theta_1 < theta_n):
            theta_R = 2 * (theta_1 + theta_h)
        print("theta_R: " + str(theta_R))
        r = math.sqrt(self.leg4 * self.leg4 + self.leg3 * self.leg3 - 2 * self.leg4 * self.leg3 * math.cos(theta_R))
        print("r: " + str(r))
        return (r, angle)

    def PolartoTheta(self, r, angle):
        cVal = (r * r + self.leg4 * self.leg4 - self.leg3 * self.leg3) / (2 * r * self.leg4)
        if (cVal > 1):
            cVal = 1
        if (cVal < -1):
            cVal = -1
        #print("r: " + str(r))
        #print("phi: "  + str(angle))
        #print("cVal: " + str(cVal))
        theta_3 = math.acos(cVal)
        #print("theta_3: " + str(theta_3))

        theta_1 = angle - theta_3
        theta_2 = angle + theta_3
        return (theta_1, theta_2)

    def CartesianToPolar(self, x, y):
        r = math.sqrt(x * x + y * y)
        theta = math.acos(y/x)
        return (r, theta)

    def PolarToCartesian(self, r, theta):
        x = r * math.cos(theta)
        y = r * math.sin(theta)
        return (x, y)
