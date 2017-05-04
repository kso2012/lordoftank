// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "RearWheel.h"




URearWheel::URearWheel()
{
	ShapeRadius = 80.f;
	ShapeWidth = 10.0f;
	bAffectedByHandbrake = true;
	SteerAngle = 0.f;

	SuspensionForceOffset = -4.0f;
	SuspensionMaxRaise = 8.0f;
	SuspensionMaxDrop = 12.0f;
	SuspensionNaturalFrequency = 9.0f;
	SuspensionDampingRatio = 1.05f;

}