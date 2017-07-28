#ifndef SFUI_APPWINDOW_H
#define SFUI_APPWINDOW_H

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
#include <SFUI/Include/Common.h>
#include <SFUI/Include/Application/Concurrent.h>
#include <SFUI/Include/UI/UI.h>
#include <SFUI/Include/UI/Theme.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class AppWindow;

  class AppWindowHandle
  {
  public:
    AppWindowHandle(AppWindow *win);
    ~AppWindowHandle();

    void Open();
    void Close();
    void Hide();
    void Show();

  private:
    AppWindow *m_Handle = nullptr;
  };

  class AppWindow
  {
  public:
    AppWindow(Vec2i Size, const std::string &Title, sf::Uint32 style);
    virtual ~AppWindow();
    friend class AppMainWindow;

    virtual void Launch();
    bool IsOpen();
    bool IsModal();
    bool IsDone();
    virtual void Close();

    virtual void Hide();
    virtual void Show();

    virtual void AddWidget(Widget::shared_ptr widget);

  protected:
    virtual void PostLaunch();

    virtual void MainLoop();
    virtual void Update();
    virtual void Render();
    virtual void QueryQueues();

    Vec2i m_WindowSize = { 0, 0 };
    std::string m_WindowTitle = "Popup";
    sf::Uint32 m_WindowStyle = sf::Style::Default;

    std::atomic<bool> m_IsModal = false;
    std::atomic<bool> m_IsVisible = false;
    std::atomic<bool> m_MarkForClose = false;
    std::atomic<bool> m_MarkForCleanup = false;
    std::atomic<bool> m_MarkForHide = false;

    std::thread m_Thread;

    concurrency::concurrent_queue<Widget::shared_ptr> m_widgetQueue;

    std::shared_ptr<sf::RenderWindow> m_Window;
    std::shared_ptr<WidgetWindow> m_Widgets;

  };
  
}  

#endif // SFUI_APPWINDOW_H
