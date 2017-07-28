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
#include <SFUI/Include/UI/Widgets/RainbowStrip.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  RainbowStrip::RainbowStrip(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : ValueSlider(theme, parent)
  {
    SetBarHeight(2);
    m_Slider->SetBackgroundColor(sf::Color::Black);
    m_Slider->SetBorderWidth(0);

    m_Strip[0].color = sf::Color(255, 0, 0); m_Strip[1].color = m_Strip[0].color;
    m_Strip[2].color = sf::Color(255, 0, 255); m_Strip[3].color = m_Strip[2].color;
    m_Strip[4].color = sf::Color(0, 0, 255); m_Strip[5].color = m_Strip[4].color;
    m_Strip[6].color = sf::Color(0, 255, 255); m_Strip[7].color = m_Strip[6].color;
    m_Strip[8].color = sf::Color(0, 255, 0); m_Strip[9].color = m_Strip[8].color;
    m_Strip[10].color = sf::Color(255, 255, 0); m_Strip[11].color = m_Strip[10].color;
    m_Strip[12].color = sf::Color(255, 0, 0); m_Strip[13].color = m_Strip[12].color;
  }

  RainbowStrip::~RainbowStrip()
  {

  }

  sfui::RainbowStrip::shared_ptr RainbowStrip::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<RainbowStrip>(theme, parent);
  }

  bool RainbowStrip::HandleEvent(const sf::Event &event)
  {
    return ValueSlider::HandleEvent(event);
  }

  sf::Color RainbowStrip::GetColor() const
  {
    return m_Color;
  }

  void RainbowStrip::SetPosition(const Vec2i &Position)
  {
    auto delta = Position - m_Position;
    super::SetPosition(Position);

    PositionStrip();
  }

  void RainbowStrip::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);
    SetBarWidth(Size.x);
    PositionStrip();
  }

  void RainbowStrip::Update()
  {
    ValueSlider::Update();
  }

  void RainbowStrip::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_Strip);
    m_Slider->Render(Target);
  }

  void RainbowStrip::Move(const Vec2i &Delta)
  {
    super::Move(Delta);

    for (std::size_t i = 0; i < 14; ++i) {
      m_Strip[i].position += sf::Vector2f(Delta);
    }
  }

  void RainbowStrip::SetTopWindow(WidgetWindow *TopWindow)
  {
    ValueSlider::SetTopWindow(TopWindow);
  }

  void RainbowStrip::OnColorChanged(boost::function<void(const sf::Color &)> func)
  {
    m_ColorChangedSignal.connect(func);
  }

  void RainbowStrip::PositionStrip()
  {
    float regionSize = m_Size.y / 6.f; //6 color "regions"

    float xLeft = static_cast< float >( m_Position.x );
    float xRight = static_cast< float >( m_Position.x + m_Size.y );

    Vec2f cPosLeft = m_Position; Vec2f cPosRight = { static_cast< float >( m_Position.x + m_Size.x ), static_cast< float >( m_Position.y ) };

    for (std::size_t i = 0; i < 7; ++i) {
      m_Strip[2 * i].position = cPosLeft;
      m_Strip[2 * i + 1].position = cPosRight;
      cPosLeft.y += regionSize; cPosRight.y += regionSize;
    }
  }

  void RainbowStrip::CalculateCurrentColor()
  {
    auto val = m_CurrentValue * 100.f;

    //Each region takes up 16.66666667% of the total area
    const float regPerc = 16.66666667f;

    size_t region = static_cast< size_t >( floor(val / regPerc) );

    if (region > 5) return; // ???? shouldn't ever happen, but just in case

    Color topColor = m_Strip[2 * region].color;
    Color btmColor = m_Strip[2 * region + 2].color;

    //Now how far between the top & bottom colors are we?
    float btwn = ( val - ( regPerc * region ) ) / regPerc;

    //Now interpolate the colors
    float r = static_cast< float >( topColor.r ) + btwn * ( btmColor.r - topColor.r );
    float g = static_cast< float >( topColor.g ) + btwn * ( btmColor.g - topColor.g );
    float b = static_cast< float >( topColor.b ) + btwn * ( btmColor.b - topColor.b );

    m_Color.r = static_cast< sf::Uint8 >( floor(r) );
    m_Color.g = static_cast< sf::Uint8 >( floor(g) );
    m_Color.b = static_cast< sf::Uint8 >( floor(b) );
  }

  void RainbowStrip::ValueChaned()
  {
    //Calc current color
    CalculateCurrentColor();
    m_ColorChangedSignal(m_Color);
  }

}  
