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
#include <SFUI/Include/UI/Widgets/TabList.h>
#include <SFUI/Include/UI/Theme.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  TabList::TabList(optional<Widget*> parent /*= optional<Widget*>()*/, optional<Theme*> theme /*= optional<Theme*>()*/)
    : Widget(theme, parent, Event::All)
  {

  }

  TabList::~TabList()
  {

  }

  sfui::TabList::shared_ptr TabList::Create(optional<Widget*> parent /*= optional<Widget*>()*/, optional<Theme*> theme /*= optional<Theme*>()*/)
  {
    return std::make_shared<TabList>(parent, theme);
  }

  Panel::shared_ptr TabList::AddTab(const std::string &Text)
  {
    Tab::shared_ptr tPtr = m_Theme->MakeTab(this, Text);
    Panel::shared_ptr pPtr = m_Theme->MakePanel(this);
    pPtr->SetPosition({ m_Position.x, m_Position.y + cast_int(m_TabHeight) });
    pPtr->SetSize({ m_Size.x, m_Size.y - cast_int(m_TabHeight) });
    pPtr->SetParent(this);

    tPtr->OnSelected([this](Tab* t) { TabSelected(t); });
    m_Tabs.emplace_back( tPtr, pPtr );
    RepositionTabs();

    if (!m_CurrentTab) {
      m_Tabs.back().first->Select();
    }

    return pPtr;
  }

  sfui::Tab::shared_ptr TabList::GetSelectedTab() const
  {
    auto it = find_if(begin(m_Tabs), end(m_Tabs),
                      [this](auto stab) { return ( stab.first.get() == m_CurrentTab ); });
    if (it != m_Tabs.end())
      return ( *it ).first;

    return nullptr;
  }

  sfui::Panel* TabList::GetCurrentPanel() const
  {
    return m_CurrentPanel;
  }

  void TabList::SelectTab(const std::string &Text)
  {
    auto it = find_if(begin(m_Tabs), end(m_Tabs),
                      [str = Text](auto wptr) { return ( wptr.first->GetText() == str ); });
    if (it != m_Tabs.end()) {
      ( *it ).first->Select();
    }
  }

  void TabList::SelectTab(Tab *tPtr)
  {
    auto it = find_if(begin(m_Tabs), end(m_Tabs),
                      [ptr = tPtr](auto wptr) { return ( wptr.first.get() == ptr ); });
    if (it != m_Tabs.end()) {
      ( *it ).first->Select();
    }
  }

  void TabList::Update()
  {
    for (auto & tab : m_Tabs) {
      tab.first->BaseUpdate();
      tab.second->BaseUpdate();
    }
  }

  void TabList::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_BackgroundRect);
    for (auto & tab : m_Tabs) {
      tab.first->Render(Target);
    }

    if (m_CurrentPanel) {
      m_CurrentPanel->Render(Target);
    }
  }

  void TabList::SetTabHeight(uint32 Height)
  {
    m_TabHeight = Height;
  }

  void TabList::SetPanelHeight(uint32 Height)
  {
    m_PanelHeight = Height;

    Vec2i pSize;

    for (auto & tabpair : m_Tabs) {
      pSize = tabpair.second->GetSize();
      pSize.y = ( int )m_PanelHeight;
      pSize.x = ( int )m_PanelWidth;
      tabpair.second->SetSize(pSize);
    }
  }

  void TabList::SetPanelWidth(uint32 Width)
  {
    m_PanelWidth = Width;

    Vec2i pSize;

    for (auto & tabpair : m_Tabs) {
      pSize = tabpair.second->GetSize();
      pSize.y = ( int )m_PanelHeight;
      pSize.x = ( int )m_PanelWidth;
      tabpair.second->SetSize(pSize);
    }
  }

  void TabList::SetMinimumTabWidth(uint32 Width)
  {
    m_MinTabWidth = Width;
  }

  void TabList::OnTabSelected(boost::function<void(Tab*)> func)
  {
    m_TabSelectedSignal.connect(func);
  }

  void TabList::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);
    m_PanelSize.x = Size.x;
    m_PanelSize.y = Size.y - cast_int(m_TabHeight);

    for (auto & tab : m_Tabs)
      tab.second->SetSize(m_PanelSize);
  }

  bool TabList::HandleEvent(const sf::Event &event)
  {
    if (m_CurrentPanel && m_CurrentPanel->HandleEvent(event))
      return true;

    //if (!Widget::HandleEvent(event)) return false;

    for (auto & w_pair : m_Tabs) {
      if (w_pair.first->HandleEvent(event)) return true;
    }

    return false;
  }

  optional<Tab::shared_ptr> TabList::GetTabUnderMouse()
  {
    auto it = find_if(begin(m_Tabs), end(m_Tabs),
                      [this](auto wptr) { return ( wptr.first->Bounds().contains(currentMousePosition) ); });
    if (it != m_Tabs.end()) {
      return ( *it ).first;
    }

    return optional<Tab::shared_ptr>();
  }

  void TabList::RepositionTabs()
  {
    Vec2i tabPos = m_Position;
    Vec2i tabSize = { (int)m_MinTabWidth, (int)m_TabHeight };
    int totalWidth = 0;
    Vec2i tabTextSize;

    for (auto & tabPtr : m_Tabs) {
      tabTextSize = tabPtr.first->GetTextSize();
      
      tabSize = { int(std::max(m_MinTabWidth, (uint32)tabTextSize.x) * 1.5) , (int)m_TabHeight };
      tabPtr.first->SetPosition(tabPos);
      tabPtr.first->SetSize(tabSize);

      tabPos.x += tabSize.x;
      totalWidth += tabSize.x;
    }

    //The position of the Panel is always at the bottom of the TabList
    Vec2i panelPos = { m_Position.x, m_Position.y + m_Size.y };
    m_PanelPosition = panelPos;

    Vec2i pSize;
    for (auto & tptr : m_Tabs) {
      tptr.second->SetSize({ m_Size.x, (int)m_PanelHeight });
      tptr.second->SetPosition(panelPos);
    }

    m_Bounds.height = ( float )m_PanelHeight;
  }

  void TabList::TabSelected(Tab* tPtr)
  {
    if (m_CurrentTab) {
      m_CurrentTab->Deselect();
    }

    auto it = find_if(begin(m_Tabs), end(m_Tabs),
                      [&tPtr](auto tabpair) { return ( tabpair.first.get() == tPtr ); });

    if (it != m_Tabs.end()) {
      m_CurrentTab = ( *it ).first.get();
      m_CurrentPanel = ( *it ).second.get();
    }
    m_CurrentTab = tPtr;
    
    m_TabSelectedSignal(m_CurrentTab);
  }

  void TabList::Moved()
  {
    RepositionTabs();
  }

  void TabList::Resized()
  {
    
  }

}  
