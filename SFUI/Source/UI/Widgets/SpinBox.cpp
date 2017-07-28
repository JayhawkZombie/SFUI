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
#include <SFUI/Include/UI/Widgets/SpinBox.h>
#include <SFUI/Include/UI/Theme.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////
#include <iomanip>

namespace sfui
{  
  
  SpinBox::SpinBox(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : GenericContainer(theme, parent)
  {
    m_CurrentValue = m_DefaultValue;

    m_DescLabel = m_Theme->MakeBitmapLabel(this);
    m_IncreaseButton = m_Theme->MakeIconButton(m_Theme->IconTexture, IntRect(153, 408, 50, 50), this);
    m_DecreaseButton = m_Theme->MakeIconButton(m_Theme->IconTexture, IntRect(204, 255, 50, 50), this);
    m_LockButton = m_Theme->MakeIconButton(m_Theme->IconTexture, IntRect(0, 357, 50, 50), this);
    m_ResetButton = m_Theme->MakeIconButton(m_Theme->IconTexture, IntRect(204, 102, 50, 50), this);
    m_ValueInput = m_Theme->MakeLineEdit("opensans", 14, this);
    m_ValueInput->SetBitmapFont(m_Theme->DefaultBitmapFont);
    m_ValueInput->SetType(LineEdit::Type::Integral);
    m_ValueInput->SetInputText(std::to_string(m_DefaultValue));

    m_IncreaseButton->OnClicked([this]() { IncreaseButtonClicked(); });
    m_DecreaseButton->OnClicked([this]() { DecreaseButtonClicked(); });
    m_LockButton->OnClicked([this]() { LockButtonClicked(); });
    m_ResetButton->OnClicked([this]() { ResetButtonClicked(); });
    m_ValueInput->OnTextEntered([this]() { ValueInputChanged(); });
    m_ValueInput->OnReturnHit([this]() { ValueEntered(); });

    Add(m_DescLabel);
    Add(m_IncreaseButton);
    Add(m_DecreaseButton);
    Add(m_LockButton);
    Add(m_ResetButton);
    Add(m_ValueInput);
  }

  SpinBox::~SpinBox()
  {
    m_DescLabel.reset();
    m_IncreaseButton.reset();
    m_DecreaseButton.reset();
    m_LockButton.reset();
    m_ResetButton.reset();
  }

  sfui::SpinBox::shared_ptr SpinBox::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<SpinBox>(theme, parent);
  }

  bool SpinBox::HandleEvent(const sf::Event &event)
  {
    if (!super::HandleEvent(event)) return false;

    return true;
  }

  void SpinBox::SetPosition(const Vec2i &Position)
  {
    super::SetPosition(Position);
    RepositionItems();
  }

