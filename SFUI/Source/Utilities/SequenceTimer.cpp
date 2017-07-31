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
#include <SFUI/Include/Utilities/SequenceTimer.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  /*
    Individual sequence nodes
  */
  SequenceNode::SequenceNode(const SequenceNode &SeqCopy)
    : m_Duration(SeqCopy.m_Duration)
    , m_EndCallback(SeqCopy.m_EndCallback)
    , m_StartCallback(SeqCopy.m_StartCallback)
    , m_UpdateCallback(SeqCopy.m_UpdateCallback)
  {
    m_Timer.setTime(m_Duration);
  }

  SequenceNode::SequenceNode(std::initializer_list<SequenceNode> Seq)
  {

  }

  SequenceNode::SequenceNode(uint32 Duration, std::function<void()> StartCall, std::function<void()> EndCall, std::function<void()> UpdateCall)
    : m_EndCallback(EndCall)
    , m_StartCallback(StartCall)
    , m_UpdateCallback(UpdateCall)
  {
    m_Duration.setFromMilliseconds(static_cast< int >( Duration ));
    m_Timer.setTime(m_Duration);
  }

  void SequenceNode::Update()
  {
    m_Timer.getTime();
    m_UpdateCallback();

    //Make sure we only fire off the call to the end callback once
    if (m_Timer.isDone() && !m_IsDone) {
      m_EndCallback();
      m_IsDone = true;
    }

  }

  bool SequenceNode::IsDone() const
  {
    return m_IsDone;
  }

  void SequenceNode::Start()
  {
    m_Timer.start();
    m_StartCallback();
    m_IsDone = false;
  }

  void SequenceNode::Pause()
  {
    m_Timer.pause();
  }

  void SequenceNode::Resume()
  {
    m_Timer.resume();
  }

  void SequenceNode::Stop()
  {
    m_Timer.stop();
    m_IsDone = true;
  }

  void SequenceNode::Restart()
  {
    m_Timer.restart();
    m_IsDone = false;
  }

  /*
    Main sequence timer
  */

  SequenceTimer::SequenceTimer(std::function<void()> SequenceStartCB, std::function<void()> SequenceUpdateCB, std::function<void()> SequenceEndCB)
    : m_SequenceStartCallback(SequenceStartCB)
    , m_SequenceUpdateCallback(SequenceUpdateCB)
    , m_SequenceEndCallback(SequenceEndCB)
  {

  }

  void SequenceTimer::Clear()
  {
    while (!m_Nodes.empty()) {
      m_Nodes.pop();
    }
  }

  void SequenceTimer::Update()
  {
    if (!m_IsTiming || m_Nodes.empty())
      return;

    m_Nodes.front().Update();

    if (m_Nodes.front().IsDone()) {
      m_Nodes.pop();
      MoveToNextNode();
    }

    m_SequenceUpdateCallback();
  }

  void SequenceTimer::AddSequence(uint32 Duration, std::function<void()> NodeStartCB, std::function<void()> NodeUpdateCB, std::function<void()> NodeEndCB)
  {
    m_Nodes.emplace(Duration, NodeStartCB, NodeUpdateCB, NodeEndCB);
  }

  void SequenceTimer::AddSequences(std::function<void()> SequenceStartCB, std::function<void()> SequenceUpdateCB, std::function<void()> SequenceEndCB, std::initializer_list<SequenceNode> Nodes)
  {
    m_SequenceStartCallback = SequenceStartCB;
    m_SequenceUpdateCallback = SequenceUpdateCB;
    m_SequenceEndCallback = SequenceEndCB;

    for (auto & node : Nodes)
      m_Nodes.push(node);
  }

  void SequenceTimer::Start()
  {
    m_IsTiming = true;

    if (!m_Nodes.empty()) {
      m_Nodes.front().Start();
    }

    m_SequenceStartCallback();
  }

  void SequenceTimer::Stop()
  {
    m_IsTiming = false;

    if (!m_Nodes.empty()) {
      m_Nodes.front().Stop();
    }
  }

  void SequenceTimer::Pause()
  {
    m_IsTiming = false;

    if (!m_Nodes.empty()) {
      m_Nodes.front().Pause();
    }
  }

  void SequenceTimer::Resume()
  {
    m_IsTiming = true;

    if (!m_Nodes.empty()) {
      m_Nodes.front().Resume();
    }

  }

  void SequenceTimer::Restart()
  {
    m_IsTiming = true;
    
    if (!m_Nodes.empty()) {
      m_Nodes.front().Restart();
    }
  }

  void SequenceTimer::MoveToNextNode()
  {
    if (!m_Nodes.empty()) {
      m_Nodes.front().Start();
    }
    else {
      //We finished the sequence
      m_SequenceEndCallback();
      m_IsTiming = false;
    }
  }

}  
