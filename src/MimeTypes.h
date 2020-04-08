// --------------------------------------------------------------------------------------------------------------------
// <copyright file="MimeTypes.h" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#pragma once

#include <string.h>

class MimeTypes
{
public:
    static const char* getType(const char* path);
    static const char* getExtension(const char* type, int skip = 0);

private:
    struct entry {
        const char* fileExtension;
        const char* mimeType;
    };
    static MimeTypes::entry types[347];
    static int strcmpi(const char* s1, const char* s2);
};
