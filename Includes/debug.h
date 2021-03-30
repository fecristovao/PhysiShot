#ifndef _FILE_INCLUDE_DEBUG_
    #define _FILE_INCLUDE_DEBUG_
    #ifdef _DEBUG
        #define DEBUG(fmt , args...) \
            fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, args)
        #define TRACE_ENTER(x)  DEBUG("<%s>\n", __func__)
        #define TRACE_EXIT(x)   DEBUG("</%s ret = %d>\n", __func__, x)
    #else
        #define DEBUG(...)
        #define TRACE_ENTER(x)
        #define TRACE_EXIT(x)
    #endif
#endif
