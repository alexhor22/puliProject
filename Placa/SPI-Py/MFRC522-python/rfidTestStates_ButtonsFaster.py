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

#Botones para los 3 tipos de alertas
GPIO.setup(13,GPIO.IN,pull_up_down=GPIO.PUD_UP)
GPIO.setup(6,GPIO.IN,pull_up_down=GPIO.PUD_UP)
GPIO.setup(5,GPIO.IN,pull_up_down=GPIO.PUD_UP)

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

def crimeAlert():
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

def personalEmergency():
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
    
def suspiciousAlert():
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
        

def buttonPressed():
    
    isPressed=False
    
    if input_state1 == False:
        isPressed=True
        global selectedButton
        selectedButton=1
    
    elif input_state2 == False:
        isPressed=True
        selectedButton=2

    elif input_state3 == False:
        isPressed=True
        selectedButton=3
    
    else:
        isPressed=False
        selectedButton=0
        
    return isPressed
    
def validateButton():
    print "entra a validate button"
    if input_state1 == False:
        global flagButton
        flagButton=1
        
        print('Button1 Pressed')
        data= {"type":"CRIME"}
        db.child("/Alerts/Status").push(data)
        db.child("Board").update({"Status":1})
        crimeAlert()
        
        global state
        state=5
        
          
    if input_state2 == False:
        flagButton=1
        print('Button2 Pressed')
        data= {"type":"PEmergency"}
        db.child("/Alerts/Status").push(data)
        db.child("Board").update({"Status":2})  
        personalEmergency()
        state=5
        
                
    if input_state3 == False:
        flagButton=1
        print('Button3 Pressed')
        data= {"type":"Suspicious"}
        db.child("/Alerts/Status").push(data)
        db.child("Board").update({"Status":3})
        suspiciousAlert()
        state=5
        
    flagButton=0
    state=3
    
    
# Welcome message
print "Community System"
print "Welcome to the MFRC522 data read example"
print "Press Ctrl-C to stop."

lastVal=0
cont=0
#MainLoop
while continue_reading:
  input_state1 = GPIO.input(13)
  input_state2 = GPIO.input(6)
  input_state3 = GPIO.input(5)
  
  
  if state == 0 or state==1 or state ==2 or state==3 or state==4:
      
      query = db.child("Board/Status").get()    
      RFIDReader()
      
      if state== 0:
          print state
          print "Community System"
          lcd.set_color(1.0, 0.0, 1.0)
          lcd.clear()
          lcd.message('Community System')
          time.sleep(1)
     
      if query.val() != 0:
          print "entra a query"
          state=2
          print state
          
      if rfidStatus ==1: #indica que se leyo algo
          print "Tarjeta Valida"
          lcd.set_color(0.0, 1.0, 1.0)
          lcd.clear()
          lcd.message('Tarjeta Valida')
          time.sleep(1)
          state=3
          
          
      if state ==2:
              validateStatusDB(query)
              query = db.child("Board/Status").get()
              
              if query.val()== 0:
                  print "entra if"
                  state=0
                  
              else:
                  print "entra else"
                  state=2
              
      if state == 3:
              print state
              validateButton()
              
              
      if state== 5:
          print "terminal el validate button"
          query = db.child("Board/Status").get()
              
          if query.val()== 0:
              state=0
              print "entra if2"
                  
          else:
              state=3
              print "entra else2"
  
# when your code ends, the last line before the program exits would be...
GPIO.cleanup()
    




