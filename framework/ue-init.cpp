// Generated C++ file by Il2CppInspector - http://www.djkaty.com - https://github.com/djkaty
// IL2CPP application initializer

#include "pch-ue.h"

#include "ue-appdata.h"
#include "ue-init.h"
#include "helpers.h"



// Application-specific functions
#define DO_APP_FUNC(addr_cn, addr_global, r, n, p) r (*n) p

namespace app {
#include "ue-functions.h"
}
#undef DO_APP_FUNC




// ue application initializer
void init_ue()
{
	// Get base address of ue module
	uintptr_t baseAddress = ue_get_base_address();

    using namespace app;



    // Define function addresses
    const auto region = curr_server;
    #define DO_APP_FUNC(addr_cn, addr_global, r, n, p) n = ((addr_cn || addr_global) ? (r (*) p)(baseAddress + ((region == GameServerRegion::CN) ? (addr_cn) : (addr_global))) : nullptr)

    #include "ue-functions.h"
    #undef DO_APP_FUNC



}