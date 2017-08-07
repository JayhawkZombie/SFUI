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
#include <SFUI/Include/UI/Widgets/Menu.h>
#include <SFUI/Include/UI/Theme.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  

  Menu::Menu(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget::pointer> parent /*= optional<Widget*>()*/)
    : Widget(theme, parent)
  {

  }

  Menu::~Menu()
  {
    for (auto & bptr : m_Items)
      bptr->Cleanup();

    m_Items.clear();
  }

  sfui::Menu::shared_ptr Menu::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget::pointer> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<Menu>(theme, parent);
  }

  void Menu::Add(const std::string &Text)
  {
    auto item = m_Theme->MakeLineItem(this);
    item->SetText(Text);
    item->SetParent(this);
    item->OnSelected([this](auto & str) { ItemSelected(str); });

    auto tSize = item->GetTextSize();
    if (tSize.x > m_MaxItemWidth && tSize.x >= m_MinWidth)
      m_MaxItemWidth = tSize.x;

    m_Items.push_back(item);
    RepositionItems();
  }

  void Menu::Remove(const std::string &Text)
  {

  }

  bool Menu::SelectItem(const std::string &Text)
  {
    auto it = find_if(begin(m_Items), end(m_Items),
                      [&Text](auto iPtr) { return ( iPtr->GetText() == Text ); });

    if (it != m_Items.end()) {
      ( *it )->Select();
      return true;
    }
    
    return false;
  }

  void Menu::SetItemHeight(uint32 Height)
  {
    m_ItemHeight = Height;
  }

  void Menu::Open()
  {
    Show();
    m_IsOpen = true;
    m_OpenedSignal(this);
  }

  void Menu::Close()
  {
    Hide();
    m_IsOpen = false;
    m_ClosedSignal(this);
  }

  void Menu::Update()
  {
    if (!m_IsOpen) return;

    for (auto & bptr : m_Items)
      bptr->BaseUpdate();
  }

  void Menu::Render(sf::RenderTarget &Target)
  {
    if (!m_IsOpen) return;

    Target.draw(m_BackgroundRect);

    for (auto & bptr : m_Items)
      bptr->Render(Target);
  }

  void Menu::SetItemSpacing(uint32 Spacing)
  {
    m_ItemSpacing = Spacing;
  }

  void Menu::SetBoxPadding(Vec2i Padding)
  {
    m_Padding = Padding;
  }

  void Menu::OnItemSelected(boost::function<void(const std::string &)> func)
  {
    m_MenuItemSelectedSignal.connect(func);
  }

  void Menu::OnOpened(boost::function<void(Menu*)> func)
  {
    m_OpenedSignal.connect(func);
  }

  void Menu::OnClosed(boost::function<void(Menu*)> func)
  {
    m_ClosedSignal.connect(func);
  }

  bool Menu::HandleEvent(const sf::Event &event)
  {
    if (!m_IsOpen) return false;

    if (!m_Bounds.contains(currentMousePosition)) return false;

    for (auto & btn : m_Items) {
      if (btn->HandleEvent(event))
        return true;
    }

    return false;
  }

  void Menu::ItemSelected(const std::string &str)
  {
    m_MenuItemSelectedSignal(str);
    Close();
  }

  void Menu::Moved()
  {
    RepositionItems();
  }

  void Menu::Resized()
  {
    RepositionItems();
  }

  void Menu::RepositionItems()
  {
    if (m_Items.empty())
      return;

    Vec2i itempos = m_Position;
    itempos += m_Padding;
    Vec2i itemsize = { cast_int(floor(1.2f * m_MaxItemWidth)) - 2 * m_Padding.x, cast_int(m_ItemHeight) };
    int totHeight = 0;
    uint32 count = 0;

    for (auto & bptr : m_Items) {
      if (bptr->GetTextSize().x > m_MaxItemWidth)
        m_MaxItemWidth = bptr->GetSize().x;

      bptr->SetPosition(itempos);      

      itempos.y += itemsize.y + m_ItemSpacing;
      totHeight += itemsize.y + m_ItemSpacing;
    }

    if (m_MaxItemWidth > m_MinWidth)
      m_Size.x = cast_int(floor(1.2f * m_MaxItemWidth)) + 2 * m_Padding.x;

    for (auto & bptr : m_Items) {
      bptr->SetSize({ m_Size.x, itemsize.y });
    }

    totHeight -= m_ItemSpacing;
    m_Size.y = totHeight;

    m_Bounds.height = m_Size.y;
    m_Bounds.width = m_Size.x;
    m_BackgroundRect.setSize(m_Size);
  }

}  
