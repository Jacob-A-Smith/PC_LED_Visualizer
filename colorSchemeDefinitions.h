#ifndef COLORSCHEMEDEFINITIONS_H
#define COLORSCHEMEDEFINITIONS_H

#include "ColorScheme.h"
#include "color_definitions.h"

#define NUMBER_OF_COLOR_PRESETS 13


const ColorScheme COLOR_PRESET_DEFAULT(3, BLACK, WHITE, RED, BLACK, BLACK, BLACK);
const ColorScheme COLOR_PRESET_RAINBOW(6, RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE);
const ColorScheme COLOR_PRESET_FIRE(5, RED, ORANGERED, DARKORANGE, ORANGE, SUNFLOWERS, BLACK);
const ColorScheme COLOR_PRESET_ICE(5, WHITE, LIGHTBLUE, LIGHTSKYBLUE, AQUA, ICE, BLACK);
const ColorScheme COLOR_PRESET_FIREICE(6, RED, ORANGERED, DARKORANGE, LIGHTBLUE, LIGHTSKYBLUE, AQUA);
const ColorScheme COLOR_PRESET_STARRYNIGHT(4, SUNFLOWERS, STARRYNIGHT, IRISES, EVENING, BLACK, BLACK);
const ColorScheme COLOR_PRESET_GTGOLD(4, PALEGOLD, SILK, PAPER, CHARCOAL, BLACK, BLACK);
const ColorScheme COLOR_PRESET_VOID(5, VOID, FUSCHIA, JEWEL, STARK, PURPLE, BLACK);
const ColorScheme COLOR_PRESET_ENTROPY(4, BLACK, INK, POSY, EMBERS, BLACK, BLACK);
const ColorScheme COLOR_PRESET_STEEL(4, NAVYBLUE, GUNMETAL, RUSTYRED, WARMYELLOW, BLACK, BLACK);
const ColorScheme COLOR_PRESET_GTBLUE(4, PRUSSIANBLUE, SKYBLUE, GOLDLEAF, IVORYBLACK, BLACK, BLACK);
const ColorScheme COLOR_PRESET_AQUAMARINE(4, FEATHER, MARINE, FOREST, SLEEKGREY, BLACK, BLACK);
const ColorScheme COLOR_PRESET_FEL(5, DARKVIOLET, ELECTRICLIME, DEEPPLUM, SHADOW, LIME, BLACK);




/*String getColorPresetStringName(int preset)
{
   switch(preset)
   {
      case 0:  return "Default";
      case 1:  return "Rainbow";
      case 2:  return "Fire";
      case 3:  return "Ice";
      case 4:  return "Fire and Ice";
      case 5:  return "Starry Night";
      case 6:  return "GT Gold";
      case 7:  return "Void";
      case 8:  return "Entropy";
      case 9:  return "Steel";
      case 10: return "GT Blue";
      case 11: return "Aquamarine";
      case 12: return "Fel";
      default: return "UNDEFINED";
   }
}*/

ColorScheme getColorPresetFromInt(int preset)
{
   switch(preset)
   {
      case 0:  return COLOR_PRESET_DEFAULT;
      case 1:  return COLOR_PRESET_RAINBOW;
      case 2:  return COLOR_PRESET_FIRE;
      case 3:  return COLOR_PRESET_ICE;
      case 4:  return COLOR_PRESET_FIREICE;
      case 5:  return COLOR_PRESET_STARRYNIGHT;
      case 6:  return COLOR_PRESET_GTGOLD;
      case 7:  return COLOR_PRESET_VOID;
      case 8:  return COLOR_PRESET_ENTROPY;
      case 9:  return COLOR_PRESET_STEEL;
      case 10: return COLOR_PRESET_GTBLUE;
      case 11: return COLOR_PRESET_AQUAMARINE;
      case 12: return COLOR_PRESET_FEL;
      default: return COLOR_PRESET_DEFAULT;
   }
}


#endif
