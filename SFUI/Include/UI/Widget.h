#ifndef SFUI_WIDGET_H
#define SFUI_WIDGET_H

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
#include <SFUI/Include/UI/Components/TextView.h>
#include <SFUI/Include/Utilities/DropShadow.h>
#include <SFUI/Include/UI/Animation/WidgetAnimator.h>
#include <SFUI/Include/Utilities/ExecutionQueue.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////
#include <boost/signals2.hpp>
#include <boost/function.hpp>

#include <Kairos.hpp>

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

#define WIDGET_TYPE_TRAITS(W_CLASS)               \
using self_type       = W_CLASS;                  \
using pointer         = W_CLASS##*;               \
using reference       = W_CLASS##&;               \
using const_pointer   = const W_CLASS##*;         \
using const_reference = const W_CLASS##&;         \
using shared_ptr      = std::shared_ptr<W_CLASS>; \
using unique_ptr      = std::unique_ptr<W_CLASS>; \

#define WIDGET_DERIVED(W_CLASS, W_PARENT) \
WIDGET_TYPE_TRAITS(W_CLASS);              \
using super = W_PARENT;                   \
virtual std::string Class() const override { return std::string( #W_CLASS ); }

namespace sfui
{  
  
  template<typename SigRet_t, typename ... SigArgs_t>
  using Signal = boost::signals2::signal<SigRet_t, SigArgs_t...>;

  enum Event : uint64
  {
    MouseMove = 0b01,
    MousePress = MouseMove << 1,
    MouseRelease = MousePress << 1,
    MouseOver = MouseRelease << 1,
    MouseExit = MouseOver << 1,
    KeyPress = MouseExit << 1,
    KeyRelease = KeyPress << 1,
    TextEntered = KeyRelease << 1,
    LongHover = TextEntered << 1,
    MouseClick = LongHover << 1,
    MouseDoubleClick = MouseClick << 1,
    Default = MouseMove | MousePress | MouseRelease | MouseOver | MouseExit
            | LongHover | MouseClick,
    All = Default | MouseDoubleClick | KeyPress | KeyRelease | TextEntered,
    Count
  };

  class Theme;
  class WidgetWindow;
  class Menu;

  class Widget 
  {
  public:
    WIDGET_TYPE_TRAITS(Widget);

    friend class WidgetWindow;
    friend class Theme;
    friend class  Menu;
    friend class GenericContainer;
    friend class Tab;
    friend class TabList;
    friend class TabWidget;
    friend class Panel;

    virtual ~Widget( );

    Widget(const_reference) = delete;
    Widget(const Widget &&) = delete;
    Widget(Widget &&) = delete;
    Widget& operator=(const Widget&) = delete;
    Widget& operator=(Widget &&) = delete;

    bool IsKeyDown(Key key) const;
    bool AreKeysDown(const std::vector<Key> &keys) const;
    bool IsLeftMouseDown( ) const;
    bool IsRightMouseDown( ) const;

    Widget(optional<Theme*> theme, std::optional<pointer> parent = nullptr, uint64 events = Event::Default);

    virtual void SetPosition(const Vec2i &Position);
    virtual void SetSize(const Vec2i &Size);
    virtual void SetDefaultSize(const Vec2i &Size);
    virtual void SetExpandSizeOffset(const Vec2i &Size);
    virtual void Move(const Vec2i &Delta);
    void SetParent(pointer parent);
    Vec2i GetPosition() const;
    Vec2i GetSize() const;

    virtual void SetText(const std::string &Text);
    void SetTextColor(const Color &c);
    void SetTextSize(uint32 Size);
    void SetTextAlignment(TextAlignment alignment);
    void SetFont(font_handle HFont);
    void SetBitmapFont(std::shared_ptr<sw::BitmapFont> fnt);

    const std::string& GetText() const;
    Vec2i GetTextPosition() const;
    Vec2i GetTextSize() const;
    
    sf::View View( ) const;

    bool IsModal( ) const;
    bool HasMouseFocus( ) const;
    bool HasKeyboardFocus( ) const;
    bool HasParent( ) const;
    bool IsTopLevel() const;
    std::optional<pointer> GetParent( ) const;

    void TakeParentMouseFocus(Widget *widget);
    void TakeParentKeyboardFocus(Widget *widget);

    void TakeMouseFocus( );
    virtual void TakeGlobalMouseFocus(Widget *widget);
    void ReleaseMouseFocus( );
    void ReleaseGlobalMouseFocus();
    void TakeKeyboardFocus( );
    virtual void TakeGlobalKeyboardFocus(Widget *widget);
    void ReleaseKeyboardFocus( );
    void ReleaseGlobalKeyboardFocus();

    void SetChildWithMouseFocus(pointer child);
    void SetChildWithKeyboardFocus(pointer child);

    std::optional<pointer> GetChildWithMouseFocus( ) const;
    std::optional<pointer> GetChildWithKeyboardFocus( ) const;

    void Enable( );
    void Disable( );
    void Show( );
    void Hide( );

    bool IsEnabled( ) const;
    bool IsVisible( ) const;
    bool IsMouseOver( ) const;

    void SetHoverDelay(ms delay);
    ms GetHoverDelay( ) const;
    
    void SetTooltip(shared_ptr widget);
    optional<Widget::shared_ptr> GetTooltip( ) const;
    void RemoveTooltip( );
    FloatRect Bounds( ) const;
    
    void SetBackgroundColor(const Color &c);
    void SetBorderColor(const Color &c);
    void SetBorderWidth(int32 Width);
    Color GetBackgroundColor( ) const;

    virtual void MoveToFront( );
    virtual void MoveToBack( );

    /* Bind functions to callbacks */
    void OnMouseOver(boost::function<void( )> func);
    void OnMouseExit(boost::function<void( )> func);
    void OnLeftMousePress(boost::function<void( )> func);
    void OnRightMousePress(boost::function<void( )> func);
    void OnLeftMouseRelease(boost::function<void( )> func);
    void OnRightMouseRelease(boost::function<void( )> func);
    void OnKeyPress(boost::function<void(Key)> func);
    void OnKeyRelease(boost::function<void(Key)> func);
    void OnMouseFocusGained(boost::function<void( )> func);
    void OnMouseFocusLost(boost::function<void( )> func);
    void OnMouseHover(boost::function<void( )> func);

    virtual void Cleanup( );
    virtual void Update();
    virtual void Render(sf::RenderTarget &Target);
    virtual void BaseUpdate();
    virtual void SetLabel(const std::string &Text);

    virtual bool HandleEvent(const sf::Event &event);
    virtual void SetTopWindow(WidgetWindow *TopWindow);
    WidgetWindow *GetTopWindow() const;

    virtual void StealMouseFocus(Widget *widget);
    virtual void ReturnMouseFocus(Widget *widget);
    virtual void StealKeyboardFocus(Widget *widget);
    virtual void ReturnKeyboardFocus(Widget *widget);

    void SetCanAnimateExpand(bool can);
    void SetCanAnimateContract(bool can);
    void SetCanAnimateBounce(bool can);
    void SetCanAnimateSlide(bool can);
    void SetCanAnimateFade(bool can);
    void DisableAnimations();

    virtual void Animate(WidgetAnimation Animation, const Vec2i &StartValue, const Vec2i &EndValue, Easing curve, uint32 Duration);
    virtual std::string Class() const = 0;
    void RenderLabel(sf::RenderTarget &Target);

    enum class Highlight
    {
      Left,
      Top,
      Right,
      Bottom,
      Fill
    };
    virtual void SetHighlight(Highlight hLight, const Color &c, int Width = 3);

  protected:
    static Vec2i currentMousePosition;
    static Vec2i previousMousePositon;
    
    virtual void ChildTakingMouseFocus(pointer child);
    virtual void ChildTakingKeyboardFocus(pointer child);
    virtual void ChildReleasingMouseFocus(pointer child);
    virtual void ChildReleasingKeyboardFocus(pointer child);

    /* virtual event handlers */
    virtual void MouseMoved();
    virtual void MousePressed(bool left, bool right);
    virtual void MouseReleased(bool left, bool right);
    virtual void MouseEntered();
    virtual void MouseLeft();
    virtual void MouseClicked();
    virtual void KeyPressed(Key key);
    virtual void KeyReleased(Key key);
    virtual void TextEntered(uint32 unicode);
    virtual void Moved();
    virtual void Resized();
    virtual void GainedMouseFocus();
    virtual void GainedKeyboardFocus();
    virtual void LostMouseFocus();
    virtual void LostKeyboardFocus();
    virtual void Enabled();
    virtual void Disabled();
    virtual void Hidden();
    virtual void Shown();
    virtual void Hovered();

    virtual void MoveChildToFront(pointer child);
    virtual void MoveChildToBack(pointer child);

    Signal<void()> m_MouseOverSignal;
    Signal<void()> m_MouseExitSignal;
    Signal<void()> m_LeftMousePressedSignal;
    Signal<void()> m_LeftMouseReleasedSignal;
    Signal<void()> m_RightMousePressedSignal;
    Signal<void()> m_RightMouseReleasedSignal;
    Signal<void(Key)> m_KeyPressedSignal;
    Signal<void(Key)> m_KeyReleasedSignal;
    Signal<void()> m_EnabledSignal;
    Signal<void()> m_DisabledSignal;
    Signal<void()> m_MouseFocusGainedSignal;
    Signal<void()> m_MouseFocusLostSignal;
    Signal<void()> m_KeyboardFocusGainedSignal;
    Signal<void()> m_KeyboardFocusLostSignal;
    Signal<void()> m_MouseHoverSignal;

    bool m_AcceptsMouseMove = false;
    bool m_AcceptsMouseOver = false;
    bool m_AcceptsMouseExit = false;
    bool m_AcceptsMousePress = false;
    bool m_AcceptsMouseRelease = false;
    bool m_AcceptsMouseHover = false;
    bool m_AcceptsKeyPress = false;
    bool m_AcceptsKeyRelease = false;
    bool m_AcceptsTextEntered = false;
    bool m_AcceptsMouseClick = false;
    bool m_AcceptsMouseDoubleClick = false;

    bool m_IsEnabled = true;
    bool m_IsVisible = true;
    bool m_IsModal = false;
    bool m_CanHaveChildren = true;
    bool m_IsLeftMouseDown = false;
    bool m_IsRightMouseDown = false;
    bool m_IsMouseOver = false;
    bool m_HasLabel = false;
    bool m_IsTopLevel = false;

    //Controlling animations
    bool m_CanAnimateExpand = true;
    bool m_CanAnimateContract = true;
    bool m_CanAnimateSlide = true;
    bool m_CanAnimateFade = true;
    bool m_CanAnimateBounce = true;

    Theme *m_Theme = nullptr;
    std::optional<TextView::shared_ptr> m_TextView;
    _shared_ptr<BitmapLabel> m_Label;
    optional<Widget::shared_ptr> m_Tooltip;

    bool m_HasMouseFocus = false;
    bool m_HasKeyboardFocus = false;

    WidgetWindow *m_TopWindow = nullptr;
    WidgetAnimator m_Animator;
    optional<pointer> m_ChildWithMouseFocus;
    optional<pointer> m_ChildWithKeyboardFocus;

    FloatRect m_Bounds = { 0.f, 0.f, 0.f, 0.f };
    Vec2i m_Position = { 0, 0 };
    Vec2i m_Size = { 0, 0 };
    Vec2i m_DefaultSize = { 0, 0 };
    Vec2i m_ExpandSizeOffset = { 0, 0 };
    Vec2i m_ContractSize = { 0, 0 };
    Vec2i m_ExpansionDelta = { 0, 0 };

    ExecutionQueue m_ActionQueue;

    PopupWindow* m_BlockingWindow = nullptr;
    optional<pointer> m_Parent = nullptr;
    Color m_BGColor = sf::Color::White;
    Color m_BorderColor = sf::Color::Transparent;
    int32 m_BorderWidth = 1;

    system_clock::time_point m_LastUpdateTick;
    system_clock::time_point m_ThisUpdateTick;
    ms m_SinceLastUpdate = ms(0);

    system_clock::time_point m_HoverBegan;
    ms m_HoverDelay = ms(2000);

    sf::RectangleShape m_BackgroundRect;
    Highlight m_Highlight = Highlight::Bottom;
    Color m_HighlightColor = sf::Color::Transparent;
    sf::RectangleShape m_HighlightRect;
    int m_HighlightThickness = 0;
    bool m_IsHighlighted = false;
    sstring m_Text = "";
  };
  
}  

#endif // SFUI_WIDGET_H
