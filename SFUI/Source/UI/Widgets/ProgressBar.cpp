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
#include <SFUI/Include/UI/Widgets/ProgressBar.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  ProgressBar::ProgressBar(optional<Theme*> theme, optional<Widget*> parent)
    : Widget(theme, parent, 0)
  {
    SetProgressColor(m_ProgressColor);
  }

  ProgressBar::shared_ptr ProgressBar::Create(optional<Theme*> theme, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<ProgressBar>(theme, parent);
  }

  void ProgressBar::SetProgress(float Prog)
  {   
    if (m_ShowPercText && m_TextView)
      ( *m_TextView )->SetText(std::to_string(m_Progress) + std::string(" %"));

    m_ValueAnimator.Animate(m_Progress, Prog, Easing::Standard, 350, [this](const float &progFill)
    { m_FillRect.setSize({ cast_float(m_Size.x * ( progFill / 100.f )), cast_float(m_Size.y) });
    });

    m_Progress = Prog;
    ProgressChanged();
  }

  void ProgressBar::SetBufferProgress(float Prog)
  {
    m_BufferProgress = Prog;
    m_BufferRect.setSize({ m_Size.x * ( Prog / 100.f ), cast_float(m_Size.y) });

    BufferChanged();
  }

  float ProgressBar::GetProgress() const
  {
    return m_Progress;
  }

  float ProgressBar::GetBufferProgress() const
  {
    return ( m_IsBuffered ? m_BufferProgress : 100.f );
  }

  void ProgressBar::SetProgressColor(const Color &c)
  {
    m_ProgressColor = c;
    m_BGColor = c;

    m_FillRect.setFillColor(c);
    m_BackgroundRect.setFillColor(Color(c.r, c.g, c.b, static_cast<sf::Uint8>( floor(c.a * m_BGOpacity) )));
    m_BufferRect.setFillColor(Color(c.r, c.g, c.b, static_cast<sf::Uint8>(floor(c.a * m_BufferOpacity))));
  }

  bool ProgressBar::IsCompleted() const
  {
    return m_IsCompleted;
  }

  void ProgressBar::OnCompleted(boost::function<void()> func)
  {
    m_CompletedSignal.connect(func);
  }

  void ProgressBar::Reset()
  {
    m_Progress = 0.f;
    m_FillRect.setSize({ 0.f, cast_float(m_Size.y) });
  }

  void ProgressBar::Update()
  {
    super::Update();
  }

  void ProgressBar::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_BackgroundRect);
    if (m_IsBuffered)
      Target.draw(m_BufferRect);
    Target.draw(m_FillRect);

    if (m_ShowPercText && m_TextView)
      ( *m_TextView )->Render(Target, {});
  }

  void ProgressBar::ShowPercentIndicator(bool show)
  {
    m_ShowPercText = show;
  }

  void ProgressBar::SetBuffered(bool buffered)
  {
    m_IsBuffered = buffered;
  }

  void ProgressBar::Moved()
  {
    m_BackgroundRect.setPosition(m_Position);
    m_FillRect.setPosition(m_Position);
    m_BufferRect.setPosition(m_Position);
  }

  void ProgressBar::Resized()
  {
    m_FillRect.setSize({ cast_float(m_Size.x * (m_Progress / 100.f)), cast_float(m_Size.y) });    
    m_BufferRect.setSize({ cast_float(m_Size.x * (m_BufferProgress / 100.f)), cast_float(m_Size.y) });
    m_BackgroundRect.setSize(m_Size);
  }

  void ProgressBar::ProgressChanged()
  {

  }

  void ProgressBar::BufferChanged()
  {

  }

}  
