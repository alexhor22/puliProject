import pyrebase 


  #Initialize Firebase
  config = {
    "apiKey": "AIzaSyCpzw7xesy5uSuQtiVLIcck969Dl11gvg4",
    "authDomain": "communitypuli-f0b9d.firebaseapp.com",
    "databaseURL": "https://communitypuli-f0b9d.firebaseio.com",
    "storageBucket": "communitypuli-f0b9d.appspot.com",
  }

 firebase= pyrebase.Initialize_app(config)
