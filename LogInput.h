#pragma once

#include "InputLibraryDefine.h"

#ifdef LOG_TO_COUT
namespace LogSeverity
{
	enum BitFlag
	{
		/// <summary> Program is likely to crash or not work correctly. </summary>
		ERROR_MSG	= 0x01,
		/// <summary>Something that may affect the programs normal behaviour.</summary>
		/// <para>The program may run fine if a warning occurs.</para>
		WARNING_MSG = 0x02,
		/// <summary>Use this for informing about events.</summary>
		/// <para>This may be that a file were successfully read.</para>
		INFO_MSG	= 0x04,
		/// <summary>Use this for messages that gives debug output</summary>
		/// <para>This may be spamming position outputs from a unit.</para>
		DEBUG_MSG	= 0x08,
		//TODOJM can this be set to 0xff?
		/// <summary>Only use this as bitmask. Includes all severities.</summary>
		ALL		= 0x0F,
	};
}
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
