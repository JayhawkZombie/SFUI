#ifndef SFUI_CANVAS_H
#define SFUI_CANVAS_H

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

namespace sfui
{  
  
  class Canvas : public Widget
  {
  public:
    WIDGET_DERIVED(Canvas, Widget);
    Canvas(optional<Theme *> theme = {}, std::optional<pointer> parent = nullptr);
    virtual ~Canvas() override;

    static shared_ptr Create(optional<Theme *> theme = { }, std::optional<pointer> parent = nullptr);
    virtual void Clear();
    virtual void Render(sf::RenderTarget &Target) override;
    virtual void Display();
    virtual bool HandleEvent(const sf::Event &event) override;
    virtual void SetPosition(const Vec2i &Position) override;
    virtual void SetSize(const Vec2i &Size) override;
    virtual void Move(const Vec2i &Delta) override;
    void SetBorder(const Color &c, int32 Width);
    void SetClearColor(const Color &c);

  protected:
    Color m_ClearColor = sf::Color::Black;
    RenderTexture m_RenderTexture;
    sf::RectangleShape m_RenderRect;
  };
  
}  

#endif // SFUI_CANVAS_H
