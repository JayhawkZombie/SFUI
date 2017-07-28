#ifndef SFUI_POPUPWINDOW_H
#define SFUI_POPUPWINDOW_H

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
#include <SFUI/Include/UI/Widgets/WidgetWindow.h>
#include <SFUI/Include/UI/Fwd.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class PopupWindow : public Widget
  {
  public:
    WIDGET_DERIVED(PopupWindow, Widget);
    friend class WidgetWindow;
    PopupWindow(WidgetWindow *mainWindow, optional<Theme*> theme = optional<Theme*>());
    virtual ~PopupWindow() override;

    static shared_ptr Create(WidgetWindow *mainWindow, Vec2i size = Vec2i(400, 400), const std::string &Title = "", sf::Uint32 style = sf::Style::Default, optional<Theme*> theme = optional<Theme*>());

    //This will create the render window and widget window with the provided size
    virtual void SetSize(const Vec2i &Size) override;

    //This will move the render window relative to the device display
    virtual void SetPosition(const Vec2i &Position) override;

    //Add a widget to the popup window
    virtual void AddWidget(Widget::shared_ptr widget);

    //Set if this window will prevent using the other
    virtual void SetIsModal(bool isModal);

    virtual void Update() override;

    bool HandleEvent(const sf::Event &event) override;

    void DoEvents();

    bool IsOpen() const;

    //Does nothing
    void Render(sf::RenderTarget &Target) override final;

    //Render with OUR window
    void Render();

    //Open.  It will immediately take focus from the other window and KEEP it
    //  until it is closed, destroyed, dismissed, or otherwise removed
    virtual void Open();

    //Close it.  This will NOT destroy the window, but will reset its state to its
    // "default".  You may still call "Open" to use it again from the default state
    virtual void Close();

    //Destroys the internal render window and WidgetWindow used
    //  Calling "Open" after this will open an empty popup
    virtual void Destroy();

    //Hides the window, but in a way that should be interpreted as "good"
    // ie, the user acknowledged something, properly finished something
    // and no error occurred
    virtual void Dismiss();

    virtual void OnOpened(boost::function<void(PopupWindow*)> func);
    virtual void OnClosed(boost::function<void(PopupWindow*)> func);
    virtual void OnDestroyed(boost::function<void(PopupWindow*)> func);
    virtual void OnDismissed(boost::function<void(PopupWindow*)> func);

  private:
    void HandleEvents();
    void RenderWidgets();

    void RequestFocus();

    Signal<void(PopupWindow*)> m_OpenedSignal;
    Signal<void(PopupWindow*)> m_ClosedSignal;
    Signal<void(PopupWindow*)> m_DestroyedSignal;
    Signal<void(PopupWindow*)> m_DismissedSignal;

    bool m_IsOpen = false;
    bool m_IsModal = false;

    Vec2i m_WindowSize = { 400, 400 };
    sf::Uint32 m_WindowStyle = sf::Style::Default;
    std::string m_WindowTitle = "Popup";

    WidgetWindow *m_ToBlock;

    sf::RenderWindow m_RenderWindow;
    WidgetWindow::shared_ptr m_WWindow;
  };
  
}  

#endif // SFUI_POPUPWINDOW_H
