//
// NumberFormatter.h
//
// $Id: //poco/1.4/Foundation/include/Poco/NumberFormatter.h#1 $
//
// Library: Foundation
// Package: Core
// Module:  NumberFormatter
//
// Definition of the NumberFormatter class.
//
// Copyright (c) 2004-2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_NumberFormatter_INCLUDED
#define Foundation_NumberFormatter_INCLUDED

#include <string>

namespace nanogear
{
   class NumberFormatter
   /// The NumberFormatter class provides static methods
   /// for formatting numeric values into strings.
   ///
   /// There are two kind of static member functions:
   ///    * format* functions return a std::string containing
   ///      the formatted value.
   ///    * append* functions append the formatted value to
   ///      an existing string.
   {
   public:
      enum BoolFormat
      {
         FMT_TRUE_FALSE,
         FMT_YES_NO,
         FMT_ON_OFF
      };

      static const unsigned NF_MAX_FMT_STRING_LEN = 16;
      static const unsigned NF_MAX_INT_STRING_LEN = 32; // increase for 64-bit binary formatting support
      static const unsigned NF_MAX_FLT_STRING_LEN = 780;

      static std::string format(int value);
      /// Formats an integer value in decimal notation.

      static std::string format(int value, int width);
      /// Formats an integer value in decimal notation,
      /// right justified in a field having at least
      /// the specified width.

      static std::string format0(int value, int width);
      /// Formats an integer value in decimal notation,
      /// right justified and zero-padded in a field
      /// having at least the specified width.

      static std::string formatHex(int value, bool prefix = false);
      /// Formats an int value in hexadecimal notation.
      /// If prefix is true, "0x" prefix is prepended to the
      /// resulting string.
      /// The value is treated as unsigned.

      static std::string formatHex(int value, int width, bool prefix = false);
      /// Formats a int value in hexadecimal notation,
      /// right justified and zero-padded in
      /// a field having at least the specified width.
      /// If prefix is true, "0x" prefix is prepended to the
      /// resulting string.
      /// The value is treated as unsigned.

      static std::string format(unsigned value);
      /// Formats an unsigned int value in decimal notation.

      static std::string format(unsigned value, int width);
      /// Formats an unsigned long int in decimal notation,
      /// right justified in a field having at least the
      /// specified width.

      static std::string format0(unsigned int value, int width);
      /// Formats an unsigned int value in decimal notation,
      /// right justified and zero-padded in a field having at
      /// least the specified width.

      static std::string formatHex(unsigned value, bool prefix = false);
      /// Formats an unsigned int value in hexadecimal notation.
      /// If prefix is true, "0x" prefix is prepended to the
      /// resulting string.

      static std::string formatHex(unsigned value, int width, bool prefix = false);
      /// Formats a int value in hexadecimal notation,
      /// right justified and zero-padded in
      /// a field having at least the specified width.
      /// If prefix is true, "0x" prefix is prepended to the
      /// resulting string.

      static std::string format(long value);
      /// Formats a long value in decimal notation.

      static std::string format(long value, int width);
      /// Formats a long value in decimal notation,
      /// right justified in a field having at least the
      /// specified width.

      static std::string format0(long value, int width);
      /// Formats a long value in decimal notation,
      /// right justified and zero-padded in a field
      /// having at least the specified width.

      static std::string formatHex(long value, bool prefix = false);
      /// Formats an unsigned long value in hexadecimal notation.
      /// If prefix is true, "0x" prefix is prepended to the
      /// resulting string.
      /// The value is treated as unsigned.

      static std::string formatHex(long value, int width, bool prefix = false);
      /// Formats an unsigned long value in hexadecimal notation,
      /// right justified and zero-padded in a field having at least the
      /// specified width.
      /// If prefix is true, "0x" prefix is prepended to the
      /// resulting string.
      /// The value is treated as unsigned.

      static std::string format(unsigned long value);
      /// Formats an unsigned long value in decimal notation.

      static std::string format(unsigned long value, int width);
      /// Formats an unsigned long value in decimal notation,
      /// right justified in a field having at least the specified
      /// width.

      static std::string format0(unsigned long value, int width);
      /// Formats an unsigned long value in decimal notation,
      /// right justified and zero-padded
      /// in a field having at least the specified width.

      static std::string formatHex(unsigned long value, bool prefix = false);
      /// Formats an unsigned long value in hexadecimal notation.
      /// If prefix is true, "0x" prefix is prepended to the
      /// resulting string.

      static std::string formatHex(unsigned long value, int width, bool prefix = false);
      /// Formats an unsigned long value in hexadecimal notation,
      /// right justified and zero-padded in a field having at least the
      /// specified width.
      /// If prefix is true, "0x" prefix is prepended to the
      /// resulting string.

#if defined(POCO_HAVE_INT64) && !defined(POCO_LONG_IS_64_BIT)

      static std::string format(Int64 value);
      /// Formats a 64-bit integer value in decimal notation.

      static std::string format(Int64 value, int width);
      /// Formats a 64-bit integer value in decimal notation,
      /// right justified in a field having at least the specified width.

      static std::string format0(Int64 value, int width);
      /// Formats a 64-bit integer value in decimal notation,
      /// right justified and zero-padded in a field having at least
      /// the specified width.

      static std::string formatHex(Int64 value, bool prefix = false);
      /// Formats a 64-bit integer value in hexadecimal notation.
      /// If prefix is true, "0x" prefix is prepended to the
      /// resulting string.
      /// The value is treated as unsigned.

      static std::string formatHex(Int64 value, int width, bool prefix = false);
      /// Formats a 64-bit integer value in hexadecimal notation,
      /// right justified and zero-padded in a field having at least
      /// the specified width.
      /// The value is treated as unsigned.
      /// If prefix is true, "0x" prefix is prepended to the resulting string.

      static std::string format(UInt64 value);
      /// Formats an unsigned 64-bit integer value in decimal notation.

      static std::string format(UInt64 value, int width);
      /// Formats an unsigned 64-bit integer value in decimal notation,
      /// right justified in a field having at least the specified width.

      static std::string format0(UInt64 value, int width);
      /// Formats an unsigned 64-bit integer value in decimal notation,
      /// right justified and zero-padded in a field having at least the
      /// specified width.

      static std::string formatHex(UInt64 value, bool prefix = false);
      /// Formats a 64-bit integer value in hexadecimal notation.
      /// If prefix is true, "0x" prefix is prepended to the
      /// resulting string.

      static std::string formatHex(UInt64 value, int width, bool prefix = false);
      /// Formats a 64-bit integer value in hexadecimal notation,
      /// right justified and zero-padded in a field having at least
      /// the specified width. If prefix is true, "0x" prefix is
      /// prepended to the resulting string.

#endif // defined(POCO_HAVE_INT64) && !defined(POCO_LONG_IS_64_BIT)

      static std::string format(float value);
      /// Formats a float value in decimal floating-point notation,
      /// according to std::printf's %g format with a precision of 8 fractional digits.

      static std::string format(double value);
      /// Formats a double value in decimal floating-point notation,
      /// according to std::printf's %g format with a precision of 16 fractional digits.

      static std::string format(double value, int precision);
      /// Formats a double value in decimal floating-point notation,
      /// according to std::printf's %f format with the given precision.

      static std::string format(double value, int width, int precision);
      /// Formats a double value in decimal floating-point notation,
      /// right justified in a field of the specified width,
      /// with the number of fractional digits given in precision.

      static std::string format(const void* ptr);
      /// Formats a pointer in an eight (32-bit architectures) or
      /// sixteen (64-bit architectures) characters wide
      /// field in hexadecimal notation.

      static std::string format(bool value, BoolFormat format = FMT_TRUE_FALSE);
      /// Formats a bool value in decimal/text notation,
      /// according to format parameter.

      static void append(std::string& str, int value);
      /// Formats an integer value in decimal notation.

      static void append(std::string& str, int value, int width);
      /// Formats an integer value in decimal notation,
      /// right justified in a field having at least
      /// the specified width.

      static void append0(std::string& str, int value, int width);
      /// Formats an integer value in decimal notation,
      /// right justified and zero-padded in a field
      /// having at least the specified width.

      static void appendHex(std::string& str, int value);
      /// Formats an int value in hexadecimal notation.
      /// The value is treated as unsigned.

      static void appendHex(std::string& str, int value, int width);
      /// Formats a int value in hexadecimal notation,
      /// right justified and zero-padded in
      /// a field having at least the specified width.
      /// The value is treated as unsigned.

      static void append(std::string& str, unsigned value);
      /// Formats an unsigned int value in decimal notation.

      static void append(std::string& str, unsigned value, int width);
      /// Formats an unsigned long int in decimal notation,
      /// right justified in a field having at least the
      /// specified width.

      static void append0(std::string& str, unsigned int value, int width);
      /// Formats an unsigned int value in decimal notation,
      /// right justified and zero-padded in a field having at
      /// least the specified width.

      static void appendHex(std::string& str, unsigned value);
      /// Formats an unsigned int value in hexadecimal notation.

      static void appendHex(std::string& str, unsigned value, int width);
      /// Formats a int value in hexadecimal notation,
      /// right justified and zero-padded in
      /// a field having at least the specified width.

      static void append(std::string& str, long value);
      /// Formats a long value in decimal notation.

      static void append(std::string& str, long value, int width);
      /// Formats a long value in decimal notation,
      /// right justified in a field having at least the
      /// specified width.

      static void append0(std::string& str, long value, int width);
      /// Formats a long value in decimal notation,
      /// right justified and zero-padded in a field
      /// having at least the specified width.

      static void appendHex(std::string& str, long value);
      /// Formats an unsigned long value in hexadecimal notation.
      /// The value is treated as unsigned.

      static void appendHex(std::string& str, long value, int width);
      /// Formats an unsigned long value in hexadecimal notation,
      /// right justified and zero-padded in a field having at least the
      /// specified width.
      /// The value is treated as unsigned.

      static void append(std::string& str, unsigned long value);
      /// Formats an unsigned long value in decimal notation.

      static void append(std::string& str, unsigned long value, int width);
      /// Formats an unsigned long value in decimal notation,
      /// right justified in a field having at least the specified
      /// width.

      static void append0(std::string& str, unsigned long value, int width);
      /// Formats an unsigned long value in decimal notation,
      /// right justified and zero-padded
      /// in a field having at least the specified width.

      static void appendHex(std::string& str, unsigned long value);
      /// Formats an unsigned long value in hexadecimal notation.

      static void appendHex(std::string& str, unsigned long value, int width);
      /// Formats an unsigned long value in hexadecimal notation,
      /// right justified and zero-padded in a field having at least the
      /// specified width.

#if defined(POCO_HAVE_INT64) && !defined(POCO_LONG_IS_64_BIT)

      static void append(std::string& str, Int64 value);
      /// Formats a 64-bit integer value in decimal notation.

      static void append(std::string& str, Int64 value, int width);
      /// Formats a 64-bit integer value in decimal notation,
      /// right justified in a field having at least the specified width.

      static void append0(std::string& str, Int64 value, int width);
      /// Formats a 64-bit integer value in decimal notation,
      /// right justified and zero-padded in a field having at least
      /// the specified width.

      static void appendHex(std::string& str, Int64 value);
      /// Formats a 64-bit integer value in hexadecimal notation.
      /// The value is treated as unsigned.

      static void appendHex(std::string& str, Int64 value, int width);
      /// Formats a 64-bit integer value in hexadecimal notation,
      /// right justified and zero-padded in a field having at least
      /// the specified width.
      /// The value is treated as unsigned.

      static void append(std::string& str, UInt64 value);
      /// Formats an unsigned 64-bit integer value in decimal notation.

      static void append(std::string& str, UInt64 value, int width);
      /// Formats an unsigned 64-bit integer value in decimal notation,
      /// right justified in a field having at least the specified width.

      static void append0(std::string& str, UInt64 value, int width);
      /// Formats an unsigned 64-bit integer value in decimal notation,
      /// right justified and zero-padded in a field having at least the
      /// specified width.

      static void appendHex(std::string& str, UInt64 value);
      /// Formats a 64-bit integer value in hexadecimal notation.

      static void appendHex(std::string& str, UInt64 value, int width);
      /// Formats a 64-bit integer value in hexadecimal notation,
      /// right justified and zero-padded in a field having at least
      /// the specified width.

#endif // defined(POCO_HAVE_INT64) && !defined(POCO_LONG_IS_64_BIT)

      static void append(std::string& str, float value);
      /// Formats a float value in decimal floating-point notation,
      /// according to std::printf's %g format with a precision of 8 fractional digits.

      static void append(std::string& str, double value);
      /// Formats a double value in decimal floating-point notation,
      /// according to std::printf's %g format with a precision of 16 fractional digits.

      static void append(std::string& str, double value, int precision);
      /// Formats a double value in decimal floating-point notation,
      /// according to std::printf's %f format with the given precision.

      static void append(std::string& str, double value, int width, int precision);
      /// Formats a double value in decimal floating-point notation,
      /// right justified in a field of the specified width,
      /// with the number of fractional digits given in precision.

      static void append(std::string& str, const void* ptr);
      /// Formats a pointer in an eight (32-bit architectures) or
      /// sixteen (64-bit architectures) characters wide
      /// field in hexadecimal notation.

   private:
   };


