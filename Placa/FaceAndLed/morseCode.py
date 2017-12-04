import RPi.GPIO as GPIO
import time
import signal
import sys

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(19,GPIO.OUT)

# Capture SIGINT for cleanup when the script is aborted
def end_read(signal,frame):
    global continue_reading
    print "Ctrl+C captured, ending read."
    continue_reading = False
    GPIO.cleanup()

# Hook the SIGINT
signal.signal(signal.SIGINT, end_read)

def SOS():
    GPIO.output(19,GPIO.HIGH)
    time.sleep(.1)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.1)
    GPIO.output(19,GPIO.HIGH)
    time.sleep(.1)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.1)
    GPIO.output(19,GPIO.HIGH)
    time.sleep(.1)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.1)
    

    GPIO.output(19,GPIO.LOW)
    time.sleep(.2)
    GPIO.output(19,GPIO.HIGH)
    time.sleep(.2)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.2)
    GPIO.output(19,GPIO.HIGH)
    time.sleep(.2)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.2)
    GPIO.output(19,GPIO.HIGH)
    time.sleep(.2)

    GPIO.output(19,GPIO.HIGH)
    time.sleep(.1)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.1)
    GPIO.output(19,GPIO.HIGH)
    time.sleep(.1)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.1)
    GPIO.output(19,GPIO.HIGH)
    time.sleep(.1)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.1)
    GPIO.output(19,GPIO.HIGH)
    time.sleep(.1)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.1)



def CRIME():
    GPIO.output(19,GPIO.HIGH)
    time.sleep(.25)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.05)
    GPIO.output(19,GPIO.HIGH)
    time.sleep(.25)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.05)
    GPIO.output(19,GPIO.HIGH)
    time.sleep(.25)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.05)

    GPIO.output(19,GPIO.HIGH)
    time.sleep(.25)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.05)
    GPIO.output(19,GPIO.HIGH)
    time.sleep(.25)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.05)
    GPIO.output(19,GPIO.HIGH)
    time.sleep(.25)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.05)

    GPIO.output(19,GPIO.HIGH)
    time.sleep(.25)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.05)
    GPIO.output(19,GPIO.HIGH)
    time.sleep(.25)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.05)
    GPIO.output(19,GPIO.HIGH)
    time.sleep(.25)
    GPIO.output(19,GPIO.LOW)
    time.sleep(.05)


def goAway():
    GPIO.output(19,GPIO.HIGH)
    time.sleep(3)

goAway()

GPIO.output(19,GPIO.LOW)
GPIO.cleanup()
