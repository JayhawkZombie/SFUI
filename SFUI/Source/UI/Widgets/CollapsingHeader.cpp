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
#include <SFUI/Include/UI/Widgets/CollapsingHeader.h>
#include <SFUI/Include/UI/Theme.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  CollapsingHeader::CollapsingHeader(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : Selectable(theme, parent)
  {
    m_IconRect.setTexture(m_Theme->IconTexture.get());
    m_IconRect.setTextureRect(m_CollapseRect);
    m_SelectionColor = m_BGColor;
  }

  CollapsingHeader::~CollapsingHeader()
  {

  }

  sfui::CollapsingHeader::shared_ptr CollapsingHeader::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<CollapsingHeader>(theme, parent);
  }

  bool CollapsingHeader::HandleEvent(const sf::Event &event)
  {
    if (!Selectable::HandleEvent(event) && !m_IsExpanded) return false;

    if (!m_IsExpanded) return false;

    for (auto & widget : m_Widgets) {
      if (widget->HandleEvent(event)) return true;
    }

    return false;
  }

  void CollapsingHeader::Update()
  {
    super::Update();

    if (m_IsExpanded) {
      for (auto & widget : m_Widgets) { widget->BaseUpdate(); }
    }

  }

  void CollapsingHeader::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_BackgroundRect);
    m_TextView.value()->Render(Target, {});
    Target.draw(m_IconRect);

    if (m_IsExpanded) {
      for (auto & widget : m_Widgets)
        widget->Render(Target);
    }
  }

  bool CollapsingHeader::IsExpanded() const
  {
    return m_IsExpanded;
  }

  void CollapsingHeader::Expand()
  {
    m_IconRect.setTextureRect(m_ExpandedIconRect);
    m_IsExpanded = true;
    m_ExpandedSignal();
  }

  void CollapsingHeader::Collapse()
  {
    m_IconRect.setTextureRect(m_CollapseRect);
    m_IsExpanded = false;
    m_CollapsedSignal();
  }

  void CollapsingHeader::Add(Widget::shared_ptr widget)
  {
    Vec2i pos = ( m_Widgets.empty() ? m_Position : m_Widgets.back()->GetPosition() );
    Vec2i lastSize = ( m_Widgets.empty() ? m_Size : m_Widgets.back()->GetSize() );
    pos.x += m_Size.y;
    pos.y += lastSize.y;

    widget->SetPosition(pos);
    widget->SetParent(this);
    m_Widgets.push_back(widget);
  }

  void CollapsingHeader::OnExpanded(boost::function<void()> func)
  {
    m_ExpandedSignal.connect(func);
  }

  void CollapsingHeader::OnCollapsed(boost::function<void()> func)
  {
    m_CollapsedSignal.connect(func);
  }

  void CollapsingHeader::Selected()
  {
    Expand();
  }

  void CollapsingHeader::Deselected()
  {
    Collapse();
  }

  void CollapsingHeader::Moved()
  {
    super::Moved();

    RepositionWidgets();
    m_IconRect.setPosition(m_Position);
    m_TextView.value()->SetPosition({ m_Position.x + m_Size.y, m_Position.y });
    m_TextView.value()->SetSize({ m_Size.x - m_Size.y, m_Size.y });
  }

  void CollapsingHeader::Resized()
  {
    RepositionWidgets();

    m_IconRect.setSize(Vec2i(m_Size.y, m_Size.y));
  }

  void CollapsingHeader::RepositionWidgets()
  {
    Vec2i pos = m_Position;
    pos += Vec2i(m_Size.y, m_Size.y);

    Vec2i itemsize = { 0, 0 };

    for (auto & widget : m_Widgets) {
      widget->SetPosition(pos);
      itemsize = widget->GetSize();
      
      pos.y += itemsize.y;
    }

  }

  void CollapsingHeader::Expanded()
  {

  }

  void CollapsingHeader::Collapsed()
  {

  }

}  
