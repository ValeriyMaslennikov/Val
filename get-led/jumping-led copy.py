import RPi.GPIO as g
import time as t

g.setmode(g.BCM)

leds = [24,22,23,27,17,25,12,16]

g.setup(leds, g.OUT)

g.output(leds, 0)

state = 0
period = 0.2

while 1:
    for led in leds:
        g.output(led, 1)
        t.sleep(period)
        g.output(led, 0)
    for led in reversed(leds):
        g.output(led, 1)
        t.sleep(period)
        g.output(led, 0)
