// Created by Credix - Licence: https://creativecommons.org/licenses/by-nc-sa/4.0/
#ifndef CREATEMOVE_H
#define CREATEMOVE_H

#include "../Headers/Interfaces.h"
#include "../Hacks/Hacks.h"

typedef void(__thiscall* CreateMove_t)(void*, float, CUserCmd*);
CreateMove_t oCreateMove;
unsigned int* oCreateMoveFunc;

void CorrectMovement(QAngle vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove)
{
	float deltaView = pCmd->viewangles.y - vOldAngles.y;
	float f1;
	float f2;

	if (vOldAngles.y < 0.f)
		f1 = 360.0f + vOldAngles.y;
	else
		f1 = vOldAngles.y;

	if (pCmd->viewangles.y < 0.0f)
		f2 = 360.0f + pCmd->viewangles.y;
	else
		f2 = pCmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);

	deltaView = 360.0f - deltaView;

	pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}

bool __fastcall hkCreateMove(void* ecx, void* edx, float sampleTime, CUserCmd* UserCmd) {
	oldViewAngles = UserCmd->viewangles;
	Bunnyhop::Tick(UserCmd);
	if (Settings::Rage::antiAimType != ANTIAIM_NONE && Settings::Rage::bAntiaim && !GetAsyncKeyState(VK_LBUTTON)) {
		UserCmd->viewangles.x = antiAimX;
		UserCmd->viewangles.y = antiAimY;
		UserCmd->viewangles.z = antiAimZ;
		if (Settings::Rage::antiAimType == ANTIAIM_SLOW_SPIN) {
			antiAimY -= 1.0f;
		} else if (Settings::Rage::antiAimType == ANTIAIM_RANDOM_SPIN) {
			srand(time(0));
			antiAimX = rand() % 360;
			srand(time(0));
			antiAimY = rand() % 360;
		} else if (Settings::Rage::antiAimType == ANTIAIM_YAW_DOWN) {
			int newX = rand() % 360;
			newX += 271;
			if (newX > 360.0f) {
				newX -= 360.0f;
			}
			antiAimX = newX;
			antiAimY = oldViewAngles.y - 180.0f;
		} else if (Settings::Rage::antiAimType == ANTIAIM_YAW_RANDOM) {
			antiAimX = 89.0f;
			antiAimY = oldViewAngles.y - 180.0f;
		}
	}
	if (GetAsyncKeyState(VK_LBUTTON)) {
		SilentAim::Shoot();
	}
	oCreateMove(ecx, sampleTime, UserCmd);
	CorrectMovement(oldViewAngles, UserCmd, UserCmd->forwardmove, UserCmd->sidemove);
	return false;
}

#endif