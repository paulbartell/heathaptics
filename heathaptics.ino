#include "types.h"
#include "pins.h"
#include <i2cmaster.h>

ModeType mode = STOP;
boolean exitFlag = false;
short setPts[2] = {0, 0};

void setup()
{
  
  // Setup pwm frequency of 7812.5 Hz
  TCCR0B = TCCR0B & 0b11111000 | 0x02;
  
  // Init serial module
  Serial.begin(57600);
  
  i2c_init(); //Initialise the i2c bus
  PORTC = (1 << PORTC4) | (1 << PORTC5); //enable pullups
  
  // Output pins
  pinMode(PIN_PWMA, OUTPUT);
  pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_DIRA, OUTPUT);
  pinMode(PIN_DIRB, OUTPUT);
  
  // For peltier devices at 6V, 1 count ~ 7.8125mA or 46.87mW
  // Init Peltier outputs
  analogWrite(PIN_PWMA, 0);
  digitalWrite(PIN_DIRA, LOW);
  analogWrite(PIN_PWMB, 0);
  digitalWrite(PIN_DIRA, LOW);
}

void loop()
{
  switch ( mode )
  {
    case STOP:
      m_stop();
      break;
    case RUN:
      m_run();
      break;
    case DEBUG:
      m_debug();
      break;
    case I2CSETUP:
      m_i2cSetup();
      break;
  }
  if(mode > I2CSETUP)
  {
    mode = STOP;
  }
}

void m_stop()
{
  while(!exitFlag)
  {
    if(Serial.available() > 0)
    {
      char cmd = Serial.read();
      if ( cmd == SETMODE)
      {
          mode = (ModeType) Serial.read();
          exitFlag = true;
      }
    }
  }
}

void m_run()
{
  // Enable controller timer interrupt
  while(!exitFlag)
  {
    if(Serial.available() > 0)
    {
      char cmd = Serial.read();
      switch ( cmd )
      {
        case STOPB:
          mode = STOP;
          exitFlag = true;
          break;
        case SETMODE:
          mode = (ModeType) Serial.read();
          exitFlag = true;
          break;
        case SETA:
          setPts[0] = Serial.read() << 8;
          setPts[0] |= Serial.read();
          break;
        case SETB:
          setPts[1] = Serial.read() << 8;
          setPts[1] |= Serial.read();
          break;
        case SETBOTH:
          setPts[0] = Serial.read() << 8;
          setPts[0] |= Serial.read();
          setPts[1] = Serial.read() << 8;
          setPts[1] |= Serial.read();
          break;
      }
    }
    setPeltiers();
  }
  //Disable controller interrupt
}

void m_debug()
{
  
}

void m_i2cSetup()
{
  while(!exitFlag)
  {
    if(Serial.available() > 0)
    {
      char cmd = Serial.read();
      switch ( cmd )
      {
        case STOP:
          mode = STOP;
          exitFlag = true;
          break;
        case SETMODE:
          mode = (ModeType) Serial.read();
          exitFlag = true;
          break;
        case SETDEVID:
          byte addr = Serial.read();
          ChangeAddress(addr,0x00);
          break;
        //case LISTDEV:
      }
    }
  }
}

void setPeltiers()
{
  // Peltier A
  analogWrite(PIN_PWMA, 0);
  if(setPts[0] < 0)
  {
    digitalWrite(PIN_DIRA, LOW);
  }else{
    digitalWrite(PIN_DIRA, HIGH);
  }
  analogWrite(PIN_PWMA, abs(setPts[0]));
  
  // Peltier B
  analogWrite(PIN_PWMB, 0);
  if(setPts[1] < 0)
  {
    digitalWrite(PIN_DIRB, LOW);
  }else{
    digitalWrite(PIN_DIRB, HIGH);
  }
  analogWrite(PIN_PWMB, abs(setPts[1]));
}
