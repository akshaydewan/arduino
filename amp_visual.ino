/* Amp Visualizer
 * ----------------
 *
 * Program using a Piezo element and an array of LEDs to visualize the loudness.
 * (copyleft) Akshay Dewan 02 June 2013
 */

int ledPin[] = {4, 5, 6, 7, 8, 9, 10, 11}; //Which pins have LEDs attached
int ledCount = 8; //Number of LEDs
int currentIndex = 0; //upto which index are the LEDs currently HIGH
int sensorInput = 0; 
byte val = 0;

int threshold_min = 3;
int threshold_max = 10;

int fallRate = 100; //the rate (ms) at which the lights should "fall" if there is no input
boolean fallingState = false;
long prevFall = 0;

int DEBOUNCE_THRESHOLD = 50; //milliseconds
long prevStateChange = 0;

int debugPin = 13;

void setup() {
  for(int i = 0; i < ledCount; i++)
  {
    pinMode(ledPin[i], OUTPUT);
  }
  pinMode(debugPin, OUTPUT);
}

void setAllLed(int state)
{
  setLed(ledCount - 1, state);
}

/**
 * Set the LEDs upto endIndex with specified state
 */
void setLed(int endIndex, int state)
{
  for(int i = 0; i < ledCount; i++)
  {
    if(i <= endIndex)
    {
      digitalWrite(ledPin[i], state);
    }
    else
    {
      digitalWrite(ledPin[i], LOW);
    }
  }
  if(state == HIGH)
  {
    currentIndex = endIndex;
  }
  else
  {
    currentIndex = -1;
  }
  if(endIndex < 0)
  {
    //digitalWrite(debugPin, HIGH);
  }
}

void loop() 
{
  val = analogRead(sensorInput);    
  //Serial.println(val);
  if (val >= threshold_min && millis() - prevStateChange > DEBOUNCE_THRESHOLD) 
  {
    if(val < threshold_min)
    {
      setAllLed(LOW);
    } 
    else if (val > threshold_max)
    {
      setAllLed(HIGH);
    }
    else
    {
      int onPins = map(val, threshold_min, threshold_max, 1, ledCount);
      setLed(onPins-1, HIGH);
    }
    
    //Serial.println("Knock!");
    prevStateChange = millis();

    //prevFall = 0;
    fallingState = false;
    //digitalWrite(debugPin, LOW);
  }
  else
  {
    //we make the lights "fall"
    //we turn off the top most led after the specified fall rate
    
    //digitalWrite(debugPin, HIGH);
    
    if(currentIndex >= 0 && millis() - prevFall >= fallRate)
    {
      setLed(currentIndex - 1, HIGH);
      prevFall = millis();
    }
    fallingState = true;
  }
  //delay(100);  // we have to make a delay to avoid overloading the serial port
}

