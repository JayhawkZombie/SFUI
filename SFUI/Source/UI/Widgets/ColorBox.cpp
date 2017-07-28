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
#include <SFUI/Include/UI/Widgets/ColorBox.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  ColorBox::ColorBox(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : Widget(theme, parent)
  {
    m_Selection.setRadius(5);
    m_Selection.setFillColor(sf::Color::Transparent);
    m_Selection.setOutlineThickness(2);
    m_Selection.setOutlineColor(sf::Color::Cyan);
    m_Selection.setOrigin({ 2.5f, 2.5f });

    m_Corners[0].color = sf::Color::White; m_Corners[0].position = { 0.f, 0.f };
    m_Corners[1].color = sf::Color::White; m_Corners[1].position = { 0.f, 0.f };
    m_Corners[2].color = sf::Color::Black; m_Corners[2].position = { 0.f, 0.f };
    m_Corners[3].color = sf::Color::Black; m_Corners[3].position = { 0.f, 0.f };

    m_Selection.setPosition({ 0.f, 0.f });

    m_BackgroundRect.setFillColor(sf::Color::Black);
    m_CurrentColor = sf::Color::Transparent;
  }

  ColorBox::~ColorBox()
  {

  }

  sfui::ColorBox::shared_ptr ColorBox::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<ColorBox>(theme, parent);
  }

  bool ColorBox::HandleEvent(const sf::Event &event)
  {
    return Widget::HandleEvent(event);
  }

  void ColorBox::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_BackgroundRect);
    Target.draw(m_Corners);

    if (m_CurrentColor != sf::Color::Transparent)
      Target.draw(m_Selection);
  }

  void ColorBox::SetPosition(const Vec2i &Position)
  {
    auto delta = Position - m_Position;
    super::SetPosition(Position);
    m_Selection.move(delta);
    m_Corners[0].position = Position;
    m_Corners[1].position.x = cast_float(m_Position.x + m_Size.x); m_Corners[1].position.y = cast_float(m_Position.y);
    m_Corners[2].position.x = cast_float(m_Position.x); m_Corners[2].position.y = cast_float(m_Position.y + m_Size.y);
    m_Corners[3].position.x = m_Corners[1].position.x; m_Corners[3].position.y = m_Corners[2].position.y;
  }

  void ColorBox::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);

    m_Corners[1].position.x = cast_float(m_Position.x + m_Size.x); m_Corners[1].position.y = cast_float(m_Position.y);
    m_Corners[2].position.x = cast_float(m_Position.x); m_Corners[2].position.y = cast_float(m_Position.y + m_Size.y);
    m_Corners[3].position.x = m_Corners[1].position.x; m_Corners[3].position.y = m_Corners[2].position.y;
  }

  void ColorBox::SetColorAt(uint32 index, const Color &c)
  {
    if (index > 3)
      throw std::out_of_range("vertex index out of range");

    m_Corners[index].color = c;
    CalculateColor();
    ColorChanged();
    m_ColorChangedSignal(m_CurrentColor);
  }

  Color ColorBox::GetColor() const
  {
    return m_CurrentColor;
  }

  void ColorBox::OnColorChanged(boost::function<void(const Color &)> func)
  {
    m_ColorChangedSignal.connect(func);
  }

  void ColorBox::ColorChanged()
  {

  }

  void ColorBox::CalculateColor()
  {
    //Get the interpolated color L -> R (fully saturated color)
    float a = ( m_Selection.getPosition().x - m_Position.x ) / ( static_cast< float >( m_Size.x ) );

    float r = m_Corners[0].color.r + a * ( m_Corners[1].color.r - m_Corners[0].color.r );
    float g = m_Corners[0].color.g + a * ( m_Corners[1].color.g - m_Corners[0].color.g );
    float b = m_Corners[0].color.b + a * ( m_Corners[1].color.b - m_Corners[0].color.b );
    sf::Color P = sf::Color(static_cast< sf::Uint8 >( floor(r) ), static_cast< sf::Uint8 >( floor(g) ), static_cast< sf::Uint8 >( floor(b) ));

    //Now interpolate vertically
    float _b = ( m_Selection.getPosition().y - m_Position.y ) / ( static_cast< float >( m_Size.y ) );
    P.r = static_cast< sf::Uint8 >( floor(P.r * ( 1 - _b )) );
    P.g = static_cast< sf::Uint8 >( floor(P.g * ( 1 - _b )) );
    P.b = static_cast< sf::Uint8 >( floor(P.b * ( 1 - _b )) );
    m_CurrentColor = P;
  }

  void ColorBox::MousePressed(bool left, bool right)
  {
    if (left) {
      m_Selection.setPosition(currentMousePosition);
      CalculateColor();
      ColorChanged();
      m_ColorChangedSignal(m_CurrentColor);
    }
  }

  void ColorBox::MouseMoved()
  {
    if (IsLeftMouseDown()) {
      m_Selection.setPosition(currentMousePosition);
      CalculateColor();
      ColorChanged();
      m_ColorChangedSignal(m_CurrentColor);
    }
  }

}  
