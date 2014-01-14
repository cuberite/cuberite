
#pragma once

#ifdef USE_GCC_VECTORS 
typedef int int2 __attribute__ ((vector_size (sizeof(int)*2)));
#endif
