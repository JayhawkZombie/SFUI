#ifndef SFUI_SCROLLBAR_H
#define SFUI_SCROLLBAR_H

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
#include <SFUI/Include/UI/Widgets/Button.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class ScrollBar : public Widget
  {
  public:
    WIDGET_DERIVED(ScrollBar, Widget);
    ScrollBar(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());
    virtual ~ScrollBar() override;

    static shared_ptr Create(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());

    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;

    bool HandleEvent(const sf::Event &event) override;

    virtual void SetPosition(const Vec2i &Position) override;
    virtual void SetSize(const Vec2i &Size) override;
    virtual void SetBarWidth(uint32 Width); // If vertical
    virtual void SetBarHeight(uint32 Height); // If hori.
    void SetIsVertical(bool isVertical);

    void OnScrolled(boost::function<void(double)> func);
    virtual void Move(const Vec2i &Delta) override;
    virtual void SetTopWindow(WidgetWindow *TopWindow) override;

    virtual void SetRange(int minPos, int visEdge, int maxVal);

  protected:
    void RecomputeSlider();
    void SliderMoved(const Vec2i &delta);

    Signal<void(double)> m_ScrolledSignal;

    //uint32 m_TotalScrollableArea = 1;
    //uint32 m_TotalViewableArea = 1;
    //double m_ScrollRate = 0.;
    //uint32 m_ScrollRegion = 0; // px on screen the slider has to move
    //uint32 m_SliderSize = 0;
    //uint32 m_FreeRoom = 0;

    Vec2i m_ScrollZero = { 0, 0 };
    Vec2i m_ScrollEnd = { 0, 0 };

    bool m_IsVertical = true;
    int m_MinimumPos = 0;
    int m_VisibleEdge = 1;
    int m_MaximumPos = 1;
    int m_SliderRegion = 1;
    int m_VisibleRange = 1;
    int m_TotalRange = 1;
    int m_FreeZone = 1;
    int m_ScrollRange = 1;
    int m_ScrollSize = 1;

    Button::shared_ptr m_UpButton;
    Button::shared_ptr m_DownButton;
    
    Slider::shared_ptr m_Slider;
    sf::RectangleShape m_SliderBG;
  };
  
}  

#endif // SFUI_SCROLLBAR_H
