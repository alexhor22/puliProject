#!/usr/bin/env python
# -*- coding: utf8 -*-

import RPi.GPIO as GPIO
import MFRC522
import signal
import time
import threading
import pyrebase


continue_reading = True
rfidStatus= 4
state=0
flagButton=0
selectedButton=0

#FirebaseConfig

config = {
  "apiKey": "AIzaSyCl7xYldaBe0Fx-GmcsRJwjYVWDtnEj7mE",
  "authDomain": "community-fbbae.firebaseapp.com",
  "databaseURL": "https://community-fbbae.firebaseio.com",
  "storageBucket": "community-fbbae.appspot.com"
}

firebase = pyrebase.initialize_app(config)

db= firebase.database()



# Capture SIGINT for cleanup when the script is aborted
def end_read(signal,frame):
    global continue_reading
    print "Ctrl+C captured, ending read."
    continue_reading = False

# Hook the SIGINT
signal.signal(signal.SIGINT, end_read)


def crimeAlert():
    
    time.sleep(5)

def personalEmergency():
    
    time.sleep(1)
    
def suspiciousAlert():
    
    time.sleep(1)

def validateStatusDB(query):
    if query.val() == 1:
        crimeAlert()
        global state
        state=2

    if query.val()==2:
        personalEmergency()
        state=2

    if query.val()==3:
        suspiciousAlert()
        state=2
        

    
# Welcome message
print "Community System"
print "Welcome to the MFRC522 data read example"
print "Press Ctrl-C to stop."

lastVal=0
cont=0
#MainLoop
while continue_reading:
  query = db.child("Board/Status").get() 
  validateStatusDB(query)
  time.sleep(1)
  
# when your code ends, the last line before the program exits would be...
    




