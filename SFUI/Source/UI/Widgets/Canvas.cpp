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
#include <SFUI/Include/UI/Widgets/Canvas.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  Canvas::Canvas(optional<Theme *> theme, std::optional<pointer> parent)
    : Widget(theme, parent)
  {

  }

  Canvas::~Canvas()
  {

  }

  Canvas::shared_ptr Canvas::Create(optional<Theme *> theme, std::optional<pointer> parent)
  {
    return std::make_shared<Canvas>(theme, parent);
  }

  void Canvas::Clear()
  {
    m_RenderTexture.clear(m_ClearColor);
  }

  void Canvas::Render(sf::RenderTarget &Target)
  {
    
  }

  void Canvas::Display()
  {
    m_RenderTexture.display();
  }

  bool Canvas::HandleEvent(const sf::Event &event)
  {
    return Widget::HandleEvent(event);
  }

  void Canvas::SetPosition(const Vec2i &Position)
  {
    super::SetPosition(Position);
    m_RenderRect.setPosition(Position);
  }

  void Canvas::SetSize(const Vec2i &Size)
  {
    if (!m_RenderTexture.create(Size.x, Size.y)) {
      std::cerr << "Unable to create render texture of size: (" << Size.x << ", " << Size.y << ")\n";
    }
    m_RenderRect.setSize(Size);
  }

  void Canvas::Move(const Vec2i &Delta)
  {
    super::Move(Delta);
    m_RenderRect.move(Delta);
  }

  void Canvas::SetBorder(const Color &c, int32 Width)
  {
    m_RenderRect.setOutlineColor(c);
    m_RenderRect.setOutlineThickness(cast_float(Width));
  }

  void Canvas::SetClearColor(const Color &c)
  {
    m_ClearColor = c;
  }

}  
