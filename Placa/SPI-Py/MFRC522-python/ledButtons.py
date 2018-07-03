import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(13,GPIO.IN,pull_up_down=GPIO.PUD_UP)
GPIO.setup(6,GPIO.IN,pull_up_down=GPIO.PUD_UP)
GPIO.setup(5,GPIO.IN,pull_up_down=GPIO.PUD_UP)

while True:
    input_state1 = GPIO.input(13)
    input_state2 = GPIO.input(6)
    input_state3 = GPIO.input(5)   
 
    if input_state1 == False:
        print('Button1 Pressed')
        time.sleep(0.2)

    if input_state2 == False:
        print('Button2 Pressed')
        time.sleep(0.2)

    if input_state3 == False:
        print('Button3 Pressed')
        time.sleep(0.2)
