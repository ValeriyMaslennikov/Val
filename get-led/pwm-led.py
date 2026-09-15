import RPi.GPIO as g
import time as t

g.setmode(g.BCM)

led = 26

g.setup(led, g.OUT)

state = 0
period = 1.0
pwm = g.PWM(led, 200)
duty = 0.0
pwm.start(duty)

while 1:
    pwm.ChangeDutyCycle(duty)
    t.sleep(0.05)

    duty+=1.0
    if duty >100.0:
        duty = 0.0