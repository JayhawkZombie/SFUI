#ifndef SFUI_COLORBOX_H
#define SFUI_COLORBOX_H

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
  
  class ColorBox : public Widget
  {
  public:
    WIDGET_DERIVED(ColorBox, Widget);
    ColorBox(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());
    virtual ~ColorBox() override;

    static shared_ptr Create(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());

    virtual bool HandleEvent(const sf::Event &event) override;

    virtual void Render(sf::RenderTarget &Target) override;
    virtual void SetPosition(const Vec2i &Position) override;
    virtual void SetSize(const Vec2i &Size) override;

    virtual void SetColorAt(uint32 index, const Color &c);
    Color GetColor() const;

    void OnColorChanged(boost::function<void(const Color &)> func);

  protected:
    Signal<void(const Color &)> m_ColorChangedSignal;
    virtual void ColorChanged();
    void CalculateColor();
    virtual void MousePressed(bool left, bool right) override;
    virtual void MouseMoved() override;

    sf::VertexArray m_Corners = sf::VertexArray(sf::TriangleStrip, 4);
    sf::CircleShape m_Selection;

    Color m_CurrentColor = sf::Color::Black;
  };
  
}  

#endif // SFUI_COLORBOX_H
