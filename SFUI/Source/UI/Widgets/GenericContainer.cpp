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

  GenericContainer::GenericContainer(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : Widget(theme, parent)
  {

  }

  GenericContainer::~GenericContainer()
  {
    for_each(
      begin(m_Widgets), end(m_Widgets),
      [ ](auto wptr) { wptr->Cleanup(); }
    );

    m_Widgets.clear();
  }

  void GenericContainer::Update()
  {
    for_each(
      begin(m_Widgets), end(m_Widgets),
      [ ](auto wptr) { wptr->BaseUpdate(); }
    );
  }

  void GenericContainer::Render(sf::RenderTarget &Target, sf::View view)
  {
    for_each(
      begin(m_Widgets), end(m_Widgets),
      [&Target, &view](auto wptr) { wptr->Render(Target); }
    );
  }

  void GenericContainer::Add(Widget::shared_ptr widget)
  {
    widget->SetParent(this);
    m_Widgets.insert(widget);
  }

  void GenericContainer::Remove(Widget::shared_ptr widget)
  {
    auto it = m_Widgets.find(widget);
    if (it != m_Widgets.end())
      m_Widgets.erase(it);
  }

  void GenericContainer::Move(const Vec2i &Delta)
  {
    super::Move(Delta);
    for (auto & wptr : m_Widgets)
      wptr->Move(Delta);
  }

  bool GenericContainer::HandleEvent(const sf::Event &event)
  {
    if (m_ChildWithMouseFocus) {
      if (( event.type == sf::Event::MouseMoved ) || ( event.type == sf::Event::MouseButtonPressed ) || ( event.type == sf::Event::MouseButtonReleased )) {
        ( *m_ChildWithMouseFocus )->HandleEvent(event);
        return true;
      }
    }

    if (m_ChildWithKeyboardFocus) {
      if (( event.type == sf::Event::KeyPressed ) || ( event.type == sf::Event::KeyReleased ) || ( event.type == sf::Event::TextEntered )) {
        ( *m_ChildWithKeyboardFocus )->HandleEvent(event);
        return true;
      }
    }

    if (!m_Bounds.contains(currentMousePosition)) return false;

    for (auto & widget : m_Widgets) {
      if (widget->HandleEvent(event))
        return true;
    }

    return false;
  }

  optional<sfui::Widget::shared_ptr> GenericContainer::GetChildUnderMouse(const Vec2i &mpos)
  {
    auto it = std::find_if(begin(m_Widgets), end(m_Widgets),
                           [&mpos](auto wptr) { return ( wptr->Bounds().contains(mpos) && wptr->IsVisible() ); });
  
    if (it != m_Widgets.end())
      return *it;
    return optional<Widget::shared_ptr>();
  }

  void GenericContainer::ItemAdded()
  {

  }

  void GenericContainer::ItemRemoved()
  {

  }

}  
