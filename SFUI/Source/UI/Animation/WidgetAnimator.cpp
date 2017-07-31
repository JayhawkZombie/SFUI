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
#include <SFUI/Include/UI/Animation/WidgetAnimator.h>
#include <SFUI/Include/UI/Widget.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  

  WidgetAnimator::WidgetAnimator(Widget *widget)
    : m_Widget(widget)
  {

  }


  WidgetAnimator::~WidgetAnimator()
  {

  }

  void WidgetAnimator::Animate(WidgetAnimation Anim, const Vec2f &initialVal, const Vec2f &finalVal, Easing curve, uint32 Duration)
  {
    Animation a;
    a.animation = Anim;
    a.duration.setFromMilliseconds(Duration);
    a.initialValue = initialVal;
    a.finalValue = finalVal;

    switch (curve)
    {
      case Easing::Accelerate:
      { a.curve = EasingCurves::acceleration; break; }
      case Easing::Decelerate:
      { a.curve = EasingCurves::deceleration; break; }
      case Easing::Standard:
      { a.curve = EasingCurves::standard; break; }
      case Easing::EaseInOut:
      { a.curve = EasingCurves::easeInOut; break; }
    }

    if (m_AnimationQueue.empty()) {
      m_Timer.setTime(a.duration);
      m_Timer.start();
    }

    m_AnimationQueue.push(a);
  }

  void WidgetAnimator::EndCurrent()
  {
    if (!m_AnimationQueue.empty()) {
      m_EndCurrent = true;
    }
    else
      m_EndCurrent = false;
  }

  void WidgetAnimator::RestartCurrent()
  {

  }

  void WidgetAnimator::Stop()
  {
    if (!m_AnimationQueue.empty()) {
      m_AnimationQueue.pop();
    }
  }

  void WidgetAnimator::Update()
  {
    if (m_EndCurrent) {
      ApplyAnimation(1.f);
      m_Timer.stop();
      m_EndCurrent = false;
    }
    else if (!m_AnimationQueue.empty()) {
      Animation &a = m_AnimationQueue.front();
      auto time = m_Timer.getTime();
      float perc = std::clamp(1.f - ( float )time.asMilliseconds() / ( float )a.duration.asMilliseconds(), 0.01f, 1.f);
      ApplyAnimation(perc);
    }

    if (m_Timer.isDone() && !m_AnimationQueue.empty()) {
      m_Timer.stop();
      ApplyAnimation(1.f);
      DequeAnimation();
    }
  }

  void WidgetAnimator::ApplyAnimation(float perc)
  {
    Animation &a = m_AnimationQueue.front();
    Vec2f curveVal = a.curve.Compute(perc);
    Vec2f interpolated = a.initialValue;
    interpolated.x += curveVal.x * ( a.finalValue.x - a.initialValue.x );
    interpolated.y += curveVal.y * ( a.finalValue.y - a.initialValue.y );

    switch (a.animation)
    {
      case WidgetAnimation::Bounce:
      {
        break;
      }

      case WidgetAnimation::Contract:
      {
        m_Widget->SetSize(interpolated);
        break;
      }
      case WidgetAnimation::Expand:
      {
        m_Widget->SetSize(interpolated);
        break;
      }

      case WidgetAnimation::FadeIn:
      {
        break;
      }

      case WidgetAnimation::FadeOut:
      {
        break;
      }

      case WidgetAnimation::SlideIn:
      {
        m_Widget->SetPosition(interpolated);
        break;
      }

      case WidgetAnimation::SlideOut:
      {
        m_Widget->SetPosition(interpolated);
        break;
      }

      case WidgetAnimation::Spin:
      {
        break;
      }
      
      
    }
  }

  void WidgetAnimator::DequeAnimation()
  {
    m_AnimationQueue.pop();

    if (!m_AnimationQueue.empty()) {
      //Begin the next animation
      m_Timer.setTime(m_AnimationQueue.front().duration);
      m_Timer.start();
    }
  }

}  
