#!/usr/bin/python3
"""
Python Practical Template
Keegan Crankshaw
Readjust this Docstring as follows:
Names: Chris Kateera
Student Number: KTRKUD001
Prac: Prac 01
Date: 23/07/2019
"""

# import Relevant Librares
import RPi.GPIO as GPIO, time, itertools



# Logic that you write
def main():
    #print("write your logic here")
    GPIO.setwarnings(False)
    out_pins = [3,5,7]
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(out_pins, GPIO.OUT)
    binaryVals = list(itertools.product([0,1], repeat = 3))
    print(binaryVals)
    
    for i in binaryVals:
        GPIO.output(out_pins, i)
        time.sleep(2)
        GPIO.output(out_pins, 0)
        time.sleep(2)    

# Only run the functions if 
if __name__ == "__main__":
    # Make sure the GPIO is stopped correctly
    try:
        while True:
            main()
    except KeyboardInterrupt:
        print("Exiting gracefully")
        # Turn off your GPIOs here
        GPIO.cleanup()
    except e:
        GPIO.cleanup()
        print("Some other error occurred")
        print(e.message)
