#ifndef SFUI_PRELOADERBOX_H
#define SFUI_PRELOADERBOX_H

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
#include <SFUI/Include/Utilities/SequenceTimer.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class PreloaderBox : public Widget
  {
  public:
    WIDGET_DERIVED(PreloaderBox, Widget);
    PreloaderBox(optional<Theme*> theme = { }, optional<Widget*> parent = { });
    virtual ~PreloaderBox() override;

    static shared_ptr Create(optional<Theme*> theme = { }, optional<Widget*> parent = { });

    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;
    virtual void SetPosition(const Vec2i &Position) override;
    virtual void SetSize(const Vec2i &Size) override;

    virtual void SetSpawnPosition(const Vec2i &Position);
    virtual void SetStopPosition(const Vec2i &Position);
    virtual void SetDespawnPosition(const Vec2i &Position);

    virtual void Start(uint32 DurationToSpawn, uint32 DurationToSit, uint32 DurationToDespawn, uint32 Delay);
    virtual void Pause();
    virtual void Stop();
    virtual void Restart();
    virtual void Resume();
    bool IsDone() const;
    void OnFinished(boost::function<void()> func);

  protected:
    Signal<void()> m_FinishedSignal;

    virtual void StartDelay();
    virtual void StartSpawn();
    virtual void StartSit();
    virtual void StartDesapwn();
    virtual void Finished();

    void UpdateSpawn();
    void UpdateDespawn();

    enum class State { Spawning, Sitting, Despawning, Hidden, Delayed };

    Vec2i m_SpawnPosition = { 0, 0 };
    Vec2i m_SitPosition = { 0, 0 };
    Vec2i m_DespawnPosition = { 0, 0 };

    int m_FallSpeed = 0;
    PreloaderBox::State m_State = State::Hidden;

    kairos::Duration m_SpawnDuration = 0;
    kairos::Duration m_SitDuration = 0;
    kairos::Duration m_DespawnDuration = 0;
    kairos::Duration m_DelayDuration = 0;
    SequenceTimer m_SeqTimer;
    bool m_IsDone = true;
    CubicBezier m_SpeedInterpolator;
    kairos::Timer m_Timer;
  };
  
}  

#endif // SFUI_PRELOADERBOX_H
