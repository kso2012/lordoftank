// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "Effects/TankCameraShake.h"



UTankCameraShake::UTankCameraShake()
{
	OscillationDuration = 0.3f;
	RotOscillation.Pitch.Amplitude = 1.f;
	RotOscillation.Pitch.Frequency = 100.f;
	RotOscillation.Yaw.Amplitude = 1.f;
	RotOscillation.Yaw.Frequency = 100.f;
	LocOscillation.X.Amplitude = -10000.f;
	LocOscillation.X.Frequency = 1.f;
	LocOscillation.X.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
	//LocOscillation.Z.Amplitude = 10.f;
	//LocOscillation.Z.Frequency = 50.f;
	
}
