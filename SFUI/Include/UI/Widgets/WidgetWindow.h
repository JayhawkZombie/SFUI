#ifndef SFUI_WIDGETWINDOW_H
#define SFUI_WIDGETWINDOW_H

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
  
  class WidgetWindow : public Widget
  {
  public:
    WIDGET_TYPE_TRAITS(WidgetWindow);
    WidgetWindow(sf::RenderWindow &Window);
    ~WidgetWindow() override final;

    static shared_ptr Create(sf::RenderWindow &Window);

    void Add(Widget::shared_ptr widget);
    void Remove(Widget::shared_ptr widget);

    void ShowAll();
    void HideAll();

    void Update() override;
    bool HandleEvent(sf::Event event);
    void Render();

    void ChildTakingMouseFocus(Widget::pointer child) override;
    void ChildTakingKeyboardFocus(Widget::pointer child) override;
    void ChildReleasingMouseFocus(Widget::pointer child) override;
    void ChildReleasingKeyboardFocus(Widget::pointer child) override;

    optional<Widget::shared_ptr> GetWidgetMouseOver() const;
    void Cleanup() override final;

    void AppExit();
    void MoveChildToFront(Widget::pointer child) override final;
    void MoveChildToBack(Widget::pointer child) override final;

    void Render(sf::RenderTarget &Target) override final;

    void PostRender();
    void AddPopupWindow(_shared_ptr<PopupWindow> win);
    void Block();
    void Unblock();

    void GiveMouseFocus(Widget *widget);
    void GiveKeyboardFocus(Widget *widget);
    void RelinquishMouseFocus();
    void RelinquishKeyboardFocus();

    virtual void StealMouseFocus(Widget *widget) override;
    virtual void ReturnMouseFocus(Widget *widget) override;
    virtual void StealKeyboardFocus(Widget *widget) override;
    virtual void ReturnKeyboardFocus(Widget *widget) override;

  private:
    bool m_IsBeingBlocked = false;
    std::vector<Widget*> m_MouseFocusPool;

    Widget* GlobalMouseFocus = nullptr;
    Widget* GlobalKeyboardFocus = nullptr;

    std::vector<_shared_ptr<PopupWindow>> m_Popups;
    
    std::vector<Widget::shared_ptr> m_Widgets;
    sf::RenderWindow &m_Window;
  };
  
}  

#endif // SFUI_WIDGETWINDOW_H
