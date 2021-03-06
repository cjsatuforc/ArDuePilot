#include "pins_map.h"
#include <Arduino.h>
#include "sound_notes.h"

static int counter_arm1 = 0;
static int counter_arm2 = 0;
static int mode_arming = 0;
static int counter_skipgps = 0;
static int counter_reset = 0;


int arming(int throttle_pos, int pitch_pos, int roll_pos, int yaw_pos, int flap_pos, float phi, float theta, int gps_fix, int gps_sats, float *altitude_home_m, float altitude_m, double *lat_home, double lat, double *lon_home, double lon)
{
  //LEDS
  int green_led = 0;
  int yellow_led = 0;
  int red_led = 0;
  int blue_led = 0;

  // MODE LOGIC
  if (mode_arming == 0)
  {
    red_led = 1;
    if (throttle_pos < 1250)
      counter_arm1++;
    if ((counter_arm1 >= 60) && (throttle_pos < 1250) && (yaw_pos < 1300))
      counter_arm2++;
    if ((counter_arm2 > 60) && (throttle_pos < 1150))
    {
      mode_arming = 1;
      //  SoundNoTimer(SOUND_PIN,300,NOTE_C4);
      // delay(600);
      // SoundNoTimer(SOUND_PIN,300,NOTE_C4);
    }
  }
  else if (mode_arming == 1)
  {
    red_led = 1;
    yellow_led = 1;
    if (phi < 15 && theta < 15)
    {
      mode_arming = 2;
    }
  }
  else if (mode_arming == 2)
  {
    yellow_led = 1;
    if (gps_fix >= 1 && gps_sats >= 3)
    {
      mode_arming = 3;
    }
    if (roll_pos > 1800 && pitch_pos > 1800)
    {
      counter_skipgps++;
      if (counter_skipgps > 300)
      {
        mode_arming = 3;
      }
    }

  }
  else if (mode_arming == 3)
  {
    yellow_led = 1;
    if (roll_pos < 1200 && pitch_pos < 1200) // perform set home control sequence here
    {
      mode_arming = 4;
      *altitude_home_m = altitude_m;
      *lat_home = lat;
      *lon_home = lon;
      // *lat_home = 34.666;  //test
      // *lon_home = -118.17;
    }
  }
  else if (mode_arming == 4)
  {
    green_led = 1;
    // RESET modes
    if((throttle_pos < 1200) && (yaw_pos > 1800) && (roll_pos > 1800) && (pitch_pos < 1200) ) // perform set home control sequence here
    {
      counter_reset++;
      if(counter_reset > 750)
      {
        mode_arming = 3;
      }
    }
    else
    {
      counter_reset = 0
    }
   

  }
  // Mode 5 = RTH


  if (gps_fix >= 1 && gps_sats >= 3)
  {
    blue_led = 1;
  }



  // BUZZs
  /*
  if (counter_arm1 == 60)
    SoundNoTimer(SOUND_PIN,300,NOTE_B5);
  if (counter_arm2 == 60)
    SoundNoTimer(SOUND_PIN,300,NOTE_B5);
  */

  // LEDS
  if (green_led > .5)
    digitalWrite(GREEN_LED, HIGH);
  else
    digitalWrite(GREEN_LED, LOW);

  if (yellow_led > .5)
    digitalWrite(YELLOW_LED, HIGH);
  else
    digitalWrite(YELLOW_LED, LOW);

  if (red_led > .5)
    digitalWrite(RED_LED, HIGH);
  else
    digitalWrite(RED_LED, LOW);

  if (blue_led > .5)
    digitalWrite(BLUE_LED, HIGH);
  else
    digitalWrite(BLUE_LED, LOW);



  return (mode_arming);
}





