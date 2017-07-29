#ifndef SFUI_TEXTVIEW_H
#define SFUI_TEXTVIEW_H

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
#include <SFUI/Include/UI/Components/Colors.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class Widget;

  enum class TextAlignment
  {
    Left,
    Right,
    Centered,
    Justified
  };

  enum TextSize
  {
    Display4 = 44,
    Display3 = 36,
    Display2 = 24, 
    Display1 = 20,
    Headline = 20,
    Title = 18,
    Subheading = 16,
    Body1 = 14,
    Body2 = 12,
    Caption = 10,
    Menu_ = 12,
    ButtonLarge = 16,
    ButtonSmall = 12
  };


  //Wrapper class that also parses the .fnt files that contain the glyph and kerning
  // info
  //class BitmapText
  //{
  //public:

  //  void LoadFontTexture(const std::string &File);
  //  void ParseFntFile(const std::string &File);

  //  sw::BitmapFont font;
  //  sw::BitmapText text;
  //};


  class TextView
  {
  public:
    using pointer = TextView*;
    using reference = TextView&;
    using const_pointer = const TextView*;
    using const_reference = const TextView&;
    using shared_ptr = _shared_ptr<TextView>;

    TextView(Widget *parent, const std::string &Text, sf::Color Color, uint32 TextSize, font_handle HFont);
    virtual ~TextView();

    static shared_ptr Clone(shared_ptr TView);
    static shared_ptr Create(Widget *parent, const std::string &Text, sf::Color Color, uint32 TexSize, font_handle HFont);

    void SetParent(Widget *parent);

    virtual void Update();
    virtual void Render(sf::RenderTarget &Target, sf::View RenderView);

    virtual void SetText(const std::string &Text);
    virtual void SetColor(sf::Color Color);
    virtual void SetTextSize(uint32 TexSize);
    virtual void SetFont(font_handle HFont);
    virtual void SetAlignment(TextAlignment alignment);
    virtual void SetBMFont(std::shared_ptr<sw::BitmapFont> fnt);
    static std::shared_ptr<sw::BitmapFont> DefaultBitmapFont;

    std::string GetText() const;
    sf::Color GetColor() const;
    uint32 GetTextSize() const;
    font_handle GetFont() const;
    TextAlignment GetAlignment() const;

    void SetPosition(Vec2i pos);
    void SetSize(Vec2i pos);
    Vec2i GetPosition() const;
    Vec2i GetSize() const;

    virtual void Reposition();
    virtual void RealignText();

  private:

    sw::BitmapText m_BMText;
    sw::BitmapFont *fnt;
    sf::RectangleShape outline;
    TextAlignment m_Alignment = TextAlignment::Centered;
    Widget *m_Parent = nullptr;
    Vec2i m_Position = { 0, 0 };
    Vec2i m_Size = { 0, 0 };
    font_handle m_Font = nullptr;
    std::string m_Text = "";
    sf::Text m_RenderText;
    sf::Color m_TextColor = WIDGET_STYLE_COLOR_LIGHT_GREY;
    uint32 m_TextSize = TextSize::Body2;
  };

}  

#endif // SFUI_TEXTVIEW_H
