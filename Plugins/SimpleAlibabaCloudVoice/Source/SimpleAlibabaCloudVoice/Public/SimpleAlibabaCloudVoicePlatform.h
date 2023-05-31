// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#if defined(__WINDOWS__) || defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN32__) || defined(__TOS_WIN__)
#include <Mmsystem.h> 
#pragma comment(lib, "Winmm.lib") 
#endif