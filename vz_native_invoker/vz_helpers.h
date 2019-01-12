// Copyright(C) 1999 - 2016 Parallels IP Holdings GmbH

// Based on VZ doc examples
// https://docs.virtuozzo.com/legacy/vz6/Virtuozzo_Virtualization_SDK_Programmers_Guide.pdf
#pragma once

#include "Parallels.h"

#include "SdkWrap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

#include <unistd.h>

using namespace std;

char * convert_to_carray(std::string str);

PRL_RESULT GetHDDDev(PRL_HANDLE &hVm, PRL_HANDLE &hVmDev);
PRL_RESULT ResizeVmTo(PRL_HANDLE hServer, PRL_HANDLE &hVm, PRL_UINT32 size);
PRL_RESULT CloneVmTo(PRL_HANDLE hServer, PRL_HANDLE &hVm, PRL_STR newVmName);
PRL_RESULT GetVmConfigByName(PRL_HANDLE hServer, PRL_STR sVmName, PRL_HANDLE &hVmConfig);
PRL_RESULT GetVmByName(PRL_HANDLE hServer, PRL_STR sVmName, PRL_HANDLE &hVm);
PRL_RESULT Login(PRL_HANDLE &hServer, char* szServer, char* szUsername, char* szPassword);
PRL_RESULT LogOff(PRL_HANDLE &hServer);
