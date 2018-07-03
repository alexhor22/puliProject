
import pyrebase

config = {
  "apiKey": "AIzaSyCl7xYldaBe0Fx-GmcsRJwjYVWDtnEj7mE",
  "authDomain": "community-fbbae.firebaseapp.com",
  "databaseURL": "https://community-fbbae.firebaseio.com",
  "storageBucket": "community-fbbae.appspot.com"
}

firebase = pyrebase.initialize_app(config)

db= firebase.database()

salidaLed1= db.child("home/led1").get()
salidaLed2= db.child("home/led2").get()
salida3 = db.child("user/FName").get()

print ("Salida de LED1 ", salidaLed1.val())
print ("Salida de LED2 ", salidaLed2.val())
print ("Salida de la tabla de user", salida3.val())

