#pragma once

#include <memory>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

class Log {
public:
  static void init();

  inline static std::shared_ptr<spdlog::logger> &getCoreLogger() {
    return s_CoreLogger;
  }
  inline static std::shared_ptr<spdlog::logger> &getClientLogger() {
    return s_ClientLogger;
  }

private:
  static std::shared_ptr<spdlog::logger> s_CoreLogger;
  static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

#define LOG_CORE_TRACE(...) ::Log::getCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_INFO(...) ::Log::getCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...) ::Log::getCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...) ::Log::getCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_CRITICAL(...) ::Log::getCoreLogger()->critical(__VA_ARGS__)

#define LOG_TRACE(...) ::Log::getClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) ::Log::getClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::Log::getClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Log::getClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Log::getClientLogger()->critical(__VA_ARGS__)

#ifdef DEBUG
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define DEBUG_BREAK __debugbreak()
#elif defined(__linux__) || defined(__APPLE__)
#define DEBUG_BREAK __builtin_trap()
#else
#define DEBUG_BREAK
#endif

#define ASSERT(x, ...)                                                         \
  {                                                                            \
    if (!(x)) {                                                                \
      LOG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);                    \
      LOG_CORE_ERROR("File: {0}, Line: {1}", __FILE__, __LINE__);              \
      DEBUG_BREAK;                                                             \
    }                                                                          \
  }
#else
#define ASSERT(x, ...)
#endif