import io
import picamera
import cv2
import numpy
import RPi.GPIO as GPIO
import time
import signal
import sys

GPIO.setwarnings(False)

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


while(True):
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(19,GPIO.OUT)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(26,GPIO.OUT)

    GPIO.output(19,GPIO.LOW)
    GPIO.output(26,GPIO.LOW)
    

    #Create a memory stream so photos doesn't need to be saved in a file
    stream = io.BytesIO()

    #Get the picture (low resolution, so it should be quite fast)
    #Here you can also specify other parameters (e.g.:rotate the image)
    with picamera.PiCamera() as camera:
        camera.resolution = (320, 240)
        camera.capture(stream, format='jpeg')

    #Convert the picture into a numpy array
    buff = numpy.fromstring(stream.getvalue(), dtype=numpy.uint8)

    #Now creates an OpenCV image
    image = cv2.imdecode(buff, 1)

    #Load a cascade file for detecting faces
    face_cascade = cv2.CascadeClassifier('/home/pi/Desktop/opcv2Face/faces.xml')

    #Convert to grayscale
    gray = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)

    #Look for faces in the image using the loaded cascade file
    faces = face_cascade.detectMultiScale(gray, 1.1, 5)

    #print "Found "+str(len(faces))+" face(s)"

    contFaces= len(faces)
    

    print contFaces

    if contFaces>=1:
        #print ("LED on")
        #Draw a rectangle around every found face
        for (x,y,w,h) in faces:
            cv2.rectangle(image,(x,y),(x+w,y+h),(255,255,0),2)

        #Save the result image
        goAway()
        cv2.imwrite('result.jpg',image)
        GPIO.output(26,GPIO.HIGH)
        time.sleep(2)
        
    elif contFaces == 0:
        #print("entra aqui")
        #print ("LED off")
        GPIO.output(26,GPIO.LOW)
        time.sleep(2)

    else:
        #print "se sale de todo"

    GPIO.cleanup()


