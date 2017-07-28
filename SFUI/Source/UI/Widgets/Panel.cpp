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
#include <SFUI/Include/UI/Widgets/Panel.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  Panel::Panel(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : Widget(theme, parent, Event::All)
  {

  }

  Panel::~Panel()
  {
    for (auto & wptr : m_Widgets) {
      wptr->Cleanup();
    }

    m_Widgets.clear();
  }

  sfui::Panel::shared_ptr Panel::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<Panel>(theme, parent);
  }

  void Panel::Add(Widget::shared_ptr widget)
  {
    //Get position and move it so it is relative to US
    auto wPos = widget->GetPosition();
    auto newpos = m_Position + wPos;
    widget->SetPosition(newpos);
    widget->SetParent(this);
    widget->SetTopWindow(m_TopWindow);
    auto wpos = widget->GetPosition();

    m_Widgets.insert(widget);
  }

  void Panel::Remove(Widget::shared_ptr widget)
  {
    auto it = m_Widgets.find(widget);
    if (it != m_Widgets.end())
      m_Widgets.erase(it);
  }

  void Panel::Update()
  {
    for (auto & wptr : m_Widgets) {

      if (wptr->IsEnabled())
        wptr->BaseUpdate();

    }
  }

  bool Panel::HandleEvent(const sf::Event &event)
  {
    //if (!Widget::HandleEvent(event)) return false;

    for (auto & wptr : m_Widgets) {
      if (wptr->HandleEvent(event))
        return true;
    }

    return false;
  }

  void Panel::SetTopWindow(WidgetWindow *TopWindow)
  {
    m_TopWindow = TopWindow;
    for (auto & wptr : m_Widgets)
      wptr->SetTopWindow(TopWindow);
  }

  void Panel::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_BackgroundRect);
    for (auto & wptr : m_Widgets) {
      if (wptr->IsVisible())
        wptr->Render(Target);
    }
  }

  void Panel::SetPosition(const Vec2i &Position)
  {
    super::SetPosition(Position);
    auto delta = Position - m_Position;

    Move(delta);
  }

  void Panel::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);

    Resized();
  }

  void Panel::Move(const Vec2i &Delta)
  {
    for (auto & wptr : m_Widgets) {
      wptr->Move(Delta);
    }

    Moved();
  }

  optional<Widget::shared_ptr> Panel::GetWidgetUnderMouse()
  {
    auto it = find_if(begin(m_Widgets), end(m_Widgets),
                      [this](auto wptr) { return ( wptr->Bounds().contains(currentMousePosition) ); });
    if (it != m_Widgets.end())
      return ( *it );

    return optional<Widget::shared_ptr>();
  }

  void Panel::ChildTakingMouseFocus(Widget* child)
  {
    if (m_ChildWithMouseFocus) {
      (*m_ChildWithMouseFocus)->LostMouseFocus();
    }

    m_ChildWithMouseFocus = child;
  }

  void Panel::ChildTakingKeyboardFocus(Widget* child)
  {
    if (m_ChildWithKeyboardFocus) {
      ( *m_ChildWithKeyboardFocus )->LostKeyboardFocus();
    }
    
    m_ChildWithKeyboardFocus = child;
  }

  void Panel::ChildReleasingMouseFocus(Widget* child)
  {
    m_ChildWithMouseFocus = optional<Widget*>();
  }

  void Panel::ChildReleasingKeyboardFocus(Widget* child)
  {
    m_ChildWithKeyboardFocus = optional<Widget*>();
  }

  void Panel::MoveChildToFront(Widget* child)
  {

  }

  void Panel::MoveChildToBack(Widget* child)
  {

  }

}  
