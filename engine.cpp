#ifndef ENGINE
    #define ENGINE


#include "event.cpp"
#include "basics.cpp"
#include <cstring>
#include <queue>
#include <memory>


#ifdef SFML
    #ifdef TX_LIB
        #error Defined two libriries.
    #endif

    #include "sfml_define.cpp"
#endif

#ifdef TX_LIB
    #include "tx_define.cpp"
    #include <windows.h>
    #include <winuser.h>
#endif

#ifndef SFML
    #ifndef TX_LIB
        #error NO lib defined!
    #endif
#endif

#endif
