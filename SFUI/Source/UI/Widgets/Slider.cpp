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
#include <SFUI/Include/UI/Widgets/Slider.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  

  Slider::Slider(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : Draggable(theme, parent)
  {

  }

  Slider::shared_ptr Slider::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<Slider>(theme, parent);
  }

  void Slider::SetIsVertical(bool isVertical)
  {
    m_IsVertical = isVertical;
  }

  void Slider::OnDragged(boost::function<void(const Vec2i &)> func)
  {
    m_DraggedSignal.connect(func);
  }

  void Slider::SetDragBounds(IntRect bds)
  {
    m_DraggableBounds = bds;
  }

  void Slider::Dragged(const Vec2i &delta)
  {
    auto one_dir = delta;

    if (m_IsVertical) {
      one_dir.x = 0;
    }
    else {
      one_dir.y = 0;
    }

    Move(one_dir);
    
    if (m_IsVertical) {
      if (m_Position.y < m_DraggableBounds.top)
        m_Position.y = m_DraggableBounds.top;
      else if (m_Position.y + m_Size.y > m_DraggableBounds.top + m_DraggableBounds.height)
        m_Position.y = m_DraggableBounds.top + m_DraggableBounds.height - m_Size.y;
    } else {
      if (m_Position.x < m_DraggableBounds.left)
        m_Position.x = m_DraggableBounds.left;
      else if (m_Position.x + m_Size.x > m_DraggableBounds.left + m_DraggableBounds.width)
        m_Position.x = m_DraggableBounds.left + m_DraggableBounds.width - m_Size.x;
    }
    m_BackgroundRect.setPosition(m_Position);
    m_Bounds.left = m_Position.x;
    m_Bounds.top = m_Position.y;

    m_DraggedSignal(one_dir);
  }

  void Slider::DragStarted()
  {
    
  }

  void Slider::Dropped()
  {
    
  }

  Slider::~Slider()
  {

  }

}  
