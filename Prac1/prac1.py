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
    #initialize pins as desired
    init_pins()
    #create list of binary numbers
    binaryVals = list(itertools.product([0,1], repeat = 3))
    print(binaryVals) #checking
    
    for i in binaryVals:
        GPIO.output(out_pins, i) #assign count value
        time.sleep(2)
        GPIO.output(out_pins, 0) #switch off all LEDs
        time.sleep(2)    

def init_pins():
    GPIO.setwarnings(False) #stop pi from displaying config warning errors
    out_pins = [3,5,7] #list of output pins
    in_pins = [] #list of intput pins
    GPIO.setmode(GPIO.BOARD) #set board numbering system
    GPIO.setup(out_pins, GPIO.OUT)
    
    
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
    except Exception as e:
        GPIO.cleanup()
        print("Some other error occurred")
        print(e.message)
