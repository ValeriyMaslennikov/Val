import RPi.GPIO as g
import time as t

g.setmode(g.BCM)

led = 26
button = 6

g.setup(led, g.OUT)
g.setup(button, g.IN)

state = 0
period = 0.2

while 1:
    g.output(led, not g.input(button))
    t.sleep(period)