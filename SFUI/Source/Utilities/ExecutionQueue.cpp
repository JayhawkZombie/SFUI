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
#include <SFUI/Include/Utilities/ExecutionQueue.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  ExecutionQueue::ExecutionQueue()
  {
    m_Timer.stop();
  }

  ExecutionQueue::~ExecutionQueue()
  {

  }

  void ExecutionQueue::Update()
  {
    if (!m_ActionQueue.empty()) {

      //Update our timer
      m_Timer.getTime();

      //If the timer is done, finish this one (ie invoke queued function)
      //  and start the next queued timer (if the queue isn't empty)
      if (m_Timer.isDone()) {
        FinishAction();
        StartNextAction();
      }

    } //if (!m_ActionQueue.empty())
  }

  bool ExecutionQueue::IsTiming() const
  {
    return ( !m_ActionQueue.empty() && !m_Timer.isPaused() );
  }

  void ExecutionQueue::Enqueue(uint32 TimeInMS, std::function<void()> Function)
  {
    PushAction(TimeInMS, Function);
    if (m_ActionQueue.size() == 1)
      StartNextAction();
  }

  size_t ExecutionQueue::QueueSize() const
  {
    return m_ActionQueue.size();
  }

  void ExecutionQueue::FinishCurrent()
  {
    //If the queue isn't empty, just stop the timer and start the next one
    if (!m_ActionQueue.empty()) {
      FinishAction();
      StartNextAction();
    }
  }

  void ExecutionQueue::SkipCurrent()
  {
    if (!m_ActionQueue.empty()) {
      m_ActionQueue.pop();
      StartNextAction();
    }
  }

  void ExecutionQueue::PauseCurrent()
  {
    m_Timer.pause();
  }

  void ExecutionQueue::StopCurrent()
  {
    m_Timer.stop();
  }

  void ExecutionQueue::RestartCurrent()
  {
    m_Timer.restart();
  }

  void ExecutionQueue::PushAction(const uint32 &TimeInMS, std::function<void()> Function)
  {
    m_ActionQueue.emplace(TimeInMS, Function);
  }

  void ExecutionQueue::PopAction()
  {
    if (!m_ActionQueue.empty()) {
      m_ActionQueue.pop();
    }
  }

  void ExecutionQueue::FinishAction()
  {
    if (!m_ActionQueue.empty()) {
      auto funcPair = m_ActionQueue.front();
      funcPair.second();
      m_ActionQueue.pop();
    }
  }

  void ExecutionQueue::StartNextAction()
  {
    if (!m_ActionQueue.empty()) {
      kairos::Duration msTime;
      msTime.setFromMilliseconds(static_cast< int >( m_ActionQueue.front().first ));
      m_Timer.setTime(msTime);
      m_Timer.start();
    }
  }

}  
