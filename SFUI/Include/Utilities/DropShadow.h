#ifndef SFUI_DROPSHADOW_H
#define SFUI_DROPSHADOW_H

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
#include <SFUI/Include/Common.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class DropShadow
  {
  public:
    DropShadow();
    ~DropShadow() = default;

    //Set the size of the shadow to draw on screen
    void SetShadowSize(const sf::Vector2f &size);

    void SetRadius(uint32 Radius);

    void Init();

    //Set the position of the shadow on screen
    void SetShadowPosition(const sf::Vector2f &position);

    //Draw the source in black for bluring
    void DrawShadowSource(const sf::Vector2f &Size);

    //Render the blurred shadow
    void Render(sf::RenderTarget &Target);

  private:
    sf::RectangleShape m_ShadowRect;
    sf::RectangleShape m_BlurringRect;
    sf::Shader m_BlurShader;
    sf::Shader m_BlackShader;

    Vec2f m_ShadowSize;
    Vec2f m_ShadowPos;
    uint32 m_Radius;

    sf::Shader m_HorizShader;
    sf::Shader m_VertShader;

    sf::RenderTexture m_BlurTexture;
    sf::RenderTexture m_HorizTexture;
    sf::RenderTexture m_InterimTexture;
  };
  
}  

#endif // SFUI_DROPSHADOW_H
