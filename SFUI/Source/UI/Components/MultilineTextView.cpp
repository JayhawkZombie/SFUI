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
#include <SFUI/Include/UI/Components/MultilineTextView.h>
#include <SFUI/Include/UI/Theme.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////
#include <fstream>

namespace sfui
{  
  
  MultilineTextView::MultilineTextView(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget::pointer> parent /*= optional<Widget*>()*/)
    : Widget(theme, parent)
  {
    SetBackgroundColor(Color(119, 0, 85));
    SetBorderColor(Color(122, 122, 122));
    SetBorderWidth(1);
  }

  MultilineTextView::~MultilineTextView()
  {

  }

  sfui::MultilineTextView::shared_ptr MultilineTextView::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget::pointer> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<MultilineTextView>(theme, parent);
  }

  void MultilineTextView::LoadFontTexture(const std::string &File)
  {
    m_BMFont.loadTexture(File);
    
  }

  void MultilineTextView::LoadFontData(const std::string &File)
  {
    std::ifstream BMPFile(File);
    if (!BMPFile) {
      std::cerr << "Unable to load bitmap data file \"" << File << "\"\n";
      return;
    }

    std::string discard, linestart, val_id, equal; int count = 0;
    int id, x, y, width, height, xoffset, yoffset, xadvance, page, chnl;

    BMPFile >> linestart >> val_id >> equal >> count; // "chars count = #"

                                                      //Read the glyph info
    for (int i = 0; i < count; ++i) {
      BMPFile >> linestart >> val_id >> equal >> id; //"chars id = #"
      BMPFile >> val_id >> equal >> x;        //std::cout << val_id << " " << equal << " " << x << " ";             //"x = #"
      BMPFile >> val_id >> equal >> y;        //std::cout << val_id << " " << equal << " " << y << " ";      //"y = #"
      BMPFile >> val_id >> equal >> width;    //std::cout << val_id << " " << equal << " " << width << " ";      //"width = #"
      BMPFile >> val_id >> equal >> height;   //std::cout << val_id << " " << equal << " " << height << " ";      //"height = #"
      BMPFile >> val_id >> equal >> xoffset;  //std::cout << val_id << " " << equal << " " << xoffset << " ";      //xoffset = #
      BMPFile >> val_id >> equal >> yoffset;  //std::cout << val_id << " " << equal << " " << yoffset << " ";
      BMPFile >> val_id >> equal >> xadvance; //std::cout << val_id << " " << equal << " " << xadvance << " ";
      BMPFile >> val_id >> equal >> page;     //std::cout << val_id << " " << equal << " " << page << " ";
      BMPFile >> val_id >> equal >> chnl;     //std::cout << val_id << " " << equal << " " << chnl << std::endl;

      m_BMFont.setTextureRect({ x, y, width, height }, id);
    }

    //Get info for kerning pairs
    int first, second, amount;
    int kerningCount;

    BMPFile >> linestart >> val_id >> equal >> kerningCount; //"kernings count = 91"
    std::string KerningString = "12";

    for (int i = 0; i < kerningCount; ++i) {
      BMPFile >> linestart >> val_id >> equal >> first; //std::cout << linestart << val_id << equal << first << "(" << (char)(first) << ") ";
      BMPFile >> val_id >> equal >> second; //std::cout << val_id << equal << second << "(" << (char)(second) << ") ";
      BMPFile >> val_id >> equal >> amount; //std::cout << val_id << equal << amount << std::endl; //"kerning first = # second = # amount = #"

      KerningString[0] = ( char )( first );
      KerningString[1] = ( char )( second );

      m_BMFont.setKerning(amount, KerningString);
    }

    BMPFile.close();
  }

  bool MultilineTextView::HandleEvent(const sf::Event &event)
  {
    return false;
  }

  void MultilineTextView::Move(const Vec2i &Delta)
  {
    super::Move(Delta);

    for (auto & block : m_TextBlocks)
      block.move(Delta);
    
  }

  void MultilineTextView::SetPosition(const Vec2i &Position)
  {
    auto delta = Position - m_Position;

    Move(delta);
  }

  void MultilineTextView::SetText(const std::string &Text)
  {
    m_FullText = Text;
    RealignText();
  }

  void MultilineTextView::SetColor(const Color &c)
  {
    m_Color = c;
  }

  void MultilineTextView::SetTracing(int tracing)
  {
    m_Tracing = tracing;
  }

  void MultilineTextView::SetTextScale(uint32 scale)
  {
    m_Scale = scale;
  }

  void MultilineTextView::Update()
  {

  }

  void MultilineTextView::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_BackgroundRect);
    for (auto & block : m_TextBlocks)
      Target.draw(block);
  }

  void MultilineTextView::SetWidth(uint32 w)
  {
    m_Width = w;
  }

  void MultilineTextView::RealignText()
  {
    //First we will split by "space" delimeters (will improve later)
    SplitText();

    Vec2f pos = m_Position; Vec2f size;
    FloatRect bounds;
    Vec2f currentWordStart = pos;
    Vec2f btmRight = pos;

    float rightSide = m_Position.x + m_Width;
    btmRight.x = rightSide;
    sf::RectangleShape rShape;

    for (auto & str : m_SplitStrings) {

      if (str != "\n") {
        m_TextBlocks.push_back({}); m_TextBlocks.back().setBitmapFont(m_BMFont);
        m_TextBlocks.back().setString(str); m_TextBlocks.back().setColor(m_Color);
        m_TextBlocks.back().setTracking(m_Tracing);

        bounds = m_TextBlocks.back().getLocalBounds();
        if (pos.x + bounds.width + bounds.left > rightSide) {
          //Insert a line break - jump to next line
          pos.x = m_Position.x; pos.y += 19.f;
        }

        m_TextBlocks.back().setPosition({ pos.x, pos.y - bounds.top });
        pos.x += bounds.width;
        pos.x += 3.f;
      }
      else {
        //Break to a new line
        pos.x = m_Position.x;
        pos.y += 19.f;
      }

      
    }

    btmRight.y = pos.y;
    SetSize({ cast_int(rightSide - m_Position.x), cast_int(btmRight.y - m_Position.y) });
  }

  void MultilineTextView::SplitText()
  {
    std::string accum{ "" };
    std::stringstream sstream(m_FullText);

    while (sstream) {

      accum = "";

      while (sstream && !isspace(sstream.peek())) {
        accum += sstream.get();
      }

      if (sstream.get() == ' ') {
        m_SplitStrings.push_back(accum);
      }
      else {
        m_SplitStrings.push_back(accum);
        m_SplitStrings.push_back("\n");
      }
    } // while (sstream)
  }

}  
