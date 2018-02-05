#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11);

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

const int gled = 7;
const int bled = 8;
const int rled = 6;
const int button = 5;

int buttonstate = 1;

void setup()
{
  BTSerial.begin(9600);
  Serial.begin(9600);

  pinMode(gled, OUTPUT);
  pinMode(rled, OUTPUT);
  pinMode(bled, OUTPUT);
  pinMode(button, INPUT);

  digitalWrite(gled, HIGH);
  digitalWrite(rled, HIGH);
  digitalWrite(bled, HIGH);

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
  
  cont = 0;

}

void rxCom()
{
  if (BTSerial.available() > 0)
  {
    // Checks whether data is comming from the serial port
    state = BTSerial.read(); // Reads the data from the serial port
    Serial.write(state);
    Serial.println();
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
  else if (state == '0')
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
      BTSerial.write("Crime\r\n");
      col = 1;
    }
    else if (blue)
    {
      BTSerial.write("Personal Emergency\r\n");
      col = 2;
    }
    else if (green)
    {
      BTSerial.write("Suspicious Activity\r\n");
      col = 3;
    }

    alarm = true;
    palarm = false;
  }
}

void checkShutdown()
{
  if (col != 3) {
    while (alarm && !off)
    {
      // Shutting alarm off
      buttonstate = digitalRead(button);
      if (alarm && buttonstate == LOW)
      {
        if (cont == 15)
        {
          off = true;
          BTSerial.write("No Alarm\r\n");
          col = 0;
          glowOff();
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
  else
  {
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
          BTSerial.write("No Alarm\r\n");
          col = 0;
          glowOff();
        }
        cont++;
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
  BTSerial.write("Crime\r\n");
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
        BTSerial.write("No Alarm\r\n");
        col = 0;
        glowOff();
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

