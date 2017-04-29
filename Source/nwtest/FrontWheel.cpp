// Fill out your copyright notice in the Description page of Project Settings.

#include "nwtest.h"
#include "FrontWheel.h"




UFrontWheel::UFrontWheel()
{
	ShapeRadius = 80.f;
	ShapeWidth = 10.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 90.f;
	//50;
}