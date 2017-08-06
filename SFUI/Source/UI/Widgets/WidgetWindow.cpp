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
#include <SFUI/Include/UI/Widgets/WidgetWindow.h>
#include <SFUI/Include/UI/Widgets/PopupWindow.h>
#include <SFUI/Include/Application/AppLogger.h>
#include <SFUI/Include/Application/AppMain.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <ppl.h>
#include <ppltasks.h>
#include <cmath>

namespace sfui
{

  WidgetWindow::WidgetWindow(sf::RenderWindow &Window)
    : m_Window(Window),
    Widget(nullptr)
  {
    m_Size = Window.getSize();
    m_BlockingWindow = nullptr;
    m_UpdateTimeFont.loadFromFile("NotoSans-Regular.ttf");
    m_UpdateTimeText.setFont(m_UpdateTimeFont);
    m_UpdateTimeText.setFillColor(sf::Color::White);
    m_UpdateTimeText.setCharacterSize(14);
    m_UpdateTimeText.setPosition({ 1100.f, 25.f });

    m_RenderTimeText.setFont(m_UpdateTimeFont);
    m_RenderTimeText.setFillColor(sf::Color::White);
    m_RenderTimeText.setCharacterSize(14);
    m_RenderTimeText.setPosition({ 1100.f, 45.f });

    m_UpdateTimeBG.setFillColor(sf::Color::Black);
    m_UpdateTimeBG.setOutlineColor(sf::Color(122, 122, 122));
    m_UpdateTimeBG.setOutlineThickness(1);
    m_UpdateTimeBG.setPosition({ 1097.f, 22.f });
    m_UpdateTimeBG.setSize({ 300.f, 50.f });
    m_UpdateTimer.restart();
    m_UpdateTimer.pause();
  }

  WidgetWindow::~WidgetWindow()
  {
    for (auto & pWin : m_Popups) {
      if (pWin) { pWin->Destroy(); }
    }
    m_Widgets.clear();
  }

  sfui::WidgetWindow::shared_ptr WidgetWindow::Create(sf::RenderWindow &Window)
  {
    auto WWindow = std::make_shared<WidgetWindow>(Window);
    
    return WWindow;
  }

  void WidgetWindow::Add(Widget::shared_ptr widget)
  {
    widget->SetParent(this);

    auto itemIterator = m_Widgets.push_back(widget);
    widget->m_TopWindow = this;

    //If it is a top level widget, it needs to be checked first for updates and rendered last
    if (widget->IsTopLevel()) {
      m_RenderOrder.emplace_back(widget.get());

      if (itemIterator != m_Widgets.begin()) {

        //This is really not safe, but right now only 1 thread will be accessing the vector
        std::iter_swap(m_Widgets.begin(), itemIterator);
      }
    }
    else {
      if (m_RenderOrder.size() > 1)
        m_RenderOrder.emplace(m_RenderOrder.end() - 1, widget.get());
      else
        m_RenderOrder.emplace_back(widget.get());
    }
  }

  void WidgetWindow::Remove(Widget::shared_ptr widget)
  {
    //TODO: Implement container/algorithm so widgets can be removed
    //  concurrent_vector cannot remove items

    //auto it = std::find(m_Widgets.begin(), m_Widgets.end(), widget);

    //if (it != m_Widgets.end()) {
    //  m_Widgets.erase(it);
    //}
  }

  void WidgetWindow::ShowAll()
  {
    std::for_each(m_Widgets.begin(), m_Widgets.end(), [ ](Widget::shared_ptr wptr) { wptr->Show(); });
  }

  void WidgetWindow::HideAll()
  {
    std::for_each(m_Widgets.begin(), m_Widgets.end(), [ ](Widget::shared_ptr wptr) { wptr->Hide(); });
  }

  void WidgetWindow::Update()
  {
    //Still seeing which would be better
    //  Sequential might be better if the # of widgets is too
    //  low to justify the cost of starting up a parallel run
    if (m_UseParallelUpdate)
      ParallelUpdate();
    else
      SequentialUpdate();

    m_CumulativeUpdateTotal += static_cast< long long >( m_UpdateElapsedTime.QuadPart );
    m_UpdateCounts++;

    double updateAvg = ( static_cast< double >( m_UpdateTimer.getTime().asMilliseconds() ) ) / static_cast< double >( m_UpdateCounts );
    double renderAvg = ( static_cast< double >( m_CumulativeRenderTotal ) * 0.001 ) / static_cast< double >( m_RenderCounts );

    m_UpdateString.str("");
    m_UpdateString << "Average Update Time (" << ( m_UseParallelUpdate ? "Parallel" : "Sequential" ) << "): ";
    m_UpdateString.precision(2);
    m_UpdateString << updateAvg << " ms";

    m_UpdateTimeText.setString(m_UpdateString.str());

    m_UpdateString.str("");
    m_UpdateString << "Average Render Time: ";
    m_UpdateString.precision(2);
    m_UpdateString << renderAvg << " ms";

    m_RenderTimeText.setString(m_UpdateString.str());

    auto hBds = m_UpdateTimeText.getLocalBounds();
    auto rBds = m_RenderTimeText.getLocalBounds();

    float txtHeight = ( 45.f + rBds.height - 25.f );
    float txtWidth = std::max(hBds.width, rBds.width);

    if (GlobalMouseFocus) {
      GlobalMouseFocus->BaseUpdate();
    }
    if (GlobalKeyboardFocus) {
      GlobalKeyboardFocus->BaseUpdate();
    }
  }

