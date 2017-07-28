#ifndef SFUI_LINEEDIT_H
#define SFUI_LINEEDIT_H

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
#include <SFUI/Include/UI/Widget.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////
#include <regex>

namespace sfui
{  
  
  class LineEdit : public Widget
  {
  public:
    enum class Type
    {
      AlpnaNumeric,
      NumericOnly,
      AlphaOnly,
      Concealed,
      FloatingPoint,
      Integral,
      Any
    };

    WIDGET_DERIVED(LineEdit, Widget);
    LineEdit(optional<Theme*> theme = optional<Theme*>(), optional<Widget::pointer> parent = optional<Widget*>());
    virtual ~LineEdit() override;

    static shared_ptr Create(optional<Theme*> theme = optional<Theme*>(), optional<Widget::pointer> parent = optional<Widget*>());

    virtual bool HandleEvent(const sf::Event &event) override;

    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;
    virtual void SetConcealed(bool b);
    virtual void UseRegex(const std::regex &rgx);
    virtual void Clear();
    virtual void Move(const Vec2i &Delta) override;

    std::string GetInputText() const;
    void SetInputText(const std::string &Text);
    void SetType(Type type);
    bool IsValid() const;

    void SetBitmapFont(std::shared_ptr<sw::BitmapFont> fnt);
    void SetBitmapFontColor(const Color &c);
    void SetBitmapFontTracking(int32 tracking);
    void SetBitmapFontScale(uint32 scale);

    void OnTextEntered(boost::function<void()> func);
    void OnReturnHit(boost::function<void()> func);

  protected:
    Signal<void()> m_TextEnteredSignal;
    Signal<void()> m_ReturnHitSignal;

    virtual void FilterInput(uint32 unicode);
    virtual void FilterAlphaNumeric(uint32 unicode);
    virtual void FilterIntegral(uint32 unicode);
    virtual void FilterFloatingPoint(uint32 unicode);
    virtual void FilterConcealed(uint32 unicode);
    virtual void StartEditing();
    virtual void StopEditing();

    static bool IsPrintable(uint32 unicode);
    static bool IsBackspace(uint32 unicode);
    static bool IsReturn(uint32 unicode);
    static bool IsNumeric(uint32 unicode);
    static bool IsAlpha(uint32 unicode);
    static bool IsDelete(uint32 unicode);
    static bool IsSpecialCharacter(uint32 unicode);

    virtual void RepositionText();

    static constexpr uint32 BS = 8;
    static constexpr uint32 HT = 9;
    static constexpr uint32 LF = 10;
    static constexpr uint32 CR = 13;
    static constexpr uint32 SP = 32;
    static constexpr uint32 DEL = 127;
    static constexpr uint32 ESC = 27;

    bool m_IsConcealed = false;
    bool m_IsEditing = false;
    bool m_UseRegex = false;

    kairos::Timestep m_BlinkTimer;

    virtual void MouseEntered() override;
    virtual void MouseLeft() override;
    virtual void MousePressed(bool left, bool right) override;
    virtual void MouseReleased(bool left, bool right) override;
    virtual void Moved() override;
    virtual void Resized() override;
    virtual void TextEntered(uint32 unicode) override;

    sf::String m_InputText;
    std::shared_ptr<sw::BitmapFont> m_BMFont;
    sf::Color m_BMTextColor = sf::Color::White;
    int32 m_BMTextTracking = 1;
    uint32 m_BMTextScale = 1;
    sw::BitmapText m_Text;
    LineEdit::Type m_InputType = LineEdit::Type::Any;
  };
  
}  

#endif // SFUI_LINEEDIT_H
