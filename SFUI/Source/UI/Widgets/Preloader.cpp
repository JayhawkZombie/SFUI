////////////////////////////////////////////////////////////
//
// MIT License
//
// Copyright(c) 2217 Kurt Slagle - kurt_slagle@yahoo.com
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
#include <SFUI/Include/UI/Widgets/Preloader.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  Preloader::Preloader(optional<Theme*> theme /* = {} */, optional<Widget*> parent /* = {} */)
    : Widget(theme, parent)
  {
    for (size_t i = 0; i < 16; i++) {
      auto bPtr = PreloaderBox::Create(m_Theme, this);
      bPtr->OnFinished([this]() { BlockFinished(); });
      m_Boxes.push_back(bPtr);
    }
  }

  Preloader::~Preloader()
  {

  }

  Preloader::shared_ptr Preloader::Create(optional<Theme*> theme /* = {} */, optional<Widget*> parent /* = {} */)
  {
    return std::make_shared<Preloader>(theme, parent);
  }

  void Preloader::Update()
  {
    m_RestartTimer.Update();

    if (m_IsPlaying) {
      bool allDone = true;

      for (auto & bPtr : m_Boxes) {
        allDone &= bPtr->IsDone();
        bPtr->BaseUpdate();
      }

      if (allDone)
        m_IsPlaying = false;
    }
  }

  void Preloader::Render(sf::RenderTarget &Target)
  {
    if (m_IsPlaying) {
      for (auto & bPtr : m_Boxes) {
        bPtr->Render(Target);
      }
    }
  }

  void Preloader::SetPosition(const Vec2i &Position)
  {
    super::SetPosition(Position);
    for (auto & bPtr : m_Boxes)
      bPtr->SetPosition(Position);
  }

  void Preloader::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);
    m_BoxHeight = static_cast< int >( floor(Size.x * 0.1) );
    m_BoxSpacing = static_cast< int >( floor(Size.x * 0.1) );
  }

  void Preloader::Move(const Vec2i &Delta)
  {
    super::Move(Delta);
    for (auto & bPtr : m_Boxes)
      bPtr->Move(Delta);
  }

  void Preloader::Start(uint32 SpawnFallDuration, uint32 SitDuration, uint32 DespawnFallDuration)
  {
    m_SpawnDur = SpawnFallDuration;
    m_SitDur = SitDuration;
    m_DespDur = DespawnFallDuration;
    m_RestartTimer.Clear();
    m_FinishedCount = 0;

    PositionBoxes();
    m_IsPlaying = true;
    uint32 CumulativeDelay = 1700;

    for (auto & box : m_Boxes) {
      box->Start(SpawnFallDuration, SitDuration, DespawnFallDuration, CumulativeDelay);
      CumulativeDelay -= 100;
    }
  }

  void Preloader::Pause()
  {
    m_IsPlaying = false;
    std::for_each(begin(m_Boxes), end(m_Boxes), [this](auto bPtr) { bPtr->Pause(); });
  }

  void Preloader::Stop()
  {
    m_IsPlaying = false;
    std::for_each(begin(m_Boxes), end(m_Boxes), [this](auto bPtr) { bPtr->Stop(); });
  }

  void Preloader::Restart()
  {
    m_IsPlaying = true;
    std::for_each(begin(m_Boxes), end(m_Boxes), [this](auto bPtr) { bPtr->Restart(); });
  }

  void Preloader::SetColor(const Color &c)
  {
    std::for_each(begin(m_Boxes), end(m_Boxes), [&c](auto bPtr) { bPtr->SetBackgroundColor(c); });
  }

  void Preloader::PositionBoxes()
  {
    Vec2i CurrentPos = m_Position;

    Vec2i PosOffset = Vec2i(0, 6 * 45);

    for (size_t i = 0; i < 4; ++i) {
      m_Boxes[4 * i]->SetSpawnPosition(CurrentPos - PosOffset);
      m_Boxes[4 * i]->SetStopPosition(CurrentPos);
      m_Boxes[4 * i]->SetPosition(CurrentPos);
      m_Boxes[4 * i]->SetDespawnPosition(CurrentPos + PosOffset);
      m_Boxes[4 * i]->SetSize({ 15, 15 });
      CurrentPos.x += 25;

      m_Boxes[4 * i + 1]->SetSpawnPosition(CurrentPos - PosOffset);
      m_Boxes[4 * i + 1]->SetStopPosition(CurrentPos);
      m_Boxes[4 * i + 1]->SetPosition(CurrentPos);
      m_Boxes[4 * i + 1]->SetDespawnPosition(CurrentPos + PosOffset);
      m_Boxes[4 * i + 1]->SetSize({ 15, 15 });
      CurrentPos.x += 25;

      m_Boxes[4 * i + 2]->SetSpawnPosition(CurrentPos - PosOffset);
      m_Boxes[4 * i + 2]->SetStopPosition(CurrentPos);
      m_Boxes[4 * i + 2]->SetPosition(CurrentPos);
      m_Boxes[4 * i + 2]->SetDespawnPosition(CurrentPos + PosOffset);
      m_Boxes[4 * i + 2]->SetSize({ 15, 15 });
      CurrentPos.x += 25;

      m_Boxes[4 * i + 3]->SetSpawnPosition(CurrentPos - PosOffset);
      m_Boxes[4 * i + 3]->SetStopPosition(CurrentPos);
      m_Boxes[4 * i + 3]->SetPosition(CurrentPos);
      m_Boxes[4 * i + 3]->SetDespawnPosition(CurrentPos + PosOffset);
      m_Boxes[4 * i + 3]->SetSize({ 15, 15 });
      CurrentPos.y += 25;
      CurrentPos.x -= 75;
    }

    CurrentPos = m_Position;
  }

  void Preloader::BlockFinished()
  {
    ++m_FinishedCount;

    if (m_FinishedCount == 16 && m_ShouldRestart) {
      m_RestartTimer.AddSequences(
        [ ]() { }, [ ]() { }, [this]() { Start(m_SpawnDur, m_SitDur, m_DespDur); },
        { 
          { 2000, [ ]() { }, [ ]() { }, [ ]() { } } 
        }
      );
      m_RestartTimer.Start();
    }
  }

}  
