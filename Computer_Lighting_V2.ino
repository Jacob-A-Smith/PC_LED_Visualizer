#include <Adafruit_NeoPixel.h>
#include "configuration.h"
#include "color_definitions.h"
#include "modes.h"
#include "ColorScheme.h"
#include "colorSchemeDefinitions.h"




/**************************
 * VARIABLE DECLARATIONS
 **************************/

Adafruit_NeoPixel LEDArray = Adafruit_NeoPixel(LED_COUNT, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

uint32_t colorArray[LED_COUNT];

LightingModes mode;
ColorScheme colorScheme;

uint8_t streamBrightness;
uint8_t streamSpeed;
uint8_t streamLength;

/**************************/


/*********
 * SETUP
 *********/
void setup() {
   LEDArray.begin();  // Call this to start up the LED strip.
   LEDArray.setBrightness(DEFAULT_BRIGHTNESS);
   LEDArray.setPixelColorAddress(colorArray);
   clearLEDs();   // This function, defined below, turns all LEDs off...
   LEDArray.show();   // ...but the LEDs don't actually update until you call this.
   Serial.begin(9600);
   Serial.flush();
   
   resetToDefaultConfiguration();
}
/*********/


/********
 * LOOP
 ********/
void loop() {
  /*
    * loop works as a state machine,
    * and parses new messages when they arrive
    */
   if(messageQueued()){
      parseMessage();
   }
   
   switch(mode)
   {  
      case STATIC:
         staticMode(true);
         break;

      case PULSE:
         pulse();
         break;

      case PULSE_2:
         pulse2();
         break;

      case HEARTBEAT:
         heartbeat();
         break;

      case RIPPLE:
         ripple();
         break;

      case STREAM:
         stream();
         break;

      case STREAM_2:
         stream2();
         break;

      case STROBE:
         strobe(false);
         break;

      case RANDOM_STROBE:
         strobe(true);
         break;

      case CYCLE_STROBE:
         cycleStrobe(false);
         break;

      case CYCLE_STROBE_2:
         cycleStrobe2(false);
         break;

      case RANDOM_CYCLE_STROBE:
         cycleStrobe(true);
         break;

      case RANDOM_CYCLE_STROBE_2:
         cycleStrobe2(true);
         break;

      case MEETING:
         meeting();
         break;

      case MEETING_2:
         meeting2();
         break;

      case SHOOTING_STAR:
         shootingStar();
         break;

      case CHAOS:
         chaos();
         break;

      case LIGHTNING_STORM:
         lightningStorm();
         break;

      case CYCLE:
         cycle();
         break;

      case RAINBOW:
         rainbow();
         break;

      case SPECTRUM:
         spectrum();
         break;

      case SPECTRUM_SHIFT:
         spectrumShift();
         break;
         
      case RANDOM_LIGHTS:
         randomLights();
         break;

      case LIGHTS_OFF:
         lightsOff();
         break;

      default:
         mode = SPECTRUM_SHIFT;
         break;
      
   } // state machine
} // END LOOP
/********/


/********************
 * HELPER FUNCTIONS
 ********************/

/**
 * Set everything to the default configuration
 */
void resetToDefaultConfiguration()
{
   mode = LIGHTS_OFF;
   colorScheme = COLOR_PRESET_DEFAULT;
   
   streamBrightness = DEFAULT_BRIGHTNESS;
   streamSpeed = DEFAULT_SPEED;
   streamLength = DEFAULT_STREAM_LENGTH;
}


/**
 * clears all LED colors, turns them off
 */
void clearLEDs(){
   for (int i = 0; i < LED_COUNT; ++i) {
      LEDArray.setPixelColor(i, 0);
   }
}


/**
 * Tells if there is a message waiting to be parsed
 */
bool messageQueued(){
   if(Serial.available() > 0) {
      return(true);
   }
   return(false);
}


/**
 * Read commands in from the serial buffer
 */
void parseMessage(){
   
   /**
    * messages come in the following format
    * ;CxxDxx, where ';' denotes the start of a new command, 
    * C stands for Command, D stands for data, and xx is an integer value
    * corresponding to what command and data
    */
    
   uint8_t command = 0;
   uint32_t data = 0;
   String msg;

   while(Serial.available() > 0) {

      Serial.readStringUntil(';');
      msg = Serial.readStringUntil('C');
      command = Serial.parseInt();
      msg = Serial.readStringUntil('D');
      data = Serial.parseInt();

      switch(command)
      {
         case 0: // color1
            colorScheme.color1 = data;
            break;
         case 1: // color2
            colorScheme.color2 = data;
            break;
         case 2: // color3
            colorScheme.color3 = data;
            break;
         case 3: // color4
            colorScheme.color4 = data;
            break;
         case 4: // color5
            colorScheme.color5 = data;
            break;
         case 5: // color6
            //Serial.print("color6\n");
            colorScheme.color6 = data;
            break;
         case 6: // numberOfColors
            colorScheme.setNumberOfColors(data);
            break;
         case 7: // mode
            mode = data > (NUMBER_OF_MODES - 1) ? SPECTRUM : static_cast<LightingModes>(data);
            break;
         case 8: // streamBrightness
            if (data != 0) {
               if (streamBrightness + 10 < MAX_BRIGHTNESS) streamBrightness += 10;
               else streamBrightness = MAX_BRIGHTNESS;
            }
            else {
               if (streamBrightness - 10 > MIN_BRIGHTNESS) streamBrightness -= 10;
               else streamBrightness = MIN_BRIGHTNESS;
            }
            LEDArray.setBrightness(streamBrightness);
            break;
         case 9: // streamSpeed
            if (data != 0) {
               if (streamSpeed - 10 > MAX_SPEED) streamSpeed -= 10;
               else streamSpeed = MAX_SPEED;
            }
            else {
               if (streamSpeed + 10 < MIN_SPEED) streamSpeed += 10;
               else streamSpeed = MIN_SPEED;
            }
            break;
         case 10: // streamLength
            if (data != 0) {
               if (streamLength + 1 <= MAX_STREAM_LENGTH) streamLength++;
            }
            else {
               if (streamLength - 1 >= MIN_STREAM_LENGTH) streamLength--;
            }
            break;
         case 11: // color preset
            if(data < NUMBER_OF_COLOR_PRESETS) colorScheme = getColorPresetFromInt(data);
            break;
         case 12: // default configuration
            Serial.flush();
            resetToDefaultConfiguration();
            break;
         default:
            Serial.flush();
            break;
      }
   }
   Serial.flush();
}


/**
 * Colors a rainbow
 */
uint32_t rainbowOrder(byte position) 
{
   // 6 total zones of color change:
   if (position < 31)  // Red -> Yellow (Red = FF, blue = 0, green goes 00-FF)
   {
      return LEDArray.Color(0xFF, position * 8, 0);
   }
   else if (position < 63)  // Yellow -> Green (Green = FF, blue = 0, red goes FF->00)
   {
      position -= 31;
      return LEDArray.Color(0xFF - position * 8, 0xFF, 0);
   }
   else if (position < 95)  // Green->Aqua (Green = FF, red = 0, blue goes 00->FF)
   {
      position -= 63;
      return LEDArray.Color(0, 0xFF, position * 8);
   }
   else if (position < 127)  // Aqua->Blue (Blue = FF, red = 0, green goes FF->00)
   {
      position -= 95;
      return LEDArray.Color(0, 0xFF - position * 8, 0xFF);
   }
   else if (position < 159)  // Blue->Fuchsia (Blue = FF, green = 0, red goes 00->FF)
   {
      position -= 127;
      return LEDArray.Color(position * 8, 0, 0xFF);
   }
   else  //160 <position< 191   Fuchsia->Red (Red = FF, green = 0, blue goes FF->00)
   {
      position -= 159;
      return LEDArray.Color(0xFF, 0x00, 0xFF - position * 8);
   }
}

/********************/





/**************************
 * LIGHTING MODE FUNCTIONS
 **************************/

/**
 *  Colors the entire strip one color
 */
void pureStaticMode(uint32_t color)
{
   for (int i = 0; i < LED_COUNT; ++i) {
      LEDArray.setPixelColor(i, color);
   }
   LEDArray.show();
}


/**
 *  Colors the LED strip according to the scheme
 */
void staticMode(bool idle) {
   int colorLength = (int)(LED_COUNT / colorScheme.numberOfColors);
   int colorLengthModulus = LED_COUNT % colorScheme.numberOfColors;
   int pos = 0;
   uint32_t color;

   for(int i = 1; i <= colorScheme.numberOfColors; ++i)
   {
      color = colorScheme.getColor(i);
      for(int j = pos; j < pos + colorLength; ++j)
      {
         LEDArray.setPixelColor(j, color);
      }
      pos += colorLength;
      if(colorLengthModulus > i)
      {
         LEDArray.setPixelColor(pos, color);
         ++pos;
      }
   }
   LEDArray.show();
   
   while(idle)
   {
      if(messageQueued()){
         idle = false;
      }
   }
}


/**
 *  Fades the static color scheme in and out
 */
void pulse() {
   while(true) {
      if(messageQueued()) {
         break;
      }
      for(int i = streamBrightness; i > 0; i-=5) {
         if(messageQueued()){
            LEDArray.setBrightness(streamBrightness);
            break;
         }
         LEDArray.setBrightness(i);
         staticMode(false);
         LEDArray.show();
         delay(streamSpeed);
      }
      for(int i = 0; i < streamBrightness; i+=5) {
         if(messageQueued()){
            LEDArray.setBrightness(streamBrightness);
            break;
         }
         LEDArray.setBrightness(i);
         staticMode(false);
         LEDArray.show();
         delay(streamSpeed);
      }
   }
}


/**
 *  Cycles through the different colors in the scheme,
 *  fading in and out
 */
void pulse2() {
   uint32_t color;
   while(true) {
      if (messageQueued()) {
         break;
      }
      for(int i = 1; i <= colorScheme.numberOfColors; ++i)
      {
         if(messageQueued()){
            LEDArray.setBrightness(streamBrightness);
            break;
         }
         color = colorScheme.getColor(i);
         for(int j = 0; j < streamBrightness; j+=5) {
            if(messageQueued()){
               LEDArray.setBrightness(streamBrightness);
               break;
            }
            LEDArray.setBrightness(j);
            pureStaticMode(color);
            LEDArray.show();
            delay(streamSpeed);
         }
         for(int j = streamBrightness; j > 0; j-=5) {
            if(messageQueued()){
               LEDArray.setBrightness(streamBrightness);
               break;
            }
            LEDArray.setBrightness(j);
            pureStaticMode(color);
            LEDArray.show();
            delay(streamSpeed);
         }
      }
   }
}


/**
 *  Pulses the strip in a heart beat rhythm
 */
void heartbeat() {
   while(true){
      if(messageQueued()){
         break;
      }
      for(int i = 0; i <= streamBrightness; ++i){
         if(messageQueued()){
            LEDArray.setBrightness(streamBrightness);
            break;
         }
         LEDArray.setBrightness(i);
         staticMode(false);
         if(i == streamBrightness){
            clearLEDs();
         }
         LEDArray.show();
      }
      for(int i = 0; i <= streamBrightness; ++i){
         if(messageQueued()){
            LEDArray.setBrightness(streamBrightness);
            break;
         }
         LEDArray.setBrightness(i);
         staticMode(false);
         if(i == streamBrightness){
            clearLEDs();
         }
         LEDArray.show();
      }
      delay(streamSpeed*10);
   }
}


/**
 *  Moves alternating colors around the strip, every other pixel off
 */
void ripple() {
   int pos = streamLength;
   bool black = false;
   uint32_t color;
   int colorPos = 1;
   clearLEDs();
   while(true){
      if(messageQueued()){
         break;
      }
      if(++pos >= streamLength) {
         if(!black){
            color = colorScheme.getColor(colorPos);
            if(++colorPos > colorScheme.numberOfColors) colorPos = 1;
         }
         else {
            color = BLACK;
         }
         black = !black;
         pos = 0;
      }
      for(int i = LED_COUNT - 1; i > 0; --i) {
         LEDArray.setPixelColor(i, LEDArray.getPixelColorFromRAM(i-1));
         if(messageQueued()){
            break;
         }
      }
      LEDArray.setPixelColor(0, color);
      LEDArray.show();
      delay(streamSpeed);
   }
}


/**
 *  Streams strips of colors around the strip
 *  Uses BLACK as second color if numberOfColors is 1
 */
void stream() {
   int pos = streamLength;
   uint32_t color;
   int colorPos = 1;
   clearLEDs();
   bool monoColor = false;
   if(colorScheme.numberOfColors == 1)
   {
      monoColor = true;
      colorScheme.setNumberOfColors(2);
      colorScheme.color2 = BLACK;
   }
   while(true){
      if(messageQueued()){
         if(monoColor){
            colorScheme.setNumberOfColors(1);
            monoColor = false;
         }
         break;
      }
      if(pos >= streamLength) {
         color = colorScheme.getColor(colorPos);
         if(++colorPos > colorScheme.numberOfColors) colorPos = 1;
         pos = 0;
      }
      ++pos;
      for(int i = LED_COUNT - 1; i > 0; --i) {
         LEDArray.setPixelColor(i, LEDArray.getPixelColorFromRAM(i-1));
         //LEDArray.setPixelColor(i-1, BLACK);
         if(messageQueued()){
            break;
         }
      }
      LEDArray.setPixelColor(0, color);
      LEDArray.show();
      delay(streamSpeed);
   }
}


/**
 *  Sends two streams of colors around the strip
 *  that will mix when they cross. Alternates colors from the scheme
 */
void stream2() {
   clearLEDs();
   int SNAKESEED = colorScheme.numberOfColors * streamLength;
   int pos1 = streamLength;
   int pos2 = LED_COUNT - streamLength;
   uint32_t color1;
   uint32_t color2;
   uint8_t colorPos = 1;
   
   color1 = colorScheme.getColor(colorPos);
   if (++colorPos > colorScheme.numberOfColors) colorPos = 1;
   color2 = colorScheme.getColor(colorPos);
   if (++colorPos > colorScheme.numberOfColors) colorPos = 1;

   for(int i = 0; i < pos1; ++i){
      LEDArray.setPixelColor(i, color1);
   }
   for(int i = LED_COUNT - 1; i > pos2; --i){
      LEDArray.setPixelColor(i, color2);
   }
   LEDArray.show();
   delay(streamSpeed);

   while(true){
      if(messageQueued()){
         break;
      }
      
      for(int i = pos1; i > pos1 - SNAKESEED; --i){
         if(((i >= pos2) && (i <= pos2 + SNAKESEED)) || ((i >= pos2 - LED_COUNT) && (i <= pos2 + SNAKESEED - LED_COUNT))){
            LEDArray.setPixelColor(i, color1 + color2);
         }
         else{
            LEDArray.setPixelColor(i, color1);
         }
      }
      if(!((pos1 - SNAKESEED >= pos2) && (pos1 <= pos2 + SNAKESEED))){
         if(pos1 - SNAKESEED >= 0){
            LEDArray.setPixelColor(pos1 - SNAKESEED, BLACK);
         }
         else{
            LEDArray.setPixelColor((LED_COUNT-1) - SNAKESEED + pos1 + 1, BLACK);
         }
      }

      for(int i = pos2; i < pos2 + SNAKESEED - 1; ++i){
         if(((i <= pos1) && (i >= pos1 - SNAKESEED)) || ((i <= pos1 + LED_COUNT) && (i >= pos1 - SNAKESEED + LED_COUNT))){
            LEDArray.setPixelColor(i, color1 + color2);
         }
         else{
            LEDArray.setPixelColor(i, color2);
         }
      }
      if(!((pos2 + SNAKESEED <= pos1) && (pos2 >= pos1 - SNAKESEED))){
         if(pos2 + SNAKESEED < LED_COUNT){
            LEDArray.setPixelColor(pos2 + SNAKESEED, BLACK);
         }
         else{
            LEDArray.setPixelColor(pos2 + SNAKESEED - 1 - (LED_COUNT-1), BLACK);
         }
      }

      
      if((++pos1 == LED_COUNT) || (--pos2 == -1)){
         pos1 = streamLength;
         pos2 = LED_COUNT - streamLength;
         
         color1 = colorScheme.getColor(colorPos);
         if (++colorPos > colorScheme.numberOfColors) colorPos = 1;
         color2 = colorScheme.getColor(colorPos);
         if (++colorPos > colorScheme.numberOfColors) colorPos = 1;

         for(int i = 0; i < pos1; ++i){
            LEDArray.setPixelColor(i, color1);
         }
         for(int i = LED_COUNT - 1; i > pos2; --i){
            LEDArray.setPixelColor(i, color2);
         }
      }
      LEDArray.show();
      delay(streamSpeed);
   }
}


/**
 *  Strobes the color scheme around the strip
 *  @param randomStrobe : tells it to delay by a random amount or not
 */
void strobe(bool randomStrobe) {
   int colorLength = (int)(LED_COUNT / colorScheme.numberOfColors);
   int colorLengthModulus = LED_COUNT % colorScheme.numberOfColors;
   int pos;
   uint32_t color;
   int interval;
   clearLEDs();
   while(true) {
      if(messageQueued()){
         break;
      }
      if(randomStrobe) {
         randomSeed(analogRead(A0)*analogRead(A1));
         interval = random(1,11);
      }
      pos = 0;
      for(int i = 1; i <= colorScheme.numberOfColors; ++i)
      {
         color = colorScheme.getColor(i);
         for(int j = pos; j < pos + colorLength; ++j)
         {
            LEDArray.setPixelColor(j, color);
            LEDArray.show();
            delay(DRAW_DELAY);
         }
         pos += colorLength;
         if(colorLengthModulus > i)
         {
            LEDArray.setPixelColor(++pos, color);
            LEDArray.show();
            delay(DRAW_DELAY);
         }
      }
      for(int i = 0; i < LED_COUNT; ++i)
      {
         if(messageQueued()){
            break;
         }
         LEDArray.setPixelColor(i, BLACK);
         LEDArray.show();
         delay(DRAW_DELAY);
      }
      if(!randomStrobe) {
         delay(streamSpeed);
      }
      else {
         while(interval-- > 0) {
            if(messageQueued()){
               break;
            }
            delay(1000);
         }
      }
   }
}


/**
 *  Cycles through the color scheme, 
 *  strobing only one color at a time
 *  @param randomStrobe : tells it to delay by a random amount or not
 */
void cycleStrobe(bool randomStrobe) {
   int colorLength = (int)(LED_COUNT / colorScheme.numberOfColors);
   int colorLengthModulus = LED_COUNT % colorScheme.numberOfColors;
   uint32_t color;
   int interval;
   while(true) {
      if(messageQueued()){
         break;
      }
      if(randomStrobe) {
         randomSeed(analogRead(A0)*analogRead(A1));
         interval = random(1,11);
      }
      for(int i = 1; i <= colorScheme.numberOfColors + 1; ++i)
      {
         if(i == colorScheme.numberOfColors + 1) {
            color = BLACK;
         }
         else {
            color = colorScheme.getColor(i);
         }
         for(int j = 0; j < LED_COUNT; ++j)
         {
            if(messageQueued()){
               break;
            }
            LEDArray.setPixelColor(j, color);
            LEDArray.show();
            delay(DRAW_DELAY);
         }
      }
      if(!randomStrobe) {
         delay(streamSpeed);
      }
      else {
         while(interval-- > 0) {
            if(messageQueued()){
               break;
            }
            delay(1000);
         }
      }
   }
}


/**
 *  Cycle strobe, but turns strip off in between colors
 *  @param randomStrobe : tells it to delay by a random amount or not
 */
void cycleStrobe2(bool randomStrobe) {
   int colorLength = (int)(LED_COUNT / colorScheme.numberOfColors);
   int colorLengthModulus = LED_COUNT % colorScheme.numberOfColors;
   uint32_t color;
   int interval;
   while(true) {
      if(messageQueued()){
         break;
      }
      if(randomStrobe) {
         randomSeed(analogRead(A0)*analogRead(A1));
         interval = random(1,11);
      }
      for(int i = 1; i <= colorScheme.numberOfColors; ++i)
      {
         color = colorScheme.getColor(i);
         for(int j = 0; j < LED_COUNT; ++j)
         {
            if(messageQueued()){
               break;
            }
            LEDArray.setPixelColor(j, color);
            LEDArray.show();
            delay(DRAW_DELAY);
         }
         for(int j = 0; j < LED_COUNT; ++j)
         {
            if(messageQueued()){
               break;
            }
            LEDArray.setPixelColor(j, BLACK);
            LEDArray.show();
            delay(DRAW_DELAY);
         }
      }
      if(!randomStrobe) {
         delay(streamSpeed);
      }
      else {
         while(interval-- > 0) {
            if(messageQueued()){
               break;
            }
            delay(1000);
         }
      }
   }
}


/**
 *  Draws strips of colors starting at opposite ends
 *  meeting in the middle, then erases from the middle
 */
void meeting() {
   uint32_t color;
   int pos;
   int colorPos;
   clearLEDs();
   LEDArray.show();
   while(true) {
      if(messageQueued()){
         break;
      }
      pos = 0;
      colorPos = 1;
      for(int i = 0; i < LED_COUNT/2; ++i)
      {
         if(messageQueued()){
            break;
         }
         if (++pos >= streamLength) {
            color = colorScheme.getColor(colorPos);
            if (++colorPos > colorScheme.numberOfColors) colorPos = 1;
            pos = 0;
         }
         LEDArray.setPixelColor(i, color);
         LEDArray.setPixelColor((LED_COUNT-1) - i, color);
         LEDArray.show();
         delay(streamSpeed);
      }
      for(int i = (LED_COUNT/2) - 1; i >= 0; --i){
         if(messageQueued()){
            break;
         }
         LEDArray.setPixelColor(i, BLACK);
         LEDArray.setPixelColor((LED_COUNT - 1) - i, BLACK);
         LEDArray.show();
         delay(streamSpeed);
      }
   }
}


/**
 *  Draws strips of colors starting at opposite ends
 *  meeting in the middle, then erases from the outside
 */
void meeting2() {
   uint32_t color;
   int pos;
   int colorPos;
   clearLEDs();
   LEDArray.show();
   while(true) {
      if(messageQueued()){
         break;
      }
      pos = 0;
      colorPos = 1;
      for(int i = 0; i < LED_COUNT/2; ++i)
      {
         if(messageQueued()){
            break;
         }
         if (++pos >= streamLength) {
            color = colorScheme.getColor(colorPos);
            if (++colorPos > colorScheme.numberOfColors) colorPos = 1;
            pos = 0;
         }
         LEDArray.setPixelColor(i, color);
         LEDArray.setPixelColor((LED_COUNT-1) - i, color);
         LEDArray.show();
         delay(streamSpeed);
      }
      for(int i = 0; i < LED_COUNT/2; ++i){
         if(messageQueued()){
            break;
         }
         LEDArray.setPixelColor(i, BLACK);
         LEDArray.setPixelColor((LED_COUNT - 1) - i, BLACK);
         LEDArray.show();
         delay(streamSpeed);
      }
   }
}


/**
 *  Randomly sends some lights around the strip
 */
void shootingStar() {
   int temp = 0;
   uint32_t color;
   //clearLEDs();
   while(true){
      for(int i = LED_COUNT - 1; i > 0; --i){
         if(messageQueued()){
            break;
         }
         LEDArray.setPixelColor(i, LEDArray.getPixelColorFromRAM(i-1));
         LEDArray.setPixelColor(i-1, BLACK);
      }
      randomSeed(analogRead(A0)*analogRead(A1)*analogRead(A0)*analogRead(A5));
      temp = random(1, 13);
      if(temp % 6 == 0){
         color = colorScheme.getColor(random(0, colorScheme.numberOfColors) + 1);
         LEDArray.setPixelColor(0, color);
      }
      LEDArray.show();
      delay(streamSpeed*4);
      if(messageQueued()){
         break;
      }
   }
}


/**
 * lights random lights random colors
 */
void chaos() {
   clearLEDs();
   uint8_t pin1 = 0;
   uint8_t pin2 = 4;
   uint8_t pseudoColor = 0;
   uint8_t red = 0;
   uint8_t green = 0;
   uint8_t blue = 0;
   int pixel = 0;

   while(true){
      if(messageQueued()){
         break;
      }
      randomSeed(analogRead(pin1)*analogRead(pin2)*analogRead(A0)*analogRead(A5));
      pixel = random(0, LED_COUNT);
      pseudoColor = random(1, 26);
      switch(pseudoColor){
         case 18:
         case 10:
         case 1: // 0x0000FF
            blue = random(0,256);
            LEDArray.setPixelColor(pixel, 0, 0, blue);
            LEDArray.show();
            break;

         case 19:
         case 11:
         case 2: // 0x00FFFF
            green = random(0, 256);
            blue = random(0, 256);
            LEDArray.setPixelColor(pixel, 0, green, blue);
            LEDArray.show();
            break;

         case 20:
         case 12:
         case 3: // 0x00FF00
            green = random(0, 256);
            LEDArray.setPixelColor(pixel, 0, green, 0);
            LEDArray.show();
            break;

         case 21:
         case 13:
         case 4: // 0xFFFF00
            red = random(0, 256);
            green = random(0, 256);
            LEDArray.setPixelColor(pixel, red, green, 0);
            LEDArray.show();
            break;

         case 22:
         case 14:
         case 5: // 0xFF0000
            red = random(0, 256);
            LEDArray.setPixelColor(pixel, red, 0, 0);
            LEDArray.show();
            break;

         case 23:
         case 15:
         case 6: // 0xFF00FF
            red = random(0, 256);
            blue = random(0, 256);
            LEDArray.setPixelColor(pixel, red, 0, blue);
            LEDArray.show();
            break;

         case 24:
         case 16:
         case 7: // 0xFFFFFF
            red = random(0, 256);
            green = random(0, 256);
            blue = random(0, 256);
            LEDArray.setPixelColor(pixel, red, green, blue);
            LEDArray.show();
            break;

         case 25:
         case 17:
         case 8: // 0x000000
            LEDArray.setPixelColor(pixel, BLACK);
            LEDArray.show();
            break;

         default:
            clearLEDs();
            LEDArray.show();
      }
      delay(DRAW_DELAY);
      if(++pin1 == 5){
         pin1 = 4;
      }
      if(--pin2 == -1){
         pin2 = 4;
      }
   }
}


/**
 * Simulates a thunder storm
 */
void lightningStorm() {
   int sequence;
   uint32_t color;
   clearLEDs();

   while(true){
      if(messageQueued()){
         break;
      }
      color = colorScheme.getColor(random(0, colorScheme.numberOfColors) + 1);
      randomSeed(analogRead(A0)*analogRead(A0)+color);
      sequence = random(0,5);

      switch(sequence){
         case 0:
            for(int i = 0; i < 3; ++i){
               for(int j = 0; j < LED_COUNT; ++j){
                  LEDArray.setPixelColor(j, color);
               }
               LEDArray.show();
               delay(streamSpeed);
               clearLEDs();
               LEDArray.show();
               delay(streamSpeed);
            }
            break;

         case 1:
            for(int i = 0; i < LED_COUNT; ++i){
               LEDArray.setPixelColor(i, color);
            }
            LEDArray.show();
            delay(streamSpeed);
            clearLEDs();
            delay(streamSpeed*5);

         case 2:
            for(int i = 0; i < 2; ++i){
               for(int j = 0; j < LED_COUNT; ++j){
                  LEDArray.setPixelColor(j, color);
               }
               LEDArray.show();
               delay(streamSpeed);
               clearLEDs();
               LEDArray.show();
               delay(streamSpeed);
            }
            break;

         case 3:
            for(int i = 0; i < 2; ++i){
               for(int j = 0; j < LED_COUNT; ++j){
                  LEDArray.setPixelColor(j, color);
               }
               LEDArray.show();
               delay(streamSpeed);
               clearLEDs();
               LEDArray.show();
               delay(streamSpeed);
            }
            delay(streamSpeed*4);

         case 4:
            for(int i = 0; i < LED_COUNT; ++i){
               LEDArray.setPixelColor(i, color);
            }
            LEDArray.show();
            delay(streamSpeed);
            clearLEDs();
            LEDArray.show();
            delay(streamSpeed*3);
            break;

         default:
            delay(streamSpeed);
      }
      sequence = random(10,21);
      for(int i = 0; i <= sequence; ++i){
         if(messageQueued()){
            break;
         }
         delay(streamSpeed);
      }
   }
}


/**
 *  Cycles through the color scheme,
 *  turning the strip that color 1 pixel at a time
 */ 
void cycle() {
   uint32_t color;
   int colorPos = 1;
   clearLEDs();
   while(true)
   {
      if(messageQueued()){
         break;
      }
      color = colorScheme.getColor(colorPos);
      if(++colorPos == colorScheme.numberOfColors + 1)
      {                                                             
         colorPos = 1;
      }
      for(int i = 0; i < LED_COUNT; ++i){
         if(messageQueued()){
            break;
         }
         LEDArray.setPixelColor(i, color);
         LEDArray.show();
         delay(streamSpeed);
      }
   }
}


/**
 * ride the rainbow
 */
void rainbow() {
   clearLEDs();
   while(true){
      if(messageQueued()){
         break;
      }
      for (int i = 0; i < LED_COUNT*10; ++i){
         if(messageQueued()){
            break;
         }
         // Need to scale our rainbow. We want a variety of colors, even if there
         // are just 10 or so pixels.
         int rainbowScale = 192 / LED_COUNT;
      
         // Next we setup each pixel with the right color
         for (int j=0; j < LED_COUNT; ++j){
            if(messageQueued()){
               break;
            }
            // There are 192 total colors we can get out of the rainbowOrder function.
            // It'll return a color between red->orange->green->...->violet for 0-191.
            LEDArray.setPixelColor(j, rainbowOrder((rainbowScale * (j + i)) % 191));
            
         }
         // Finally, actually turn the LEDs on:
         LEDArray.show();
         delay(streamSpeed);  // Delay between rainbow slides
      }
   }
}


/**
 * pulses through the color spectrum
 */
void spectrum() {
   clearLEDs();
   while(true){
      if(messageQueued()){
         break;
      }
      for(int green = 0; green <= 255; ++green){
         if(messageQueued()){
            break;
         }
         for(int i = 0; i < LED_COUNT; ++i){
            LEDArray.setPixelColor(i, 0, green, 255);
         }
         delay(streamSpeed);
         LEDArray.show();
      } // 0x00FFFF
      for(int blue = 255; blue >= 0; --blue){
         if(messageQueued()){
            break;
         }
         for(int i = 0; i < LED_COUNT; ++i){
            LEDArray.setPixelColor(i, 0, 255, blue);
         }
         delay(streamSpeed);
         LEDArray.show();
      } // 0x00FF00
      for(int red = 0; red <= 255; ++red){
         if(messageQueued()){
            break;
         }
         for(int i = 0; i < LED_COUNT; ++i){
            LEDArray.setPixelColor(i, red, 255, 0);
         }
         delay(streamSpeed);
         LEDArray.show();
      } // 0xFFFF00
      for(int green = 255; green >= 0; --green){
         if(messageQueued()){
            break;
         }
         for(int i = 0; i < LED_COUNT; ++i){
            LEDArray.setPixelColor(i, 255, green, 0);
         }
         delay(streamSpeed);
         LEDArray.show();
      } // 0xFF0000
      for(int blue = 0; blue <= 255; ++blue){
         if(messageQueued()){
            break;
         }
         for(int i = 0; i < LED_COUNT; ++i){
            LEDArray.setPixelColor(i, 255, 0, blue);
         }
         delay(streamSpeed);
         LEDArray.show();
      }// 0xFF00FF
      for(int red = 255; red >= 0; --red){
         if(messageQueued()){
            break;
         }
         for(int i = 0; i < LED_COUNT; ++i){
            LEDArray.setPixelColor(i, red, 0, 255);
         }
         delay(streamSpeed);
         LEDArray.show();
      } // 0x0000FF
   }
}


/**
 * displays a rainbow on the strip
 */
void spectrumShift() {
   int pixel = 0;
   while(true){
      if(messageQueued()){
         break;
      }
      for(int green = 0; green <= 255; green+=17){
         if(messageQueued()){
            break;
         }
         LEDArray.setPixelColor(pixel, 0, green, 255);
         LEDArray.show();
         delay(streamSpeed);
         if(++pixel == LED_COUNT){
            pixel = 0;
         }
      } // 0x00FFFF
      for(int blue = 255; blue >= 0; blue-=17){
         if(messageQueued()){
            break;
         }
         LEDArray.setPixelColor(pixel, 0, 255, blue);
         LEDArray.show();
         delay(streamSpeed);
         if(++pixel == LED_COUNT){
            pixel = 0;
         }
      } // 0x00FF00
      for(int red = 0; red <= 255; red+=17){
         if(messageQueued()){
            break;
         }
         LEDArray.setPixelColor(pixel, red, 255, 0);
         LEDArray.show();
         delay(streamSpeed);
         if(++pixel == LED_COUNT){
            pixel = 0;
         }
      } // 0xFFFF00
      for(int green = 255; green >= 0; green-=17){
         if(messageQueued()){
            break;
         }
         LEDArray.setPixelColor(pixel, 255, green, 0);
         LEDArray.show();
         delay(streamSpeed);
         if(++pixel == LED_COUNT){
            pixel = 0;
         }
      } // 0xFF0000
      for(int blue = 0; blue <= 255; blue+=17){
         if(messageQueued()){
            break;
         }
         LEDArray.setPixelColor(pixel, 255, 0, blue);
         LEDArray.show();
         delay(streamSpeed);
         if(++pixel == LED_COUNT){
            pixel = 0;
         }
      } // 0xFF00FF
      for(int red = 255; red >= 0; red-=17){
         if(messageQueued()){
            break;
         }
         LEDArray.setPixelColor(pixel, red, 0, 255);
         LEDArray.show();
         delay(streamSpeed);
         if(++pixel == LED_COUNT){
            pixel = 0;
         }
      } //0x0000FF
   }
}


/**
 *  Turns all pixels white, like a flash light
 */
void flashLight() {
   for(int i = 0; i < LED_COUNT; ++i)
   {
      LEDArray.setPixelColor(i, WHITE);
   }
   LEDArray.show();
   while(true)
   {
      if(messageQueued()){
         break;
      }
   }
}


/**
 *  Lights a random light one of the colors
 *  and sends it around the strip
 */
void randomLights() {
   int tempColor = 0;
   uint32_t color;
   clearLEDs();   
   bool monoColor = false;
   if(colorScheme.numberOfColors == 1)
   {
      monoColor = true;
      colorScheme.setNumberOfColors(2);
      colorScheme.color2 = BLACK;
   }
   while(true){
      for(int i = LED_COUNT - 1; i > 0; --i){
         if(messageQueued()){
            break;
         }
         LEDArray.setPixelColor(i, LEDArray.getPixelColorFromRAM(i-1));
      }
      randomSeed(analogRead(A0)*analogRead(A1)*analogRead(A0)*analogRead(A5));
      color = colorScheme.getColor(random(0, colorScheme.numberOfColors) + 1);
      LEDArray.setPixelColor(0, color);
      LEDArray.show();
      delay(streamSpeed);
      if(messageQueued()){
         if(monoColor){
            colorScheme.setNumberOfColors(1);
            monoColor = false;
         }
         break;
      }
   }
}
         

/**
 *  Turns all lights off
 */
void lightsOff() {
   clearLEDs();
   LEDArray.show();
   while(true)
   {
      if(messageQueued()){
         break;
      }
   }
}

