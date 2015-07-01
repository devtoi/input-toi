#pragma once

//#include "Alloc.h"
#include INPUT_ALLOCATION_HEADER

#ifdef LOG_TO_COUT
static void LogInput( const rString& message, const char* category = "Input", int severityMask = 0)
{
	std::cout << category << ": " << message << std::endl;
}
#else
#include <utility/Logger.h>
static void LogInput( const rString& message, const char* category = "Input", int severityMask = LogSeverity::INFO_MSG )
{
	Logger::Log( message, category, static_cast<LogSeverity::BitFlag>( severityMask ) );
}
#endif