   //
   // inlines
   //

   inline std::string NumberFormatter::format(int value)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%d", value);
      return std::string(buf);
   }


   inline std::string NumberFormatter::format(int value, int width)
   {
      char fmt[NF_MAX_FMT_STRING_LEN] = {0};
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(fmt, NF_MAX_FMT_STRING_LEN, "%%%dd", width);
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, fmt, value);
      return std::string(buf);
   }


   inline std::string NumberFormatter::format0(int value, int width)
   {
      char fmt[NF_MAX_FMT_STRING_LEN] = {0};
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(fmt, NF_MAX_FMT_STRING_LEN, "%%0%dd", width);
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, fmt, value);
     	return std::string(buf);
   }


   inline std::string NumberFormatter::formatHex(int value, bool prefix)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%x", value);
      return std::string(buf);
   }


   inline std::string NumberFormatter::formatHex(int value, int width, bool prefix)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%x", value);
      return std::string(buf);
   }


   inline std::string NumberFormatter::format(unsigned value)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%d", value);
      return std::string(buf);
   }


   inline std::string NumberFormatter::format(unsigned value, int width)
   {
      char fmt[NF_MAX_FMT_STRING_LEN] = {0};
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(fmt, NF_MAX_FMT_STRING_LEN, "%%%dd", width);
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, fmt, value);
      return std::string(buf);

   }


   inline std::string NumberFormatter::format0(unsigned int value, int width)
   {
      char fmt[NF_MAX_FMT_STRING_LEN] = {0};
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(fmt, NF_MAX_FMT_STRING_LEN, "%%0%dd", width);
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, fmt, value);
      return std::string(buf);
   }


   inline std::string NumberFormatter::formatHex(unsigned value, bool prefix)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%x", value);
      return std::string(buf);   }


   inline std::string NumberFormatter::formatHex(unsigned value, int width, bool prefix)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%x", value);
      return std::string(buf);
   }


   inline std::string NumberFormatter::format(long value)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%ld", value);
      return std::string(buf);
   }


   inline std::string NumberFormatter::format(long value, int width)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%ld", value);
      return std::string(buf);
   }


   inline std::string NumberFormatter::format0(long value, int width)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%ld", value);
      return std::string(buf);
   }


   inline std::string NumberFormatter::formatHex(long value, bool prefix)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%ld", value);
      return std::string(buf);
   }


   inline std::string NumberFormatter::formatHex(long value, int width, bool prefix)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%ld", value);
      return std::string(buf);
   }


   inline std::string NumberFormatter::format(unsigned long value)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%ld", value);
      return std::string(buf);
   }


   inline std::string NumberFormatter::format(unsigned long value, int width)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%ld", value);
      return std::string(buf);
   }


   inline std::string NumberFormatter::format0(unsigned long value, int width)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%ld", value);
      return std::string(buf);

   }


   inline std::string NumberFormatter::formatHex(unsigned long value, bool prefix)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%ld", value);
      return std::string(buf);
   }


   inline std::string NumberFormatter::formatHex(unsigned long value, int width, bool prefix)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%ld", value);
      return std::string(buf);
   }


