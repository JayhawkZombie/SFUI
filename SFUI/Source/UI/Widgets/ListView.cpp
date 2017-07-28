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
#include <SFUI/Include/UI/Widgets/ListView.h>
#include <SFUI/Include/UI/Theme.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  ListView::ListView(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : Widget(theme, parent)
  {
    m_ScrollBar = m_Theme->MakeScrollBar(this);
    m_ScrollBar->SetTopWindow(m_TopWindow);
    m_ScrollBar->OnScrolled([this](auto pos) {ScrollTo(pos); });
  }

  ListView::~ListView()
  {

  }

  sfui::ListView::shared_ptr ListView::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<ListView>(theme, parent);
  }

  void ListView::AddItem(const std::string &Text)
  {
    Vec2i itempos = m_Position;
    itempos.y += m_CumulativeSize.y;
    auto selBle = m_Theme->MakeSelectableButton(Text, this);
    selBle->SetBorderColor(m_Theme->WidgetOutlineColor);
    selBle->SetBorderWidth(-1);
    selBle->SetParent(this);
    selBle->SetTextSize(15);
    selBle->SetPosition(itempos);
    selBle->SetTopWindow(m_TopWindow);
    auto sSize = selBle->GetTextSize();
    selBle->OnSelected([this](auto txt) { ItemSelected(txt); });

    int height = ( int )( floor(1.15 * sSize.y) );

    selBle->SetSize({ m_Size.x, height });
    m_CumulativeSize.y += height;
    m_Items.push_back(selBle);

    if (height > m_MaxItemHeight) {
      m_MaxItemHeight = height;
    }

    AdjustList();

    if (m_CumulativeSize.y > m_Size.y)
      AdjustScrollbar();
  }

  void ListView::Update()
  {
    if (m_ScrollbarEnabled)
      m_ScrollBar->BaseUpdate();

    for (auto & item : m_Items)
      item->BaseUpdate();
  }

  void ListView::Render(sf::RenderTarget &Target)
  {
    sf::View view;
    view.reset(m_Bounds);
    auto tSize = Target.getSize();

    view.setViewport(
      {
        m_Position.x / (float)tSize.x,
        m_Position.y / (float)tSize.y,
        m_Size.x / (float)tSize.x,
        m_Size.y / (float)tSize.y
      }
    );

    auto oldview = Target.getView();
    Target.setView(view);

    Target.draw(m_BackgroundRect);
    for (auto & item : m_Items)
      item->Render(Target);

    if (m_ScrollbarEnabled)
      m_ScrollBar->Render(Target);
  
    Target.setView(oldview);
  }

  bool ListView::HandleEvent(const sf::Event &event)
  {
    if (!Widget::HandleEvent(event)) return false;

    if (m_ScrollbarEnabled && m_ScrollBar->HandleEvent(event)) return true;

    for (auto & item : m_Items) {
      if (item->HandleEvent(event))
        return true;
    }

    return false;
  }

  std::string ListView::GetCurrentSelection() const
  {
    if (m_CurrentSelection)
      return m_CurrentSelection->GetText();
    else
      return "";
  }

  bool ListView::SelectItem(const std::string &Text)
  {
    auto it = find_if(begin(m_Items), end(m_Items),
                      [&Text](auto itemptr) { return ( itemptr->GetText() == Text ); });

    if (it != m_Items.end()) {
      ( *it )->Select();
      return true;
    }

    return false;
  }

  void ListView::OnItemSelected(boost::function<void(const std::string &)> func)
  {
    m_ItemSelectedSignal.connect(func);
  }

  void ListView::SetPosition(const Vec2i &Position)
  {
    super::SetPosition(Position);
    //auto delta = Position - m_Position;

    AdjustList();
    AdjustScrollbar();
  }

  void ListView::Move(const Vec2i &Delta)
  {
    super::Move(Delta);

    for (auto & item : m_Items) {
      item->Move(Delta);
    }

    m_ScrollBar->Move(Delta);
    //AdjustScrollbar();
  }

  void ListView::ScrollTo(int ListTop)
  {
    Vec2i itempos = { m_Position.x, ListTop };
    
    for (auto & item : m_Items) {
      item->SetPosition(itempos);
      itempos.y += m_MaxItemHeight;
    }

  }

  void ListView::AdjustScrollbar()
  {
    m_ScrollBar->SetPosition({ m_Position.x + m_Size.x - 15, m_Position.y });
    m_ScrollBar->SetSize({ 15, m_Size.y });
    m_ScrollBar->SetRange(m_Position.y, m_Position.y + m_Size.y, m_Position.y + m_CumulativeSize.y);
    m_ScrollBar->SetIsVertical(true);
    
    m_ScrollbarEnabled = m_CumulativeSize.y >= m_Size.y;
    if (m_ScrollbarEnabled)
      m_ScrollBar->Enable();
    else
      m_ScrollBar->Disable();
  }

  void ListView::AdjustList()
  {
    if (m_Items.empty()) return;

    Vec2i itempos = { m_Position.x, m_Position.y };
    Vec2i itemsize = { m_Size.x, m_MaxItemHeight };

    int cumHeight = 0;

    for (auto & item : m_Items) {
      item->SetPosition(itempos);
      item->SetSize(itemsize);

      itempos.y += itemsize.y;

      cumHeight += itemsize.y;
    }

    m_ListTop = m_Items[0]->GetPosition().y;
    m_CumulativeSize.y = cumHeight;
  }

  void ListView::ScrollItems(double posToScrollTo)
  {
    ScrollTo(( int )( floor(posToScrollTo) ));
  }

  void ListView::ItemSelected(const std::string &str)
  {
    auto it = find_if(begin(m_Items), end(m_Items),
                      [&str](auto itemptr) { return ( itemptr->GetText() == str ); });

    if (it != m_Items.end()) {
      if (m_CurrentSelection)
        m_CurrentSelection->Deselect();

      m_CurrentSelection = ( *it ).get();
    }

    m_ItemSelectedSignal(str);
  }

}  
