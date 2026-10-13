import RPi.GPIO as g
g.setmode(g.BCM)

pins = [16, 20, 21, 25, 26, 17, 27, 22] ##[22,27,17,26,25,21,20,16]
g.setup(pins, g.OUT)

delta = 3.179

print(reversed(pins))

def number2dac(value):
    binary = [int(element) for element in (bin(value)[2:].zfill(8))]
    for i in range(8):
        g.output(pins[i], binary[i])

def voltage2number(voltage):
    if not (0.0 <= voltage <= delta):
        print(f"Overvoltage! (0.00 - {delta:.2f} V)")
        print("Go to 0.0")
        return 0

    return int(voltage/delta * 255)

try:
    while 1:
        try:
            voltage = float(input("Enter voltage: "))
            number = voltage2number(voltage)
            number2dac(number)
        except ValueError:
            print("Try again")
finally:
    g.output(pins, 0)
    g.cleanup()