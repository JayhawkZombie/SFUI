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
#include <SFUI/Include/UI/Widgets/ChildWindow.h>
#include <SFUI/Include/UI/Theme.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{

  ChildWindow::ChildWindow(optional<Theme*> theme /* = { } */, optional<Widget*> parent /* = { } */)
    : GenericContainer(theme, parent)
  {
    m_CloseButton = m_Theme->MakeIconButton(m_Theme->IconTexture, IntRect(259, 106, 42, 42), this);
    m_CloseButton->SetSize({ 20, 20 });
    //m_Title = m_Theme->MakeBitmapLabel(this
    m_Title = TextView::Create(this, "ChildWindow", sf::Color(198, 198, 198), 12., m_Theme->DefaultFont);

    auto c = m_BGColor;
    c.r -= 15; c.g -= 15; c.b -= 15;
    
    m_CloseButton->OnClicked([this]() { Close(); });
    m_Highlight = Widget::Highlight::Top;
    m_HighlightColor = sf::Color(0, 204, 204);

    m_DragBar = m_Theme->MakeDraggableFree(this);
    m_DragBar->OnDragged([this](auto v) { Move(v); });
  }

  ChildWindow::~ChildWindow()
  {

  }

  ChildWindow::shared_ptr ChildWindow::Create(optional<Theme*> theme /* = { } */, optional<Widget*> parent /* = { } */)
  {
    return std::make_shared<ChildWindow>(theme, parent);
  }

  bool ChildWindow::HandleEvent(const sf::Event &event)
  {
    if (!m_IsOpen)
      return false;

    if (m_CloseButton->HandleEvent(event))
      return true;

    m_DragBar->HandleEvent(event);

    return super::HandleEvent(event);
  }

  void ChildWindow::Update()
  {
    if (!m_IsOpen)
      return;

    super::Update();
    //m_Title->BaseUpdate();
    m_CloseButton->BaseUpdate();
    m_DragBar->BaseUpdate();
  }

  void ChildWindow::Render(sf::RenderTarget &Target)
  {
    if (!m_IsOpen)
      return;

    Widget::Render(Target);
    super::Render(Target, {});
    m_Title->Render(Target, {});
    if (m_CanUserClose)
      m_CloseButton->Render(Target);
    Target.draw(m_HighlightRect);
    m_DragBar->Render(Target);
  }

  void ChildWindow::SetPosition(const Vec2i &Position)
  {
    super::SetPosition(Position);
    m_Title->SetPosition({ m_Position.x + 5, m_Position.y + 18 });
    Vec2i hPos = { m_Position.x, m_Position.y + 22 };
    m_CloseButton->SetPosition({ m_Position.x + m_Size.x - 21, m_Position.y + 1 });
    SetHighlight(Widget::Highlight::Top, m_HighlightColor, 2);
    m_HighlightRect.setPosition({ ( float )m_Position.x, ( float )m_Position.y + 22.f });
    m_DragBar->SetPosition(Position);
  }

  void ChildWindow::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);
    m_CloseButton->SetSize({ 20, 20 });
    m_CloseButton->SetPosition({ m_Position.x + m_Size.x - 21, m_Position.y + 1 });
    m_Title->SetPosition({ m_Position.x + 5, m_Position.y + 18 });
    m_Title->SetSize({ m_Size.x, 20 });
    Vec2i hSize = { m_Size.x, 2 };
    SetHighlight(Widget::Highlight::Top, m_HighlightColor, 2);
    m_HighlightRect.setPosition({ ( float )m_Position.x, ( float )m_Position.y + 22.f });
    m_DragBar->SetSize({ m_Size.x, 20 });
  }

  void ChildWindow::Move(const Vec2i &Delta)
  {
    super::Move(Delta);
    m_CloseButton->Move(Delta);
    m_Title->Move(Delta);
    m_HighlightRect.move(Delta);
  }

  void ChildWindow::Open()
  {
    StealMouseFocus(this);
    StealKeyboardFocus(this);
    m_IsOpen = true;

    Opened();
    m_OpenedSignal();
  }

  void ChildWindow::Close()
  {
    if (m_CanUserClose) {
      m_IsOpen = false;
      ReturnMouseFocus(this);
      ReturnKeyboardFocus(this);
      Closed();
      m_ClosedSignal();
    }
  }

  void ChildWindow::Dismiss()
  {
    ReturnMouseFocus(this);
    ReturnKeyboardFocus(this);
    m_IsOpen = false;
    Dismissed();
    m_DisabledSignal();
  }

  bool ChildWindow::CanUserClose() const
  {
    return m_CanUserClose;
  }

  void ChildWindow::SetUserCanClose(bool CanClose)
  {
    m_CanUserClose = CanClose;
  }

  bool ChildWindow::IsOpen() const
  {
    return m_IsOpen;
  }

  const sstring& ChildWindow::GetTitle() const
  {
    return m_Title->GetText();
  }

  void ChildWindow::SetTitle(const sstring &Title)
  {
    m_Title->SetText(Title);
    m_Title->SetPosition({ m_Position.x + 5, m_Position.y + m_Title->GetSize().y });
  }

  void ChildWindow::OnOpened(boost::function<void()> func)
  {
    m_OpenedSignal.connect(func);
  }

  void ChildWindow::OnClosed(boost::function<void()> func)
  {
    m_ClosedSignal.connect(func);
  }

  void ChildWindow::OnDismissed(boost::function<void()> func)
  {
    m_DismissedSignal.connect(func);
  }

  void ChildWindow::Opened()
  {

  }

  void ChildWindow::Closed()
  {

  }

  void ChildWindow::Dismissed()
  {

  }

}
