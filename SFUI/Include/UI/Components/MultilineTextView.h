#ifndef SFUI_MULTILINETEXTVIEW_H
#define SFUI_MULTILINETEXTVIEW_H

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
#include <SFUI/Include/UI/Widget.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////
#include <list>

namespace sfui
{  
  
  class MultilineTextView : public Widget
  {
  public:
    WIDGET_DERIVED(MultilineTextView, Widget);
    MultilineTextView(optional<Theme*> theme = optional<Theme*>(), optional<Widget::pointer> parent = optional<Widget*>());
    virtual ~MultilineTextView() override;

    static shared_ptr Create(optional<Theme*> theme = optional<Theme*>(), optional<Widget::pointer> parent = optional<Widget*>());

    void LoadFontTexture(const std::string &File);
    void LoadFontData(const std::string &File);

    bool HandleEvent(const sf::Event &event) override;

    virtual void Move(const Vec2i &Delta) override;
    virtual void SetPosition(const Vec2i &Position) override;

    void SetText(const std::string &Text);
    void SetColor(const Color &c);
    void SetTracing(int tracing);
    void SetTextScale(uint32 scale);

    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;
    void SetWidth(uint32 w);

  protected:
    void RealignText();
    void SplitText();

    std::string m_FullText = "";
    Color m_Color = sf::Color::Black;
    int m_Tracing = 0;
    uint32 m_Scale = 1;
    uint32 m_Width = 300;
    sw::BitmapFont m_BMFont;
    std::list<sw::BitmapText> m_TextBlocks;

    std::vector<sf::RectangleShape> m_TextRects;
    std::vector<std::string> m_SplitStrings;
  };
  
}  

#endif // SFUI_MULTILINETEXTVIEW_H
