#ifndef SFUI_POPUP_H
#define SFUI_POPUP_H

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
#include <SFUI/Include/UI/Widgets/Panel.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class Popup : public Widget
  {
  public:
    WIDGET_DERIVED(Popup, Widget);
    Popup(WidgetWindow *TopWindow, optional<Theme*> theme = optional<Theme*>());
    virtual ~Popup() override;

    static shared_ptr Create(WidgetWindow *TopWindow, optional<Theme*> theme = optional<Theme*>());

    void Add(Widget::shared_ptr widget);
    virtual void Open();
    virtual void Close();

    virtual void InitDropShadow();

    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;
    void SetStealsFocus(bool steals);

    virtual void SetPosition(const Vec2i &Position) override;
    virtual void SetSize(const Vec2i &Size) override;

    virtual bool HandleEvent(const sf::Event &event) override;

  protected:
    //virtual void MouseMoved() override;
    //virtual void MouseEntered() override;
    //virtual void MouseLeft() override;
    //virtual void MousePressed(bool left, bool right) override;
    //virtual void MouseReleased(bool left, bool right) override;

    DropShadow m_DropShadow;

    sf::RectangleShape DropShadowRect;
    sf::Texture dropShadowTexture;
    bool m_StealFocus = false;
    bool m_MoveToFront = true;
    bool m_IsOpen = false;
    Panel::shared_ptr m_Panel = nullptr;
    WidgetWindow *m_StealFocusFrom = nullptr;
  };

}  

#endif // SFUI_POPUP_H
