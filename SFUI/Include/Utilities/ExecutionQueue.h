#ifndef SFUI_EXECUTIONQUEUE_H
#define SFUI_EXECUTIONQUEUE_H

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
#include <Kairos.hpp>

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////
#include <queue>

namespace sfui
{  
  
  class ExecutionQueue
  {
  public:
    ExecutionQueue();
    ~ExecutionQueue();

    void Update();

    bool IsTiming() const;
    void Enqueue(uint32 TimeInMS, std::function<void()> Function);
    size_t QueueSize() const;
    void FinishCurrent();
    void SkipCurrent();
    void PauseCurrent();
    void StopCurrent();
    void RestartCurrent();

  private:
    void PushAction(const uint32 &TimeInMS, std::function<void()> Function);
    void PopAction();
    void FinishAction();
    void StartNextAction();

    std::queue<std::pair<uint32, std::function<void()>>> m_ActionQueue;
    kairos::Timer m_Timer;
  };
  
}  

#endif // SFUI_EXECUTIONQUEUE_H
