#include <FastLED.h>
#include "sprites.h"

#define NUM_LEDS 256 //wait for array to arrive
#define DATA_PIN 2

#define WIDTH 16
#define HEIGHT 16
unsigned char sprite[WIDTH * HEIGHT * 3 + 1];

CRGB leds[NUM_LEDS];

void setup() {
  delay(2000);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(10); //Number 0-255
  FastLED.clear();
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
const unsigned char * sprite_data[] = {mspacman1, mspacman2, mspacman0, blinky_left1, blinky_left2, blinky_up1, blinky_up2,
                                      blinky_right1, blinky_right2, blinky_down1, blinky_down2, galaga, phoenix, galaxian, bosconian, qbert, 
                                      burgertime, donkeykong, pengo, mappy, mrdo, pacman, ladybug, frogger, digdug, berzerk};
const unsigned char frames[] = { 0,1,0,2,0,1,0,2,0,1,0,2,0,1,0,2,0,1,0,2,3,4,3,4,3,4,3,4,3,4,5,6,5,6,5,6,5,6,5,6,
                                      7,8,7,8,7,8,7,8,7,8,9,10,9,10,9,10,9,10,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
#define ANIMATION_END 60
unsigned int current_led = 0;
unsigned int current_frame = ANIMATION_END;
unsigned int line_odd = 0;
unsigned int line_even = 0;

void loop() {
  
  for(int i = 0; i < (WIDTH * HEIGHT * 3 + 1); i++) {
    sprite[i] = pgm_read_byte(&(sprite_data[frames[current_frame]][i]));  // Read array from Flash
  }

  // All sprite characters in a 16x16 serpatine array like:
  // 15-14-13-12-11-10- 9- 8- 7- 6- 5- 4- 3- 2- 1- 0
  // 16-17-18-19-20-21-22-23-24-25-26-27-28-29-30-31
  // grab the bits a shove them into pixels
  for(int line = 0; line < HEIGHT; line += 2) {
    line_odd = (line*WIDTH*3);
    line_even =  ((line+1)*WIDTH*3);   
       
    for(int i = 15; i >= 0; i--) { // For each pixel...
      leds[current_led++].setRGB (sprite[line_odd+(i*3)], 
                      sprite[line_odd+(i*3) + 1],
                      sprite[line_odd+(i*3) + 2]);
    }
    for(int i = 0; i < WIDTH; i++) { // For each pixel...
      leds[current_led++ ].setRGB (sprite[line_even+(i*3)], 
                      sprite[line_even+(i*3) + 1],
                      sprite[line_even+(i*3) + 2]);
    }
    
  }
  FastLED.show();   // Send the updated pixel colors to the hardware.

  if (current_frame < ANIMATION_END)
    delay(133);
  else
    delay(3000);
    
  /* Increment current character pointer */
  current_frame = (current_frame + 1) % ARRAY_SIZE (frames);
  current_led = 0;
}
