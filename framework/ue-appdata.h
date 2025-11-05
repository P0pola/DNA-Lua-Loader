#pragma once

#include <cstdint>

// Application-specific types
#include "../appdata/ue-types.h"





// Application-specific functions
#define DO_APP_FUNC(addr_cn, addr_global, r, n, p) extern r (*n) p

namespace app {
	#include "../appdata/ue-functions.h"
}
#undef DO_APP_FUNC


