#ifndef COLORSCHEME_H
#define COLORSCHEME_H

#include "color_definitions.h"


/**
 * Defines a color scheme to use while drawing patterns
 * on the arduino
 */

 
class ColorScheme 
{

public:
   int numberOfColors;
   uint32_t color1;
   uint32_t color2;
   uint32_t color3;
   uint32_t color4;
   uint32_t color5;
   uint32_t color6;

   ColorScheme()
   {
      numberOfColors = 1;
      color1 = RED;
      color2 = BLACK;
      color3 = BLACK;
      color4 = BLACK;
      color5 = BLACK;
      color6 = BLACK;
   }


   ColorScheme(uint32_t number_Of_Colors, uint32_t color_1, uint32_t color_2, uint32_t color_3, uint32_t color_4, uint32_t color_5, uint32_t color_6)
   {
      numberOfColors = number_Of_Colors;
      color1 = color_1;
      color2 = color_2;
      color3 = color_3;
      color4 = color_4;
      color5 = color_5;
      color6 = color_6;
   }
   
   void setNumberOfColors(int number_Of_Colors)
   {
      if(numberOfColors > 0 && numberOfColors <= 6)
      {
         numberOfColors = number_Of_Colors;
      }
      else
      {
         numberOfColors = 1;
      }
   }
   
   void setColorScheme(uint32_t color_1, uint32_t color_2, uint32_t color_3, uint32_t color_4, uint32_t color_5, uint32_t color_6)
   {
      color1 = color_1;
      color2 = color_2;
      color3 = color_3;
      color4 = color_4;
      color5 = color_5;
      color6 = color_6;
   }
   
   uint32_t getColor(int color)
   {
      if ((color > 0) && (color <= numberOfColors))
      {
         switch(color)
         {
            case 1: return color1;
            case 2: return color2;
            case 3: return color3;
            case 4: return color4;
            case 5: return color5;
            case 6: return color6;
         }
      }
      else return color1;
   }
};

#endif
