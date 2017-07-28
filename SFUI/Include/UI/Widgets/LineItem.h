#ifndef SFUI_LINEITEM_H
#define SFUI_LINEITEM_H

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
  
  class LineItem : public Widget
  {
  public:
    WIDGET_DERIVED(LineItem, Widget);

    LineItem(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());
    virtual ~LineItem() override;

    static shared_ptr Create(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());

    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;

    void OnSelected(boost::function<void(const std::string &)> func);
    void OnDeselected(boost::function<void()> func);
    bool HandleEvent(const sf::Event &event) override;

    void SetHoverColor(const Color &c);

    void Select();
    void Deselect();

  protected:
    Signal<void(const std::string &)> m_SelecetdSignal;
    Signal<void()> m_DeselectedSignal;

    sf::Color m_HoverColor;

    virtual void Moved() override;
    virtual void Resized() override;

    virtual void MouseMoved() override;
    virtual void MouseEntered() override;
    virtual void MouseLeft() override;
    virtual void MousePressed(bool left, bool right) override;
    virtual void MouseReleased(bool left, bool right) override;
  };
  
}  

#endif // SFUI_LINEITEM_H
