import RPi.GPIO as g


class PWM_DAC:
    def __init__(self, pin, freq, delta, verbose = False):
        self.pin = pin
        self.delta = delta
        self.verbose = verbose
        self.freq = freq

        g.setmode(g.BCM)
        g.setup(self.pin, g.OUT, initial = 0)
        self.pwm = g.PWM(self.pin, self.freq)
        self.pwm.start(100)

    def deinit(self):
        g.output(self.pin, 0)
        g.cleanup()

    def set_voltage(self, voltage):
        
        self.pwm.ChangeDutyCycle(voltage*100/self.delta)

if __name__ == "__main__":
    try:
        dac = PWM_DAC(12, 500, 3.290, True)

        while 1:
            try:
                voltage = float(input("Enter voltage: "))
                dac.set_voltage(voltage)
            except ValueError:
                print("Try again")

    finally:
        dac.deinit()