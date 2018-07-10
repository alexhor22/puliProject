SYSTEM_MODE(SEMI_AUTOMATIC);

bool connectToCloud = false;
char data = 0;            //Variable for storing received data

int col = 0;
int cont = 0;
int cont2 = 0;
int cont3 = 0;
int state = 0;

bool alarm = false;
bool palarm = false;
bool green;
bool red;
bool blue;
bool off = false;
bool poff = false;
bool emergency =  false;
bool vibration = false;

const int gled = 5;
const int bled = 4;
const int rled = 3;
const int button = 2;
const int vibe = 6;

int buttonstate = 1;

void setup()
{
  Serial1.begin(9600);

  pinMode(gled, OUTPUT);
  pinMode(rled, OUTPUT);
  pinMode(bled, OUTPUT);
  pinMode(vibe, OUTPUT);
  pinMode(button, INPUT);

  digitalWrite(gled, HIGH);
  digitalWrite(rled, HIGH);
  digitalWrite(bled, HIGH);
  
  attachInterrupt(D1, connect, FALLING);

}
void loop()
{
  //always
  rxCom();

  //when not alarm and clicked. Activates palarm.
  checkButton();

  //for 10 seconds when palarm is activated. Inner loop may desactivate palarm
  checkCancel();

  //When palarm is activated; desactivates palarm, defines col, activates alarm and sends bluetooth data.
  sendAlarm();

  //Inner loop when alarm is activated; resets col and turns off palarm
  checkShutdown();

  if(emergency)
  {
    checkEmergency();
  }
  
  checkConnect();
  
  cont = 0;

}

void rxCom()
{
  if (Serial1.available() > 0)
  {
    // Checks whether data is comming from the serial port
    state = Serial1.read(); // Reads the data from the serial port
    Serial.write(state);
    //Serial.println();
  };

  //Recieving bledtooth Commands
  if (state == 'G')
  {
    digitalWrite(gled, LOW); // LED ON
    digitalWrite(rled, HIGH); // LED ON
    digitalWrite(bled, HIGH); // LED ON
  }
  else if (state == 'R')
  {
    digitalWrite(rled, LOW); // LED ON
    digitalWrite(gled, HIGH); // LED ON
    digitalWrite(bled, HIGH); // LED ON
  }
  else if (state == 'B')
  {
    digitalWrite(bled, LOW); // LED ON
    digitalWrite(gled, HIGH); // LED ON
    digitalWrite(rled, HIGH); // LED ON
  }
  else if (state == 0)
  {
    digitalWrite(bled, HIGH); // LED ON
    digitalWrite(gled, HIGH); // LED ON
    digitalWrite(rled, HIGH); // LED ON
  }
}

void checkButton()
{
  // Reading the button
  buttonstate = digitalRead(button);
  while (!alarm && buttonstate == LOW && cont != 10)
  {
    //BTSerial.write("Ni hao\r\n"); // Sends '1' to the master to turn on LED
    cont++;

    if (cont >= 2 && cont <= 4)
    {
      glowRed();
      palarm = true;
    }
    else if (cont >= 4 && cont <= 6)
    {
      glowBlue();
      palarm = true;
    }
    else if (cont >= 6 && cont <= 8)
    {
      glowGreen();
      palarm = true;
    }
    else
    {
      glowOff();
      palarm = false;
    }

    delay(1000);
    buttonstate = digitalRead(button);
  }
  cont = 0;

}

void checkCancel()
{
  while (cont2 != 10 && palarm && !poff && !emergency)
  {
    // Shutting alarm off

    buttonstate = digitalRead(button);
    if (palarm && buttonstate == LOW)
    {
      if (cont3 == 3)
      {
        poff = true;
        glowOff();
      }
      cont3++;
    }
    else
    {
      cont3 = 0;
    }

    delay(1000);
    cont2++;
  }

  buttonstate = digitalRead(button);
  if (poff && buttonstate == HIGH)
  {
    palarm = false;
    poff = false;
  }
  cont2 = 0;

}

void sendAlarm()
{
  if (palarm)
  {
    if (red)
    {
      Serial1.write("1\r\n");
      //Serial.write("Crime\r\n");
      col = 1;
    }
    else if (blue)
    {
      Serial1.write("2\r\n");
      col = 2;
    }
    else if (green)
    {
      Serial1.write("3\r\n");
      col = 3;
    }

    alarm = true;
    palarm = false;
  }
}

