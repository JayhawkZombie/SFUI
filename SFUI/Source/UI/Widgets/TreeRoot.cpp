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
#include <SFUI/Include/UI/Widgets/TreeRoot.h>
#include <SFUI/Include/UI/Widgets/WidgetTree.h>
#include <SFUI/Include/UI/Theme.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  TreeRoot::TreeRoot(WidgetTree *ParentTree, optional<Theme*> theme)
    : Widget(theme, ParentTree)
  {
    m_IconTexture = m_Theme->IconTexture;
    m_IconRect.setTexture(m_IconTexture.get());
    m_IconRect.setTextureRect(m_CollapseRect);
    m_BackgroundRect.setFillColor(sf::Color::Transparent);
    m_TextView.value()->SetAlignment(TextAlignment::Left);
  }

  TreeRoot::~TreeRoot()
  {

  }

  sfui::TreeRoot::shared_ptr TreeRoot::Create(WidgetTree *ParentTree, optional<Theme*> theme)
  {
    return std::make_shared<TreeRoot>(ParentTree, theme);
  }

  void TreeRoot::Update()
  {
    if (m_IsExpanded) {
      std::for_each(begin(m_Widgets), end(m_Widgets), 
                    [this](auto wptr) { wptr->BaseUpdate(); });
    }
  }

  void TreeRoot::Render(sf::RenderTarget &Target)
  {
    super::Render(Target);
    m_TextView.value()->Render(Target, {});
    Target.draw(m_IconRect);
    if (m_IsExpanded) {
      for (auto & wptr : m_Widgets) wptr->Render(Target);
    }
  }

  bool TreeRoot::HandleEvent(const sf::Event &event)
  {
    if (Widget::HandleEvent(event)) return true;

    if (m_IsExpanded) {
      for (auto & wptr : m_Widgets)
        if (wptr->HandleEvent(event))
          return true;
    }

    return false;
  }

  void TreeRoot::Expand()
  {
    m_IsExpanded = true;
    m_IconRect.setTextureRect(m_ExpandedIconRect);
    m_ExpandedSignal();
  }

  void TreeRoot::Collapse()
  {
    m_IconRect.setTextureRect(m_CollapseRect);
    m_IsExpanded = false;
    m_CollapsedSignal();
  }

  bool TreeRoot::IsExpanded() const
  {
    return m_IsExpanded;
  }

  void TreeRoot::Add(Widget::shared_ptr widget)
  {
    auto size = widget->GetSize();
    if (size.x > m_TreeSize.x)
      m_TreeSize.x = size.x;
    m_TreeSize.y += size.y;
    widget->SetParent(this);
    
    Vec2i wPos = { m_Position.x + 7, m_Position.y + m_TreeSize.y };
    m_TreeSize.y += 3;
    widget->SetPosition(wPos);
    widget->SetTopWindow(m_TopWindow);
    m_Widgets.push_back(widget);
  }

  void TreeRoot::SetPosition(const Vec2i &Position)
  {
    auto delta = Position - m_Position;
    super::SetPosition(Position);

    m_IconRect.setPosition(m_Position + 0.25f * Vec2f(m_Size.y, m_Size.y));
    m_TextView.value()->SetPosition({ m_Position.x + m_Size.y, m_Position.y });
    m_TextView.value()->SetSize({ m_Size.x - m_Size.y, m_Size.y });

    for (auto & wptr : m_Widgets) wptr->Move(delta);
  }

  void TreeRoot::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);
    m_IconRect.setSize(Vec2f(0.5f * m_Size.y, 0.5f * m_Size.y));
    m_IconRect.setPosition(m_Position + 0.25f * Vec2f(m_Size.y, m_Size.y));
    m_TextView.value()->SetPosition({ m_Position.x + m_Size.y, m_Position.y });
    m_TextView.value()->SetSize({ m_Size.x - m_Size.y, m_Size.y });
  }

  void TreeRoot::SetTopWindow(WidgetWindow *TopWindow)
  {
    super::SetTopWindow(TopWindow);
    for (auto & wptr : m_Widgets) wptr->SetTopWindow(TopWindow);
  }

  void TreeRoot::OnExpanded(boost::function<void()> func)
  {
    m_ExpandedSignal.connect(func);
  }

  void TreeRoot::OnCollapsed(boost::function<void()> func)
  {
    m_CollapsedSignal.connect(func);
  }

  Vec2i TreeRoot::GetExpansionSize() const
  {
    return m_TreeSize;
  }

  void TreeRoot::Move(const Vec2i &Delta)
  {
    super::Move(Delta);
    m_TextView.value()->SetPosition({ m_Position.x + m_Size.y, m_Position.y });
    m_IconRect.move(Delta);
    for (auto & wptr : m_Widgets)
      wptr->Move(Delta);
  }

  void TreeRoot::MousePressed(bool left, bool right)
  {
    if (left) {
      if (m_IsExpanded) Collapse();
      else Expand();
    }
  }

}  
