#ifndef SFUI_SPINBOX_H
#define SFUI_SPINBOX_H

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
#include <SFUI/Include/UI/Widgets/GenericContainer.h>
#include <SFUI/Include/UI/Widgets/Button.h>
#include <SFUI/Include/UI/Widgets/LineEdit.h>
#include <SFUI/Include/UI/Widgets/BitmapLabel.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////
#include <algorithm>

namespace sfui
{  
  
  class SpinBox : public GenericContainer
  {
  public:
    WIDGET_DERIVED(SpinBox, GenericContainer);
    SpinBox(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());
    virtual ~SpinBox() override;

    static shared_ptr Create(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());

    virtual bool HandleEvent(const sf::Event &event) override;

    virtual void SetPosition(const Vec2i &Position) override;
    virtual void SetSize(const Vec2i &Size) override;
    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;
    virtual void Move(const Vec2i &Delta) override;
        
    virtual void LockValue();
    virtual void UnlockValue();
    virtual void ResetValue();

    void SetDefaultValue(float val);
    void SetMaximumValue(float val);
    void SetMinimumValue(float val);
    void SetValueIncrease(float delta);
    void SetValueDecrease(float delta);
    void SetIsIntegral(bool IsIntegral);
    void SetLabel(const std::string &Text);

    float GetValue() const;
    
    template<typename T>
    T GetValueAs() const;

    void OnValueChanged(boost::function<void(const float &val)> func);
    void OnValueLocked(boost::function<void(float)> func);
    void OnValueUnlocked(boost::function<void(float)> func);
    void OnValueEntered(boost::function<void()> func);

  protected:
    Signal<void()> m_ValueEnteredSignal;
    Signal<void(float)> m_ValueChangedSignal;
    Signal<void(float)> m_ValueLockedSignal;
    Signal<void(float)> m_ValueUnlockedSignal;
    virtual void RepositionItems();

    virtual void IncreaseButtonClicked();
    virtual void DecreaseButtonClicked();
    virtual void LockButtonClicked();
    virtual void ResetButtonClicked();
    virtual void ValueInputChanged();
    virtual void ValueEntered();
    void HandleValueChanged();

    virtual void Resized() override;
    virtual void Moved() override;
    virtual void ValueIncreased();
    virtual void ValueDecreased();
    virtual void ValueLocked();
    virtual void ValueReset();

    float m_DefaultValue = 0.f;
    float m_MinimumValue = 0.f;
    float m_MaximumValue = 0.f;
    float m_IncreaseDelta = 0.f;
    float m_DecreaseDelta = 0.f;
    float m_CurrentValue = 0.f;

    BitmapLabel::shared_ptr m_DescLabel = nullptr;
    LineEdit::shared_ptr m_ValueInput = nullptr;
    Button::shared_ptr m_IncreaseButton = nullptr;
    Button::shared_ptr m_DecreaseButton = nullptr;
    Button::shared_ptr m_LockButton = nullptr;
    Button::shared_ptr m_ResetButton = nullptr;

    bool m_IsLocked = false;
    bool m_IsIntegral = true;
  };

  template<typename T>
  T sfui::SpinBox::GetValueAs() const
  {
    return static_cast< T >( m_CurrentValue );
  }

}  

#endif // SFUI_SPINBOX_H