  void WidgetWindow::ParallelUpdate()
  {
    m_UpdateTimer.resume();
    for (auto & pWin : m_Popups) {
      if (pWin && pWin->IsOpen()) {
        pWin->Update();
      }
    }

    concurrency::parallel_for_each(
      m_Widgets.begin(),
      m_Widgets.end(),
      [this](Widget::shared_ptr wptr) { wptr->BaseUpdate(); }
    );

    m_UpdateTimer.pause();
  }

  void WidgetWindow::SequentialUpdate()
  {
    m_UpdateTimer.resume();
    std::for_each(begin(m_Popups), 
                  end(m_Popups), 
                  [ ](auto &pWin) { if (pWin && pWin->IsOpen()) pWin->Update(); });

    std::for_each(begin(m_Widgets),
                  end(m_Widgets),
                  [ ](Widget::shared_ptr wPtr) { wPtr->BaseUpdate(); }
    );
    m_UpdateTimer.pause();
  }

  bool WidgetWindow::HandleEvent(sf::Event event)
  {
    Widget *mFocus = ( m_MouseFocusQueue.empty() ? nullptr : m_MouseFocusQueue.front() );
    optional<Widget::pointer> kFocus = GetChildWithKeyboardFocus();

    Widget::previousMousePositon = Widget::currentMousePosition;
    Widget::currentMousePosition = sf::Mouse::getPosition(m_Window);

    //auto uMouse = GetWidgetMouseOver();

    if (mFocus && (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::MouseMoved) ) {
      mFocus->HandleEvent(event);
      return true;
    }

    if (GlobalKeyboardFocus && ( event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased || event.type == sf::Event::TextEntered )) {
      GlobalKeyboardFocus->HandleEvent(event);
      return true;
    }

    for (auto & wptr : m_Widgets) {
      if (wptr->IsEnabled() && wptr->HandleEvent(event))
        return true;
    }

    switch (event.type)
    {
      case sf::Event::Closed: return false;

      case sf::Event::KeyPressed:
      {
        if (GlobalKeyboardFocus) {
          GlobalKeyboardFocus->KeyPressed(event.key.code);
          //GlobalKeyboardFocus->HandleKeyPressed(event.key.code);
          return true;
        }
        if (kFocus && kFocus.value()->m_AcceptsKeyPress) {
          //kFocus.value()->HandleKeyPressed(event.key.code);
          kFocus.value()->KeyPressed(event.key.code);
          return true;
        }
        break;
      } // sf::Event::KeyPressed
    
      case sf::Event::KeyReleased:
      {
        if (GlobalKeyboardFocus) {
          //GlobalKeyboardFocus->HandleKeyReleased(event.key.code);
          GlobalKeyboardFocus->KeyReleased(event.key.code);
          return true;
        }
        if (kFocus && kFocus.value()->m_AcceptsKeyRelease) {
          //kFocus.value()->HandleKeyReleased(event.key.code);
          kFocus.value()->KeyReleased(event.key.code);
          return true;
        }
        break;
      } // sf::Event::KeyReleased

      case sf::Event::TextEntered:
      {
        if (GlobalKeyboardFocus) {
          //GlobalKeyboardFocus->HandleTextEntered(event.text.unicode);
          GlobalKeyboardFocus->TextEntered(event.text.unicode);
          return true;
        }
        if (kFocus && kFocus.value()->m_AcceptsTextEntered) {
          //kFocus.value()->HandleTextEntered(event.text.unicode);
          kFocus.value()->TextEntered(event.text.unicode);
          return true;
        }
        break;
      }

      default:
      {
        break;
      }
    } // switch

    return false;
  }

  void WidgetWindow::Render()
  {
    START_PERFORMANCE_TIMING(m_RenderStartTime, m_Frequency);

    std::for_each(
      begin(m_RenderOrder),
      end(m_RenderOrder),
      [this](Widget *wPtr) 
    { 
      if (!wPtr->IsVisible()) 
        return;

      wPtr->Render(m_Window); 
      wPtr->RenderLabel(m_Window); 
    }
    );

    if (!m_MouseFocusQueue.empty()) {
      m_MouseFocusQueue.front()->Render(m_Window);
    }
    if (GlobalKeyboardFocus) {
      GlobalKeyboardFocus->Render(m_Window);
    }
    
    m_Window.draw(m_UpdateTimeBG);
    m_Window.draw(m_UpdateTimeText);
    m_Window.draw(m_RenderTimeText);

    END_PERFORMANCE_TIMING(m_RenderEndTime, m_RenderStartTime, m_Frequency, m_RenderElapsedTime);
    m_RenderCounts++;
    m_CumulativeRenderTotal += static_cast< long long >( m_RenderElapsedTime.QuadPart );
  }

