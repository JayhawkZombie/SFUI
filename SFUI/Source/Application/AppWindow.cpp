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
#include <SFUI/Include/Application/AppWindow.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  

  AppWindow::AppWindow(Vec2i Size, const std::string &Title, sf::Uint32 style)
  {
    m_WindowSize = Size;
    m_WindowTitle = Title;
    m_WindowStyle = style;
  }

  AppWindow::~AppWindow()
  {

  }

  void AppWindow::Launch()
  {
    m_Thread = std::thread([this]() { this->MainLoop(); });
    return;
  }

  bool AppWindow::IsOpen()
  {
    return ( m_Window && m_Window->isOpen() );
  }

  bool AppWindow::IsModal()
  {
    return m_IsModal;
  }

  bool AppWindow::IsDone()
  {
    return m_MarkForCleanup;
  }

  void AppWindow::Close()
  {
    m_MarkForClose = true;
  }

  void AppWindow::Hide()
  {
    m_MarkForHide = true;
  }

  void AppWindow::Show()
  {
    m_MarkForHide = false;
  }

  void AppWindow::AddWidget(Widget::shared_ptr widget)
  {
    m_widgetQueue.push(widget);
  }

  void AppWindow::PostLaunch()
  {

  }

  void AppWindow::MainLoop()
  {
    m_Window = std::make_shared<sf::RenderWindow>(sf::VideoMode(m_WindowSize.x, m_WindowSize.y), m_WindowTitle, m_WindowStyle);
    m_Window->setVerticalSyncEnabled(true);
    m_Widgets = std::make_shared<WidgetWindow>(*m_Window);

    sf::Event event;
    bool closed = false;

    PostLaunch();

    while (m_Window && m_Window->isOpen()) {

      if (m_MarkForClose) {
        closed = true;
        break;
      }

      while (m_Window->pollEvent(event)) {

        closed = ( event.type == sf::Event::Closed );

        m_Widgets->HandleEvent(event);
      }

      if (closed) break;

      Update();

      Render();

      QueryQueues();
    }

    std::cerr << "Popup closed. Marking for cleanup" << std::endl;

    // Mark for cleanup and return from the main thread
    m_MarkForCleanup = true;
    m_Window.reset();
    return;
  }

  void AppWindow::Update()
  {
    m_Widgets->Update();

  }

  void AppWindow::Render()
  {
    m_Window->setActive(true);
    m_Window->clear(sf::Color(122, 122, 122));
    m_Widgets->Render();
    m_Window->display();
    m_Window->setActive(false);
  }

  void AppWindow::QueryQueues()
  {
    Widget::shared_ptr widgetPtr = nullptr;

    while (m_widgetQueue.try_pop(widgetPtr)) {
      m_Widgets->Add(widgetPtr);
    }

  }

  AppWindowHandle::AppWindowHandle(AppWindow *win)
    : m_Handle(win)
  {

  }

  AppWindowHandle::~AppWindowHandle()
  {
    /*if (m_Handle) {
      m_Handle->Close();
      m_Handle = nullptr;
    }*/
  }

  void AppWindowHandle::Open()
  {
    if (m_Handle) {
      m_Handle->Launch();
    }
  }

  void AppWindowHandle::Close()
  {
    if (m_Handle) {
      m_Handle->Close();
    }
  }

  void AppWindowHandle::Hide()
  {
    if (m_Handle) {
      m_Handle->Hide();
    }
  }

  void AppWindowHandle::Show()
  {
    if (m_Handle) {
      m_Handle->Show();
    }
  }

}  
