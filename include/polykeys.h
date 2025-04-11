#ifndef POLYKEYS_H
#define POLYKEYS_H

#ifdef _MSC_VER
#define U64_POLY(u) (u##ui64) // Win32
#else
#define U64_POLY(u) (u##ULL) // POSIX
#endif

#endif
