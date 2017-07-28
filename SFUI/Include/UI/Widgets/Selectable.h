#ifndef SFUI_SELECTABLE_H
#define SFUI_SELECTABLE_H

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
#include <SFUI/Include/UI/Widgets/Button.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class Selectable : public Widget
  {
  public:
    WIDGET_DERIVED(Selectable, Button);
    Selectable(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());
    virtual ~Selectable() override = default;

    static shared_ptr Create(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());

    void Select();
    void Deselect();

    bool IsSelected() const;
    void SetSelectColor(const Color &c);
    virtual void Move(const Vec2i &Delta) override;
    virtual void Render(sf::RenderTarget &Target) override;
    virtual void OnSelected(boost::function<void(const std::string &)> func);
    virtual void OnDeselected(boost::function<void(const std::string &)> func);

  protected:
    Signal<void(const std::string &)> m_SelectedSignal;
    Signal<void(const std::string &)> m_DeselectedSignal;

    virtual void MouseEntered() override;
    virtual void MouseLeft() override;
    virtual void MousePressed(bool left, bool right) override;
    virtual void MouseReleased(bool left, bool right) override;
  
    virtual void Selected();
    virtual void Deselected();

    constexpr static sf::Uint8 m_BrightFactor = 15_ui8;
    Color m_SelectionColor = sf::Color(0, 92, 242, 155);
    sf::RectangleShape m_SelectedRect;
    bool m_IsSelected = false;
  };
  
}  

#endif // SFUI_SELECTABLE_H
