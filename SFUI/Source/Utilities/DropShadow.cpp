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

  }

  DropShadow::~DropShadow()
  {

  }

  void DropShadow::SetRadius(float Radius)
  {
    m_Radius = Radius;
  }

  void DropShadow::SetCasterSize(const Vec2f &Size)
  {
    m_CasterSize = Size;
  }

  void DropShadow::SetCasterPosition(const Vec2f &Position)
  {
    m_CasterPosition = Position;
  }

  void DropShadow::SetOffset(const Vec2f &Offset)
  {
    m_ShadowOffset = Offset;
  }

  void DropShadow::Render(sf::RenderTarget &Target)
  {
    m_DrawingRect.setPosition(m_CasterPosition + m_ShadowOffset - Vec2f(m_Radius, m_Radius));
    
    Target.draw(m_DrawingRect);
  }

  void DropShadow::Create()
  {
    Vec2f SizeCasterPlusShadow = m_CasterSize + Vec2f(4.f * m_Radius, 4.f * m_Radius);
    Vec2u CeilSize = Vec2u(
      static_cast< unsigned int >( ceil(SizeCasterPlusShadow.x) ),
      static_cast< unsigned int >( ceil(SizeCasterPlusShadow.y) )
    );

    m_ShadowTexture.reset(new sf::RenderTexture);
    m_HorizontalBlurTexture.reset(new sf::RenderTexture);
    m_VerticalBlurTexture.reset(new sf::RenderTexture);

    //First create the shadow texture
    
    if (m_ShadowTexture->create(CeilSize.x, CeilSize.y)) { std::cout << "Created shadowTexture\n"; } else { std::cout << "Failed to create shadow texture\n"; }
    if (m_HorizontalBlurTexture->create(CeilSize.x, CeilSize.y)) { std::cout << "Created horizontalBlurTexture\n"; } else { std::cout << "Failed to create horizontal blur texture\n"; }
    if (m_VerticalBlurTexture->create(CeilSize.x, CeilSize.y)) { std::cout << "Created verticalBlurTexture\n"; } else { std::cout << "Failed to create vertical blur texture\n"; }

    //Set size of casting object and offset it by the radius
    m_CasterRect.setSize({ 50.f, 50.f });
    m_CasterRect.setPosition(Vec2f(2.f * m_Radius, 2.f * m_Radius));
    m_CasterRect.setFillColor(sf::Color(0, 0, 0));

    m_ShadowTexture->clear(sf::Color::Transparent);
    m_ShadowTexture->draw(m_CasterRect);
    m_ShadowTexture->display();
    //m_DrawingRect.setTexture(&m_ShadowTexture->getTexture());
    m_DrawingRect.setPosition({ 0.f, 0.f });
    m_DrawingRect.setSize(static_cast< sf::Vector2f >( m_ShadowTexture->getSize() ));

    //Now blur it
    auto tSize = m_ShadowTexture->getSize();

    //Horizontally first
    m_HorizontalBlurShader->setUniform("image", m_ShadowTexture->getTexture());
    m_HorizontalBlurShader->setUniform("imagesize", sf::Glsl::Vec2(m_ShadowTexture->getSize()));
    m_HorizontalBlurTexture->clear(sf::Color::Transparent);
    m_HorizontalBlurTexture->draw(m_DrawingRect, m_HorizontalBlurShader);
    m_HorizontalBlurTexture->display();
    //m_HorizontalBlurTexture->setSmooth(true);
    //m_DrawingRect.setTexture(&m_HorizontalBlurTexture->getTexture());

    //Then vertically
    m_VerticalBlurShader->setUniform("image", m_HorizontalBlurTexture->getTexture());
    m_VerticalBlurShader->setUniform("imagesize", sf::Glsl::Vec2(m_ShadowTexture->getSize()));
    m_VerticalBlurTexture->clear(sf::Color::Transparent);
    m_VerticalBlurTexture->draw(m_DrawingRect, m_VerticalBlurShader);
    m_VerticalBlurTexture->display();
    //m_VerticalBlurTexture->setSmooth(true);
    m_DrawingRect.setTexture(&m_VerticalBlurTexture->getTexture());

    static bool saved = false;
    if (!saved) {
      saved = true;
      m_ShadowTexture->getTexture().copyToImage().saveToFile("shadowTexture.png");
      m_HorizontalBlurTexture->getTexture().copyToImage().saveToFile("horizBlurTexture.png");
      m_VerticalBlurTexture->getTexture().copyToImage().saveToFile("vertBlurTexture.png");
    }
  }

  void DropShadow::LoadShaders()
  {
    ExecuteOnlyOnce;
    m_BlackingOpacityShader = new sf::Shader();
    m_HorizontalBlurShader = new sf::Shader();
    m_VerticalBlurShader = new sf::Shader();

    //m_BlackingOpacityShader->loadFromMemory(R"( #version 130                              
    //                                            void main()                               
    //                                            {                                         
    //                                               vec4 color = gl_Color;                 
    //                                               gl_FragColor = vec4(0, 0, 0, color.w); 
    //                                            }
    //                                         )"
    //                                       , sf::Shader::Fragment);


    m_HorizontalBlurShader->loadFromMemory(
      R"( // From https://www.shadertoy.com/view/Mtl3Rj

          #version 130

          uniform sampler2D image;
          uniform vec2 imagesize;

          float Scurve(float x) {

            x = x * 2.0 - 1.0;
            return ( -x * abs(x) * 0.5 + x + 0.5 );
          }

          vec4 blurH(sampler2D source, vec2 size, vec2 uv, float radius) {

            if (radius >= 1.0) {
  
              vec4 a = vec4(0.0);
              vec4 c = vec4(0.0);

              float width = 1.0 / size.x;
              float divisor = 0.0;
              float weight = 0.0;
              float radiusMult = 1.0 / radius;

              for (float x = -30.0; x <= 30.0; x++) {
    
                a = texture2D(source, uv + vec2(x * width, 0.0));
                weight = Scurve(1.0 - (abs(x) * radiusMult));
                c += a * weight;

                divisor += weight;
              }
    
              return vec4(c.r / divisor, c.g / divisor, c.b / divisor, c.a / divisor);
            }

            return texture2D(source, uv);
          }

          void main()
          {
            vec2 uv = gl_FragCoord.xy / imagesize.xy;
            gl_FragColor = blurH(image, imagesize.xy, uv, 20.0);
          }
      )"
      , sf::Shader::Fragment
    );

    m_VerticalBlurShader->loadFromMemory(
      R"( // From https://www.shadertoy.com/view/Mtl3Rj

          #version 130

          uniform sampler2D image;
          uniform vec2 imagesize;

          float SCurve(float x) {

            x = x * 2.0 - 1.0;
            return ( -x * abs(x) * 0.5 + x + 0.5 );
          }

          vec4 BlurV (sampler2D source, vec2 size, vec2 uv, float radius) {

	          if (radius >= 1.0)
	          {
		          vec4 A = vec4(0.0); 
		          vec4 C = vec4(0.0); 

		          float height = 1.0 / size.y;

		          float divisor = 0.0; 
                  float weight = 0.0;
        
                  float radiusMultiplier = 1.0 / radius;

                  for (float y = -30.0; y <= 30.0; y++)
		          {
			          A = texture(source, uv + vec2(0.0, y * height));
            	
            	          weight = SCurve(1.0 - (abs(y) * radiusMultiplier)); 
            
            	          C += A * weight; 
            
			          divisor += weight; 
		          }

		          return vec4(C.r / divisor, C.g / divisor, C.b / divisor, C.a / divisor);
	          }

	          return texture(source, uv);
          }

          void main()
          {
            vec2 uv = gl_FragCoord.xy / imagesize.xy;
            gl_FragColor = BlurV(image, imagesize.xy, uv, 20.0);
          }
        )"
      , sf::Shader::Fragment );
  }

  void DropShadow::CleanupShaders()
  {
    delete m_BlackingOpacityShader;
    delete m_HorizontalBlurShader;
    delete m_VerticalBlurShader;
  }

  sf::Shader* DropShadow::m_BlackingOpacityShader = nullptr;

  sf::Shader* DropShadow::m_VerticalBlurShader = nullptr;

  sf::Shader* DropShadow::m_HorizontalBlurShader = nullptr;

}  
