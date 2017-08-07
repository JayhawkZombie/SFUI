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
#include <SFUI/Include/UI/Widgets/Selectable.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  Selectable::Selectable(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : Widget(theme, parent)
  {
    m_SelectedRect.setFillColor(m_SelectionColor);
  }

  sfui::Selectable::shared_ptr Selectable::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<Selectable>(theme, parent);
  }

  void Selectable::Select()
  {
    m_IsSelected = true;
    m_SelectedSignal(GetText());
    Selected();
  }

  void Selectable::Deselect()
  {
    m_IsSelected = false;
    m_BackgroundRect.setFillColor(m_BGColor);
    m_DeselectedSignal(GetText());
    Deselected();
  }

  void Selectable::SetPosition(const Vec2i &Position)
  {
    super::SetPosition(Position);
  }

  void Selectable::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);
  }

  bool Selectable::IsSelected() const
  {
    return m_IsSelected;
  }

  void Selectable::SetSelectColor(const Color &c)
  {
    m_SelectionColor = c;
    m_HighlightRect.setFillColor(c);
    SetHighlight(m_Highlight, c, m_HighlightThickness);
  }

  void Selectable::Move(const Vec2i &Delta)
  {
    Widget::Move(Delta);
    m_SelectedRect.move(Delta);
  }

  void Selectable::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_BackgroundRect);
    if (m_IsSelected)
      Target.draw(m_HighlightRect);
    if (m_TextView)
      ( *m_TextView )->Render(Target, { });
  }

  void Selectable::OnSelected(boost::function<void(const std::string &)> func)
  {
    m_SelectedSignal.connect(func);
  }

  void Selectable::OnDeselected(boost::function<void(const std::string &)> func)
  {
    m_DeselectedSignal.connect(func);
  }

  void Selectable::MouseEntered()
  {
    Color c = m_BGColor + Color(m_BrightFactor, m_BrightFactor, m_BrightFactor, 0);
    m_BackgroundRect.setFillColor(c);
  }

  void Selectable::MouseLeft()
  {
    m_BackgroundRect.setFillColor(m_BGColor);
  }

  void Selectable::MousePressed(bool left, bool right)
  {
    if (left) {
      if (m_IsSelected) Deselect();
      else Select();
    }
  }

  void Selectable::MouseReleased(bool left, bool right)
  {

  }

  void Selectable::Selected()
  {

  }

  void Selectable::Deselected()
  {

  }

}  
