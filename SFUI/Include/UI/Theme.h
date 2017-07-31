#ifndef SFUI_THEME_H
#define SFUI_THEME_H

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
#include <SFUI/Include/Common.h>
#include <SFUI/Include/UI/Fwd.h>
#include <SFUI/Include/UI/Components/Colors.h>
#include <SFUI/Include/UI/Components/BitmapFontStore.h>
#include <SFUI/Include/Application/AppLogger.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

#if defined ( LOG_WIDGET_CREATION )
#define APP_LOG_WIDGET_CONSTRUCTION(WIDGET_CLASS) \
APP_LOG("Construction: Widget (Class = " ## #WIDGET_CLASS ##")")
#else

#endif //LOG_WIDGET_CREATION

namespace sfui
{  

  class Theme
  {
  public:

    using pointer = Theme*;
    using reference = Theme&;
    using const_pointer = const Theme*;
    using const_reference = const Theme&;
    using shared_ptr = _shared_ptr<Theme>;

    static shared_ptr Create();

    _shared_ptr<Button> MakeButton(const std::string &Text, optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<Button> MakeButton(const std::string &Text, uint32 textSize, optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<Button> MakeIconButton(texture_handle tex, IntRect iconRect, optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<Selectable> MakeSelectableButton(const std::string &Text, optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<Tab> MakeTab(TabList *list, const std::string &Text);
    _shared_ptr<Panel> MakePanel(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<TabList> MakeTabList(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<Popup> MakePopup(WidgetWindow* stealFocus);
    _shared_ptr<ScrollBar> MakeScrollBar(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<Slider> MakeSlider(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<LineItem> MakeLineItem(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<MenuBar> MakeMenuBar(WidgetWindow* WWindow);
    _shared_ptr<ListView> MakeListView(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<CollapsingHeader> MakeCollapsingHeader(const std::string &Text, optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<ComboBox> MakeComboBox(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<LineEdit> MakeLineEdit(const std::string &fontfamily, uint32 fontsize, optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<ValueSlider> MakeValueSlider(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<RainbowStrip> MakeRainbowStrip(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<ColorBox> MakeColorBox(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<BitmapLabel> MakeBitmapLabel(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<ColorPicker> MakeColorPicker(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<WidgetTree> MakeTree(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<SpinBox> MakeSpinBox(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<LoadingSpinner> MakeLoadingSpinner(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<ProgressBar> MakeProgressBar(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<CheckBox> MakeCheckBox(const std::string &Text, optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<Menu> MakeMenu(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<NavigationPanel> MakeNavPanel(optional<Widget*> parent = optional<Widget*>());
    _shared_ptr<Preloader> MakePreloader(optional<Widget*> parent = optional<Widget*>());

    static const std::string UserOptionalSkip;
    static const std::string UserActionCancel;
    static const std::string UserActionComplete;
    static const std::string UserMoreInfo;
    static const std::string UserOptionalDecline;
    static const std::string UserOptionAccept;
    static const std::string UserOptionPostpone;
    static const std::string UserMessageAcknowledge;
    static const std::string SadMessageUnknownError;
    static const std::string SadMessageCrash;
    static const std::string SadMessageInvalidAction;

    font_handle DefaultFont = nullptr;
    font_handle CodeFont = nullptr;
    
    std::shared_ptr<sw::BitmapFont> DefaultBitmapFont = nullptr;

    BitmapFontCollection BitmapFonts;

    texture_handle IconTexture = nullptr;

    /* Info on text sizes */
    uint32 TextSizeDisplay4 = 48;
    uint32 TextSizeDisplay3 = 36;
    uint32 TextSizeDisplay2 = 28;
    uint32 TextSizeDisplay1 = 22;
    uint32 TextSizeHeadline = 18;
    uint32 TextSizeTitle = 16;
    uint32 TextSizeSubheading = 12;
    uint32 TextSizeBody1 = 12;
    uint32 TextSizeBody2 = 12;
    uint32 TextSizeCaption = 10;
    uint32 TextSizeMenu = 12;
    uint32 TextSizeButtonLarge = 16;
    uint32 TextSizeButtonSmall = 12;

    uint32 TextOpacityDisplay4 = TEXT_OPACITY_HIGH_CONTRAST_LARGE_TEXT;
    uint32 TextOpacityDisplay3 = TEXT_OPACITY_HIGH_CONTRAST_LARGE_TEXT;
    uint32 TextOpacityDisplay2 = TEXT_OPACITY_HIGH_CONTRAST_LARGE_TEXT;
    uint32 TextOpacityDisplay1 = TEXT_OPACITY_HIGH_CONTRAST_LARGE_TEXT;
    uint32 TextOpacityHeadline = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    uint32 TextOpacityTitle = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    uint32 TextOpacitySubheading = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    uint32 TextOpacityBody1 = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    uint32 TextOpacityBody2 = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    uint32 TextOpacityCaption = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    uint32 TextOpacityMenu = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    uint32 TextOpacityButtonLarge = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;
    uint32 TextOpacityButtonSmall = TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT;

    sf::Color WidgetOutlineColor = WIDGET_STYLE_COLOR_MEDIUM_DARK;
    sf::Color WidgetBackgroundColor = WIDGET_STYLE_COLOR_VERY_DARK;
    uint32 WidgetOutlineWidth = 1;

  };

}  

#endif // SFUI_THEME_H
