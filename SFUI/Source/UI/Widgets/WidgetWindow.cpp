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
#include <SFUI/Include/UI/Widgets/PopupWindow.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////
#include <algorithm>

namespace sfui
{

  WidgetWindow::WidgetWindow(sf::RenderWindow &Window)
    : m_Window(Window),
    Widget(nullptr)
  {
    m_Size = Window.getSize();
    m_BlockingWindow = nullptr;
  }

  WidgetWindow::~WidgetWindow()
  {
    for (auto & pWin : m_Popups) {
      if (pWin) { pWin->Destroy(); }
    }
  }

  sfui::WidgetWindow::shared_ptr WidgetWindow::Create(sf::RenderWindow &Window)
  {
    return std::make_shared<WidgetWindow>(Window);
  }

  void WidgetWindow::Add(Widget::shared_ptr widget)
  {
    widget->SetParent(this);
    m_Widgets.push_back(widget);
    widget->m_TopWindow = this;
  }

  void WidgetWindow::Remove(Widget::shared_ptr widget)
  {
    auto it = std::find(m_Widgets.begin(), m_Widgets.end(), widget);

    if (it != m_Widgets.end()) {
      m_Widgets.erase(it);
    }
  }

  void WidgetWindow::ShowAll()
  {
    std::for_each(m_Widgets.begin(), m_Widgets.end(), [ ](Widget::shared_ptr wptr) { wptr->Show(); });
  }

  void WidgetWindow::HideAll()
  {
    std::for_each(m_Widgets.begin(), m_Widgets.end(), [ ](Widget::shared_ptr wptr) { wptr->Hide(); });
  }

  void WidgetWindow::Update()
  {
    for (auto & pWin : m_Popups) {
      if (pWin && pWin->IsOpen()) {
        pWin->Update();
      }
    }

    std::for_each(m_Widgets.begin(), m_Widgets.end(),
                  [this](Widget::shared_ptr wptr) { wptr->BaseUpdate(); });

    if (GlobalMouseFocus || GlobalKeyboardFocus) {
      GlobalMouseFocus->BaseUpdate();
    }
  }

  bool WidgetWindow::HandleEvent(sf::Event event)
  {
    optional<Widget::pointer> mFocus = GetChildWithMouseFocus();
    optional<Widget::pointer> kFocus = GetChildWithKeyboardFocus();

    Widget::previousMousePositon = Widget::currentMousePosition;
    Widget::currentMousePosition = sf::Mouse::getPosition(m_Window);

    //auto uMouse = GetWidgetMouseOver();

    if (GlobalMouseFocus && (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::MouseMoved) ) {
      GlobalMouseFocus->HandleEvent(event);
      return true;
    }

    if (GlobalKeyboardFocus && ( event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased || event.type == sf::Event::TextEntered )) {
      GlobalKeyboardFocus->HandleEvent(event);
      return true;
    }

    for (auto & wptr : m_Widgets) {
      if (wptr->IsEnabled() && wptr->HandleEvent(event))
        return true;
    }

    switch (event.type)
    {
      case sf::Event::Closed: return false;

      case sf::Event::KeyPressed:
      {
        if (GlobalKeyboardFocus) {
          GlobalKeyboardFocus->KeyPressed(event.key.code);
          //GlobalKeyboardFocus->HandleKeyPressed(event.key.code);
          return true;
        }
        if (kFocus && kFocus.value()->m_AcceptsKeyPress) {
          //kFocus.value()->HandleKeyPressed(event.key.code);
          kFocus.value()->KeyPressed(event.key.code);
          return true;
        }
        break;
      } // sf::Event::KeyPressed
    
      case sf::Event::KeyReleased:
      {
        if (GlobalKeyboardFocus) {
          //GlobalKeyboardFocus->HandleKeyReleased(event.key.code);
          GlobalKeyboardFocus->KeyReleased(event.key.code);
          return true;
        }
        if (kFocus && kFocus.value()->m_AcceptsKeyRelease) {
          //kFocus.value()->HandleKeyReleased(event.key.code);
          kFocus.value()->KeyReleased(event.key.code);
          return true;
        }
        break;
      } // sf::Event::KeyReleased

      case sf::Event::TextEntered:
      {
        if (GlobalKeyboardFocus) {
          //GlobalKeyboardFocus->HandleTextEntered(event.text.unicode);
          GlobalKeyboardFocus->TextEntered(event.text.unicode);
          return true;
        }
        if (kFocus && kFocus.value()->m_AcceptsTextEntered) {
          //kFocus.value()->HandleTextEntered(event.text.unicode);
          kFocus.value()->TextEntered(event.text.unicode);
          return true;
        }
        break;
      }

      default:
      {
        break;
      }
    } // switch

    return false;
  }

