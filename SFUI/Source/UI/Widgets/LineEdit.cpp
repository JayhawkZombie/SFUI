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
#include <SFUI/Include/UI/Widgets/LineEdit.h>
#include <SFUI/Include/UI/Theme.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  LineEdit::LineEdit(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget::pointer> parent /*= optional<Widget*>()*/)
    : Widget(theme, parent)
  {

  }

  LineEdit::~LineEdit()
  {

  }

  LineEdit::shared_ptr LineEdit::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget::pointer> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<LineEdit>(theme, parent);
  }

  bool LineEdit::HandleEvent(const sf::Event &event)
  {
    if (m_IsEditing) {
      //If user clicks out of our bounds, let focus go
      if (event.type == sf::Event::MouseButtonPressed && !m_Bounds.contains(currentMousePosition)) {
        StopEditing();
        return true;
      }

      else if (event.type == sf::Event::TextEntered) {
        TextEntered(event.text.unicode);
        return true;
      }
    }
    else {
      if (Widget::HandleEvent(event))
        return true;
    }

    return false;
  }

  void LineEdit::Update()
  {
    
  }

  void LineEdit::Render(sf::RenderTarget &Target)
  {
    sf::View view; view.reset(FloatRect(m_Position.x, m_Position.y, m_Size.x, m_Size.y));
    auto size = Target.getSize();
    view.setViewport(
      {
        cast_float((float)m_Position.x / size.x),
        cast_float((float)m_Position.y / size.y),
        cast_float((float)m_Size.x / size.x),
        cast_float((float)m_Size.y / size.y)
      }
    );

    Target.draw(m_BackgroundRect);
    auto oldview = Target.getView();
    Target.setView(view);
    Target.draw(m_Text);
    Target.setView(oldview);
    RenderLabel(Target);
  }

  void LineEdit::SetConcealed(bool b)
  {

  }

  void LineEdit::UseRegex(const std::regex &rgx)
  {

  }

  void LineEdit::Clear()
  {

  }

  void LineEdit::Move(const Vec2i &Delta)
  {
    super::Move(Delta);
    m_Text.move(Delta);
  }

  std::string LineEdit::GetInputText() const
  {
    return m_InputText;
  }

  void LineEdit::SetInputText(const std::string &Text)
  {
    m_InputText = Text;
    m_Text.setString(m_InputText);
  }

  void LineEdit::SetType(Type type)
  {
    m_InputType = type;
  }

  bool LineEdit::IsValid() const
  {
    return true;
  }

  void LineEdit::SetBitmapFont(std::shared_ptr<sw::BitmapFont> fnt)
  {
    m_Text.setBitmapFont(*fnt);
  }

  void LineEdit::SetBitmapFontColor(const Color &c)
  {
    m_Text.setColor(c);
  }

  void LineEdit::SetBitmapFontTracking(int32 tracking)
  {
    m_Text.setTracking(tracking);
  }

  void LineEdit::SetBitmapFontScale(uint32 scale)
  {
    m_Text.setScale(scale);
  }

  void LineEdit::OnTextEntered(boost::function<void()> func)
  {
    m_TextEnteredSignal.connect(func);
  }

  void LineEdit::OnReturnHit(boost::function<void()> func)
  {
    m_ReturnHitSignal.connect(func);
  }

  void LineEdit::FilterInput(uint32 unicode)
  {
    if (IsReturn(unicode)) {
      StopEditing();
      return;
    }

    if (IsBackspace(unicode) && m_InputText.getSize() != 0) {
      m_InputText.erase(m_InputText.getSize() - 1, 1);
      m_Text.setString(m_InputText);
    }
    else if (IsPrintable(unicode)) {

      switch (m_InputType)
      {
        case Type::Any:
        {
          m_InputText += unicode; break;
        }
        case Type::AlpnaNumeric:
        {
          FilterAlphaNumeric(unicode); break;
        }
        case Type::FloatingPoint:
        {
          FilterFloatingPoint(unicode); break;
        }
        case Type::Integral:
        {
          FilterIntegral(unicode); break;
        }
        case Type::Concealed:
        {
          FilterConcealed(unicode); break;
        }
        default:
        {
          m_InputText += unicode; break;
        }
      }
    }

    RepositionText();
    m_TextEnteredSignal();
  }

  void LineEdit::FilterAlphaNumeric(uint32 unicode)
  {
    if (!IsNumeric(unicode) || !IsAlpha(unicode))
      return;

    m_InputText += unicode;
    m_Text.setString(m_InputText);
  }

  void LineEdit::FilterIntegral(uint32 unicode)
  {
    if (unicode == 45 && m_InputText.getSize() != 0)
      return;

    if (!IsNumeric(unicode) || unicode != 45) return;

    m_InputText += unicode;
  }

  void LineEdit::FilterFloatingPoint(uint32 unicode)
  {
    //If not numeric and not a decimal (Full Stop) then reject the input
    if (!IsNumeric(unicode) && ( unicode != 46 ))
      return;

    //If it is a neg. sign AND the stored string is not empty, reject the input
    if (unicode == 45 && m_InputText.getSize() != 0)
      return;

    //See if there is a decimal already entered and reject the input
    // if the entered character is decimal
    auto decimalPt = m_InputText.find('.');
    if (decimalPt != std::string::npos && unicode == 46)
      return;

    //Append the input
    m_InputText += unicode;
  }

  void LineEdit::FilterConcealed(uint32 unicode)
  {
    m_InputText += unicode;
    std::string starstring = std::string(m_InputText.getSize(), '*');
    m_Text.setString(starstring);
  }

  void LineEdit::StartEditing()
  {
    StealMouseFocus(this);
    StealKeyboardFocus(this);
    SetBorderColor(sf::Color::Green);
    m_IsEditing = true;
  }

  void LineEdit::StopEditing()
  {
    ReturnMouseFocus(this);
    ReturnKeyboardFocus(this);
    SetBorderColor(m_Theme->WidgetOutlineColor);
    m_IsEditing = false;
  }

  bool LineEdit::IsPrintable(uint32 unicode)
  {
    return ( unicode >= 32 && unicode <= 126 );
  }

  bool LineEdit::IsBackspace(uint32 unicode)
  {
    return ( unicode == BS );
  }

  bool LineEdit::IsReturn(uint32 unicode)
  {
    return ( unicode == CR );
  }

  bool LineEdit::IsNumeric(uint32 unicode)
  {
    return ( unicode >= 48 && unicode <= 57 );
  }

  bool LineEdit::IsAlpha(uint32 unicode)
  {
    return ( ( unicode >= 65 && unicode <= 90 ) || ( unicode >= 97 && unicode <= 122 ) );
  }

  bool LineEdit::IsDelete(uint32 unicode)
  {
    return ( unicode == DEL );
  }

  bool LineEdit::IsSpecialCharacter(uint32 unicode)
  {
    return (
      ( unicode >= 33 && unicode <= 47 ) ||
      ( unicode >= 58 && unicode <= 64 ) ||
      ( unicode >= 91 && unicode <= 96 ) ||
      ( unicode >= 123 && unicode <= 126 )
      );
  }

  void LineEdit::RepositionText()
  {
    auto txBds = m_Text.getLocalBounds();

    float xDiff = abs( m_Size.x - txBds.width );
    float yDiff = abs( m_Size.y - txBds.height );

    Vec2i tPos = m_Position;

    tPos.x = cast_int(std::floor(m_Position.x + 5));
    tPos.y = cast_int(std::floor(m_Position.y + m_Size.y));

    if (tPos.x + txBds.width > m_Position.x + m_Size.x) {
      tPos.x = m_Position.x + m_Size.x - txBds.width;
    }

    tPos.y -= cast_int(floor(0.20 * m_Size.y));

    m_Text.setPosition(tPos);
  }

  void LineEdit::MouseEntered()
  {
    if (!m_IsEditing)
      SetBorderColor(sf::Color::White);
  }

  void LineEdit::MouseLeft()
  {
    if (!m_IsEditing)
      SetBorderColor(m_Theme->WidgetOutlineColor);
  }

  void LineEdit::MousePressed(bool left, bool right)
  {
    if (left) {
      StartEditing();
    }
  }

  void LineEdit::MouseReleased(bool left, bool right)
  {

  }

  void LineEdit::Moved()
  {
    auto bds = m_Text.getLocalBounds();

    auto yDiff = abs(m_Size.y - bds.height);
    m_Text.setPosition({cast_float(m_Position.x), m_Position.y + m_Size.y - yDiff / 2.f});
    RepositionText();
  }

  void LineEdit::Resized()
  {
    auto bds = m_Text.getLocalBounds();

    auto yDiff = abs(m_Size.y - bds.height);
    m_Text.setPosition({ cast_float(m_Position.x), m_Position.y + m_Size.y - yDiff / 2.f });
    RepositionText();
  }

  void LineEdit::TextEntered(uint32 unicode)
  {
    FilterInput(unicode);
    m_Text.setString(m_InputText);
  }

  constexpr uint32 LineEdit::ESC;

  constexpr uint32 LineEdit::DEL;

  constexpr uint32 LineEdit::SP;

  constexpr uint32 LineEdit::CR;

  constexpr uint32 LineEdit::LF;

  constexpr uint32 LineEdit::HT;

  constexpr uint32 LineEdit::BS;

}  
