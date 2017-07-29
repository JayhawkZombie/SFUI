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
#include <SFUI/Include/UI/Widgets/Button.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////
#include <algorithm>

namespace sfui
{  
  
  Button::Button(optional<Theme*> theme, optional<Widget::pointer> parent, uint32 events)
    : Widget(theme, parent, events)
  {

  }

  sfui::Button::shared_ptr Button::Create(optional<Theme*> theme, optional<Widget::pointer> parent, uint32 events)
  {
    return std::make_shared<Button>(theme, parent, events);
  }

  Button::shared_ptr Button::CreateIcon(texture_handle tex, IntRect texRect, optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget::pointer> parent /*= optional<Widget*>()*/, uint32 events /*= Event::Default*/)
  {
    auto ptr = std::make_shared<Button>(theme, parent, events);
    ptr->m_IconTexture = tex;
    ptr->m_IconRect.setTexture(tex.get());
    ptr->m_IconRect.setTextureRect(texRect);
    return ptr;
  }

  bool Button::IsPressed() const
  {
    return ( m_IsMouseOver && m_IsLeftMouseDown );
  }

  void Button::OnClicked(boost::function<void()> func)
  {
    m_ClickedSignal.connect(func);
  }

  void Button::Update()
  {
    super::Update();
  }

  void Button::Render(sf::RenderTarget &Target)
  {
    super::Render(Target);

    if (m_IconTexture) {
      Target.draw(m_IconRect);
    }
    else {
      if (m_TextView) {
        ( *m_TextView )->Render(Target, { });
      }
    }
  }

  void Button::SetText(const std::string &Text)
  {
    Widget::SetText(Text);

    auto tSize = GetTextSize();
    tSize.x = cast_int(floor(tSize.x * 1.2f));

    //SetSize(tSize);
  }

  void Button::MouseMoved()
  {
    super::MouseMoved();
  }

  void Button::MouseEntered()
  {
    super::MouseEntered();
    if (m_CanAnimateExpand) 
      m_Animator.Animate(WidgetAnimation::Expand, m_Size, m_ExpandSize, Easing::EaseInOut, 150);

    if (m_IsLeftMouseDown) {
      Button::MousePressed(true, m_IsRightMouseDown);
    }
    else {
      Color bright = m_BGColor + sf::Color(m_BrightFactor, m_BrightFactor, m_BrightFactor, 0);
      std::clamp(bright.r, 0_ui8, 255_ui8);
      std::clamp(bright.g, 0_ui8, 255_ui8);
      std::clamp(bright.b, 0_ui8, 255_ui8);
      std::clamp(bright.a, 0_ui8, 255_ui8);
      m_BackgroundRect.setFillColor(bright);
      Hovered();
    }
  }

  void Button::MouseLeft()
  {
    super::MouseLeft();
    if (m_CanAnimateContract)
      m_Animator.Animate(WidgetAnimation::Expand, m_Size, m_ContractSize, Easing::EaseInOut, 150);

    m_BackgroundRect.setFillColor(m_BGColor);
    Unhovered();
  }

  void Button::MousePressed(bool left, bool right)
  {
    super::MousePressed(left, right);

    if (left) {
      Color dark = m_BGColor - sf::Color(m_DarkFactor, m_DarkFactor, m_DarkFactor, 0);
      std::clamp(dark.r, 0_ui8, 255_ui8);
      std::clamp(dark.g, 0_ui8, 255_ui8);
      std::clamp(dark.b, 0_ui8, 255_ui8);
      std::clamp(dark.a, 0_ui8, 255_ui8);
      m_BackgroundRect.setFillColor(dark);

      Pressed();
    }
  }

  void Button::MouseReleased(bool left, bool right)
  {
    super::MouseReleased(left, right);

    if (left) {
      Color bright = m_BGColor + sf::Color(m_BrightFactor, m_BrightFactor, m_BrightFactor, 0);
      std::clamp(bright.r, 0_ui8, 255_ui8);
      std::clamp(bright.g, 0_ui8, 255_ui8);
      std::clamp(bright.b, 0_ui8, 255_ui8);
      std::clamp(bright.a, 0_ui8, 255_ui8);
      m_BackgroundRect.setFillColor(bright);
      Clicked();
      m_ClickedSignal();
    }
  }

  void Button::Resized()
  {
    
    m_IconRect.setSize(m_Size);
    ( *m_TextView )->SetPosition(m_Position);
    m_TextView.value()->RealignText();

    //auto tSize = GetTextSize();
    //tSize.x = cast_int(floor(tSize.x * 1.2f));
    //
    //m_Size = tSize;
    //m_Bounds.width = m_Size.x;
    //m_Bounds.height = m_Size.y;
    //m_BackgroundRect.setSize(m_Size);
  }

  void Button::SetDefaultSize(const Vec2i &Size)
  {
    m_ExpandSize = Size + Vec2i(5, 5);
    m_ContractSize = Size;
    SetSize(Size);
  }

  void Button::Hovered()
  {
    
  }

  void Button::Unhovered()
  {

  }

  void Button::Moved()
  {
    m_IconRect.setPosition(m_Position);
    ( *m_TextView )->SetPosition(m_Position);

    //auto tSize = GetTextSize();
    //tSize.x = cast_int(floor(tSize.x * 1.2f));

    //SetSize(tSize);
  }

  void Button::Pressed()
  {

  }

  void Button::Released()
  {

  }

  void Button::Clicked()
  {
    
  }

}  