  void SpinBox::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);
    RepositionItems();
  }

  void SpinBox::Update()
  {
    super::Update();
  }

  void SpinBox::Render(sf::RenderTarget &Target)
  {
    if (!IsVisible())
      return;

    super::Render(Target, {});
  }

  void SpinBox::Move(const Vec2i &Delta)
  {
    super::Move(Delta);
  }

  void SpinBox::LockValue()
  {
    m_IsLocked = true;
    m_ValueLockedSignal(m_CurrentValue);
  }

  void SpinBox::UnlockValue()
  {
    m_IsLocked = false;
    m_ValueLockedSignal(m_CurrentValue);
  }

  void SpinBox::ResetValue()
  {
    m_CurrentValue = m_DefaultValue;
    m_ValueInput->SetText(std::to_string(m_CurrentValue));
  }

  void SpinBox::SetDefaultValue(float val)
  {
    m_DefaultValue = val;
  }

  void SpinBox::SetMaximumValue(float val)
  {
    m_MaximumValue = val;
  }

  void SpinBox::SetMinimumValue(float val)
  {
    m_MinimumValue = val;
  }

  void SpinBox::SetValueIncrease(float delta)
  {
    m_IncreaseDelta = delta;
  }

  void SpinBox::SetValueDecrease(float delta)
  {
    m_DecreaseDelta = delta;
  }

  void SpinBox::SetIsIntegral(bool IsIntegral)
  {
    m_IsIntegral = IsIntegral;
    if (m_IsIntegral)
      m_ValueInput->SetType(LineEdit::Type::Integral);
    else
      m_ValueInput->SetType(LineEdit::Type::FloatingPoint);
    HandleValueChanged();
  }

  void SpinBox::SetLabel(const std::string &Text)
  {
    m_DescLabel->SetText(Text);
  }

  float SpinBox::GetValue() const
  {
    return m_CurrentValue;
  }

  void SpinBox::OnValueChanged(boost::function<void(const float &val)> func)
  {
    m_ValueChangedSignal.connect(func);
  }

  void SpinBox::OnValueLocked(boost::function<void(float)> func)
  {
    m_ValueLockedSignal.connect(func);
  }

  void SpinBox::OnValueUnlocked(boost::function<void(float)> func)
  {
    m_ValueUnlockedSignal.connect(func);
  }

  void SpinBox::OnValueEntered(boost::function<void()> func)
  {
    m_ValueEnteredSignal.connect(func);
  }

  void SpinBox::RepositionItems()
  {
    int wLessHeight = m_Size.x - m_Size.y;
    int heightBy2 = static_cast< int >( std::floor(m_Size.y / 2.f) );

    m_LockButton->SetPosition({ m_Position.x + wLessHeight, m_Position.y });
    m_LockButton->SetSize({ heightBy2, heightBy2 });

    m_ResetButton->SetPosition({ m_Position.x + wLessHeight, m_Position.y + heightBy2 });
    m_ResetButton->SetSize({ heightBy2, heightBy2 });

    m_IncreaseButton->SetPosition({ m_Position.x + m_Size.x - heightBy2, m_Position.y });
    m_IncreaseButton->SetSize({ heightBy2, heightBy2 });

    m_DecreaseButton->SetPosition({ m_Position.x + m_Size.x - heightBy2, m_Position.y + heightBy2 });
    m_DecreaseButton->SetSize({ heightBy2, heightBy2 });

    m_ValueInput->SetPosition(m_Position);
    m_ValueInput->SetSize({ wLessHeight, m_Size.y });
  }

  void SpinBox::IncreaseButtonClicked()
  {
    if (m_IsLocked)
      return;

    m_CurrentValue += m_IncreaseDelta;
    if (m_CurrentValue >= m_MaximumValue)
      m_CurrentValue = m_MaximumValue;

    HandleValueChanged();
    m_ValueChangedSignal(m_CurrentValue);
  }

  void SpinBox::DecreaseButtonClicked()
  {
    if (m_IsLocked)
      return;

    m_CurrentValue -= m_DecreaseDelta;
    if (m_CurrentValue <= m_MinimumValue)
      m_CurrentValue = m_MinimumValue;

    HandleValueChanged();
    m_ValueChangedSignal(m_CurrentValue);
  }

  void SpinBox::LockButtonClicked()
  {
    m_IsLocked = !m_IsLocked;

    if (m_IsLocked) {
      m_ValueInput->Disable();
    }
    else {
      m_ValueInput->Enable();
    }
  }

  void SpinBox::ResetButtonClicked()
  {
    m_CurrentValue = m_DefaultValue;
  
    m_CurrentValue = m_DefaultValue;
    HandleValueChanged();
    ValueReset();
  }

  void SpinBox::ValueInputChanged()
  {

    sstring str = m_ValueInput->GetInputText();
    if (str.empty()) return;

    if (m_IsIntegral) {
      m_CurrentValue = static_cast< float >( std::stoi(str) );
    }
    else {
      m_CurrentValue = std::stof(str);
    }

    m_ValueChangedSignal(m_CurrentValue);
  }

  void SpinBox::ValueEntered()
  {
    sstring str = m_ValueInput->GetInputText();
    if (str.empty()) return;

    if (m_IsIntegral) {
      m_CurrentValue = static_cast< float >( std::stoi(str) );
    } else {
      m_CurrentValue = std::stof(str);
    }
    
    m_ValueEnteredSignal();
  }

  void SpinBox::HandleValueChanged()
  {
    if (m_IsIntegral) {
      m_ValueInput->SetInputText(std::to_string(static_cast< int >( m_CurrentValue )));
    } else {
      std::stringstream fval;
      fval << std::setprecision(3) << m_CurrentValue;
      m_ValueInput->SetInputText(fval.str());
    }
  }

  void SpinBox::Resized()
  {
    RepositionItems();
  }

  void SpinBox::Moved()
  {
    RepositionItems();
  }

  void SpinBox::ValueIncreased()
  {

  }

  void SpinBox::ValueDecreased()
  {

  }

  void SpinBox::ValueLocked()
  {

  }

  void SpinBox::ValueReset()
  {

  }

}  
