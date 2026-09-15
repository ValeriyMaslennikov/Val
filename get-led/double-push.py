import RPi.GPIO as g
import time as t

g.setmode(g.BCM)

leds = [24,22,23,27,17,25,12,16]
up = 9
down = 10
num = 0

def dec2bin (value):
    return [int(element) for element in (bin(value)[2:].zfill(9))]

g.setup(leds, g.OUT)
g.setup(up, g.IN)
g.setup(down, g.IN)

g.output(leds, 0)

state = 0
period = 0.2

while 1:
    if g.input(up) and g.input(down):
        for led in leds:
            g.output(led, 1)
        state = not state
        t.sleep(2)
        
    elif g.input(up) and num < 512:
        num+=1
        t.sleep(period)
    elif g.input(down) and num >=1:
        num-=1
        t.sleep(period)
    
    for i in range(8):
        g.output(leds[i], dec2bin(num)[i])