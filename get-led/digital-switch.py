import RPi.GPIO as g
import time as t

g.setmode(g.BCM)

led = 26
button = 13

g.setup(led, g.OUT)
g.setup(button, g.IN)

state = 0
period = 0.2

while 1:
    if g.input(button):
        g.output(led, state)
        state = not state
        t.sleep(period)