
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
#include <SFUI/Include/UI/Widgets/Tab.h>
#include <SFUI/Include/UI/Widgets/TabList.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  

  Tab::Tab(const std::string &Title, optional<TabList*> parentList /*= optional<TabList*>()*/, optional<Theme*> theme /*= optional<Theme*>()*/)
    : Widget(theme, parentList)
  {
    if (parentList)
      m_ParentTabList = parentList.value();
    m_TextView.value()->SetAlignment(TextAlignment::Centered);
  }

  Tab::~Tab()
  {

  }

  sfui::Tab::shared_ptr Tab::Create(const std::string &Title, optional<TabList*> parentList /*= optional<TabList*>()*/, optional<Theme*> theme /*= optional<Theme*>()*/)
  {
    return std::make_shared<Tab>(Title, parentList, theme);
  }

  void Tab::Select()
  {
    if (m_IsSelected) return;

    m_IsSelected = true;
    //m_BackgroundRect.setFillColor(m_SelectionColor);
    m_SelectedSignal(this);
  }

  void Tab::Deselect()
  {
    m_IsSelected = false;
    m_BackgroundRect.setFillColor(m_BGColor);

    //When we switch tabs, anything that had focus (that is one of our children)
    // should lose focus
    if (m_ChildWithMouseFocus) {
      ( *m_ChildWithMouseFocus )->LostMouseFocus();
      m_ChildWithMouseFocus = {};
    }

    if (m_ChildWithKeyboardFocus) {
      ( *m_ChildWithKeyboardFocus )->LostKeyboardFocus();
      m_ChildWithKeyboardFocus = {};
    }
  }

  void Tab::SetSelectColor(const Color &c)
  {
    m_SelectionColor = c;
    m_SelectedRect.setFillColor(c);
    SetHighlight(m_Highlight, c, m_HighlightThickness);
    //if (m_IsSelected)
    //  m_BackgroundRect.setFillColor(c);
  }

  void Tab::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_BackgroundRect);
    if (m_TextView)
      ( *m_TextView )->Render(Target, {});
    if (m_IsSelected)
      Target.draw(m_HighlightRect);
  }

  void Tab::OnSelected(boost::function<void(Tab*)> func)
  {
    m_SelectedSignal.connect(func);
  }

  void Tab::SetPosition(const Vec2i &Position)
  {
    super::SetPosition(Position);
  }

  void Tab::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);
  }

  void Tab::Move(const Vec2i &Delta)
  {
    super::Move(Delta);
    m_SelectedRect.move(Delta);
  }

  void Tab::MouseEntered()
  {
    if (!m_IsSelected) {
      Color c = m_BGColor + Color(15, 15, 15, 0);
      m_BackgroundRect.setFillColor(c);
    }
  }

  void Tab::MouseLeft()
  {
    if (!m_IsSelected) {
      m_BackgroundRect.setFillColor(m_BGColor);
    }
  }

  void Tab::MousePressed(bool left, bool right)
  {
    if (!m_IsSelected) {
      Select();
    }
    //Cannot deselect this way.  Must be Deselected by another Tab being selected
  }

}  
