// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "FrontWheel.h"




UFrontWheel::UFrontWheel()
{
	ShapeRadius = 80.f;
	ShapeWidth = 10.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 40.f;
	//50;

	SuspensionForceOffset = -4.0f;
	SuspensionMaxRaise = 8.0f;
	SuspensionMaxDrop = 12.0f;
	SuspensionNaturalFrequency = 9.0f;
	SuspensionDampingRatio = 1.05f;



}