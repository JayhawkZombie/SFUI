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
#include <SFUI/Include/UI/Widgets/PreloaderBox.h>
#include <SFUI/Include/UI/Widgets/Preloader.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  PreloaderBox::PreloaderBox(optional<Theme*> theme /* = {} */, optional<Widget*> parent /* = {} */)
    : Widget(theme, parent)
  {

  }

  PreloaderBox::~PreloaderBox()
  {

  }

  PreloaderBox::shared_ptr PreloaderBox::Create(optional<Theme*> theme /* = {} */, optional<Widget*> parent /* = {} */)
  {
    return std::make_shared<PreloaderBox>(theme, parent);
  }

  void PreloaderBox::Update()
  {
    if (m_State == State::Hidden)
      return;

    m_SeqTimer.Update();
  }

  void PreloaderBox::Render(sf::RenderTarget &Target)
  {
    if (m_State != State::Hidden && m_State != State::Delayed)
      Target.draw(m_BackgroundRect);
  }

  void PreloaderBox::SetPosition(const Vec2i &Position)
  {
    super::SetPosition(Position);
  }

  void PreloaderBox::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);
  }

  void PreloaderBox::SetSpawnPosition(const Vec2i &Position)
  {
    m_SpawnPosition = Position;
  }

  void PreloaderBox::SetStopPosition(const Vec2i &Position)
  {
    m_SitPosition = Position;
  }

  void PreloaderBox::SetDespawnPosition(const Vec2i &Position)
  {
    m_DespawnPosition = Position;
  }

  void PreloaderBox::Start(uint32 DurationToSpawn, uint32 DurationToSit, uint32 DurationToDespawn, uint32 Delay)
  {
    m_SpawnDuration.setFromMilliseconds(DurationToSpawn);
    m_SitDuration.setFromMilliseconds(DurationToSit);
    m_DespawnDuration.setFromMilliseconds(DurationToDespawn);
    m_DelayDuration.setFromMilliseconds(Delay);

    m_IsDone = false;
    SetPosition(m_SpawnPosition);

    m_SeqTimer.AddSequences(
      [ ]() { }, [ ]() { }, [this]() { Finished(); },
      {
        { /* Delay sequence node */ Delay,             [this]() { StartDelay(); },   /* No update */ [ ]() { }, /* No end cb */ [ ]() { } },
        { /* Spawn sequence node */ DurationToSpawn,   [this]() { StartSpawn(); },   /* No update */ [ ]() { }, /* No end cb */ [this]() { UpdateSpawn(); } },
        { /* Sit sequence node */   DurationToSit,     [this]() { StartSit();   },   /* No update */ [ ]() { }, /* No end cb */ [ ]() { } },
        { /* Despawn seq node */    DurationToDespawn, [this]() { StartDesapwn(); }, /* No update */ [ ]() { }, /* No end cb */ [this]() { UpdateDespawn(); } }
      }
      );

    m_SeqTimer.Start();
  }

  void PreloaderBox::Pause()
  {
    m_Timer.pause();
  }

  void PreloaderBox::Stop()
  {
    Finished();
  }

  void PreloaderBox::Restart()
  {
    m_State = State::Delayed;
    m_Timer.setTime(m_DelayDuration);
    m_Timer.start();
  }

  void PreloaderBox::Resume()
  {
    m_Timer.resume();
  }

  bool PreloaderBox::IsDone() const
  {
    return m_IsDone;
  }

  void PreloaderBox::OnFinished(boost::function<void()> func)
  {
    m_FinishedSignal.connect(func);
  }

  void PreloaderBox::StartDelay()
  {
    m_State = State::Delayed;
  }

  void PreloaderBox::StartSpawn()
  {
    m_State = State::Spawning;
    m_Timer.stop();
    m_Timer.setTime(m_SpawnDuration);
    m_Timer.start();
    Color bgC = m_BackgroundRect.getFillColor();
    bgC.a = 0;
    m_BackgroundRect.setFillColor(bgC);

    m_Animator.Animate(
      WidgetAnimation::SlideIn,
      m_SpawnPosition,
      m_SitPosition,
      Easing::Accelerate,
      ( uint32 )m_SpawnDuration.asMilliseconds()
    );
  }

  void PreloaderBox::StartSit()
  {
    m_State = State::Sitting;
    m_Timer.stop();
    m_Timer.setTime(m_SitDuration);
    m_Timer.start();
    m_Animator.Stop();
    Color bgC = m_BackgroundRect.getFillColor();
    bgC.a = 255;
    m_BackgroundRect.setFillColor(bgC);
    SetPosition(m_SitPosition);
  }

  void PreloaderBox::StartDesapwn()
  {
    m_State = State::Despawning;
    m_Timer.stop();
    m_Timer.setTime(m_DespawnDuration);
    m_Timer.start();
    m_Animator.Animate(
      WidgetAnimation::SlideIn,
      m_SitPosition,
      m_DespawnPosition,
      Easing::Accelerate,
      ( uint32 )m_DespawnDuration.asMilliseconds()
    );
  }

  void PreloaderBox::Finished()
  {
    m_State = State::Hidden;
    m_SeqTimer.Clear();
    Color bgC = m_BackgroundRect.getFillColor();
    bgC.a = 255;
    m_BackgroundRect.setFillColor(bgC);
    m_IsDone = true;
    m_FinishedSignal();
  }

  void PreloaderBox::UpdateSpawn()
  {
    Color bgC = m_BackgroundRect.getFillColor();
    auto totTime = m_Timer.getTime();
    float perc = cast_float(totTime.asMilliseconds()) / cast_float(m_SpawnDuration.asMilliseconds());
    bgC.a = static_cast< sf::Uint8 >( ceil(255 * ( 1.f - perc )) );
    m_BackgroundRect.setFillColor(bgC);
  }

  void PreloaderBox::UpdateDespawn()
  {
    Color bgC = m_BackgroundRect.getFillColor();
    auto totTime = m_Timer.getTime();
    float perc = cast_float(totTime.asMilliseconds()) / cast_float(m_DespawnDuration.asMilliseconds());
    bgC.a = static_cast< sf::Uint8 >( ceil(255 * perc) );
    m_BackgroundRect.setFillColor(bgC);
  }

}  
