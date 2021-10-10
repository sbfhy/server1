#include "mgr_base_config.h"

#include <string>

const std::string & G_GetProjectDir() 
{
#ifdef WIN32 
    static std::string g_projectDir = "..\\..\\..\\";
#else 
    static std::string g_projectDir = "../../..";
#endif
    return g_projectDir;
}
