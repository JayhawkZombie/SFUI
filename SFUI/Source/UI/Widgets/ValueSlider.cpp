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
#include <SFUI/Include/UI/Widgets/ValueSlider.h>
#include <SFUI/Include/UI/Theme.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  ValueSlider::ValueSlider(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : Widget(theme, parent)
  {
    m_Slider = m_Theme->MakeSlider(this);
    m_Slider->SetIsVertical(true);
    m_Slider->SetBackgroundColor(sf::Color(122, 122, 122));
    m_Slider->SetBorderColor(sf::Color(25, 25, 25));
    m_Slider->SetBorderWidth(1);
    m_Slider->OnDragged([this](auto delta) { SliderMoved(delta); });
  }

  ValueSlider::~ValueSlider()
  {

  }

  sfui::ValueSlider::shared_ptr ValueSlider::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<ValueSlider>(theme, parent);
  }

  bool ValueSlider::HandleEvent(const sf::Event &event)
  {
    if (!Widget::HandleEvent(event)) return false;
    if (m_Slider->HandleEvent(event)) return true;

    return false;
  }

  void ValueSlider::SetPosition(const Vec2i &Position)
  {
    super::SetPosition(Position);
    m_Slider->SetPosition(Position);
    RecomputeSlider();
  }

  void ValueSlider::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);
    m_Slider->SetPosition(m_Position);
    RecomputeSlider();
  }

  void ValueSlider::SetBarWidth(uint32 Width)
  {
    m_BarSize.x = Width;
    m_Slider->SetSize(m_BarSize);
    RecomputeSlider();
  }

  void ValueSlider::SetBarHeight(uint32 Height)
  {
    m_BarSize.y = Height;
    m_Slider->SetSize(m_BarSize);
    RecomputeSlider();
  }

  void ValueSlider::SetVertical(bool v)
  {
    m_IsVertical = v;
    m_Slider->SetIsVertical(v);
  }

  void ValueSlider::SetSliderColor(const Color &c)
  {
    m_Slider->SetBackgroundColor(c);
  }

  void ValueSlider::Update()
  {
    m_Slider->BaseUpdate();
  }

  void ValueSlider::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_BackgroundRect);
    m_Slider->Render(Target);

  }

  void ValueSlider::Move(const Vec2i &Delta)
  {
    super::Move(Delta);
    m_Slider->Move(Delta);
    RecomputeSlider();
  }

  void ValueSlider::SetTopWindow(WidgetWindow *TopWindow)
  {
    m_TopWindow = TopWindow;
    m_Slider->SetTopWindow(TopWindow);
  }

  void ValueSlider::OnValuChanged(boost::function<void(float)> func)
  {
    m_ValueChangedSignal.connect(func);
  }

  float ValueSlider::GetValue() const
  {
    return m_CurrentValue;
  }

  void ValueSlider::RecomputeSlider()
  {
    if (m_IsVertical) {
      m_SliderPos.y = cast_float(floor(m_Position.y + m_CurrentValue * m_Size.y));

      //Adjust for height of slider, since the "position" is the center line
      // going through the slider
      int halfSize = cast_int(floor(m_BarSize.y / 2.f));
      int xDiff = cast_int(floor(( m_BarSize.x - m_Size.x ) / 2.f));

      m_Slider->SetPosition({ m_Position.x - xDiff, m_SliderPos.y - halfSize });

      IntRect bds = { m_Position.x - xDiff, m_Position.y - halfSize, m_Size.x + cast_int(m_BarSize.x), m_Size.y + cast_int(m_BarSize.y) };
      m_Slider->SetDragBounds(bds);
    }
    else {
      m_SliderPos.x = cast_float(floor(m_Position.x + m_CurrentValue * m_Size.x));
      int halfSize = cast_int(floor(m_BarSize.x / 2.f));
      int yDiff = cast_int(floor(m_BarSize.y - m_Size.y) / 2.f);

      m_Slider->SetPosition({ m_SliderPos.x, m_Position.y - halfSize });

      IntRect bds = { m_Position.x - halfSize, m_Position.y - yDiff, m_Size.x + cast_int(m_BarSize.x), m_Size.y + cast_int(m_BarSize.y) };
      m_Slider->SetDragBounds(bds);
    }
  }

  void ValueSlider::SliderMoved(const Vec2i &delta)
  {
    Vec2i sPos = m_Slider->GetPosition();
    bool atMax = ( m_CurrentValue >= 1.f );
    bool atMin = ( m_CurrentValue <= 0.f );

    float sliderval = 0.f;

    if (m_IsVertical) {
      //Adjust to get the center of the slider (median)
      sPos.y += cast_int(floor(m_BarSize.y / 2.f));

      //Calculate new "value"
      sliderval = ( sPos.y - m_Position.y ) / ( float )( m_Size.y );
    }
    else {
      //Middle of the slider in the horiz. direction
      sPos.x += cast_int(floor(m_BarSize.x / 2.f));

      //Get how far the slider is across us horizontally
      sliderval = ( sPos.x - m_Position.x ) / ( float )( m_Size.x );
    }
    m_CurrentValue = sliderval;

    ValueChaned();
    m_ValueChangedSignal(m_CurrentValue);
  }

  void ValueSlider::ValueChaned()
  {

  }

  void ValueSlider::MousePressed(bool left, bool right)
  {
    if (left) {
      //We want to try to put the center of the slider at the mouse position
      
      Vec2i sPos = m_Slider->GetPosition();

      if (m_IsVertical) {
        sPos.y = currentMousePosition.y;

        float xDiff = ( m_BarSize.x - m_Size.x ) / 2.f;
        sPos.x = cast_int(floor(m_Position.x - xDiff));
        m_Slider->SetPosition(sPos);
      }
      else {
        float yDiff = ( m_BarSize.y - m_Size.y ) / 2.f;
        float halfSize = cast_int(floor(m_BarSize.x / 2.f));
        sPos.x = currentMousePosition.x - halfSize;
        
        sPos.y = cast_int(floor(m_Position.y - yDiff));
        m_Slider->SetPosition(sPos);
      }

      //Trigger a value changed signal and position it properly
      // so we can ignore invalid regions
      SliderMoved({ 0, 0 });
    }
  }

}  
