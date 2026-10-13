import RPi.GPIO as g


class R2R_DAC:
    def __init__(self, pins, delta, verbose = False):
        self.pins = pins
        self.delta = delta
        self.verbose = verbose

        g.setmode(g.BCM)
        g.setup(self.pins, g.OUT, initial = 0)

    def deinit(self):
        g.output(self.pins, 0)
        g.cleanup()

    def set_number(self, number):
        binary = [int(element) for element in (bin(number)[2:].zfill(8))]
        print(binary)
        for i in range(8):
            g.output(self.pins[i], binary[i])

    def set_voltage(self, voltage):
        self.set_number(int(voltage/self.delta * 255))

if __name__ == "__main__":
    try:
        dac = R2R_DAC([16, 20, 21, 25, 26, 17, 27, 22], 3.183, True)

        while 1:
            try:
                voltage = float(input("Enter voltage: "))
                dac.setvoltage(voltage)
            except ValueError:
                print("Try again")

    finally:
        dac.deinit()