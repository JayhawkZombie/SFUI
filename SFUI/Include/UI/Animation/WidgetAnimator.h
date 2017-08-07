#ifndef SFUI_WIDGETANIMATOR_H
#define SFUI_WIDGETANIMATOR_H

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
#include <SFUI/Include/Utilities/CubicBezier.hpp>
#include <SFUI/Include/Common.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////
#include <Kairos.hpp>

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////
#include <queue>

namespace sfui
{  
  
  enum class WidgetAnimation : uint32
  {
    SlideIn,
    SlideOut,
    FadeIn,
    FadeOut,
    Bounce,
    Expand,
    Contract,
    Spin
  };

  enum class Easing
  {
    Standard,
    Decelerate,
    Accelerate,
    EaseInOut
  };

  namespace EasingCurves
  {  
    const CubicBezier standard = CubicBezier(0.4f, 0.f, 0.2f, 1.f);
    const CubicBezier deceleration = CubicBezier(0.075f, 0.82f, 0.165f, 1.f);
    const CubicBezier acceleration = CubicBezier(0.6f, 0.04f, 0.98f, 0.335f);
    const CubicBezier easeInOut = CubicBezier(0.86f, 0.f, 0.07f, 1.f);
  }  

  struct Animation
  {
    CubicBezier curve;
    WidgetAnimation animation;
    kairos::Duration duration;
    Vec2f initialValue;
    Vec2f finalValue;
  };

  class Widget;

  class WidgetAnimator
  {
  public:
    WidgetAnimator(Widget *widget);
    ~WidgetAnimator();

    void Animate(WidgetAnimation Anim, const Vec2f &initialVal, const Vec2f &finalVal, Easing curve, uint32 Duration);
    void EndCurrent();
    void RestartCurrent();
    void Stop();
    void Update();
    
  private:
    void ApplyAnimation(float perc);
    void DequeAnimation();

    Widget *m_Widget = nullptr;
    kairos::Duration m_Duration;
    kairos::Timer m_Timer;
    std::queue<Animation> m_AnimationQueue;
    bool m_EndCurrent = false;
  };


  struct ValueAnimation
  {
    float LowValue;
    float HighValue;
    kairos::Duration Duration;
    CubicBezier EasingCurve;
    std::function<void(const float &)> Callback;
  };

  class ValueAnimator
  {
  public:
    ValueAnimator();
    ~ValueAnimator();

    void Animate(const float &LowVal, const float &HighVal, Easing curve, uint32 Duration, std::function<void(const float &)> Callback);
    void Update();

  private:
    void ApplyAnimation(float perc);
    void DequeAnimation();

    kairos::Timer m_Timer;
    std::queue<ValueAnimation> m_AnimationQueue;
  };

}  

#endif // SFUI_WIDGETANIMATOR_H
