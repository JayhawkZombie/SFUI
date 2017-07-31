#ifndef SFUI_APPLOGGER_H
#define SFUI_APPLOGGER_H

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
#include <concurrent_queue.h>
#include <agents.h>
#include <ppltasks.h>

#include <string>
#include <iostream>
#include <fstream>

#define LOG_LEVEL 5
#define LOG_OBJECT_DUMPS
#define LOG_WIDGET_CREATION
#define LOG_PERFORMANCE_TIMINGS

namespace sfui
{  
  
  enum Severity
  {
    Debug = 1,
    Warning,
    Exception,
    Recoverable,
    Fatal
  };
  
  class LogPolicyInterface
  {
  public:
    LogPolicyInterface() = default;
    virtual ~LogPolicyInterface() = default;

    virtual void OpenStream(const std::string &Name) = 0;
    virtual void CloseStream() = 0;
    virtual void Write(const std::string &Message) = 0;
  };

  class FileLogPolicy : public LogPolicyInterface
  {
  public:
    FileLogPolicy();
    ~FileLogPolicy() override final;

    void OpenStream(const std::string &Name) override final;
    void CloseStream() override final;
    void Write(const std::string &Message) override final;

  private:
    std::unique_ptr<std::ofstream> m_OutStream = nullptr;
  };

  template<typename policy>
  class Logger
  {
  public:
    Logger(const std::string &logFile);
    ~Logger();

    template<Severity severity, typename... Args>
    void Print(Args... args);

  private:
    uint32 m_LogLineNumber = 0;
    std::string GetTime() const;
    std::string GetLogLineHeader();
    std::stringstream m_LogStream;

    LogPolicyInterface *m_PolicyInterface;
    std::mutex m_WriteMutex;

    void IntPrint();

    template<typename First, typename... Rest>
    void IntPrint(const First &f1, const Rest &... rem);
  };
  
#include <SFUI/Include/Application/AppLogger.inl>

  __declspec( selectany ) Logger<FileLogPolicy> AppLogger("AppRun.log");

//Level 5 turns on reporting of all exceptions
#if (LOG_LEVEL >= 5)
#define  APP_LOG_EXCEPTION(MESSAGE, EXCEPTION_OBJECT) { sfui::AppLogger.Print<sfui::Severity::Exception>( MESSAGE + std::string(": ") + std::string(EXCEPTION_OBJECT.what()) + std::string("\"") ); }
#else
#define  APP_LOG_EXCEPTION(...)
#endif

// Level 4 turns on reporting of fatal errors
#if (LOG_LEVEL >= 4)
#define APP_LOG_FAT sfui::AppLogger.Print<sfui::Severity::Fatal>
#else
#define APP_LOG_FAT(...)
#endif

// Level 3 turns on reporting of recoverable errors
#if (LOG_LEVEL >= 3)
#define APP_LOG_REC sfui::AppLogger.Print<sfui::Severity::Recoverable>
#else
#define APP_LOG_REC(...)
#endif // #else

// Level 2 turns on warnigns
#if (LOG_LEVEL >= 2)
#define APP_LOG_WAR sfui::AppLogger.Print<sfui::Severity::Warning>
#else
#define APP_LOG_WAR(...)
#endif

// Level 1 turns on debug messages
#if (LOG_LEVEL >= 1)
#define APP_LOG sfui::AppLogger.Print<sfui::Severity::Debug>
#define APP_LOG_LOAD_FAILURE(FILENAME) APP_LOG("Failed to load resource \"" + std::string(FILENAME) + std::string("\""))
#else
#define APP_LOG(...)
#endif

#ifdef LOG_OBJECT_DUMPS

//Dump of object's properties immediately after construction
#define APP_LOG_OBJECT_DUMP(OBJ_PTR) \
{ \
  auto objclass = OBJ_PTR->Class(); \
  auto objpos = OBJ_PTR->GetPosition(); \
  auto objsize = OBJ_PTR->GetSize(); \
  std::stringstream objdump; \
  objdump.write("Object construction dump\n", 25); \
  objdump.write("\t( Class    = ", 14); objdump.write(objclass.c_str(), objclass.size()); objdump.put('\n'); \
  objdump.write("\t  Position = (", 15); objdump << objpos.x << ", " << objpos.y << ")\n"; \
  objdump.write("\t  Size     = (", 15); objdump << objsize.x << ", " << objsize.y << ")\n"; \
  objdump.write("\t)", 2); \
  APP_LOG(objdump.str()); \
} \

#else
#define APP_LOG_OBJECT_DUMP(...)
#endif // #ifdef LOG_OBJECT_DUMPS

#ifdef LOG_PERFORMANCE_TIMINGS
#define APP_LOG_TASK_PERFORMANCE(TASK_DESCRIPTION, ELAPSED_TIME) \
{ \
  std::string ElapsedString = std::to_string(ELAPSED_TIME.QuadPart); \
  std::stringstream msg_str; msg_str.str(""); \
  msg_str << "Task \"" << TASK_DESCRIPTION << "\" took " << static_cast<uint32>(floor(ELAPSED_TIME.QuadPart * 0.001)) << " ms"; \
  APP_LOG(msg_str.str()); \
}

#else
#define APP_LOG_TASK_PERFORMANCE(...)
#endif //LOG_PERFORMANCE_TIMINGS

}  

#endif // SFUI_APPLOGGER_H
