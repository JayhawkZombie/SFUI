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
#include <SFUI/Include/UI/Widgets/Popup.h>
#include <SFUI/Include/UI/Theme.h>
#include <SFUI/Include/UI/Widgets/WidgetWindow.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  Popup::Popup(WidgetWindow *TopWindow, optional<Theme*> theme /*= optional<Theme*>()*/)
    : Widget(theme, nullptr), m_StealFocusFrom(TopWindow)
  {
    m_Panel = m_Theme->MakePanel(this);

    dropShadowTexture.loadFromFile("../Images/DropShadowFull2.png");
    DropShadowRect.setTexture(&dropShadowTexture);
  }

  Popup::~Popup()
  {

  }

  sfui::Popup::shared_ptr Popup::Create(WidgetWindow *TopWindow, optional<Theme*> theme /*= optional<Theme*>()*/)
  {
    return std::make_shared<Popup>(TopWindow, theme);
  }

  void Popup::Add(Widget::shared_ptr widget)
  {
    m_Panel->Add(widget);
  }

  void Popup::Open()
  {
    m_StealFocusFrom->TakeGlobalMouseFocus(this);
    m_StealFocusFrom->TakeGlobalKeyboardFocus(this);
    m_Parent = m_StealFocusFrom;
    m_IsOpen = true;
  }

  void Popup::Close()
  {
    m_StealFocusFrom->ReleaseGlobalMouseFocus();
    m_StealFocusFrom->ReleaseGlobalKeyboardFocus();
    m_IsOpen = false;
  }

  void Popup::InitDropShadow()
  {
    //m_DropShadow.SetImageSize(Vec2i(floor(1.25f * m_Size.x), floor(1.25f * m_Size.y)));
    m_DropShadow.SetShadowSize(1.025f * m_Size);
    m_DropShadow.SetShadowPosition(m_Position - 0.0125f * m_Size);
    m_DropShadow.SetRadius(50);

    m_DropShadow.Init();
    m_DropShadow.DrawShadowSource(m_Size);
  }

  void Popup::Update()
  {
    m_Panel->BaseUpdate();
  }

  void Popup::Render(sf::RenderTarget &Target)
  {
    //Target.draw(DropShadowRect);
    m_DropShadow.Render(Target);
    Target.draw(m_BackgroundRect);
    m_Panel->Render(Target);
  }

  void Popup::SetStealsFocus(bool steals)
  {
    m_StealFocus = steals;
  }

  void Popup::SetPosition(const Vec2i &Position)
  {
    super::SetPosition(Position);
    m_Panel->SetPosition(Position);
    //Drop shadow image is 800x800, with the empty space at
    // (100, 100) and 600x600
    Vec2f dropShadowPos;
    dropShadowPos.x = cast_float(m_Position.x) - 0.125f * cast_float(m_Size.x);
    dropShadowPos.y = cast_float(m_Position.y) - 0.125f * cast_float(m_Size.y);
    DropShadowRect.setPosition(dropShadowPos);
  }

  void Popup::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);
    m_Panel->SetSize(Size);
    Vec2f dropShadowPos;
    dropShadowPos.x = cast_float(m_Position.x) - 0.125f * cast_float(m_Size.x);
    dropShadowPos.y = cast_float(m_Position.y) - 0.125f * cast_float(m_Size.y);
    DropShadowRect.setSize(1.25f * Vec2f(Size));
  }

  bool Popup::HandleEvent(const sf::Event &event)
  {
    if (!m_IsOpen) return false;

    m_Panel->HandleEvent(event);
    return true;
  }

    /*void Popup::MouseMoved()
  {
    m_Panel->HandleMouseMoved();
  }

  void Popup::MouseEntered()
  {
    m_Panel->HandleMouseMovedOver();
  }

  void Popup::MouseLeft()
  {
    m_Panel->HandleMouseMovedOff();
  }

  void Popup::MousePressed(bool left, bool right)
  {
    if (left) m_Panel->HandleLeftMousePressed();
    if (right) m_Panel->HandleRightMousePressed();
  }

  void Popup::MouseReleased(bool left, bool right)
  {
    if (left) m_Panel->HandleLeftMouseReleased();
    if (right) m_Panel->HandleRightMouseReleased();
  }*/

}  
