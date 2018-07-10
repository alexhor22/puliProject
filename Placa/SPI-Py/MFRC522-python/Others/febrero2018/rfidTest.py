#!/usr/bin/env python
# -*- coding: utf8 -*-

import RPi.GPIO as GPIO
import MFRC522
import signal
import time
import Adafruit_CharLCD as LCD
import threading
import pyrebase

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(26,GPIO.OUT)

continue_reading = True
rfidStatus= 4

#FirebaseConfig

config = {
  "apiKey": "AIzaSyCl7xYldaBe0Fx-GmcsRJwjYVWDtnEj7mE",
  "authDomain": "community-fbbae.firebaseapp.com",
  "databaseURL": "https://community-fbbae.firebaseio.com",
  "storageBucket": "community-fbbae.appspot.com"
}

firebase = pyrebase.initialize_app(config)

db= firebase.database()

# Raspberry Pi configuration:
lcd_rs = 27  # Change this to pin 21 on older revision Raspberry Pi's
lcd_en = 22
lcd_d4 = 21
lcd_d5 = 24
lcd_d6 = 23
lcd_d7 = 18
lcd_red   = 4
lcd_green = 17
lcd_blue  = 7  # Pin 7 is CE1

# Define LCD column and row size for 16x2 LCD.
lcd_columns = 16
lcd_rows    = 2

# Create an object of the class MFRC522
MIFAREReader = MFRC522.MFRC522()

# Capture SIGINT for cleanup when the script is aborted
def end_read(signal,frame):
    global continue_reading
    print "Ctrl+C captured, ending read."
    continue_reading = False
    GPIO.cleanup()

# Hook the SIGINT
signal.signal(signal.SIGINT, end_read)


# Initialize the LCD using the pins above.
lcd = LCD.Adafruit_RGBCharLCD(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7,
                                  lcd_columns, lcd_rows, lcd_red, lcd_green, lcd_blue)

def RFIDReader(): 
    # Scan for cards    
    (status,TagType) = MIFAREReader.MFRC522_Request(MIFAREReader.PICC_REQIDL)

    # If a card is found
    if status == MIFAREReader.MI_OK:
        print "Card detected"
    
    # Get the UID of the card
    (status,uid) = MIFAREReader.MFRC522_Anticoll()

    # If we have the UID, continue
    if status == MIFAREReader.MI_OK:
        # Print UID
        print "Card read UID: "+str(uid[0])+","+str(uid[1])+","+str(uid[2])+","+str(uid[3])
        global rfidStatus
        rfidStatus=1
        #print "RFID STATUS:", rfidStatus 
        # This is the default key for authentication
        key = [0xFF,0xFF,0xFF,0xFF,0xFF,0xFF]
        
        # Select the scanned tag
        MIFAREReader.MFRC522_SelectTag(uid)

        # Authenticate
        status = MIFAREReader.MFRC522_Auth(MIFAREReader.PICC_AUTHENT1A, 8, key, uid)

        # Check if authenticated
        if status == MIFAREReader.MI_OK:
            MIFAREReader.MFRC522_Read(8) 
            MIFAREReader.MFRC522_StopCrypto1()
            
        else:
            print "Authentication error"
            
    else:
        #global rfidStatus
        rfidStatus=0
        #print "RFID STATUS:", rfidStatus 

   
def validateStatusDB(query):
    
    if query.val() == 1:
        GPIO.output(26,GPIO.HIGH)
        time.sleep(0.25)
        GPIO.output(26,GPIO.LOW)
        GPIO.output(26,GPIO.HIGH)
        time.sleep(0.25)
        GPIO.output(26,GPIO.LOW)
    
        lcd.set_color(1.0, 0.0, 0.0)
        lcd.clear()
        #lcd.message('RED')
        lcd.message('CRIME ALERT')
        time.sleep(1)
        lcd.set_color(1.0, 0.0, 0.0)
        lcd.clear()
        lcd.message('REPORTS ZONE 1,\nTecnologico')
        time.sleep(1)

    if query.val()==2:
        GPIO.output(26,GPIO.HIGH)
        time.sleep(0.25)
        GPIO.output(26,GPIO.LOW)
        GPIO.output(26,GPIO.HIGH)
        time.sleep(0.25)
        GPIO.output(26,GPIO.LOW)
    
        lcd.set_color(0.0, 0.0, 1.0)
        lcd.clear()
        ##lcd.message('BLUE')
        lcd.message('HEALTH EMERGENCY')
        time.sleep(1)
        lcd.set_color(0.0, 0.0, 1.0)
        lcd.clear()
        lcd.message('REPORTS ZONE 2,\nRotonda')
        time.sleep(1)

    if query.val()==3:
        GPIO.output(26,GPIO.HIGH)
        time.sleep(0.25)
        GPIO.output(26,GPIO.LOW)
        GPIO.output(26,GPIO.HIGH)
        time.sleep(0.25)
        GPIO.output(26,GPIO.LOW)
    
        lcd.set_color(1.0, 1.0, 0.0)
        lcd.clear()
        ##lcd.message('YELLOW')
        lcd.message('SUSPICIOUS \nSITUATION')
        time.sleep(1)        
        lcd.set_color(1.0, 1.0, 0.0)
        lcd.clear()
        lcd.message('REPORTS ZONE 5,\nLa Primavera')
        time.sleep(1)
    
    if query.val()==0:   
        lcd.set_color(1.0, 1.0, 1.0)
        lcd.clear()
        print "nothing to report"
        lcd.message('Nothing to \nreport')
        time.sleep(1)
   
        
# Welcome message
print "Community System"
print "Welcome to the MFRC522 data read example"
print "Press Ctrl-C to stop."

lastVal=0
#tLCD = threading.Thread(name='LCD', target=threadLCD)
#tLCD.start()

# This loop keeps checking for chips. If one is near it will get the UID and authenticate
threads = []

#MainLoop
while continue_reading:
  RFIDReader();
  
  if rfidStatus ==1:
      print "Tarjeta Valida"
      lcd.set_color(0.0, 1.0, 1.0)
      lcd.clear()
      lcd.message('Tarjeta Valida')
      time.sleep(1)

  else:
      print "no es valido"
      lcd.set_color(1.0, 0.0, 1.0)
      lcd.clear()
      lcd.message('Tarjeta no valida')
      time.sleep(1)
  
# when your code ends, the last line before the program exits would be...
GPIO.cleanup()
    


