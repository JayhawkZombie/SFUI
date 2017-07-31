#ifndef SFUI_APPLOGGERINL_H
#define SFUI_APPLOGGERINL_H

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

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////
#include <ctime>

template<typename policy>
sfui::Logger<policy>::Logger(const std::string &logFile)
{
  m_PolicyInterface = new policy();
  if (!m_PolicyInterface) {
    throw std::runtime_error("Logger: Unable to create logging policy interface\n");
  }

  m_PolicyInterface->OpenStream(logFile);
}

template<typename policy>
sfui::Logger<policy>::~Logger()
{
  if (m_PolicyInterface) {
    m_PolicyInterface->CloseStream();
    delete m_PolicyInterface;
  }
}

template<typename policy>
template<Severity severity, typename ... Args>
void sfui::Logger<policy>::Print(Args... args)
{
  m_WriteMutex.lock();

  switch (severity)
  {
    case Severity::Debug:
    {
      m_LogStream << "<DEBUG>     :"; break;
    }
    case Severity::Warning:
    {
      m_LogStream << "<WARNING>   :"; break;
    }
    case Severity::Exception:
    {
      m_LogStream << "<EXCEPTION> :"; break;
    }
    case Severity::Recoverable:
    {
      m_LogStream << "<RECOVERED> :"; break;
    }
    case Severity::Fatal:
    {
      m_LogStream << "<FATAL>     :"; break;
    }
  }

  IntPrint(args...);
  m_WriteMutex.unlock();
}

template<typename policy>
std::string sfui::Logger<policy>::GetLogLineHeader()
{
  std::stringstream Header;
  Header.str("");
  Header.fill('0');
  Header.width(7);
  Header << m_LogLineNumber++ << " < " << GetTime() << " - ";
  Header.fill('0');
  Header.width(7);
  Header << clock() << " > ~ ";
  return Header.str();
}

template<typename policy>
std::string sfui::Logger<policy>::GetTime() const
{
  auto epoch_time = std::chrono::system_clock::now().time_since_epoch();
  auto epoch_milliseconds = std::chrono::duration_cast< std::chrono::milliseconds >( epoch_time );
  return std::to_string(epoch_milliseconds.count());
  /*return std::to_string(
    std::chrono::duration_cast< std::chrono::milliseconds >
    ( std::chrono::system_clock::now().time_since_epoch() ).count()
  );

  std::string TimeString;
  time_t RawTime;
  time(&RawTime);

#pragma warning ( push )
#pragma warning ( disable : 4996 )
  TimeString = ctime(&RawTime);
#pragma warning ( pop )
  TimeString.erase(TimeString.back());
  return TimeString;*/
}

template<typename policy>
void sfui::Logger<policy>::IntPrint()
{
  m_PolicyInterface->Write(GetLogLineHeader() + m_LogStream.str());
  m_LogStream.str("");
}

template<typename policy>
template<typename First, typename ... Rest>
void sfui::Logger<policy>::IntPrint(const First &f1, const Rest &... rem)
{
  m_LogStream << f1;
  IntPrint(rem...);
}

#endif // SFUI_APPLOGGERINL_H
