#include "MovesetPlus.h"
#include "Main.h"
#include <iostream>
#include <vector>

#include <WinSock2.h>
#include <windows.h>

using namespace std;
using namespace moddingApi;

#define CHECK_BIT(var,pos) (((var)>>(pos)) & 1)
int prevFrame = 0;
int prevBattle = 0;

uintptr_t me_FindHealthPointer(int playerId);
void me_EnemyDispOff(__int64 p);
void me_TeleportPlayer(__int64 p, short axis, float newpos);
void me_ChangeSize(__int64 p, float newsize);
void me_PlaySoundEffect(short sound);
void me_SetPlayerVisibility(__int64 p, int cansee);
void me_SetPlayerParam(__int64 s, int param2, float value);
void me_ChangePlayerParam(__int64 s, int param2, float value);

__int64 __fastcall MovesetPlus::meTest(__int64 a1, __int64 a2)
{
	int character = *((int*)(a1 + 0xC8C));
	short param1 = *((short*)(a2 + 0x24));
	short param2 = *((short*)(a2 + 0x26));
	float param3 = *((float*)(a2 + 0x2C));
	//// cout << "A1: " << hex << a1 << ", id: " << hex << character << ", PARAM1: " << param1 << ", PARAM2: " << param2 << ", PARAM3: " << param3 << endl;

	int cha = -1;
	uintptr_t chaptr = 0;
	vector<uintptr_t> enemyptr;

	// Find player & enemy pointer
	for (int x = 0; x < 6; x++)
	{
		uintptr_t h = me_FindHealthPointer(x);

		if (h != 0)
		{
			if (*(uintptr_t*)(h - 0x8) == a1)
			{
				//// cout << "The correct character is " << x << endl;
				cha = x;
				chaptr = h;
			}
			else
			{
				//// cout << "Enemy pointer at " << x << endl;
				enemyptr.push_back(h);
			}
		}
	}

	if (cha != -1)
	{
		// cout << "Health: " << *(float*)(chaptr) << ", ptr: " << (chaptr) << endl;
	}

	switch (param1)
	{
	default:
		me_EnemyDispOff(a1);
		break;
	case 1:
		me_TeleportPlayer(a1, param2, param3);
		break;
	case 2:
		me_ChangeSize(a1, param3);
		break;
	case 3:
		me_PlaySoundEffect(param2);
		break;
	case 4:
		me_SetPlayerVisibility(a1, param2);
		break;
	case 5:
		me_SetPlayerParam(chaptr, param2, param3);
		break;
	case 6:
		me_ChangePlayerParam(chaptr, param2, param3);
		break;
	case 7:
		for (int x = 0; x < enemyptr.size(); x++) me_SetPlayerParam(enemyptr[x], param2, param3);
		break;
	case 8:
		for (int x = 0; x < enemyptr.size(); x++) me_ChangePlayerParam(enemyptr[x], param2, param3);
		break;
	}

	return 1;
}

uintptr_t me_FindHealthPointer(int playerId)
{
	uintptr_t *p1 = 0;
	uintptr_t *p2 = 0;
	uintptr_t *p3 = 0;
	uintptr_t *p4 = 0;
	uintptr_t *p5 = 0;
	uintptr_t *p6 = 0;
	uintptr_t *p7 = 0;

	p1 = (uintptr_t*)(Main::moduleBase - 0xC00 + 0x0161C948);
	//// cout << "p1: " << p1 << endl;
	if (p1 == 0 || *p1 == 0) return 0;
	p2 = (uintptr_t*)(*p1 + 0x80);
	//// cout << "p2: " << p2 << endl;
	if (p2 == 0 || *p2 == 0) return 0;
	p3 = (uintptr_t*)(*p2 + 0x68);
	//// cout << "p3: " << p3 << endl;
	if (p3 == 0 || *p3 == 0) return 0;
	p4 = (uintptr_t*)(*p3 + 0x60);
	//// cout << "p4: " << p4 << endl;
	if (p4 == 0 || *p4 == 0) return 0;
	p5 = (uintptr_t*)(*p4 + 0x00);
	//// cout << "p5: " << p5 << endl;
	if (p5 == 0 || *p5 == 0) return 0;

	switch (playerId)
	{
	case 0:
		p6 = p5 + 0;
		break;
	case 1:
		p6 = p5 + 1;
		break;
	case 2:
		p6 = p5 + 2;
		break;
	case 3:
		p6 = p5 + 0x12; // 0x90
		break;
	case 4:
		p6 = p5 + 0x13; // 0x98
		break;
	case 5:
		p6 = p5 + 0x14; // 0xA0
		break;
	}

	//// cout << "p6: " << p6 << endl;
	if (p6 == 0 || *p6 == 0) return 0;
	//// cout << "*p6: " << *p6 << endl;

	p7 = ((uintptr_t*)(*p6)) + 7; // 7 * 8 = 0x38
	if (p7 == 0 || *p7 == 0) return 0;
	//// cout << "p7: " << p7 << endl;
	//// cout << "*p7: " << *p7 << endl;

	//// cout << "Finished looking" << endl << endl;

	return (uintptr_t)p7;
}

