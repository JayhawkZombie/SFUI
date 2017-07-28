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
#include <SFUI/Include/Utilities/DropShadow.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  DropShadow::DropShadow()
  {
    if (!m_BlurShader.loadFromFile("Shaders/GaussBlur.glsl", sf::Shader::Fragment)) {
      std::cerr << "Unable to load blur shader\n";
    }

    if (!m_BlackShader.loadFromFile("Shaders/DrawBlack.glsl", sf::Shader::Fragment)) {
      std::cerr << "Unable to load blackening shader\n";
    }
    m_HorizShader.loadFromFile("Shaders/GaussHoriz.glsl", sf::Shader::Fragment);
    m_VertShader.loadFromFile("Shaders/GaussVert.glsl", sf::Shader::Fragment);
  }

  void DropShadow::SetShadowSize(const sf::Vector2f &size)
  {
    m_ShadowSize = size;
    m_BlurringRect.setSize(size);
    m_ShadowRect.setSize(size);
  }

  void DropShadow::SetRadius(uint32 Radius)
  {
    m_Radius = Radius;
  }

  void DropShadow::Init()
  {
    if (!m_BlurTexture.create(( uint32 )( m_ShadowSize.x + 2 * m_Radius ), ( uint32 )( m_ShadowSize.y + 2 * m_Radius )) ||
        !m_InterimTexture.create(( uint32 )m_ShadowSize.x + 2 * m_Radius, ( uint32 )( m_ShadowSize.y + 2 * m_Radius )) ||
        !m_HorizTexture.create(( uint32 )m_ShadowSize.x + 2 * m_Radius, ( uint32 )( m_ShadowSize.y + 2 * m_Radius ))) {
      std::cerr << "Unable to set render texture size to (" << m_ShadowSize.x << ", " << m_ShadowSize.y << ")\n";
    }
    m_BlurringRect.setSize(m_ShadowSize + 2.f * Vec2f(m_Radius, m_Radius));
    m_ShadowRect.setPosition(m_ShadowPos - Vec2f(m_Radius, m_Radius));
    m_ShadowRect.setSize(m_ShadowSize + 2.f * Vec2f(m_Radius, m_Radius));
  }

  void DropShadow::SetShadowPosition(const sf::Vector2f &position)
  {
    m_ShadowPos = position;
  }

  void DropShadow::DrawShadowSource(const sf::Vector2f &Size)
  {
    m_InterimTexture.setActive(true);

    m_InterimTexture.clear(sf::Color::Transparent);
    sf::RectangleShape srcRect;
    srcRect.setSize(m_ShadowSize);

    srcRect.setPosition(Vec2f(m_Radius, m_Radius));

    //Draw it in all black
    m_InterimTexture.draw(srcRect, &m_BlackShader);

    m_InterimTexture.display();

    ////Blur it
    m_HorizTexture.clear(sf::Color::Transparent);
    m_BlurringRect.setTexture(&m_InterimTexture.getTexture());

    m_HorizShader.setUniform("image", m_InterimTexture.getTexture());
    m_HorizShader.setUniform("imagesize", sf::Glsl::Vec2(m_InterimTexture.getSize()));
    m_HorizTexture.draw(m_BlurringRect, &m_HorizShader);
    //m_HorizTexture.draw(m_BlurringRect, &m_HorizShader);
    m_HorizTexture.display();

    m_BlurTexture.clear(sf::Color::Transparent);
    m_VertShader.setUniform("image", m_HorizTexture.getTexture());
    m_VertShader.setUniform("imagesize", sf::Glsl::Vec2(m_InterimTexture.getSize()));
    m_BlurTexture.draw(m_BlurringRect, &m_VertShader);
    //m_BlurTexture.draw(m_BlurringRect, &m_VertShader);
    m_BlurTexture.display();

    m_ShadowRect.setTexture(&m_BlurTexture.getTexture());
  }

  void DropShadow::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_ShadowRect);
  }

}  
