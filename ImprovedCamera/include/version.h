/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#pragma comment(lib, "Version.lib")

#define MAKE_STR_HELPER(a_str) #a_str
#define MAKE_STR(a_str) MAKE_STR_HELPER(a_str)

#define VERSION_PRODUCTNAME_DESCRIPTION_STR "Improved Camera SE"
#define VERSION_MAJOR                       1
#define VERSION_MINOR                       1
#define VERSION_REVISION                    0b2
#define VERSION_BUILD                       4127
#define VERSION_STR                         MAKE_STR(VERSION_MAJOR) "." MAKE_STR(VERSION_MINOR) "." MAKE_STR(VERSION_REVISION) "." MAKE_STR(VERSION_BUILD)

#define VERSION_PRODUCTNAME_STR             "ImprovedCameraSE"
#define VERSION_YEARS_STR                   "© 2021-2023"
#define VERSION_AUTHOR_STR                  "IC Team"
#define VERSION_LICENSE_STR                 "(MPL 2.0)"
#define VERSION_COPYRIGHT_STR               VERSION_YEARS_STR " " VERSION_AUTHOR_STR " " VERSION_LICENSE_STR
