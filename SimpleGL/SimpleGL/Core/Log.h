#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace SGL {

    struct Log {
    public:
        static std::shared_ptr<spdlog::logger>& getLogger() {
            static Log instance;
            return instance.logger;
        }

    private:
        std::shared_ptr<spdlog::logger> logger;

        Log() {
            spdlog::set_pattern("%^[%T] %n: %v%$");
            logger = spdlog::stdout_color_mt("FOCAL ENGINE");
            logger->set_level(spdlog::level::trace);
        }
    };

}

#define SGL_LOG_TRACE(...)    SGL::Log::getLogger()->trace(__VA_ARGS__)
#define SGL_LOG_DEBUG(...)    SGL::Log::getLogger()->debug(__VA_ARGS__)
#define SGL_LOG_INFO(...)     SGL::Log::getLogger()->info(__VA_ARGS__)
#define SGL_LOG_WARN(...)     SGL::Log::getLogger()->warn(__VA_ARGS__)
#define SGL_LOG_ERROR(...)    SGL::Log::getLogger()->error(__VA_ARGS__)
#define SGL_LOG_FATAL(...)    SGL::Log::getLogger()->critical(__VA_ARGS__)

#ifdef SGL_ENABLE_ASSERTS
#    define SGL_ASSERT(x, ...) { if(!(x)) { SGL_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#    define SGL_ASSERT(x, ...)
#endif
