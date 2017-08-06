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
#include <SFUI/Include/UI/Components/TextView.h>
#include <SFUI/Include/UI/Widget.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  TextView::TextView(Widget *parent, const std::string &Text, sf::Color Color, uint32 TextSize, font_handle HFont)
    : m_Parent(parent), m_Text(Text), m_TextColor(Color), m_TextSize(TextSize), m_Font(HFont)
  {
    m_RenderText.setString(m_Text);
    m_RenderText.setFillColor(m_TextColor);
    if (m_Font) m_RenderText.setFont(*m_Font);
    m_RenderText.setCharacterSize(m_TextSize);
    
    auto bds = m_RenderText.getLocalBounds();
    m_Size = { cast_int(floor(bds.width)), cast_int(floor(bds.height)) };
  }


  TextView::~TextView()
  {

  }

  
  TextView::shared_ptr TextView::Clone(shared_ptr TView)
  {
    return Create(TView->m_Parent, TView->m_Text, TView->m_TextColor, TView->m_TextSize, TView->m_Font);
  }

  sfui::TextView::shared_ptr TextView::Create(Widget *parent, const std::string &Text, sf::Color Color, uint32 TexSize, font_handle HFont)
  {
    return std::make_shared<TextView>(parent, Text, Color, TexSize, HFont);
  }

  void TextView::SetParent(Widget *parent)
  {
    m_Parent = parent;
  }

  void TextView::Update()
  {

  }

  void TextView::Render(sf::RenderTarget &Target, sf::View RenderView)
  {
    //Target.draw(m_RenderText);
    Target.draw(m_BMText);
  }

  void TextView::SetText(const std::string &Text)
  {
    m_Text = Text;
    m_BMText.setString(Text);
    //m_RenderText.setString(m_Text);
  }

  void TextView::SetColor(sf::Color Color)
  {
    m_TextColor = Color;
    m_BMText.setColor(Color);
    //m_RenderText.setFillColor(m_TextColor);
  }

  void TextView::SetTextSize(uint32 TexSize)
  {
    m_TextSize = TexSize;
    //m_RenderText.setCharacterSize(m_TextSize);
  }

  void TextView::SetFont(font_handle HFont)
  {
    m_Font = HFont;
    m_RenderText.setFont(*m_Font);
  }

  void TextView::SetAlignment(TextAlignment alignment)
  {
    m_Alignment = alignment;
    Reposition();
  }

  void TextView::SetBMFont(std::shared_ptr<sw::BitmapFont> _fnt)
  {
    m_BMText.setBitmapFont(*_fnt);
    fnt = _fnt.get();
  }

  std::shared_ptr<sw::BitmapFont> TextView::DefaultBitmapFont;

  const std::string& TextView::GetText() const
  {
    return m_Text;
  }

  sf::Color TextView::GetColor() const
  {
    return m_TextColor;
  }

  uint32 TextView::GetTextSize() const
  {
    return m_TextSize;
  }

  font_handle TextView::GetFont() const
  {
    return m_Font;
  }

  sfui::TextAlignment TextView::GetAlignment() const
  {
    return m_Alignment;
  }

  void TextView::SetPosition(Vec2i pos)
  {
    m_Position = pos;
    m_BMText.setPosition(pos); //m_RenderText.setPosition(pos);
    RealignText();
  }

  void TextView::SetSize(Vec2i pos)
  {
    m_Size = pos;
    RealignText();
  }

  Vec2i TextView::GetPosition() const
  {
    return m_BMText.getPosition();// m_RenderText.getPosition();
  }

  Vec2i TextView::GetSize() const
  {
    auto bds = m_BMText.getLocalBounds();// m_RenderText.getLocalBounds();
    return Vec2i((int)floor(bds.width), (int)floor(bds.height));
  }

  void TextView::Reposition()
  {
    RealignText();
  }

  void TextView::RealignText()
  {
    FloatRect tBds = m_BMText.getLocalBounds();// m_RenderText.getLocalBounds();
    auto tPos = m_BMText.getPosition();
    Vec2i pSiz = m_Parent->GetSize();
    Vec2i pPos = m_Position;

    uint32 xDiff = abs(pSiz.x - cast_int(floor(tBds.width)));
    uint32 yDiff = abs(pSiz.y - cast_int(floor(tBds.height)));

    Vec2i newPos = pPos;

    auto gBds = m_BMText.getLocalBounds();
    auto topDiff = abs(tBds.height - pPos.y - m_Size.y);    

    switch (m_Alignment)
    {
      case TextAlignment::Centered:
      {
        newPos.x = pPos.x + cast_int(floor(xDiff / 2.f) - tBds.left);
        newPos.y = pPos.y + pSiz.y;// -cast_int(topDiff / 2.f);

        break;
      }

      case TextAlignment::Justified:
      {
        newPos.x = pPos.x + cast_int(floor(xDiff / 2.f) - tBds.left);
        newPos.y = pPos.y + pSiz.y;// -cast_int(topDiff / 2.f);

        break;
      }

      case TextAlignment::Left:
      {
        newPos.x = pPos.x + 5;
        newPos.y = pPos.y + pSiz.y;// -cast_int(topDiff / 2.f);

        break;
      }

      case TextAlignment::Right:
      {
        newPos.x = pPos.x + cast_int(floor(xDiff / 2.f) - tBds.left);
        newPos.y = pPos.y + pSiz.y;// -cast_int(topDiff / 2.f);

        break;
      }

      default:
      {
        newPos.x = pPos.x + cast_int(floor(xDiff / 2.f) - tBds.left);
        newPos.y = pPos.y + pSiz.y;// -cast_int(topDiff / 2.f);

        break;
      }
    }

    //m_RenderText.setPosition(newPos);
    newPos.y -= cast_int(floor(0.25 * pSiz.y));
    m_BMText.setPosition(newPos);
    //outline.setPosition(newPos);
  }

}  
