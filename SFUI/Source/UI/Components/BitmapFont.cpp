//////////////////////////////////////////////////////////////
////
//// MIT License
////
//// Copyright(c) 2017 Kurt Slagle - kurt_slagle@yahoo.com
//// 
//// Permission is hereby granted, free of charge, to any person obtaining a copy
//// of this software and associated documentation files(the "Software"), to deal
//// in the Software without restriction, including without limitation the rights
//// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//// copies of the Software, and to permit persons to whom the Software is
//// furnished to do so, subject to the following conditions :
////
//// The above copyright notice and this permission notice shall be included in all
//// copies or substantial portions of the Software.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//// SOFTWARE.
////
//// The origin of this software must not be misrepresented; you must not claim
//// that you wrote the original software.If you use this software in a product,
//// an acknowledgment of the software used is required.
////
//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
//// Internal Headers
//////////////////////////////////////////////////////////////
//#include <SFUI/Include/UI/Components/TextView.h>
//
//////////////////////////////////////////////////////////////
//// Dependency Headers
//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
//// Standard Library Headers
//////////////////////////////////////////////////////////////
//#include <fstream>
//
//namespace sfui
//{  
//  
//  void BitmapText::LoadFontTexture(const std::string &File)
//  {
//    font.loadTexture(File);
//  }
//
//  void BitmapText::ParseFntFile(const std::string &File)
//  {
//    std::ifstream BMPFile(File);
//    if (!BMPFile) {
//      std::cerr << "Unable to load bitmap fnt file \"" << File << "\"\n";
//      return;
//    }
//
//    std::string discard, linestart, val_id, equal; int count = 0;
//    int id, x, y, width, height, xoffset, yoffset, xadvance, page, chnl;
//
//    BMPFile >> linestart >> val_id >> equal >> count; // "chars count = #"
//
//    //Read the glyph info
//    for (int i = 0; i < count; ++i) {
//      BMPFile >> linestart >> val_id >> equal >> id; //"chars id = #"
//      BMPFile >> val_id >> equal >> x;        //std::cout << val_id << " " << equal << " " << x << " ";             //"x = #"
//      BMPFile >> val_id >> equal >> y;        //std::cout << val_id << " " << equal << " " << y << " ";      //"y = #"
//      BMPFile >> val_id >> equal >> width;    //std::cout << val_id << " " << equal << " " << width << " ";      //"width = #"
//      BMPFile >> val_id >> equal >> height;   //std::cout << val_id << " " << equal << " " << height << " ";      //"height = #"
//      BMPFile >> val_id >> equal >> xoffset;  //std::cout << val_id << " " << equal << " " << xoffset << " ";      //xoffset = #
//      BMPFile >> val_id >> equal >> yoffset;  //std::cout << val_id << " " << equal << " " << yoffset << " ";
//      BMPFile >> val_id >> equal >> xadvance; //std::cout << val_id << " " << equal << " " << xadvance << " ";
//      BMPFile >> val_id >> equal >> page;     //std::cout << val_id << " " << equal << " " << page << " ";
//      BMPFile >> val_id >> equal >> chnl;     //std::cout << val_id << " " << equal << " " << chnl << std::endl;
//
//      font.setTextureRect({ x, y, width, height }, id);
//    }
//
//    //Get info for kerning pairs
//    int first, second, amount;
//    int kerningCount;
//
//    BMPFile >> linestart >> val_id >> equal >> kerningCount; //"kernings count = 91"
//    std::string KerningString = "12";
//
//    for (int i = 0; i < kerningCount; ++i) {
//      BMPFile >> linestart >> val_id >> equal >> first; //std::cout << linestart << val_id << equal << first << "(" << (char)(first) << ") ";
//      BMPFile >> val_id >> equal >> second; //std::cout << val_id << equal << second << "(" << (char)(second) << ") ";
//      BMPFile >> val_id >> equal >> amount; //std::cout << val_id << equal << amount << std::endl; //"kerning first = # second = # amount = #"
//
//      KerningString[0] = ( char )( first );
//      KerningString[1] = ( char )( second );
//
//      font.setKerning(amount, KerningString);
//    }
//
//
//
//  }
//  
//}  
