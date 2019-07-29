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
binaryVals = list(itertools.product([0,1], repeat = 3))
index=0
out_pins = [3,5,7] #list of output pins
in_pins = [11,13] #list of intput pins
print(binaryVals)


# Logic that you write
def main():
    #initialize pins as desired
    #create list of binary numbers
    '''
    global binaryVals, index
    binaryVals = list(itertools.product([0,1], repeat = 3))
    print(binaryVals) #checking
    '''
    
    '''
    for i in binaryVals:
        GPIO.output(out_pins, i) #assign count value
        time.sleep(2)
        GPIO.output(out_pins, 0) #switch off all LEDs
        time.sleep(2)   '''

def count_up(pin):
    time.sleep(.01)
    if GPIO.input(pin):
        global index, out_pins
        index+=1
        GPIO.output(out_pins, binaryVals[index])
        time.sleep(1)
        #GPIO.output(out_pins, 0) #switch off all LEDs
        #time.sleep(1)
        if index==7:
            index=0
    
    
def count_down(pin):
    time.sleep(.01)
    if GPIO.input(pin):
        global index, out_pins
        index-=1
        GPIO.output(out_pins, binaryVals[index])
        time.sleep(2)
        #GPIO.output(out_pins, 0) #switch off all LEDs
        #time.sleep(2)
        if index ==0:
            index=7

def init_pins():
    GPIO.setwarnings(False) #stop pi from displaying config warning errors
    GPIO.setmode(GPIO.BOARD) #set board numbering system    
    GPIO.setup(out_pins, GPIO.OUT, initial=0)
    # Wire the button to +3.3, then enable an internal pulldown.
    GPIO.setup(in_pins, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
    
    
# Only run the functions if 
if __name__ == "__main__":
    init_pins()    
    GPIO.add_event_detect(11, GPIO.BOTH, callback=count_up, bouncetime=200) #count up
    GPIO.add_event_detect(13, GPIO.BOTH, callback=count_down, bouncetime=200) #count down
    
    try:
        time.sleep(1000)
        #while True:
         #   main()
    except KeyboardInterrupt:
        print("Exiting gracefully")
        # Turn off your GPIOs here
        # Make sure the GPIO is stopped correctly
        GPIO.cleanup()
    except Exception as e:
        GPIO.cleanup()
        print("Some other error occurred")
        print(e.message)
