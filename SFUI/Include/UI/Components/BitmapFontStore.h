#ifndef SFUI_BITMAPFONTSTORE_H
#define SFUI_BITMAPFONTSTORE_H

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
#include <SFUI/Include/UI/Components/BitmapFontReader.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class BitmapFontStore
  {
  public:
    BitmapFontStore() = default;
    ~BitmapFontStore() = default;

    bool IsSizeAvailable(uint32 size) const { return ( m_SavedFonts.find(size) != m_SavedFonts.end() ); }

    optional<std::shared_ptr<sw::BitmapFont>> GetFont(uint32 size)
    {
      auto it = m_SavedFonts.find(size);
      if (it != m_SavedFonts.end())
        return ( *it ).second;

      return optional<std::shared_ptr<sw::BitmapFont>>(); //empty
    }

    void SaveFont(uint32 size, std::shared_ptr<sw::BitmapFont> fnt) { m_SavedFonts[size] = fnt; }
    void LoadFont(const std::string &Tex, const std::string &Data, uint32 texSize)
    {
      m_SavedFonts[texSize] = std::make_shared<sw::BitmapFont>();
      try
      {
        m_Reader.LoadTexture(Tex, m_SavedFonts[texSize]);
        m_Reader.LoadData(Data, m_SavedFonts[texSize]);
      }
      catch (const std::exception&)
      {
        std::cerr << "Unable to load font texture from \"" << Tex << " and data from \"" << Data << "\"\n";
        m_SavedFonts.erase(texSize);
      }
    }

  private:
    BitmapFontReader m_Reader;
    std::map<uint32, std::shared_ptr<sw::BitmapFont>> m_SavedFonts;
  };

  class BitmapFontCollection
  {
  public:
    BitmapFontCollection() = default;
    ~BitmapFontCollection() = default;

    bool IsFontFamilyAvailable(const std::string &family) { return ( m_FontCollections.find(family) != m_FontCollections.end() ); }
    bool IsFontSizeAvailable(const std::string &family, uint32 size)
    {
      auto it = m_FontCollections.find(family);
      if (it == m_FontCollections.end()) return false;

      return ( ( *it ).second->IsSizeAvailable(size) );
    }

    void LoadFont(const std::string &family, const std::string &texture, const std::string &data, uint32 texsize)
    {
      if (m_FontCollections.find(family) == m_FontCollections.end()) {
        m_FontCollections[family] = std::make_shared<BitmapFontStore>();
      }

      m_FontCollections[family]->LoadFont(texture, data, texsize);
    }

    optional<std::shared_ptr<sw::BitmapFont>> GetFont(const std::string &family, uint32 size)
    {
      if (m_FontCollections.find(family) == m_FontCollections.end())
        return {};

      auto fPtr = m_FontCollections[family];

      if (fPtr->IsSizeAvailable(size))
        return fPtr->GetFont(size);
      return {};
    }

  private:
    std::map<std::string, std::shared_ptr<BitmapFontStore>> m_FontCollections;

  };
  
}  

#endif // SFUI_BITMAPFONTSTORE_H