  void WidgetWindow::Render()
  {
    std::for_each(m_Widgets.begin(), m_Widgets.end(),
                  [this](auto wptr) { wptr->Render(m_Window); });

    if (GlobalMouseFocus) {
      GlobalMouseFocus->Render(m_Window);
    }
    if (GlobalKeyboardFocus) {
      GlobalKeyboardFocus->Render(m_Window);
    }
  }

  void WidgetWindow::Render(sf::RenderTarget &Target)
  {
    // meh
  }

  void WidgetWindow::PostRender()
  {
    for (auto & pWin : m_Popups) {
      if (pWin && pWin->IsOpen()) {
        pWin->DoEvents();
        pWin->Render();
      }
    }
  }

  void WidgetWindow::AddPopupWindow(PopupWindow::shared_ptr win)
  {
    m_Popups.push_back(win);
  }

  void WidgetWindow::Block()
  {
    std::cout << "WidgetWindow blocked\n";
    m_IsBeingBlocked = true;
  }

  void WidgetWindow::Unblock()
  {
    std::cout << "WidgetWindow unblocked\n";
    m_IsBeingBlocked = false;
  }

  void WidgetWindow::GiveMouseFocus(Widget *widget)
  {
    GlobalMouseFocus = widget;
  }

  void WidgetWindow::GiveKeyboardFocus(Widget *widget)
  {
    GlobalKeyboardFocus = widget;
  }

  void WidgetWindow::RelinquishMouseFocus()
  {
    GlobalMouseFocus = nullptr;
  }

  void WidgetWindow::RelinquishKeyboardFocus()
  {
    GlobalKeyboardFocus = nullptr;
  }

  void WidgetWindow::StealMouseFocus(Widget *widget)
  {
    GlobalMouseFocus = widget;
  }

  void WidgetWindow::ReturnMouseFocus(Widget *widget)
  {
    GlobalMouseFocus = nullptr;
  }

  void WidgetWindow::StealKeyboardFocus(Widget *widget)
  {
    GlobalKeyboardFocus = widget;
  }

  void WidgetWindow::ReturnKeyboardFocus(Widget *widget)
  {
    GlobalKeyboardFocus = nullptr;
  }

  void WidgetWindow::ChildTakingMouseFocus(Widget::pointer child)
  {
    m_ChildWithMouseFocus = child;
  }

  void WidgetWindow::ChildTakingKeyboardFocus(Widget::pointer child)
  {

  }

  void WidgetWindow::ChildReleasingMouseFocus(Widget::pointer child)
  {
    m_ChildWithMouseFocus = nullptr;
  }

  void WidgetWindow::ChildReleasingKeyboardFocus(Widget::pointer child)
  {
    m_ChildWithKeyboardFocus = child;
  }

  optional<Widget::shared_ptr> WidgetWindow::GetWidgetMouseOver() const
  {
    Widget::shared_ptr sptr = nullptr;

    for (auto & wptr : m_Widgets) {
      if (wptr->Bounds().contains(currentMousePosition)) {
        sptr = wptr;
        break;
      }
    }

    if (sptr != nullptr) return sptr;

    return std::optional<Widget::shared_ptr>();
  }

  void WidgetWindow::Cleanup()
  {
    std::for_each(m_Widgets.begin(), m_Widgets.end(), [ ](auto wptr) { wptr->Cleanup(); });
  }

  void WidgetWindow::AppExit()
  {

  }

  void WidgetWindow::MoveChildToFront(Widget::pointer child)
  {
    auto wptr = std::find_if(m_Widgets.begin(), m_Widgets.end(),
                                        [this, &child](Widget::shared_ptr ptr) { return ( ptr.get() == child ); });
    
    if (wptr != m_Widgets.end()) {

      if (m_Widgets.front() == *wptr)
        return;

      std::iter_swap(wptr, m_Widgets.begin());
    }
  }

  void WidgetWindow::MoveChildToBack(Widget::pointer child)
  {
    auto wptr = std::find_if(m_Widgets.begin(), m_Widgets.end(),
                             [this, &child](Widget::shared_ptr ptr) { return ( ptr.get() == child ); });

    if (wptr != m_Widgets.end()) {

      if (m_Widgets.back() == *wptr)
        return;

      std::iter_swap(wptr, m_Widgets.end() - 1);
    }
  }

}
