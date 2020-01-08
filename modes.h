#ifndef LIGHTINGMODES_H
#define LIGHTINGMODES_H

/********************************************************
 * STATE MACHINE:
 *    each ENUM defines a different mode of lighting to
 *    draw on the arduino.
 ********************************************************/

#define NUMBER_OF_MODES          24

enum LightingModes {
   STATIC,
   PULSE,
   PULSE_2,
   HEARTBEAT,
   RIPPLE,
   STREAM,
   STREAM_2,
   STROBE,
   RANDOM_STROBE,
   CYCLE_STROBE,
   CYCLE_STROBE_2,
   RANDOM_CYCLE_STROBE,
   RANDOM_CYCLE_STROBE_2,
   MEETING,
   MEETING_2,
   SHOOTING_STAR,
   CHAOS,
   LIGHTNING_STORM,
   CYCLE,
   RAINBOW,
   SPECTRUM,
   SPECTRUM_SHIFT,
   RANDOM_LIGHTS,
   LIGHTS_OFF
};
/*
String getModeStringName(int mode)
{
   switch(mode) 
   {
      case 0:     return   "Static";
      case 1:     return   "Pulse";
      case 2:     return   "Pulse 2";
      case 3:     return   "Heartbeat";
      case 4:     return   "Ripple";
      case 5:     return   "Stream";
      case 6:     return   "Stream 2";
      case 7:     return   "Strobe";
      case 8:     return   "Random Strobe";
      case 9:     return   "Cycle Strobe";
      case 10:    return   "Cycle Strobe 2";
      case 11:    return   "Random Cycle Strobe";
      case 12:    return   "Random Cycle Strobe 2";
      case 13:    return   "Meeting";
      case 14:    return   "Meeting 2";
      case 15:    return   "Shooting Star";
      case 16:    return   "Chaos";
      case 17:    return   "Lightning Storm";
      case 18:    return   "Cycle";
      case 19:    return   "Rainbow";
      case 20:    return   "Spectrum";
      case 21:    return   "Spectrum Shift";
      case 22:    return   "Random Mode";
      case 23:    return   "Lights Off";
      default:    return   "UNDEFINED";
   }
}*/

#endif
