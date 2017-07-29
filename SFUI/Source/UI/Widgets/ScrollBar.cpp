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
#include <SFUI/Include/UI/Widgets/ScrollBar.h>
#include <SFUI/Include/UI/Theme.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  

  
  ScrollBar::ScrollBar(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : Widget(theme, parent)
  {
    m_UpButton = m_Theme->MakeButton("U");
    m_UpButton->SetCanAnimateContract(false);
    m_UpButton->SetCanAnimateExpand(false);

    m_DownButton = m_Theme->MakeButton("D");
    m_DownButton->SetCanAnimateContract(false);
    m_DownButton->SetCanAnimateExpand(false);

    m_Slider = m_Theme->MakeSlider(this);
    m_Slider->SetBackgroundColor(sf::Color(211, 217, 226));
    m_Slider->SetBorderColor(Color(125, 0, 160));
    m_Slider->SetBorderWidth(1);
    m_Slider->OnDragged([this](auto vec) { SliderMoved(vec); });
  }

  ScrollBar::~ScrollBar()
  {

  }

  sfui::ScrollBar::shared_ptr ScrollBar::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<ScrollBar>(theme, parent);
  }

  void ScrollBar::Update()
  {
    m_Slider->BaseUpdate();
    m_UpButton->BaseUpdate();
    m_DownButton->BaseUpdate();
  }

  void ScrollBar::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_BackgroundRect);
    m_Slider->Render(Target);
    m_UpButton->Render(Target);
    m_DownButton->Render(Target);
  }

  bool ScrollBar::HandleEvent(const sf::Event &event)
  {
    if (!Widget::HandleEvent(event)) return false;

    if (m_UpButton->HandleEvent(event)) return true;
    if (m_DownButton->HandleEvent(event)) return true;

    if (!m_Slider->IsBeingDragged() && m_Slider->HandleEvent(event)) return true;
    return false;
  }

  void ScrollBar::SetPosition(const Vec2i &Position)
  {
    super::SetPosition(Position);

    IntRect bds = { m_Position.x, m_Position.y, m_Size.x, m_Size.y };
    m_Slider->SetDragBounds(bds);

    m_SliderBG.setPosition(Position);
    m_UpButton->SetPosition(m_Position);
    m_DownButton->SetPosition({ m_Position.x, m_Position.y + m_Size.y - m_Size.x });
    RecomputeSlider();
  }

  void ScrollBar::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);

    IntRect bds = { m_Position.x, m_Position.y + m_Size.x, m_Size.x, m_Size.y - m_Size.x };
    m_Slider->SetDragBounds(bds);

    if (m_IsVertical) {
      m_UpButton->SetSize({ Size.x, Size.x });
      m_DownButton->SetSize({ Size.x, Size.x });
      super::SetSize(Size);
    }
    RecomputeSlider();
  }

  void ScrollBar::SetBarWidth(uint32 Width)
  {

  }

  void ScrollBar::SetBarHeight(uint32 Height)
  {

  }

  void ScrollBar::SetIsVertical(bool isVertical)
  {
    m_IsVertical = isVertical;
    m_Slider->SetIsVertical(true);
  }

  void ScrollBar::OnScrolled(boost::function<void(double)> func)
  {
    m_ScrolledSignal.connect(func);
  }

  void ScrollBar::Move(const Vec2i &Delta)
  {
    m_Slider->Move(Delta);
    m_UpButton->Move(Delta);
    m_DownButton->Move(Delta);

    Widget::Move(Delta);
  }

  void ScrollBar::SetTopWindow(WidgetWindow *TopWindow)
  {
    m_TopWindow = TopWindow;
    m_DownButton->SetTopWindow(TopWindow);
    m_DownButton->SetTopWindow(TopWindow);
    m_Slider->SetTopWindow(TopWindow);
  }

  void ScrollBar::SetRange(int minPos, int visEdge, int maxVal)
  {
    m_MinimumPos = minPos;
    m_VisibleEdge = visEdge;
    m_MaximumPos = maxVal;
    RecomputeSlider();
  }

  void ScrollBar::RecomputeSlider()
  {
    m_ScrollZero.y = m_Position.y + m_Size.x;
    m_ScrollEnd.y = m_Position.y + m_Size.y - m_Size.x;
    m_ScrollRange = m_Size.y - ( 2 * m_Size.x );
    m_VisibleRange = m_VisibleEdge - m_MinimumPos;
    m_TotalRange = m_MaximumPos - m_MinimumPos;
    m_ScrollSize = cast_int(floor(( ( ( double )m_VisibleRange ) / ( ( double )m_TotalRange ) ) * m_ScrollRange));

    m_Slider->SetPosition({ m_Position.x + 1, m_Position.y + m_Size.x });
    m_Slider->SetDefaultSize({ m_Size.x - 2, m_ScrollSize });
  }

  void ScrollBar::SliderMoved(const Vec2i &delta)
  {
    //Figure out how far it has scrolled up/down/left/right on the bar
    if (m_IsVertical) {
      
      auto sPos = m_Slider->GetPosition();
      auto sSize = m_Slider->GetSize();

      if (sPos.y <= m_ScrollZero.y) {
        sPos.y = m_ScrollZero.y;
      } 
      else if (sPos.y + sSize.y >= m_ScrollEnd.y) {
        sPos.y = m_ScrollEnd.y - sSize.y;
      }

      double dFromTop = ( double )sPos.y - ( m_Position.y + m_Size.x );
      double percScrolled = dFromTop / m_ScrollRange;

      m_Slider->SetPosition(sPos);

      double posToScrollTo = floor(m_MinimumPos - percScrolled * m_TotalRange);
      m_ScrolledSignal(posToScrollTo);
    }
  }

}  
