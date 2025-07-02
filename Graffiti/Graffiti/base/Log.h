#pragma once
#include "gfpch.h"
#include"Core.h"
#include "spdlog/spdlog.h"
#include <spdlog/fmt/ostr.h>


namespace Graffiti {
	
	
	class  Log
	{
		public:
			

			static void Init();

			inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
			inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		private:
			static std::shared_ptr<spdlog::logger> s_CoreLogger;
			static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};

}

#define GF_CORE_TRACE(...)    ::Graffiti::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GF_CORE_INFO(...)     ::Graffiti::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GF_CORE_WARN(...)     ::Graffiti::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GF_CORE_ERROR(...)    ::Graffiti::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GF_CORE_CRITICAL(...) ::Graffiti::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define GF_TRACE(...)         ::Graffiti::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GF_INFO(...)          ::Graffiti::Log::GetClientLogger()->info(__VA_ARGS__)
#define GF_WARN(...)          ::Graffiti::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GF_ERROR(...)         ::Graffiti::Log::GetClientLogger()->error(__VA_ARGS__)
#define GF_CRITICAL(...)      ::Graffiti::Log::GetClientLogger()->critical(__VA_ARGS__)