#if defined(POCO_HAVE_INT64) && !defined(POCO_LONG_IS_64_BIT)


   inline std::string NumberFormatter::format(Int64 value)
   {
      std::string result;
      intToStr(value, 10, result);
      return result;
   }


   inline std::string NumberFormatter::format(Int64 value, int width)
   {
      std::string result;
      intToStr(value, 10, result, false, width, ' ');
      return result;
   }
   
   
   inline std::string NumberFormatter::format0(Int64 value, int width)
   {
      std::string result;
      intToStr(value, 10, result, false, width, '0');
      return result;
   }
   
   
   inline std::string NumberFormatter::formatHex(Int64 value, bool prefix)
   {
      std::string result;
      uIntToStr(static_cast<UInt64>(value), 0x10, result, prefix);
      return result;
   }
   
   
   inline std::string NumberFormatter::formatHex(Int64 value, int width, bool prefix)
   {
      std::string result;
      uIntToStr(static_cast<UInt64>(value), 0x10, result, prefix, width, '0');
      return result;
   }
   
   
   inline std::string NumberFormatter::format(UInt64 value)
   {
      std::string result;
      uIntToStr(value, 10, result);
      return result;
   }
   
   
   inline std::string NumberFormatter::format(UInt64 value, int width)
   {
      std::string result;
      uIntToStr(value, 10, result, false, width, ' ');
      return result;
   }
   
   
   inline std::string NumberFormatter::format0(UInt64 value, int width)
   {
      std::string result;
      uIntToStr(value, 10, result, false, width, '0');
      return result;
   }
   
   
   inline std::string NumberFormatter::formatHex(UInt64 value, bool prefix)
   {
      std::string result;
      uIntToStr(value, 0x10, result, prefix);
      return result;
   }
   
   
   inline std::string NumberFormatter::formatHex(UInt64 value, int width, bool prefix)
   {
      std::string result;
      uIntToStr(value, 0x10, result, prefix, width, '0');
      return result;
   }
   
   
#endif // defined(POCO_HAVE_INT64) && !defined(POCO_LONG_IS_64_BIT)
   
   
   inline std::string NumberFormatter::format(float value)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%f", value);
      return std::string(buf);
   }
   
   
   inline std::string NumberFormatter::format(double value)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%f", value);
      return std::string(buf);;
   }
   
   
   inline std::string NumberFormatter::format(double value, int precision)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%f", value);
      return std::string(buf);
   }
   
   
   inline std::string NumberFormatter::format(double value, int width, int precision)
   {
      char buf[NF_MAX_INT_STRING_LEN] = {0};
      std::snprintf(buf, NF_MAX_INT_STRING_LEN, "%f", value);
      return std::string(buf);
   }
   
   
   inline std::string NumberFormatter::format(const void* ptr)
   {
      std::string result;
      append(result, ptr);
      return result;
   }

} // namespace nanogear


#endif // Foundation_NumberFormatter_INCLUDED
