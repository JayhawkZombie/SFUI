#ifndef SFUI_LOGGER_H
#define SFUI_LOGGER_H

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
#include <SFUI/Include/Application/Concurrent.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  /*class ConcurrentLogger
  {
  public:
    static int Init()
    {
      try
      {
        LogBuffer = std::make_shared<concurrency::concurrent_queue<std::string>>();
        return 0;
      }
      catch (const std::exception &e)
      {
        std::cerr << "ConcurrentLogger: Init failure. " << e.what() << "\n";
        return -1;
      }
    }

    static void Log(const std::string &Message)
    {
      static auto try_flush = [ ](int a) { ConcurrentLogger::TimedPurge(); };
      LogBuffer->push(Message);

      concurrency::timer<int> purge_timer(100, 0, &try_flush, false);
    }

    static void TimedPurge()
    {
      std::string out;
      while (!LogBuffer->empty()) {
        if (LogBuffer->try_pop(out)) {
          std::cout << out << "\n";
        }
        else {
          std::cout << "Failed to pop message from log\n";
          return;
        }
      }
    }

    static std::shared_ptr<concurrency::concurrent_queue<std::string>> LogBuffer;
  };

  std::shared_ptr<concurrency::concurrent_queue<std::string>> ConcurrentLogger::LogBuffer;*/
}  

#endif // SFUI_LOGGER_H
