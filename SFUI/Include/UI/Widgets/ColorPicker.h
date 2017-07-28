#ifndef SFUI_COLORPICKER_H
#define SFUI_COLORPICKER_H

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
#include <SFUI/Include/UI/Widgets/Panel.h>
#include <SFUI/Include/UI/Widgets/RainbowStrip.h>
#include <SFUI/Include/UI/Widgets/Button.h>
#include <SFUI/Include/UI/Widgets/LineEdit.h>
#include <SFUI/Include/UI/Widgets/BitmapLabel.h>
#include <SFUI/Include/UI/Widgets/ColorBox.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class ColorPicker : public Panel
  {
  public:
    WIDGET_DERIVED(ColorPicker, Panel);
    ColorPicker(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());
    virtual ~ColorPicker() override;

    static shared_ptr Create(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());

    Color GetColor() const;

    virtual void Render(sf::RenderTarget &Target) override;

    virtual void SetPosition(const Vec2i &Position) override;
    virtual void SetSize(const Vec2i &Size) override;

    virtual void Move(const Vec2i &Delta) override;
    
    void OnColorCancelled(boost::function<void()> func);
    void OnColorAccepted(boost::function<void(const Color &)> func);
  
  protected:
    Signal<void()> m_CancelledSignal;
    Signal<void(const Color &)> m_AcceptedSignal;

    void BoxColorChanged(const Color &c);
    void CancelButtonHit();
    void AcceptButtonHit();
    void OpacityChanged(float perc);
    void RepositionItems();

    RainbowStrip::shared_ptr m_ColorStrip;
    ValueSlider::shared_ptr m_OpacitySlider;
    ColorBox::shared_ptr m_ColorBox;
    sf::RectangleShape m_PreviewBox;
    Button::shared_ptr m_OKBtn;
    Button::shared_ptr m_CancelBtn;

    BitmapLabel::shared_ptr m_RGBLabel;
    BitmapLabel::shared_ptr m_HexLabel;

    sf::Color m_CurrentColor = sf::Color::White;
    sf::Uint8 m_Opacity = 255;

  };
  
}  

#endif // SFUI_COLORPICKER_H
