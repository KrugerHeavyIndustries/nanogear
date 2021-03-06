//
// Timestamp.cpp
//
// $Id: //poco/1.4/Foundation/src/Timestamp.cpp#2 $
//
// Library: Foundation
// Package: DateTime
// Module:  Timestamp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "timestamp.h"
#include "timespan.h"
//#include "Poco/Exception.h"
#include <algorithm>
#undef min
#undef max
#include <limits>
#if defined(__APPLE__)
#include <time.h>
#include <unistd.h>
#if defined(POCO_VXWORKS)
#include <timers.h>
#else
#include <sys/time.h>
#include <sys/times.h>
#endif
#elif defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/UnWindows.h"
#if defined(_WIN32_WCE)
#include <cmath>
#endif
#endif

#include <chrono>

#if defined(_WIN32_WCE)


//
// See <http://community.opennetcf.com/articles/cf/archive/2007/11/20/getting-a-millisecond-resolution-datetime-under-windows-ce.aspx>
// for an explanation of the following code.
//
// In short: Windows CE system time in most cases only has a resolution of one second.
// But we want millisecond resolution.
//


namespace {
   
   
   class TickOffset
   {
   public:
      TickOffset()
      {
         SYSTEMTIME st1, st2;
         std::memset(&st1, 0, sizeof(SYSTEMTIME));
         std::memset(&st2, 0, sizeof(SYSTEMTIME));
         GetSystemTime(&st1);
         while (true)
         {
            GetSystemTime(&st2);
            
            // wait for a rollover
            if (st1.wSecond != st2.wSecond)
            {
               _offset = GetTickCount() % 1000;
               break;
            }
         }
      }
      
      void calibrate(int seconds)
      {
         SYSTEMTIME st1, st2;
         systemTime(&st1);
         
         WORD s = st1.wSecond;
         int sum = 0;
         int remaining = seconds;
         while (remaining > 0)
         {
            systemTime(&st2);
            WORD s2 = st2.wSecond;
            
            if (s != s2)
            {
               remaining--;
               // store the offset from zero
               sum += (st2.wMilliseconds > 500) ? (st2.wMilliseconds - 1000) : st2.wMilliseconds;
               s = st2.wSecond;
            }
         }
         
         // adjust the offset by the average deviation from zero (round to the integer farthest from zero)
         if (sum < 0)
            _offset += (int) std::floor(sum / (float)seconds);
         else
            _offset += (int) std::ceil(sum / (float)seconds);
      }
      
      void systemTime(SYSTEMTIME* pST)
      {
         std::memset(pST, 0, sizeof(SYSTEMTIME));
         
         WORD tick = GetTickCount() % 1000;
         GetSystemTime(pST);
         WORD ms = (tick >= _offset) ? (tick - _offset) : (1000 - (_offset - tick));
         pST->wMilliseconds = ms;
      }
      
      void systemTimeAsFileTime(FILETIME* pFT)
      {
         SYSTEMTIME st;
         systemTime(&st);
         SystemTimeToFileTime(&st, pFT);
      }
      
   private:
      WORD _offset;
   };
   
   
   static TickOffset offset;
   
   
   void GetSystemTimeAsFileTimeWithMillisecondResolution(FILETIME* pFT)
   {
      offset.systemTimeAsFileTime(pFT);
   }
   
   
} // namespace


#endif // defined(_WIN32_WCE)


namespace nanogear {
   
   
   const Timestamp::TimeVal Timestamp::TIMEVAL_MIN = std::numeric_limits<Timestamp::TimeVal>::min();
   const Timestamp::TimeVal Timestamp::TIMEVAL_MAX = std::numeric_limits<Timestamp::TimeVal>::max();
   
   
   Timestamp::Timestamp()
   {
      update();
   }
   
   
   Timestamp::Timestamp(TimeVal tv)
   {
      m_ts = tv;
   }
   
   
   Timestamp::Timestamp(const Timestamp& other)
   {
      m_ts = other.m_ts;
   }
   
   
   Timestamp::~Timestamp()
   {
   }
   
   
   Timestamp& Timestamp::operator = (const Timestamp& other)
   {
      m_ts = other.m_ts;
      return *this;
   }
   
   
   Timestamp& Timestamp::operator = (TimeVal tv)
   {
      m_ts = tv;
      return *this;
   }
   
   
   void Timestamp::swap(Timestamp& timestamp)
   {
      std::swap(m_ts, timestamp.m_ts);
   }
   
   
   Timestamp Timestamp::fromEpochTime(std::time_t t)
   {
      return Timestamp(TimeVal(t)*resolution());
   }
   
   
   Timestamp Timestamp::fromUtcTime(UtcTimeVal val)
   {
      val -= (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
      val /= 10;
      return Timestamp(val);
   }
   
   
   void Timestamp::update()
   {
       m_ts = std::chrono::duration_cast<std::chrono::microseconds>
       (std::chrono::system_clock::now().time_since_epoch()).count();
   }
   
   
   Timestamp  Timestamp::operator +  (const Timespan& span) const
   {
      return *this + span.totalMicroseconds();
   }
   
   
   Timestamp  Timestamp::operator -  (const Timespan& span) const
   {
      return *this - span.totalMicroseconds();
   }
   
   
   Timestamp& Timestamp::operator += (const Timespan& span)
   {
      return *this += span.totalMicroseconds();
   }
   
   
   Timestamp& Timestamp::operator -= (const Timespan& span)
   {
      return *this -= span.totalMicroseconds();
   }
   
   
#if defined(_WIN32)
   
   
   Timestamp Timestamp::fromFileTimeNP(UInt32 fileTimeLow, UInt32 fileTimeHigh)
   {
      ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
      epoch.LowPart  = 0xD53E8000;
      epoch.HighPart = 0x019DB1DE;
      
      ULARGE_INTEGER ts;
      ts.LowPart  = fileTimeLow;
      ts.HighPart = fileTimeHigh;
      ts.QuadPart -= epoch.QuadPart;
      
      return Timestamp(ts.QuadPart/10);
   }
   
   
   void Timestamp::toFileTimeNP(UInt32& fileTimeLow, UInt32& fileTimeHigh) const
   {
      ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
      epoch.LowPart  = 0xD53E8000;
      epoch.HighPart = 0x019DB1DE;
      
      ULARGE_INTEGER ts;
      ts.QuadPart  = m_ts*10;
      ts.QuadPart += epoch.QuadPart;
      fileTimeLow  = ts.LowPart;
      fileTimeHigh = ts.HighPart;
   }
   
   
#endif
   
   
} // namespace Poco
