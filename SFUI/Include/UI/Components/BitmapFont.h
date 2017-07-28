#ifndef SFUI_BITMAPFONT_H
#define SFUI_BITMAPFONT_H

////////////////////////////////////////////////////////////
//
// MIT License
//
// Copyright(c) 2017 Kurt Slagle - kurt_slagle@yahoo.com
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// The origin of this software must not be misrepresented; you must not claim
// that you wrote the original software.If you use this software in a product,
// an acknowledgment of the software used is required.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Internal Headers
////////////////////////////////////////////////////////////
#include <SFUI/Include/Common.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  struct BitmapFontInfo
  {
    signed fontSize : 16; //In binary file, font size is 2 bytes long (why??)
    signed style : 8; // bit0 = smooth, bit1 = unicode, bit2 = italic, bit3 = bold, bit4 = fixedHeight, 5-7 reserved
    unsigned charset : 4;
    unsigned stretchH : 4;
    unsigned aa : 4;
    unsigned paddingUp : 4;
    unsigned paddingRight : 4;
    unsigned paddingLeft : 4;
    unsigned paddingDown : 4;
    unsigned paddingHoriz : 4;
    unsigned paddingVert : 4;
    unsigned outline : 4;
    std::string fontName; //Null terminated string (will see '0x0000' for byte value
  };

  class BitmapReader
  {
  public:

  };

  class BitmapFont
  {
  public:



  private:



  };
  
}  

#endif // SFUI_BITMAPFONT_H
