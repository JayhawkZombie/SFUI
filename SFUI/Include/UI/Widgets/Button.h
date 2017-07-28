#ifndef SFUI_BUTTON_H
#define SFUI_BUTTON_H

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
  
  class Button : public Widget
  {
  public:
    WIDGET_DERIVED(Button, Widget);

    Button(optional<Theme*> theme = optional<Theme*>(), optional<Widget::pointer> parent = optional<Widget*>(), uint32 events = Event::Default);
    virtual ~Button() override = default;

    static shared_ptr Create(optional<Theme*> theme = optional<Theme*>(), optional<Widget::pointer> parent = optional<Widget*>(), uint32 events = Event::Default);
    static shared_ptr CreateIcon(texture_handle tex, IntRect texRect, optional<Theme*> theme = optional<Theme*>(), optional<Widget::pointer> parent = optional<Widget*>(), uint32 events = Event::Default);

    bool IsPressed() const;
    void OnClicked(boost::function<void()> func);

    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;

    //bool HandleEvent(const sf::Event &event) override;

  protected:
    Signal<void()> m_ClickedSignal;

    virtual void MouseMoved() override;
    virtual void MouseEntered() override;
    virtual void MouseLeft() override;
    virtual void MousePressed(bool left, bool right) override;
    virtual void MouseReleased(bool left, bool right) override;
    virtual void Resized() override;

    virtual void Hovered() override;
    virtual void Unhovered();
    virtual void Moved() override;
    virtual void Pressed();
    virtual void Released();
    virtual void Clicked();

  private:
    texture_handle m_IconTexture;
    sf::RectangleShape m_IconRect;

    constexpr static sf::Uint8 m_BrightFactor = 15_ui8;
    constexpr static sf::Uint8 m_DarkFactor = 15_ui8;
  };
  
}  

#endif // SFUI_BUTTON_H
