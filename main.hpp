#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>
#include <inttypes.h>
#include <iostream>
#include <string>

#define DEPRECATED [[deprecated("don`t use it")]]
#define NODISCARD [[nodiscard("did you left something")]]

#ifdef DEBUG

#define debug(str) std::wcout << str << std::endl

#else

#define debug(str)

#endif

#define exept(str) throw Exeption(str, __FILE__, __LINE__)

constexpr double PI = 3.1415926535897932384626433832795;
constexpr double DEG_TO_RAD = 0.017453292519943295769236907684886;
constexpr double EPS = 1e-6;

constexpr float F_PI = 3.1415926535897932384626433832795F;
constexpr float F_DEG_TO_RAD = 0.017453292519943295769236907684886F;
constexpr float F_EPS = 1e-6F;

constexpr float S_TEXTURE = 16.F;
constexpr float S_TILE = 64.F;

inline NODISCARD std::wstring wide(const std::string str) {
  return std::wstring(str.begin(), str.end());
}

#pragma warning(push)
#pragma warning(disable: 4244)
inline NODISCARD std::string shrink(const std::wstring wstr) {
  return std::string(wstr.begin(), wstr.end());
}
#pragma warning(pop)

template <class E, typename T = std::underlying_type_t<E>>
NODISCARD __forceinline T to_underlying(E enumeration) {
  return static_cast<T>(enumeration);
}
