#ifndef SFUI_SEQUENCETIMER_H
#define SFUI_SEQUENCETIMER_H

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

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////
#include <Kairos.hpp>
#include <queue>

namespace sfui
{  
  
  class SequenceTimer;

  class SequenceNode
  {
  public:
    SequenceNode() = default;
    SequenceNode(const SequenceNode &SeqCopy);
    SequenceNode(std::initializer_list<SequenceNode> Seq);
    SequenceNode(uint32 Duration,
                 std::function<void()> StartCall,
                 std::function<void()> EndCall,
                 std::function<void()> UpdateCall);
    ~SequenceNode() = default;

    void Update();
    bool IsDone() const;
    void Start();
    void Pause();
    void Resume();
    void Stop();
    void Restart();

  private:
    bool m_IsDone = false;
    kairos::Duration m_Duration;
    kairos::Timer m_Timer;
    std::function<void()> m_StartCallback = [ ]() { };
    std::function<void()> m_UpdateCallback = [ ]() { };
    std::function<void()> m_EndCallback = [ ]() { };
  };

  class SequenceTimer
  {
  public:
    SequenceTimer() = default;
    SequenceTimer(std::function<void()> SequenceStartCB,
                  std::function<void()> SequenceUpdateCB,
                  std::function<void()> SequenceEndCB);
    ~SequenceTimer() = default;
    void SetCallbacks(std::function<void()> StartCB,
                      std::function<void()> UpdateCB,
                      std::function<void()> EndCB);
    void Clear();
    void Update();
    void AddSequence(uint32 Duration,
                     std::function<void()> NodeStartCB,
                     std::function<void()> NodeUpdateCB,
                     std::function<void()> NodeEndCB);
    void AddSequences(std::function<void()> SequenceStartCB,
                      std::function<void()> SequenceUpdateCB,
                      std::function<void()> SequenceEndCB,
                      std::initializer_list<SequenceNode> Nodes);

    void Start();
    void Stop();
    void Pause();
    void Resume();
    void Restart();

  private:
    void MoveToNextNode();

    bool m_IsTiming = false;

    std::function<void()> m_SequenceStartCallback = [ ]() { };
    std::function<void()> m_SequenceUpdateCallback = [ ]() { };
    std::function<void()> m_SequenceEndCallback = [ ]() { };

    std::queue<SequenceNode> m_Nodes;

  };
  
}  

#endif // SFUI_SEQUENCETIMER_H
