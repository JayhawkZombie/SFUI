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
#include <SFUI/Include/UI/Widgets/LineItem.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  

  
  LineItem::LineItem(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : Widget(theme, parent)
  {

  }

  LineItem::~LineItem()
  {

  }

  sfui::LineItem::shared_ptr LineItem::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<LineItem>(theme, parent);
  }

  void LineItem::Update()
  {

  }

  void LineItem::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_BackgroundRect);
    ( *m_TextView )->Render(Target, {});
  }

  void LineItem::OnSelected(boost::function<void(const std::string &)> func)
  {
    m_SelecetdSignal.connect(func);
  }

  void LineItem::OnDeselected(boost::function<void()> func)
  {
    m_DeselectedSignal.connect(func);
  }

  bool LineItem::HandleEvent(const sf::Event &event)
  {
    if (!m_Bounds.contains(currentMousePosition)) return false;

    if (!Widget::HandleEvent(event)) return false;

    return true;
  }

  void LineItem::SetHoverColor(const Color &c)
  {
    m_HoverColor = c;
  }

  void LineItem::SetText(const std::string &Text)
  {
    super::SetText(Text);
    auto tSize = m_TextView.value()->GetSize();
    SetSize(tSize);
  }

  void LineItem::Select()
  {
    m_BackgroundRect.setFillColor(sf::Color::Transparent);
    m_SelecetdSignal(GetText());
  }

  void LineItem::Deselect()
  {
    m_BackgroundRect.setFillColor(sf::Color::Transparent);
    m_DeselectedSignal();
  }

  void LineItem::Moved()
  {
    ( *m_TextView )->SetAlignment(TextAlignment::Left);
    ( *m_TextView )->SetPosition(m_Position);
  }

  void LineItem::Resized()
  {
    ( *m_TextView )->SetAlignment(TextAlignment::Left);
    ( *m_TextView )->SetPosition(m_Position);
  }

  void LineItem::MouseMoved()
  {
    if (!IsMouseOver())
      MouseEntered();
    
  }

  void LineItem::MouseEntered()
  {
    //Highlighted
    m_BackgroundRect.setFillColor(m_HoverColor);
  }

  void LineItem::MouseLeft()
  {
    m_BackgroundRect.setFillColor(sf::Color::Transparent);
  }

  void LineItem::MousePressed(bool left, bool right)
  {
    sf::Color c = m_HoverColor;
    c -= sf::Color(30, 30, 30);
    m_BackgroundRect.setFillColor(c);
  }

  void LineItem::MouseReleased(bool left, bool right)
  {
    if (left) {
      Select();
    }
    m_BackgroundRect.setFillColor(m_HoverColor);
  }

}  
