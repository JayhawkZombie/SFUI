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
#include <SFUI/Include/UI/Widgets/ColorPicker.h>
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
  
  ColorPicker::ColorPicker(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : Panel(theme, parent)
  {
    m_HexLabel = m_Theme->MakeBitmapLabel(this);
    m_HexLabel->SetBMText("#FFFFFF");
    m_RGBLabel = m_Theme->MakeBitmapLabel(this);
    m_RGBLabel->SetBMText("(255, 255, 255");

    m_CancelBtn = m_Theme->MakeButton("Cancel", this);
    m_OKBtn = m_Theme->MakeButton("OK", this);

    m_ColorStrip = m_Theme->MakeRainbowStrip(this);
    m_OpacitySlider = m_Theme->MakeValueSlider(this);
    m_ColorBox = m_Theme->MakeColorBox(this);
    m_OpacitySlider->SetBarWidth(21);
    m_OpacitySlider->SetBarHeight(5);
    m_OpacitySlider->OnValuChanged([this]( auto val ) { OpacityChanged(val); });
    m_CancelBtn->OnClicked([this]() { CancelButtonHit(); });
    m_OKBtn->OnClicked([this]() {AcceptButtonHit(); });

    //Notify the box that the rainbow strip was changed
    m_ColorStrip->OnColorChanged([colBox = m_ColorBox.get()](auto color) { colBox->SetColorAt(1, color); });

    //Notify us when the box updates its selected color
    m_ColorBox->OnColorChanged([this](auto color) { BoxColorChanged(color); });
    m_ColorBox->SetColorAt(1, sf::Color::Red);

    m_CurrentColor = sf::Color::White;

    m_PreviewBox.setFillColor(m_CurrentColor);
    m_PreviewBox.setOutlineColor(sf::Color::Black);
    m_PreviewBox.setOutlineThickness(-1);

    Add(m_HexLabel);
    Add(m_RGBLabel);
    Add(m_CancelBtn);
    Add(m_ColorBox);
    Add(m_OKBtn);
    Add(m_ColorStrip);
    Add(m_OpacitySlider);
  }

  ColorPicker::~ColorPicker()
  {

  }

  sfui::ColorPicker::shared_ptr ColorPicker::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<ColorPicker>(theme, parent);
  }

  Color ColorPicker::GetColor() const
  {
    return m_CurrentColor;
  }

  void ColorPicker::Render(sf::RenderTarget &Target)
  {
    Panel::Render(Target);
    Target.draw(m_PreviewBox);
  }

  void ColorPicker::SetPosition(const Vec2i &Position)
  {
    super::SetPosition(Position);
    RepositionItems();
  }

  void ColorPicker::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);
    RepositionItems();
  }

  void ColorPicker::Move(const Vec2i &Delta)
  {
    Panel::Move(Delta);
    m_PreviewBox.move(Delta);
  }

  void ColorPicker::OnColorCancelled(boost::function<void()> func)
  {
    m_CancelledSignal.connect(func);
  }

  void ColorPicker::OnColorAccepted(boost::function<void(const Color &)> func)
  {
    m_AcceptedSignal.connect(func);
  }

  void ColorPicker::BoxColorChanged(const Color &c)
  {
    m_CurrentColor = c;
    m_CurrentColor.a = m_Opacity;
    m_PreviewBox.setFillColor(c);

    std::string newRGB = "(" + std::to_string(m_CurrentColor.r) +
      ", " + std::to_string(m_CurrentColor.g) +
      ", " + std::to_string(m_CurrentColor.b) + ")";

    m_RGBLabel->SetBMText(newRGB);

    std::string hexStr{ "#000000" };

    int dRes = m_CurrentColor.r / 16; //Intentional integer division
    hexStr[1] = CharToAlphaAscii(dRes);
    int dResRem = m_CurrentColor.r - ( dRes << 4 );
    hexStr[2] = CharToAlphaAscii(dResRem);
    dRes = m_CurrentColor.g / 16;
    hexStr[3] = CharToAlphaAscii(dRes);
    dResRem = m_CurrentColor.g - ( dRes << 4 );
    hexStr[4] = CharToAlphaAscii(dResRem);
    dRes = m_CurrentColor.b / 16;
    hexStr[5] = CharToAlphaAscii(dRes);
    dResRem = m_CurrentColor.b - ( dRes << 4 );
    hexStr[6] = CharToAlphaAscii(dResRem);

    m_HexLabel->SetBMText(hexStr);
  }

  void ColorPicker::CancelButtonHit()
  {
    m_CancelledSignal();
  }

  void ColorPicker::AcceptButtonHit()
  {
    m_AcceptedSignal(m_CurrentColor);
  }

  void ColorPicker::OpacityChanged(float perc)
  {
    m_Opacity = static_cast< sf::Uint8 >( round(255.f * perc) );
    m_CurrentColor.a = m_Opacity;
    m_PreviewBox.setFillColor(m_CurrentColor);
  }

  void ColorPicker::RepositionItems()
  {
    int cBoxSize = m_Size.x - 70;
    int spacing = 10;

    int curHoriz = m_Position.x + spacing;
    int curVert = m_Position.y + spacing;

    m_ColorBox->SetPosition({ curHoriz, curVert });
    m_ColorBox->SetSize({ cBoxSize, cBoxSize });

    curHoriz += cBoxSize + spacing;

    m_ColorStrip->SetPosition({ curHoriz, curVert });
    m_ColorStrip->SetSize({ 15, cBoxSize - 35 });

    m_PreviewBox.setPosition(Vec2i(curHoriz, m_Position.y + cBoxSize - 25 + spacing));
    m_PreviewBox.setSize(Vec2i(45, 25));

    curHoriz += 15 + spacing;

    m_OpacitySlider->SetPosition({ curHoriz, curVert });
    m_OpacitySlider->SetSize({ 15, cBoxSize - 35 });    

    curVert += cBoxSize + 15;
    curHoriz = m_Position.x + spacing;

    int _25PercWidth = static_cast< int >( floor(0.25 * m_Size.x) );

    int _35PercWidth = static_cast< int >( floor(0.35 * m_Size.x) );
    int _10PercWidth = static_cast< int >( floor(0.10 * m_Size.x) );

    m_RGBLabel->SetPosition({ curHoriz, curVert });
    m_RGBLabel->SetSize({ _25PercWidth, 15 });
    curHoriz += _35PercWidth + spacing + _10PercWidth;
    m_HexLabel->SetPosition({ curHoriz, curVert });
    m_HexLabel->SetSize({ _25PercWidth, 15 });

    curVert += 15;

    curHoriz = m_Position.x + _10PercWidth;
    m_CancelBtn->SetPosition({ curHoriz, curVert });
    m_CancelBtn->SetDefaultSize({ _35PercWidth, 15 });

    curHoriz += _35PercWidth + _10PercWidth;
    m_OKBtn->SetPosition({ curHoriz, curVert });
    m_OKBtn->SetDefaultSize({ _35PercWidth, 15 });
  }

}  
