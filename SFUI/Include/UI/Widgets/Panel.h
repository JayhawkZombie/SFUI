#ifndef SFUI_PANEL_H
#define SFUI_PANEL_H

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
#include <SFUI/Include/UI/Widgets/GenericContainer.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class Panel : public Widget
  {
  public:
    WIDGET_DERIVED(Panel, Widget);
    Panel(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());
    virtual ~Panel() override;

    static shared_ptr Create(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());

    virtual void Render(sf::RenderTarget &Target) override;
    virtual void Add(Widget::shared_ptr widget);
    virtual void Remove(Widget::shared_ptr widget);
    virtual void SetPosition(const Vec2i &Position) override;
    virtual void SetSize(const Vec2i &Size) override;
    virtual void Move(const Vec2i &Delta) override;
    virtual void Update() override;

    bool HandleEvent(const sf::Event &event) override;
    virtual void SetTopWindow(WidgetWindow *TopWindow) override;

  protected:
    optional<Widget::shared_ptr> GetWidgetUnderMouse();

    virtual void ChildTakingMouseFocus(Widget* child) override;
    virtual void ChildTakingKeyboardFocus(Widget* child) override;
    virtual void ChildReleasingMouseFocus(Widget* child) override;
    virtual void ChildReleasingKeyboardFocus(Widget* child) override;
    virtual void MoveChildToFront(Widget* child) override;
    virtual void MoveChildToBack(Widget* child) override;

    unordered_set<Widget::shared_ptr> m_Widgets;

  };
  
}  

#endif // SFUI_PANEL_H
