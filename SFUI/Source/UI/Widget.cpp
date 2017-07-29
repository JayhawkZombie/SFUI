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
#include <SFUI/Include/UI/Widget.h>
#include <SFUI/Include/UI/Theme.h>
#include <SFUI/Include/UI/Widgets/PopupWindow.h>
#include <SFUI/Include/UI/Widgets/WidgetWindow.h>
#include <SFUI/Include/UI/Widgets/BitmapLabel.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  

  Widget::Widget(optional<Theme*> theme, optional<pointer> parent /*= nullptr*/, uint64 events /*= Event::Default*/)
    : m_Parent(parent), m_Theme(theme.value_or(nullptr))
    , m_TextView(TextView::Create(this, "", WIDGET_STYLE_COLOR_LIGHT_GREY, Body2, nullptr))
    , m_Animator(this)
  {
    if (( events & Event::MouseMove ) != 0) { m_AcceptsMouseMove = true; }
    if (( events & Event::MouseExit ) != 0) { m_AcceptsMouseExit = true; }
    if (( events & Event::MouseOver ) != 0) { m_AcceptsMouseOver = true; }
    if (( events & Event::MousePress ) != 0) { m_AcceptsMousePress = true; }
    if (( events & Event::MouseRelease ) != 0) { m_AcceptsMouseRelease = true; }
    if (( events & Event::MouseClick ) != 0) { m_AcceptsMouseClick = true; }
    if (( events & Event::MouseDoubleClick ) != 0) { m_AcceptsMouseDoubleClick = true; }
    if (( events & Event::KeyPress ) != 0) { m_AcceptsKeyPress = true; }
    if (( events & Event::KeyRelease ) != 0) { m_AcceptsKeyRelease = true; }
    if (( events & Event::TextEntered ) != 0) { m_AcceptsTextEntered = true; }
  }

  Widget::~Widget()
  {
    if (m_TextView) m_TextView.reset();
  }

  bool Widget::IsKeyDown(Key key) const
  {
    return sf::Keyboard::isKeyPressed(key);
  }

  bool Widget::AreKeysDown(const std::vector<Key> &keys) const
  {
    return
      std::all_of(keys.begin(), keys.end(),
                  [ ](Key k) { return sf::Keyboard::isKeyPressed(k); });
  }

  bool Widget::IsLeftMouseDown() const
  {
    return sf::Mouse::isButtonPressed(sf::Mouse::Left);
  }

  bool Widget::IsRightMouseDown() const
  {
    return sf::Mouse::isButtonPressed(sf::Mouse::Right);
  }

  void Widget::SetPosition(const Vec2i &Position)
  {
    m_Position = Position;
    m_BackgroundRect.setPosition(m_Position);
    m_Bounds.left = cast_float(m_Position.x);
    m_Bounds.top = cast_float(m_Position.y);
    if (m_TextView) ( *m_TextView )->SetPosition(m_Position);
    if (m_Label) {
      auto lSize = m_Label->GetSize();
      m_Label->SetPosition({ m_Position.x - lSize.x - 10, m_Position.y + m_Size.y - 3 });
    }
    Moved();
  }

  void Widget::SetSize(const Vec2i &Size)
  {
    m_Size = Size;
    m_BackgroundRect.setSize(m_Size);
    m_Bounds.width = cast_float(m_Size.x);
    m_Bounds.height = cast_float(m_Size.y);
    if (m_TextView) ( *m_TextView )->SetPosition(m_Position);
    if (m_Label) {
      auto lSize = m_Label->GetSize();
      m_Label->SetPosition({ m_Position.x - lSize.x - 10, m_Position.y + m_Size.y - 3 });
    }
    Resized();
  }

  void Widget::SetDefaultSize(const Vec2i &Size)
  {
    Widget::SetSize(Size);
    m_ContractSize = Size;
  }

  void Widget::SetExpandSize(const Vec2i &Size)
  {
    m_ExpandSize = Size;
  }

  void Widget::Move(const Vec2i &Delta)
  {
    m_BackgroundRect.move(Delta);
    m_Bounds.left += cast_float(Delta.x);
    m_Bounds.top += cast_float(Delta.y);
    m_Position += Delta;
    if (m_TextView) ( *m_TextView )->SetPosition(m_Position);
    if (m_Label) m_Label->Move(Delta);
    Moved();
  }

  void Widget::SetParent(pointer parent)
  {
    m_Parent = parent;
  }

  Vec2i Widget::GetPosition() const
  {
    return m_Position;
  }

  Vec2i Widget::GetSize() const
  {
    return m_Size;
  }

  void Widget::SetText(const std::string &Text)
  {
    if (m_TextView)
      ( *m_TextView )->SetText(Text);
  }

  void Widget::SetTextColor(const Color &c)
  {
    if (m_TextView)
      ( *m_TextView )->SetColor(c);
  }

  void Widget::SetTextSize(uint32 Size)
  {
    if (m_TextView)
      ( *m_TextView )->SetTextSize(Size);
  }

  void Widget::SetTextAlignment(TextAlignment alignment)
  {
    if (m_TextView) ( *m_TextView )->SetAlignment(alignment);
  }

  void Widget::SetFont(font_handle HFont)
  {
    if (m_TextView)
      ( *m_TextView )->SetFont(HFont);
  }

  void Widget::SetBitmapFont(std::shared_ptr<sw::BitmapFont> fnt)
  {
    if (m_TextView.has_value())
      m_TextView.value()->SetBMFont(fnt);
  }

  std::string Widget::GetText() const
  {
    if (m_TextView)
      return ( *m_TextView )->GetText();
    return "";
  }

  Vec2i Widget::GetTextPosition() const
  {
    if (m_TextView) {
      return ( *m_TextView )->GetPosition();
    }
    return Vec2i(0, 0);
  }

  Vec2i Widget::GetTextSize() const
  {
    if (m_TextView)
      return ( *m_TextView )->GetSize();
    return Vec2i(0, 0);
  }

  sf::View Widget::View() const
  {
    return {};
  }

  bool Widget::IsModal() const
  {
    return m_IsModal;
  }

  bool Widget::HasMouseFocus() const
  {
    return m_HasMouseFocus;
  }

  bool Widget::HasKeyboardFocus() const
  {
    return m_HasKeyboardFocus;
  }

  bool Widget::HasParent() const
  {
    return ( m_Parent.has_value() );
  }

  optional<Widget::pointer> Widget::GetParent() const
  {
    return m_Parent;
  }

  void Widget::TakeMouseFocus()
  {
    if (m_Parent)
      (*m_Parent)->ChildTakingMouseFocus(this);
    m_HasMouseFocus = true;
  }

  void Widget::TakeGlobalMouseFocus(Widget *widget)
  {
    if (m_TopWindow) {
      m_TopWindow->TakeGlobalMouseFocus(widget);
    }
    else if (m_Parent.value() != nullptr)
      ( *m_Parent )->TakeGlobalMouseFocus(widget);
    //else if (m_TopWindow)
    //  m_TopWindow->GiveMouseFocus(widget);
  }

  void Widget::ReleaseMouseFocus()
  {
    if (m_Parent)
      (*m_Parent)->ChildReleasingMouseFocus(this);
    m_HasMouseFocus = false;
  }

  void Widget::ReleaseGlobalMouseFocus()
  {
    if (m_TopWindow)
      m_TopWindow->RelinquishMouseFocus();
    else if (m_Parent.value() != nullptr)
      ( *m_Parent )->ReleaseGlobalMouseFocus();
  }

  void Widget::TakeKeyboardFocus()
  {
    if (m_Parent)
      (*m_Parent)->ChildTakingKeyboardFocus(this);
    m_HasKeyboardFocus = true;
  }

  void Widget::TakeGlobalKeyboardFocus(Widget *widget)
  {
    GetTopWindow()->GiveKeyboardFocus(widget);
  }

  void Widget::ReleaseKeyboardFocus()
  {
    if (m_Parent)
      (*m_Parent)->ChildReleasingKeyboardFocus(this);
    m_HasKeyboardFocus = false;
  }

  void Widget::ReleaseGlobalKeyboardFocus()
  {
    GetTopWindow()->RelinquishKeyboardFocus();
  }

  void Widget::SetChildWithMouseFocus(pointer child)
  {
    m_ChildWithMouseFocus = child;
  }

  void Widget::SetChildWithKeyboardFocus(pointer child)
  {
    m_ChildWithKeyboardFocus = child;
  }

  optional<Widget::pointer> Widget::GetChildWithMouseFocus() const
  {
    return m_ChildWithMouseFocus;
  }

  optional<Widget::pointer> Widget::GetChildWithKeyboardFocus() const
  {
    return m_ChildWithKeyboardFocus;
  }

  void Widget::Enable()
  {
    m_IsEnabled = true;
    Enabled();
  }

  void Widget::Disable()
  {
    m_BackgroundRect.setFillColor(sf::Color(58, 58, 58));
    ( *m_TextView )->SetColor(sf::Color(102, 102, 102, 100));
    m_IsEnabled = false;
    Disabled();
  }

  void Widget::Show()
  {
    m_IsVisible = true;
    Shown();
  }

  void Widget::Hide()
  {
    m_IsVisible = false;
    Hidden();
  }

  bool Widget::IsEnabled() const
  {
    return m_IsEnabled;
  }

  bool Widget::IsVisible() const
  {
    return m_IsVisible;
  }

  bool Widget::IsMouseOver() const
  {
    return m_IsMouseOver;
  }

  void Widget::SetHoverDelay(ms delay)
  {
    m_HoverDelay = delay;
  }

  ms Widget::GetHoverDelay() const
  {
    return m_HoverDelay;
  }

  void Widget::SetTooltip(shared_ptr widget)
  {
    m_Tooltip = widget;
    widget->SetParent(this);
  }

  optional<Widget::shared_ptr> Widget::GetTooltip() const
  {
    return m_Tooltip;
  }

  void Widget::RemoveTooltip()
  {
    m_Tooltip.reset();
  }

  FloatRect Widget::Bounds() const
  {
    return m_Bounds;
  }

  void Widget::SetBackgroundColor(const Color &c)
  {
    m_BGColor = c;
    m_BackgroundRect.setFillColor(c);
  }

  void Widget::SetBorderColor(const Color &c)
  {
    m_BackgroundRect.setOutlineColor(c);
  }

  void Widget::SetBorderWidth(int32 Width)
  {
    m_BackgroundRect.setOutlineThickness(cast_float(Width));
  }

  Color Widget::GetBackgroundColor() const
  {
    return m_BGColor;
  }

  void Widget::MoveToFront()
  {
    if (m_Parent)
      (*m_Parent)->MoveChildToFront(this);
  }

  void Widget::MoveToBack()
  {
    if (m_Parent)
      (*m_Parent)->MoveChildToBack(this);
  }

  void Widget::OnMouseOver(boost::function<void()> func)
  {
    m_MouseOverSignal.connect(func);
  }

  void Widget::OnMouseExit(boost::function<void()> func)
  {
    m_MouseExitSignal.connect(func);
  }

  void Widget::OnLeftMousePress(boost::function<void()> func)
  {
    m_LeftMousePressedSignal.connect(func);
  }

  void Widget::OnRightMousePress(boost::function<void()> func)
  {
    m_RightMousePressedSignal.connect(func);
  }

  void Widget::OnLeftMouseRelease(boost::function<void()> func)
  {
    m_LeftMouseReleasedSignal.connect(func);
  }

  void Widget::OnRightMouseRelease(boost::function<void()> func)
  {
    m_RightMouseReleasedSignal.connect(func);
  }

  void Widget::OnKeyPress(boost::function<void(Key)> func)
  {
    m_KeyPressedSignal.connect(func);
  }

  void Widget::OnKeyRelease(boost::function<void(Key)> func)
  {
    m_KeyReleasedSignal.connect(func);
  }

  void Widget::OnMouseFocusGained(boost::function<void()> func)
  {
    m_MouseFocusGainedSignal.connect(func);
  }

  void Widget::OnMouseFocusLost(boost::function<void()> func)
  {
    m_MouseFocusLostSignal.connect(func);
  }

  void Widget::OnMouseHover(boost::function<void()> func)
  {
    m_MouseHoverSignal.connect(func);
  }

  void Widget::Cleanup()
  {

  }

  void Widget::Update()
  {

  }

  void Widget::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_BackgroundRect);
  }

  Vec2i Widget::currentMousePosition;
  Vec2i Widget::previousMousePositon;

  void Widget::BaseUpdate()
  {
    m_LastUpdateTick = m_ThisUpdateTick;
    m_ThisUpdateTick = system_clock::now();

    m_SinceLastUpdate = ch::duration_cast< ms >( m_ThisUpdateTick - m_LastUpdateTick );

    if (m_IsMouseOver && !m_Bounds.contains(currentMousePosition)) {
      m_IsMouseOver = false;
      MouseLeft();
    }
      //HandleMouseMovedOff();
    m_Animator.Update();
    Update();
  }

  void Widget::ChildTakingMouseFocus(pointer child)
  {

  }

  void Widget::ChildTakingKeyboardFocus(pointer child)
  {

  }

  void Widget::ChildReleasingMouseFocus(pointer child)
  {

  }

  void Widget::ChildReleasingKeyboardFocus(pointer child)
  {

  }

  bool Widget::HandleEvent(const sf::Event &event)
  {
    if (!IsEnabled() || !IsVisible())
      return false;

    if (!m_Bounds.contains(currentMousePosition)) return false;
    switch (event.type)
    {
      case sf::Event::MouseMoved:
      {
        if (!m_IsMouseOver) {
          m_IsMouseOver = true;
          MouseEntered();
        }
        else {
          m_IsMouseOver = true;
          MouseMoved();
        }
        return true;
      }

      case sf::Event::MouseButtonPressed:
      {
        if (event.mouseButton.button == sf::Mouse::Left)
          MousePressed(true, false);
          //HandleLeftMousePressed();
        else
          MousePressed(false, true);
          //HandleRightMousePressed();
        return true;
      }

      case sf::Event::MouseButtonReleased:
      {
        if (event.mouseButton.button == sf::Mouse::Left)
          MouseReleased(true, false);
          //HandleLeftMouseReleased();
        else
          MouseReleased(false, true);
          //HandleRightMouseReleased();
        return true;
      }
    }

    return false;
  }

  void Widget::SetTopWindow(WidgetWindow *TopWindow)
  {
    m_TopWindow = TopWindow;
  }

  sfui::WidgetWindow * Widget::GetTopWindow() const
  {
    if (m_Parent.value() != nullptr)
      return ( *m_Parent )->GetTopWindow();
    return m_TopWindow;
  }

  void Widget::StealMouseFocus(Widget *widget)
  {
    if (m_Parent.value() != nullptr)
      m_Parent.value()->StealMouseFocus(widget);
  }

  void Widget::ReturnMouseFocus(Widget *widget)
  {
      if (m_Parent.value() != nullptr)
        m_Parent.value()->ReturnMouseFocus(widget);
  }

  void Widget::StealKeyboardFocus(Widget *widget)
  {
    if (m_Parent.value() != nullptr)
      m_Parent.value()->StealKeyboardFocus(widget);
  }

  void Widget::ReturnKeyboardFocus(Widget *widget)
  {
    if (m_Parent.value() != nullptr)
      m_Parent.value()->ReturnKeyboardFocus(widget);
  }

  void Widget::SetCanAnimateExpand(bool can)
  {
    m_CanAnimateExpand = can;
  }

  void Widget::SetCanAnimateContract(bool can)
  {
    m_CanAnimateContract = can;
  }

  void Widget::SetCanAnimateBounce(bool can)
  {
    m_CanAnimateBounce = can;
  }

  void Widget::SetCanAnimateSlide(bool can)
  {
    m_CanAnimateSlide = can;
  }

  void Widget::SetCanAnimateFade(bool can)
  {
    m_CanAnimateFade = can;
  }

  void Widget::MouseMoved()
  {
    m_IsMouseOver = true;
  }

  void Widget::MousePressed(bool left, bool right)
  {
    m_IsLeftMouseDown = left;
    m_IsRightMouseDown = right;
  }

  void Widget::MouseReleased(bool left, bool right)
  {
    m_IsLeftMouseDown = !left;
    m_IsRightMouseDown = !right;
  }

  void Widget::MouseEntered()
  {
    m_IsMouseOver = true;
  }

  void Widget::MouseLeft()
  {
    m_IsMouseOver = false;
  }

  void Widget::MouseClicked()
  {

  }

  void Widget::KeyPressed(Key key)
  {

  }

  void Widget::KeyReleased(Key key)
  {

  }

  void Widget::TextEntered(uint32 unicode)
  {

  }

  void Widget::Moved()
  {

  }

  void Widget::Resized()
  {

  }

  void Widget::GainedMouseFocus()
  {

  }

  void Widget::GainedKeyboardFocus()
  {

  }

  void Widget::LostMouseFocus()
  {

  }

  void Widget::LostKeyboardFocus()
  {

  }

  void Widget::Enabled()
  {

  }

  void Widget::Disabled()
  {

  }

  void Widget::Hidden()
  {

  }

  void Widget::Shown()
  {

  }

  void Widget::Hovered()
  {

  }

  void Widget::MoveChildToFront(pointer child)
  {

  }

  void Widget::MoveChildToBack(pointer child)
  {

  }

  void Widget::RenderLabel(sf::RenderTarget &Target)
  {
    if (m_HasLabel && m_Label)
      m_Label->Render(Target);
  }

  void Widget::SetLabel(const std::string &Text)
  {
    m_HasLabel = true;
    if (!m_Label) {
      m_Label = m_Theme->MakeBitmapLabel(this);
    }

    m_Label->SetBMText(Text);
    auto lSize = m_Label->GetSize();
    m_Label->SetPosition({ m_Position.x - lSize.x - 10, m_Position.y + m_Size.y - 3 });
    //else {
    //  auto oldLabelSize = m_Label->GetSize();
    //  m_Label->SetBMText(Text);

    //  auto lSize = m_Label->GetSize();
    //  auto xDiff = lSize.x - oldLabelSize.x;

    //  Move({ xDiff, 0 });
    //}
  }

}  