void checkShutdown()
{
  if (col == 1 || col == 2) {
    vibeBreak();
    while (alarm && !off)
    {
      // Shutting alarm off
      buttonstate = digitalRead(button);
      if (alarm && buttonstate == LOW)
      {
        if (cont == 15)
        {
          off = true;
          Serial1.write("0\r\n");
          col = 0;
          glowOff();
          vibeBreak();
        }
        cont++;
      }
      else
      {
        cont = 0;
      }

      blinkLed();

      delay(200);
    }
  }
  else if(col == 3)
  {
    vibeBreak();
    while (alarm && !off)
    {
      // Shutting alarm off
      cont = 0;
      buttonstate = digitalRead(button);
      while (alarm && buttonstate == LOW)
      {
        if ( cont > 5 && cont < 24)
        {
          emergency = true;
          col = 1;
          off = true;
          glowRed();
        }
        else if (cont == 25)
        {
          emergency = false;
          off = true;
          Serial1.write("0\r\n");
          col = 0;
          glowOff();
          vibeBreak();
        }
        buttonstate = digitalRead(button);
        cont++;
        delay(200);
      }

      blinkLed();

      delay(200);
    }
  }

  buttonstate = digitalRead(button);
  if (off && buttonstate == HIGH)
  {
    alarm = false;
    off = false;
    //BTSerial.write("\r\n");
  }
  cont = 0;
}

void checkEmergency()
{
  emergency = false;
  Serial1.write("1\r\n");
  alarm = true;
  col = 1;
  cont = 0;
  Serial.println("Emergency");
  
  while (alarm && !off)
  {
    // Shutting alarm off
    buttonstate = digitalRead(button);
    if (alarm && buttonstate == LOW)
    {
      if (cont == 15)
      {
        off = true;
        Serial1.write("0\r\n");
        col = 0;
        glowOff();
        vibeBreak();
      }
      cont++;
    }
    else
    {
      cont = 0;
    }

    blinkLed();

    delay(200);
  }

  buttonstate = digitalRead(button);
  if (off && buttonstate == HIGH)
  {
    alarm = false;
    off = false;
    //BTSerial.write("\r\n");
  }
  cont = 0;
}

void blinkLed()
{
  if (col == 1)
  {
    if (red)
    {
      glowOff();
    }
    else
    {
      glowRed();
    }
  }
  else if (col == 2)
  {
    if (blue)
    {
      glowOff();
    }
    else
    {
      glowBlue();
    }
  }
  else if (col == 3)
  {
    if (green)
    {
      glowOff();
    }
    else
    {
      glowGreen();
    }
  }

}

void glowRed()
{
  digitalWrite(rled, LOW); // LED ON
  digitalWrite(gled, HIGH); // LED ON
  digitalWrite(bled, HIGH); // LED ON
  red = true;
  green = false;
  blue = false;
}

void glowBlue()
{
  digitalWrite(bled, LOW); // LED ON
  digitalWrite(gled, HIGH); // LED ON
  digitalWrite(rled, HIGH); // LED ON
  blue = true;
  green = false;
  red = false;
}

void glowGreen()
{
  digitalWrite(gled, LOW); // LED ON
  digitalWrite(rled, HIGH); // LED ON
  digitalWrite(bled, HIGH); // LED ON
  green = true;
  red = false;
  blue = false;
}

void glowOff()
{
  digitalWrite(gled, HIGH); // LED ON
  digitalWrite(rled, HIGH); // LED ON
  digitalWrite(bled, HIGH); // LED ON
  green = false;
  red = false;
  blue = false;
}

void vibeBreak()
{
    vibeOn();
    delay(500);
    vibeOff();
}

void vibeOn()
{
  digitalWrite(vibe, HIGH); // LED ON
  vibration = true;
}

void vibeOff()
{
  digitalWrite(vibe, LOW); // LED ON
  vibration = false;
}

void checkConnect()
{
    if(connectToCloud && Particle.connected() == false) 
    {
        Particle.connect();
        connectToCloud = false;
    }
}

void connect() {
    connectToCloud = true;
}
