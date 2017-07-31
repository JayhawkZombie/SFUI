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
#include <SFUI/Include/UI/Theme.h>
#include <SFUI/Include/UI/UI.h>
#include <SFUi/Include/UI/Components/TextView.h>
#include <SFUI/Include/Application/AppLogger.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{

  sfui::Theme::shared_ptr Theme::Create()
  {
    return std::make_shared<Theme>();
  }

  _shared_ptr<sfui::Button> Theme::MakeButton(const std::string &Text, optional<Widget*> parent)
  {
    APP_LOG_WIDGET_CONSTRUCTION(Button);
    Button::shared_ptr btn = std::make_shared<Button>(this, parent);
    btn->SetBackgroundColor(WidgetBackgroundColor);
    btn->SetBorderColor(WidgetOutlineColor);
    btn->SetBorderWidth(WidgetOutlineWidth);
    Color tColor = WIDGET_STYLE_COLOR_LIGHT_GREY;
    tColor.a = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    btn->SetTextColor(tColor);
    btn->SetTextSize(Subheading);
    btn->SetTextAlignment(TextAlignment::Centered);
    btn->SetBitmapFont(DefaultBitmapFont);
    btn->SetText(Text);
    return btn;
  }

  _shared_ptr<sfui::Button> Theme::MakeButton(const std::string &Text, uint32 textSize, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(Button);
    Button::shared_ptr btn = std::make_shared<Button>(this, parent);
    btn->SetBackgroundColor(WidgetBackgroundColor);
    btn->SetBorderColor(WidgetOutlineColor);
    btn->SetBorderWidth(WidgetOutlineWidth);
    Color tColor = WIDGET_STYLE_COLOR_LIGHT_GREY;
    tColor.a = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    btn->SetTextColor(tColor);
    btn->SetTextSize(textSize);
    btn->SetTextAlignment(TextAlignment::Centered);
    btn->SetBitmapFont(DefaultBitmapFont);
    btn->SetText(Text);
    return btn;
  }

  _shared_ptr<sfui::Button> Theme::MakeIconButton(texture_handle tex, IntRect iconRect, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(Button);
    Button::shared_ptr btn = Button::CreateIcon(tex, iconRect, this, parent);
    btn->SetBackgroundColor(WidgetBackgroundColor);
    return btn;
  }

  _shared_ptr<sfui::Selectable> Theme::MakeSelectableButton(const std::string &Text, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(Selectable);
    Selectable::shared_ptr sel = Selectable::Create(this, parent);
    sel->SetBackgroundColor(WidgetBackgroundColor);
    sel->SetBorderColor(WidgetOutlineColor);
    sel->SetBorderWidth(WidgetOutlineWidth);
    Color tColor = WIDGET_STYLE_COLOR_LIGHT_GREY;
    tColor.a = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    sel->SetTextColor(tColor);
    sel->SetTextSize(12);
    sel->SetTextAlignment(TextAlignment::Left);
    sel->SetBitmapFont(DefaultBitmapFont);
    sel->SetText(Text);
    return sel;
  }

  _shared_ptr<sfui::Tab> Theme::MakeTab(TabList* list, const std::string &Text)
  {
    APP_LOG_WIDGET_CONSTRUCTION(Tab);
    Tab::shared_ptr tPtr = Tab::Create(Text, list, this);
    tPtr->SetBackgroundColor(WidgetBackgroundColor);
    tPtr->SetBorderColor(WidgetOutlineColor);
    tPtr->SetBorderWidth(WidgetOutlineWidth);
    tPtr->SetBitmapFont(DefaultBitmapFont);
    tPtr->SetTextSize(12);
    tPtr->SetText(Text);
    tPtr->SetTextAlignment(TextAlignment::Centered);
    Color tColor = WIDGET_STYLE_COLOR_LIGHT_GREY;
    tColor.a = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    tPtr->SetTextColor(tColor);
    return tPtr;
  }

  _shared_ptr<sfui::Panel> Theme::MakePanel(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(Panel);
    Panel::shared_ptr pPtr = Panel::Create(this, parent);
    pPtr->SetBackgroundColor(WidgetBackgroundColor);
    pPtr->SetBorderWidth(WidgetOutlineWidth);
    pPtr->SetBorderColor(WidgetOutlineColor);
    return pPtr;
  }

  _shared_ptr<sfui::TabList> Theme::MakeTabList(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(TabList);
    TabList::shared_ptr tList = TabList::Create(parent, this);
    tList->SetBackgroundColor(WidgetBackgroundColor);
    tList->SetBorderColor(WidgetOutlineColor);
    tList->SetBorderWidth(WidgetOutlineWidth);
    tList->SetBitmapFont(DefaultBitmapFont);
    return tList;
  }

  _shared_ptr<sfui::Popup> Theme::MakePopup(WidgetWindow* stealFocus)
  {
    APP_LOG_WIDGET_CONSTRUCTION(Popup);
    Popup::shared_ptr pPtr = Popup::Create(stealFocus, this);
    pPtr->SetBackgroundColor(WidgetBackgroundColor);
    pPtr->SetBorderColor(WidgetOutlineColor);
    pPtr->SetBorderWidth(3);
    return pPtr;
  }

  _shared_ptr<sfui::ScrollBar> Theme::MakeScrollBar(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(ScrollBar);
    ScrollBar::shared_ptr sBar = ScrollBar::Create(this, parent);
    if (parent) {
      sBar->m_TopWindow = ( *parent )->m_TopWindow;
    }
    sBar->SetBackgroundColor(WidgetBackgroundColor);
    sBar->SetBorderColor(WidgetOutlineColor);
    sBar->SetBorderWidth(WidgetOutlineWidth);
    return sBar;
  }

  _shared_ptr<sfui::Slider> Theme::MakeSlider(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(Slider);
    Slider::shared_ptr slid = Slider::Create(this, parent);
    slid->SetBackgroundColor(WidgetBackgroundColor);
    slid->SetBorderColor(WidgetOutlineColor);
    slid->SetBorderWidth(WidgetOutlineWidth);
    return slid;
  }

  _shared_ptr<sfui::LineItem> Theme::MakeLineItem(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(LineItem);
    LineItem::shared_ptr item = LineItem::Create(this, parent);
    Color tColor = WIDGET_STYLE_COLOR_LIGHT_GREY;
    tColor.a = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    item->SetTextColor(tColor);
    item->SetTextSize(12);
    item->SetBackgroundColor(sf::Color::Transparent);
    item->SetHoverColor(WidgetBackgroundColor + sf::Color(15, 15, 15));
    item->SetBitmapFont(DefaultBitmapFont);
    item->SetTextAlignment(TextAlignment::Left);
    return item;
  }

  _shared_ptr<sfui::MenuBar> Theme::MakeMenuBar(WidgetWindow* WWindow)
  {
    APP_LOG_WIDGET_CONSTRUCTION(MenuBar);
    MenuBar::shared_ptr mPtr = MenuBar::Create(WWindow, this);
    mPtr->SetBackgroundColor(WidgetBackgroundColor);
    mPtr->SetBorderColor(WidgetOutlineColor);
    mPtr->SetBorderWidth(WidgetOutlineWidth);
    mPtr->SetPosition({ 0, 0 });
    mPtr->SetSize({ WWindow->GetSize().x, 15 });
    return mPtr;
  }

  _shared_ptr<sfui::ListView> Theme::MakeListView(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(ListView);
    ListView::shared_ptr lView = ListView::Create(this, parent);
    lView->SetBackgroundColor(WidgetBackgroundColor);
    return lView;
  }

  _shared_ptr<sfui::CollapsingHeader> Theme::MakeCollapsingHeader(const std::string &Text, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(CollapsingHeader);
    CollapsingHeader::shared_ptr cHeader = CollapsingHeader::Create(this, parent);
    cHeader->SetBackgroundColor(WidgetBackgroundColor);
    Color tColor = WIDGET_STYLE_COLOR_LIGHT_GREY;
    tColor.a = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    cHeader->SetTextColor(tColor);
    cHeader->SetTextSize(12);
    cHeader->SetTextAlignment(TextAlignment::Left);
    cHeader->SetBitmapFont(DefaultBitmapFont);
    cHeader->SetText(Text);
    return cHeader;
  }

  _shared_ptr<sfui::ComboBox> Theme::MakeComboBox(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(ComboBox);
    ComboBox::shared_ptr cBox = ComboBox::Create(this, parent);
    cBox->SetBackgroundColor(WidgetBackgroundColor);
    cBox->SetBorderColor(WidgetOutlineColor);
    cBox->SetBorderWidth(1);
    Color tColor = WIDGET_STYLE_COLOR_LIGHT_GREY;
    tColor.a = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    cBox->SetTextColor(tColor);
    cBox->SetTextSize(14);
    cBox->SetTextAlignment(TextAlignment::Left);
    cBox->SetBitmapFont(DefaultBitmapFont);
    cBox->SetText("combo box");
    return cBox;
  }

  _shared_ptr<sfui::LineEdit> Theme::MakeLineEdit(const std::string &fontfamily, uint32 fontsize, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(LineEdit);
    LineEdit::shared_ptr lEdit = LineEdit::Create(this, parent);

    auto fnt = BitmapFonts.GetFont(fontfamily, fontsize);

    if (fnt.has_value())
      lEdit->SetBitmapFont(*fnt);
    else
      std::cerr << "Unable to assign font \"" << fontfamily << "\" of size \"" << fontsize << "\"\n";

    Color tColor = WIDGET_STYLE_COLOR_LIGHT_GREY;
    tColor.a = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    lEdit->SetBitmapFontColor(sf::Color::White);
    lEdit->SetBackgroundColor(WidgetBackgroundColor);
    lEdit->SetBorderColor(WidgetOutlineColor);
    lEdit->SetBorderWidth(1);
    return lEdit;
  }

  _shared_ptr<sfui::ValueSlider> Theme::MakeValueSlider(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(ValueSlider);
    ValueSlider::shared_ptr vSlider = ValueSlider::Create(this, parent);
    vSlider->SetBackgroundColor(WidgetBackgroundColor);
    vSlider->SetBorderColor(WidgetOutlineColor);
    vSlider->SetBorderWidth(1);
    return vSlider;
  }

  _shared_ptr<sfui::RainbowStrip> Theme::MakeRainbowStrip(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(RainbowStrip);
    RainbowStrip::shared_ptr rStrip = RainbowStrip::Create(this, parent);
    return rStrip;
  }

  _shared_ptr<sfui::ColorBox> Theme::MakeColorBox(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(ColorBox);
    ColorBox::shared_ptr cBox = ColorBox::Create(this, parent);
    cBox->SetBorderColor(WidgetOutlineColor);
    cBox->SetBorderWidth(1);
    return cBox;
  }

  _shared_ptr<sfui::BitmapLabel> Theme::MakeBitmapLabel(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(BitmapLabel);
    BitmapLabel::shared_ptr bmLabel = BitmapLabel::Create(this, parent);
    bmLabel->SetBMFont(DefaultBitmapFont);
    bmLabel->SetBMText("");
    return bmLabel;
  }

  _shared_ptr<sfui::ColorPicker> Theme::MakeColorPicker(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(ColorPicker);
    ColorPicker::shared_ptr cPicker = ColorPicker::Create(this, parent);
    cPicker->SetBackgroundColor(WidgetBackgroundColor);
    cPicker->SetBorderColor(WidgetOutlineColor);
    cPicker->SetBorderWidth(WidgetOutlineWidth);
    return cPicker;
  }

  _shared_ptr<sfui::WidgetTree> Theme::MakeTree(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(WidgetTree);
    WidgetTree::shared_ptr wTree = WidgetTree::Create(this, parent);
    wTree->SetBackgroundColor(WidgetBackgroundColor);
    wTree->SetBorderColor(WidgetOutlineColor);
    wTree->SetBorderWidth(WidgetOutlineWidth);
    return wTree;
  }

  _shared_ptr<sfui::SpinBox> Theme::MakeSpinBox(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(SpinBox);
    SpinBox::shared_ptr sBox = SpinBox::Create(this, parent);
    sBox->SetBackgroundColor(WidgetBackgroundColor);
    return sBox;
  }

  _shared_ptr<sfui::LoadingSpinner> Theme::MakeLoadingSpinner(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(LoadingSpinner);
    LoadingSpinner::shared_ptr lSpinner = LoadingSpinner::Create(this, parent);
    
    return lSpinner;
  }

  _shared_ptr<sfui::ProgressBar> Theme::MakeProgressBar(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(ProgressBar);
    ProgressBar::shared_ptr pBar = ProgressBar::Create(this, parent);
    pBar->SetBackgroundColor(WidgetBackgroundColor);
    pBar->SetBorderColor(WidgetOutlineColor);
    pBar->SetBorderWidth(WidgetOutlineWidth);
    Color tColor = WIDGET_STYLE_COLOR_LIGHT_GREY;
    tColor.a = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    pBar->SetTextColor(tColor);
    pBar->SetTextSize(Subheading);
    pBar->SetTextAlignment(TextAlignment::Left);
    pBar->SetBitmapFont(DefaultBitmapFont);
    pBar->SetText("0 %");
    return pBar;
  }

  _shared_ptr<sfui::CheckBox> Theme::MakeCheckBox(const std::string &Text, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(CheckBox);
    CheckBox::shared_ptr cBox = CheckBox::Create(this, parent);
    cBox->SetBackgroundColor(WidgetBackgroundColor);
    cBox->SetBorderColor(WidgetOutlineColor);
    cBox->SetBorderWidth(WidgetOutlineWidth);
    Color tColor = WIDGET_STYLE_COLOR_LIGHT_GREY;
    tColor.a = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    cBox->SetTextColor(tColor);
    cBox->SetTextSize(14);
    cBox->SetTextAlignment(TextAlignment::Left);
    cBox->SetBitmapFont(DefaultBitmapFont);
    cBox->SetText(Text);
    return cBox;
  }

  _shared_ptr<Menu> Theme::MakeMenu(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    APP_LOG_WIDGET_CONSTRUCTION(Menu);
    Menu::shared_ptr menu = Menu::Create(this, parent);
    menu->SetBackgroundColor(WidgetBackgroundColor);
    menu->SetBorderColor(WidgetOutlineColor);
    menu->SetBorderWidth(WidgetOutlineWidth);
    return menu;
  }

  _shared_ptr<sfui::NavigationPanel> Theme::MakeNavPanel(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    NavigationPanel::shared_ptr nPanel = NavigationPanel::Create(this, parent);
    nPanel->SetBackgroundColor(sf::Color(0, 0, 0));
    nPanel->SetBorderColor(sf::Color(122, 122, 122, 120));
    nPanel->SetBorderWidth(1);
    return nPanel;
  }

  _shared_ptr<sfui::Preloader> Theme::MakePreloader(optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    Preloader::shared_ptr pPtr = Preloader::Create(this, parent);
    pPtr->SetBackgroundColor(sf::Color::Transparent);
    return pPtr;
  }

}
