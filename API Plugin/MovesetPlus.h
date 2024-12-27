#ifndef MovesetPlus_H 
#define MovesetPlus_H
#pragma once

#include <stdio.h>
#include "Vector3.h"

namespace moddingApi
{
	class MovesetPlus
	{
	public:
		static uintptr_t plMain[6];
		static int plMainId[6];

		static __int64 __fastcall meTest(__int64 a1, __int64 a2);
	};
}

#endif