  void WidgetWindow::Render(sf::RenderTarget &Target)
  {
    // meh
  }

  void WidgetWindow::PostRender()
  {
    for (auto & pWin : m_Popups) {
      if (pWin && pWin->IsOpen()) {
        pWin->DoEvents();
        pWin->Render();
      }
    }
  }

  void WidgetWindow::AddPopupWindow(PopupWindow::shared_ptr win)
  {
    m_Popups.push_back(win);
  }

  void WidgetWindow::Block()
  {
    std::cout << "WidgetWindow blocked\n";
    m_IsBeingBlocked = true;
  }

  void WidgetWindow::Unblock()
  {
    std::cout << "WidgetWindow unblocked\n";
    m_IsBeingBlocked = false;
  }

  void WidgetWindow::GiveMouseFocus(Widget *widget)
  {
    GlobalMouseFocus = widget;
  }

  void WidgetWindow::GiveKeyboardFocus(Widget *widget)
  {
    GlobalKeyboardFocus = widget;
  }

  void WidgetWindow::RelinquishMouseFocus()
  {
    GlobalMouseFocus = nullptr;
  }

  void WidgetWindow::RelinquishKeyboardFocus()
  {
    GlobalKeyboardFocus = nullptr;
  }

  void WidgetWindow::StealMouseFocus(Widget *widget)
  {
    GlobalMouseFocus = widget;
    m_MouseFocusQueue.push_front(widget);
  }

  void WidgetWindow::ReturnMouseFocus(Widget *widget)
  {
    GlobalMouseFocus = nullptr;
    auto it = m_MouseFocusQueue.begin();
    while (it != m_MouseFocusQueue.end() && *it != widget)
      it++;

    if (it != m_MouseFocusQueue.end())
      m_MouseFocusQueue.erase(it);
  }

  void WidgetWindow::StealKeyboardFocus(Widget *widget)
  {
    GlobalKeyboardFocus = widget;
  }

  void WidgetWindow::ReturnKeyboardFocus(Widget *widget)
  {
    GlobalKeyboardFocus = nullptr;
  }

  void WidgetWindow::UseParallelUpdate(bool UseParallel)
  {
    m_UseParallelUpdate = UseParallel;
    
    //And reset the cumulative totals whenever we switch update methods
    m_UpdateTimer.restart();
    m_UpdateTimer.pause();
    m_CumulativeRenderTotal = 0;
    m_CumulativeRenderTotal = 0;
    m_RenderCounts = 0;
    m_UpdateCounts = 0;
  }

  void WidgetWindow::ChildTakingMouseFocus(Widget::pointer child)
  {
    m_ChildWithMouseFocus = child;
  }

  void WidgetWindow::ChildTakingKeyboardFocus(Widget::pointer child)
  {

  }

  void WidgetWindow::ChildReleasingMouseFocus(Widget::pointer child)
  {
    m_ChildWithMouseFocus = nullptr;
  }

  void WidgetWindow::ChildReleasingKeyboardFocus(Widget::pointer child)
  {
    m_ChildWithKeyboardFocus = child;
  }

  optional<Widget::shared_ptr> WidgetWindow::GetWidgetMouseOver() const
  {
    Widget::shared_ptr sptr = nullptr;

    for (auto & wptr : m_Widgets) {
      if (wptr->Bounds().contains(currentMousePosition)) {
        sptr = wptr;
        break;
      }
    }

    if (sptr != nullptr) return sptr;

    return std::optional<Widget::shared_ptr>();
  }

  void WidgetWindow::Cleanup()
  {
    std::for_each(m_Widgets.begin(), m_Widgets.end(), [ ](auto wptr) { wptr->Cleanup(); });
  }

  void WidgetWindow::AppExit()
  {

  }

  void WidgetWindow::MoveChildToFront(Widget::pointer child)
  {
    auto wptr = std::find_if(m_Widgets.begin(), m_Widgets.end(),
                                        [this, &child](Widget::shared_ptr ptr) { return ( ptr.get() == child ); });
    
    if (wptr != m_Widgets.end()) {

      if (m_Widgets.front() == *wptr)
        return;

      std::iter_swap(wptr, m_Widgets.begin());
    }
  }

  void WidgetWindow::MoveChildToBack(Widget::pointer child)
  {
    auto wptr = std::find_if(m_Widgets.begin(), m_Widgets.end(),
                             [this, &child](Widget::shared_ptr ptr) { return ( ptr.get() == child ); });

    if (wptr != m_Widgets.end()) {

      if (m_Widgets.back() == *wptr)
        return;

      std::iter_swap(wptr, m_Widgets.end() - 1);
    }
  }

}
