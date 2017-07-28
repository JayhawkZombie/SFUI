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
#include <SFUI/Include/UI/Widgets/Draggable.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  

  Draggable::Draggable(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : Widget(theme, parent)
  {

  }

  Draggable::~Draggable()
  {

  }

  bool Draggable::HandleEvent(const sf::Event &event)
  {
    //if (!Widget::HandleEvent(event)) return false;

    switch (event.type)
    {
      case sf::Event::MouseMoved:
      {
        if (m_IsDragging)
        {
          //continue drag
          Vec2i delta = currentMousePosition - previousMousePositon;
          Drag(delta);
          return true;
        }
        return true;
      }

      case sf::Event::MouseButtonPressed:
      {
        if (!m_IsDragging)
        {
          //Begin the drag
          StartDrag();
          return true;
        }
        return true;
      }

      case sf::Event::MouseButtonReleased:
      {
        if (m_IsDragging)
        {
          //Stop the drag
          Drop();
          return true;
        }
        return true;
      }
    }

    return false;
  }

  bool Draggable::IsBeingDragged() const
  {
    return m_IsDragging;
  }

  void Draggable::MouseMoved()
  {

  }

  void Draggable::DragStarted()
  {

  }

  void Draggable::Dragged(const Vec2i &delta)
  {

  }

  void Draggable::Dropped()
  {

  }

  void Draggable::StartDrag()
  {
    m_IsDragging = true;
    StealMouseFocus(this);
    //TakeGlobalMouseFocus(this);
    DragStarted();
  }

  void Draggable::Drag(const Vec2i &Delta)
  {
    //Move(Delta);
    Dragged(Delta);
  }

  void Draggable::Drop()
  {
    m_IsDragging = false;
    //ReleaseGlobalMouseFocus();
    ReturnMouseFocus(this);
    Dropped();
  }

}  
