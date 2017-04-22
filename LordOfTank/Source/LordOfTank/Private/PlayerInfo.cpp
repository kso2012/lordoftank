// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "PlayerInfo.h"
#include "LOTDrone.h"
#include "LOTPlayer.h"

PlayerInfoSaved SavedPlayer = PlayerInfoSaved::PlayerInfoSaved();

PlayerInfo::PlayerInfo() {
}

void PlayerInfo::InsertPawn(ALOTPlayer *T, ALOTDrone *D, int PlayerNum) {
	SavedPlayer.SavePlayerInfo(PlayerNum, T, D, AP);
}

PlayerInfo PlayerInfo::ReturnData(int PN) {
	return SavedPlayer.ReturnData(PN);
}

PlayerInfoSaved::PlayerInfoSaved() {

}

void PlayerInfoSaved::SavePlayerInfo(int PN, ALOTPlayer *T, ALOTDrone *D, float AP) {
	SavedPlayer.PlayerInfoData[PN] = PlayerInfo(PN, T, D, AP);
}