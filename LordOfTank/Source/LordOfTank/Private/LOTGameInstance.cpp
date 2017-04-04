// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTGameInstance.h"




ULOTGameInstance::ULOTGameInstance()
{
	;
}

void ULOTGameInstance::ClickMultiBT()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("멀티버튼 클릭"));

}