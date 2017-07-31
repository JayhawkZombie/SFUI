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
    ~DropShadow();

    void SetRadius(float Radius);
    void SetCasterSize(const Vec2f &Size);
    void SetCasterPosition(const Vec2f &Position);
    void SetOffset(const Vec2f &Offset);
    void Render(sf::RenderTarget &Target);
    void Create();

    static void LoadShaders();
    static void CleanupShaders();

  private:
    static sf::Shader *m_BlackingOpacityShader;
    static sf::Shader *m_VerticalBlurShader;
    static sf::Shader *m_HorizontalBlurShader;

    _shared_ptr<sf::RenderTexture> m_ShadowTexture = nullptr;
    _shared_ptr<sf::RenderTexture> m_HorizontalBlurTexture = nullptr;
    _shared_ptr<sf::RenderTexture> m_VerticalBlurTexture = nullptr;

    sf::RectangleShape m_DrawingRect;
    sf::RectangleShape m_CasterRect;
    Vec2f m_ShadowOffset = { 0.f, 0.f };
    Vec2f m_CasterPosition = { 0.f, 0.f };
    Vec2d m_CasterSize = { 0.f, 0.f };
    float m_Radius = 0.f;
  };
  
}  

#endif // SFUI_DROPSHADOW_H
