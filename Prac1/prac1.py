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
    pass #do nothing

#callback function to count up
def count_up(pin):
    if GPIO.input(pin):
        global index, out_pins
        index+=1
        if index==8:
                index=0
        GPIO.output(out_pins, binaryVals[index])
        time.sleep(1)

#callback function to count down    
def count_down(pin):
    if GPIO.input(pin):
        global index, out_pins
        index-=1
        if index<0:
                index=7
        GPIO.output(out_pins, binaryVals[index])
        time.sleep(1)

#pins' initialization  function
def init_pins():
    GPIO.setwarnings(False) #stop pi from displaying config warning errors
    GPIO.setmode(GPIO.BOARD) #set board numbering system    
    GPIO.setup(out_pins, GPIO.OUT, initial=0) #setup LED pins initially OFF
    GPIO.setup(in_pins, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) # Wire the button to +3.3, then enable an internal pulldown. 
    
# Only run the functions if 
if __name__ == "__main__":
    init_pins()    
    GPIO.add_event_detect(11, GPIO.BOTH, callback=count_up, bouncetime=200) #count up event detection
    GPIO.add_event_detect(13, GPIO.BOTH, callback=count_down, bouncetime=200) #count down event detection
    
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
