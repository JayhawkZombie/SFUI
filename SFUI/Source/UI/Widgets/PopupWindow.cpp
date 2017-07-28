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
#include <SFUI/Include/UI/Widgets/PopupWindow.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  PopupWindow::PopupWindow(WidgetWindow *mainWindow, optional<Theme*> theme /*= optional<Theme*>()*/)
    : Widget(theme, nullptr), m_ToBlock(nullptr)
  {

  }

  PopupWindow::~PopupWindow()
  {
    std::cerr << "Popup window got destroyed -- You're doing down\n";
  }

  PopupWindow::shared_ptr PopupWindow::Create(WidgetWindow *mainWindow, Vec2i size, const std::string &Title, sf::Uint32 style, optional<Theme*> theme)
  {
    PopupWindow::shared_ptr wPtr = std::make_shared<PopupWindow>(mainWindow, theme);
    wPtr->m_WindowSize = size;
    wPtr->m_WindowStyle = style;
    wPtr->m_WindowTitle = Title;
    wPtr->m_WWindow = WidgetWindow::Create(wPtr->m_RenderWindow);
    return wPtr;
  }

  void PopupWindow::SetSize(const Vec2i &Size)
  {
    m_RenderWindow.setSize(Size);
  }

  void PopupWindow::SetPosition(const Vec2i &Position)
  {
    m_RenderWindow.setPosition(Position);
  }

  void PopupWindow::AddWidget(Widget::shared_ptr widget)
  {
    m_WWindow->Add(widget);
  }

  void PopupWindow::SetIsModal(bool isModal)
  {
    m_IsModal = isModal;
  }

  void PopupWindow::Update()
  {
    if (!m_IsOpen || !m_RenderWindow.isOpen() || !m_WWindow) return;

    if (m_RenderWindow.isOpen() && m_IsModal && !m_RenderWindow.hasFocus()) {
      m_RenderWindow.requestFocus();
    }

    m_WWindow->Update();
  }

  bool PopupWindow::HandleEvent(const sf::Event &event)
  {
    if (m_IsOpen) {
      DoEvents();
      RenderWidgets();
    }

    return false;
  }

  void PopupWindow::DoEvents()
  {
    if (!m_IsOpen) return;

    HandleEvents();
  }

  bool PopupWindow::IsOpen() const
  {
    return m_IsOpen;
  }

  void PopupWindow::Render(sf::RenderTarget &Target)
  {
    RenderWidgets();
  }

  void PopupWindow::Render()
  {
    if (!m_IsOpen) return;

    m_RenderWindow.setActive(true);
    RenderWidgets();
  }

  void PopupWindow::Open()
  {

    if (!m_RenderWindow.isOpen()) {
      m_RenderWindow.create(sf::VideoMode(m_WindowSize.x, m_WindowSize.y), m_WindowTitle, m_WindowStyle);
      m_RenderWindow.requestFocus();
    }

    std::cout << "Popup opened. Taking focus\n";

    if (m_IsModal) {
      m_WWindow->Block();
    }

    m_IsVisible = true;
    m_IsOpen = true;
    m_RenderWindow.setVisible(true);
    m_RenderWindow.setActive(true);
    m_RenderWindow.requestFocus();
  }

  void PopupWindow::Close()
  {
    std::cout << "Popup closed, but not destroyed. Can still open\n";
    m_IsVisible = false;
    m_IsOpen = false;
    m_RenderWindow.setVisible(false);

    if (m_IsModal) {
      m_WWindow->Unblock();
    }
  }

  void PopupWindow::Destroy()
  {
    std::cout << "Popup destroyed. It can be opened again, but the window must be recreated\n";
    m_RenderWindow.setVisible(false);
    
    if (m_IsModal) {
      m_WWindow->Unblock();
    }

    if (!m_RenderWindow.isOpen()) return;
  }

  void PopupWindow::Dismiss()
  {
    std::cout << "Popup dismissed\n";
    if (!m_RenderWindow.isOpen()) return;

    if (m_IsModal) {
      m_WWindow->Unblock();
    }

    m_RenderWindow.setVisible(false);
    m_IsOpen = false;
    m_IsVisible = false;
  }

  void PopupWindow::OnOpened(boost::function<void(PopupWindow*)> func)
  {

  }

  void PopupWindow::OnClosed(boost::function<void(PopupWindow*)> func)
  {

  }

  void PopupWindow::OnDestroyed(boost::function<void(PopupWindow*)> func)
  {

  }

  void PopupWindow::OnDismissed(boost::function<void(PopupWindow*)> func)
  {

  }

  void PopupWindow::HandleEvents()
  {
    static sf::Event event;
    static bool closed = false;
    if (!m_RenderWindow.isOpen()) return;

    while (m_RenderWindow.pollEvent(event)) {
      closed = ( event.type == sf::Event::Closed );

      if (closed) {
        Close();
        return;
      }

      m_WWindow->HandleEvent(event);
    }
  }

  void PopupWindow::RenderWidgets()
  {
    if (!m_RenderWindow.isOpen()) return;

    m_RenderWindow.clear(sf::Color(102, 102, 102));
    m_WWindow->Render();
    m_RenderWindow.display();
  }

  void PopupWindow::RequestFocus()
  {
    if (!m_RenderWindow.isOpen()) return;

    m_RenderWindow.requestFocus();
  }

}  
