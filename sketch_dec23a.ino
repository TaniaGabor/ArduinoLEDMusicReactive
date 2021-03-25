//Gabor Tania Loreana 30234
#include <FastLED.h>
#include <EEPROM.h>  
#define NUM_OF_LEDS 30  //numarul de leduri
#define DATA_PIN 9  //banda de leduri conectata la D4
#define BUTTON_PIN 3

#define LED_TYPE    WS2812B
#define COLOR_ORDER RGB
CRGB leds[NUM_OF_LEDS];
int buttonState = HIGH;             // the current reading from the input pin
int lastButtonState = HIGH;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN,INPUT_PULLUP);
  pinMode(A0, INPUT); //analog pin A0
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_OF_LEDS);  
  
  for(int i=0; i<NUM_OF_LEDS; i++)  //inchide toate ledurile
  {leds[i] = CRGB::Black; }
  FastLED.show(); //updateaza ledurile
  Serial.println("Setup");
  
}


void loop() {
  int ledMode = 0;
  int reading = digitalRead(BUTTON_PIN);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        EEPROM.get(0, ledMode);
        ledMode = (ledMode + 1 )% 4;
        Serial.println(ledMode);
        EEPROM.put(0, ledMode);
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  
  int data = analogRead(A0);
  Serial.println(data);

  EEPROM.get(0, ledMode);
  lightLeds(data, ledMode);

}


void lightLeds(int data, int mode)
{
  int sensitivity = 1000;
  int val1 = map(data, 0, sensitivity+1, 0, NUM_OF_LEDS);
  int val2 = map(data, 0, sensitivity+1, 0, 255);
  int val3 = map(data, 0, sensitivity+1, 0, NUM_OF_LEDS/2);
  

/*----------------------------------------------------------
  - codul pentru 4 moduri de functionare a ledurilor
  - trebuie scos comentariul de pe linia care se doreste a fii functionala        
-----------------------------------------------------------*/

  if (mode == 0) 
  {
    inLine(100,val1 );
  }
  else if (mode == 1)
  {
    inBrightness(val2);  
  }
  else if (mode == 2)
  {
    inCenter(100,val3 );
  }
  else if (mode == 3)
  {
     inEdge(100,val3 );
  }
  
  /*se poate schimba intensitatea luminoasa*/
  
  FastLED.show();  
}

void inLine( int brightness,int val){
  int hue = 0;
  for(int i=0; i<=val; i++)
  {
  leds[i] = CHSV(hue+=40,200,brightness); /*nuanta,saturatie,luminozitate*/
  }

  for(int i=val+1; i<=NUM_OF_LEDS; i++)
  {
    leds[i].nscale8(10);
  }
}
void inEdge( int brightness,int val){
  int hue = 0;
  for(int i=0; i<=val; i++)
  {
  leds[i] = CHSV(hue+=20,255,brightness);
  leds[NUM_OF_LEDS-i] = CHSV(hue+=20,255,brightness);
  }

  for(int i=val+1; i<=(NUM_OF_LEDS/2); i++)
  {
  leds[i].nscale8(10);
  leds[NUM_OF_LEDS-i].nscale8(10);
  }
}


void inCenter( int brightness,int val){
  int hue = 0;
  for(int i=0; i<=val; i++)
  {
  leds[(NUM_OF_LEDS/2)+i] = CHSV(hue+=20,255,brightness);
  leds[(NUM_OF_LEDS/2)-i] = CHSV(hue+=20,255,brightness);
  }

  for(int i=val+1; i<=(NUM_OF_LEDS/2); i++)
  {
  leds[(NUM_OF_LEDS/2)+i].nscale8(10);
  leds[(NUM_OF_LEDS/2)-i].nscale8(10);
  }
}

void inBrightness(int val){
  int hue = 0;
  for(int i=0; i<=NUM_OF_LEDS; i++)
  {
  leds[i] = CHSV(hue+=2,255,val);
  }
}
