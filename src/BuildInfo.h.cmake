
#pragma once

#cmakedefine BUILD_ID

#ifdef BUILD_ID

#undef BUILD_ID

#define BUILD_SERIES_NAME "@BUILD_SERIES_NAME@"
#define BUILD_ID          "@BUILD_ID@"
#define BUILD_COMMIT_ID   "@BUILD_COMMIT_ID@"
#define BUILD_DATETIME    "@BUILD_DATETIME@"
#endif