void me_EnemyDispOff(__int64 p)
{
	// original function in assembly
	char functioncode[] = {
		0x48, 0x83, 0xEC, 0x28,
		0x48, 0x8B, 0x01,
		0xFF, 0x90, 0x88, 0x0D, 0x00, 0x00,
		0x48, 0x85, 0xC0,
		0x74, 0xC,
		0x48, 0x8B, 0x10,
		0x48, 0x8B, 0xC8,
		0xFF, 0x92, 0x98, 0x0B, 0x00, 0x00,
		0xB8, 0x01, 0x00, 0x00, 0x00,
		0x48, 0x83, 0xC4, 0x28,
		0xC3 };

	// cout << "Changing protection of " << &functioncode[0] << "..." << endl;
	DWORD dwOld = 0;
	VirtualProtect(&functioncode[0], sizeof(functioncode), PAGE_EXECUTE_READWRITE, &dwOld);

	// cout << "Creating function..." << endl;
	typedef __int64(__fastcall * funct)(__int64 a);
	funct fc;
	fc = (funct)(&functioncode[0]); // UPDATED

	// cout << "Executing function..." << endl;
	fc(p);

	// cout << "Function executed successfully" << endl;
}

void me_TeleportPlayer(__int64 p, short axis, float newpos)
{
	switch (axis)
	{
	case 0:
	{
		// cout << "Function 00: Teleport x to " << newpos << endl;
		float * pos = (float*)(p + 0x70);
		*pos = newpos;
		break;
	}
	case 1:
	{
		// cout << "Function 00: Teleport y to " << newpos << endl;
		float * pos = (float*)(p + 0x74);
		*pos = newpos;
		break;
	}
	case 2:
	{
		// cout << "Function 00: Teleport z to " << newpos << endl;
		float * pos = (float*)(p + 0x78);
		*pos = newpos;
		break;
	}
	}
}

void me_ChangeSize(__int64 p, float newsize)
{
	// cout << "Function 01: Changing size to " << newsize << endl;

	float * size = (float*)(p + 0x200);
	*size = newsize;
}

void me_PlaySoundEffect(short se)
{
	// cout << "Function 02: Playing sound effect with ID " << se << endl;
	//ccAdvPlaySound_2D(se);
}

void me_SetPlayerVisibility(__int64 p, int cansee)
{
	int * visible = (int*)(p + 0xCB8);
	*visible = cansee;
}

void me_SetPlayerParam(__int64 s, int param2, float value)
{
	float * floatParameter = 0;

	switch (param2)
	{
	case 0: // health
		floatParameter = (float*)(s + 0x00);
		break;
	case 1: // maxhealth
		floatParameter = (float*)(s + 0x04);
		break;
	case 2: // chakra
		floatParameter = (float*)(s + 0x08);
		break;
	case 3: // maxchakra
		floatParameter = (float*)(s + 0x0C);
		break;
	case 4: // sub
		floatParameter = (float*)(s + 0x10);
		break;
	case 5: // maxsub
		floatParameter = (float*)(s + 0x14);
		break;
	}

	*floatParameter = value;
}

void me_ChangePlayerParam(__int64 s, int param2, float value)
{
	float * floatParameter = 0;

	switch (param2)
	{
	case 0: // health
		floatParameter = (float*)(s + 0x00);
		break;
	case 1: // maxhealth
		floatParameter = (float*)(s + 0x04);
		break;
	case 2: // chakra
		floatParameter = (float*)(s + 0x08);
		break;
	case 3: // maxchakra
		floatParameter = (float*)(s + 0x0C);
		break;
	case 4: // sub
		floatParameter = (float*)(s + 0x10);
		break;
	case 5: // maxsub
		floatParameter = (float*)(s + 0x14);
		break;
	}

	*floatParameter += value;
}