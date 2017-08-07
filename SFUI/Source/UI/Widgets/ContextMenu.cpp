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
#include <SFUI/Include/UI/Widgets/ContextMenu.h>
#include <SFUI/Include/UI/Theme.h>
#include <SFUI/Include/UI/Widgets/LineItem.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{
  
  ContextMenu::ContextMenu(optional<Theme*> theme /* = { } */, optional<Widget*> parent /* = { } */)
    : GenericContainer(theme, parent)
  {
    SetSize({ 0, m_ItemPadding.y });
  }

  ContextMenu::~ContextMenu()
  {

  }

  ContextMenu::shared_ptr ContextMenu::Create(optional<Theme*> theme /* = { } */, optional<Widget*> parent /* = { } */)
  {
    return std::make_shared<ContextMenu>(theme, parent);
  }

  bool ContextMenu::HandleEvent(const sf::Event &event)
  {
    if (!m_IsOpen)
      return false;

    if (!super::HandleEvent(event) && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
      Close();
      return true;
    }

    return false;
  }

  void ContextMenu::Update()
  {
    if (!m_IsOpen)
      return;

    super::Update();
  }

  void ContextMenu::Render(sf::RenderTarget &Target)
  {
    if (!m_IsOpen)
      return;

    Widget::Render(Target);
    super::Render(Target, {});
  }

  void ContextMenu::SetPosition(const Vec2i &Position)
  {
    auto delta = Position - m_Position;
    super::SetPosition(Position);

    for (auto & wptr : m_Widgets)
      wptr->Move(delta);
  }

  void ContextMenu::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);

    Vec2i wSize;
    for (auto & wptr : m_Widgets) {
      wSize = wptr->GetSize();
      wptr->SetSize({ m_Size.x - 2 * m_ItemPadding.x, wSize.y });
    }
  }

  void ContextMenu::Move(const Vec2i &Delta)
  {
    super::Move(Delta);
  }

  void ContextMenu::Add(Widget::shared_ptr widget)
  {
    Vec2f nPos = m_Position;
    Vec2f nSize = m_Size;

    auto wBds = widget->Bounds();
    auto wSize = widget->GetSize();
    auto mBds = Bounds();

    Vec2i wPos = m_Position;
    wPos.x += m_ItemPadding.x;
    wPos.y += nSize.y;

    nSize.y += wSize.y + m_ItemSpacing;
    nSize.x = std::max(nSize.x, cast_float(wSize.x + 2 * m_ItemPadding.x));

    widget->SetPosition(wPos);
    SetSize(static_cast< Vec2i >( nSize ));
    super::Add(widget);
  }

  void ContextMenu::AddMenuItem(const std::string &Text)
  {
    auto lItem = m_Theme->MakeLineItem(this);
    lItem->SetText(Text);
    lItem->SetTextAlignment(TextAlignment::Left);
    lItem->OnSelected([this, lp = lItem.get()](auto txt){ Close(); lp->Deselect(); });

    auto tSize = lItem->GetSize();
    if (tSize.x < m_Size.x)
      lItem->SetSize({ m_Size.x, tSize.y });

    Add(lItem);
  }

  void ContextMenu::Open()
  {
    m_IsOpen = true;
    std::cout << "Context menu opened\n";
    SetPosition(currentMousePosition);
    StealMouseFocus(this);
    StealKeyboardFocus(this);
    Opened();
    m_OpenedSignal();
  }

  void ContextMenu::Close()
  {
    m_IsOpen = false;
    std::cout << "Context menu closed\n";
    ReturnMouseFocus(this);
    ReturnKeyboardFocus(this);
    Closed();
    m_ClosedSignal();
  }

  void ContextMenu::OnOpened(boost::function<void()> func)
  {
    m_OpenedSignal.connect(func);
  }

  void ContextMenu::OnClosed(boost::function<void()> func)
  {
    m_ClosedSignal.connect(func);
  }

  void ContextMenu::Opened()
  {

  }

  void ContextMenu::Closed()
  {

  }

}
