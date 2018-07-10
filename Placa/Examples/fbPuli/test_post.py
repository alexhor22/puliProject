
import pyrebase

config = {
  "apiKey": "AIzaSyCl7xYldaBe0Fx-GmcsRJwjYVWDtnEj7mE",
  "authDomain": "community-fbbae.firebaseapp.com",
  "databaseURL": "https://community-fbbae.firebaseio.com",
  "storageBucket": "community-fbbae.appspot.com"
}

firebase = pyrebase.initialize_app(config)

db= firebase.database()

data= {"newval":"3"}

db.child("/Alerts/Status").push(data)