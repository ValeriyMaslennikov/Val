import RPi.GPIO as g
import time as t

g.setmode(g.BCM)

led = 26

g.setup(led, g.OUT)

state = 0
period = 1.0

while 1:
    g.output(led, state)
    state = not state
    t.sleep(period)