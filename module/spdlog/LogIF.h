#ifndef __LOGIF_H__
#define __LOGIF_H__

#include "spdlog/spdlog.h"

using namespace spd = spdlog;
namespace wendouzi
{
#ifndef CONSOLE_LOG_INIT
#define CONSOLE_LOG_INIT {\
    auto console = spd::stdout_color_mt("console");\
}
#endif

#ifdef CONSOLE_DEBUG_ON
#define CONSOLE_DEBUG(...) spd::get("console")->debug(__VA_ARGS__)
#define CONSOLE_DEBUG_IF(flag, ...) spd::get("console")->debug_if(flag, __VA_ARGS__)
#else 
#define CONSOLE_DEBUG(...) 
#define CONSOLE_DEBUG_IF(flag, ...) 
#endif 

}

#endif