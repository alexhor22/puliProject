#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11);

int cont = 0;
int state = 0;

bool alarm = false;
bool palarm = false;
bool green;
bool red;
bool blue;
bool off = false;

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
  rxCom();

  checkButton();

  checkShutdown();
  
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

    if(cont >= 2 && cont <= 4)
    {
      glowRed();
      palarm = true;
    }
    else if(cont >= 4 && cont <= 6)
    {
      glowBlue();
      palarm = true;
    }
    else if(cont >= 6 && cont <= 8)
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
  
  if(palarm)
  {
    if(red)
    {
      BTSerial.write("Crime\r\n");
    }
    else if(blue)
    {
      BTSerial.write("Personal Emergency\r\n");
    }
    else if(green)
    {
      BTSerial.write("Suspicious Activity\r\n");
    }
    
    alarm = true;
    palarm = false;
  }
}

void checkShutdown()
{
  // Shutting alarm off
  buttonstate = digitalRead(button);
  while (alarm && buttonstate == LOW)
  {
    if(cont == 3)
    {
      off = true;
      BTSerial.write("No Alarm\r\n");

      glowOff();
    }
 
    delay(1000);
    cont++;
    buttonstate = digitalRead(button);
  }

  if(off)
  {
    alarm = false;
    off = false;
    //BTSerial.write("\r\n");
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

