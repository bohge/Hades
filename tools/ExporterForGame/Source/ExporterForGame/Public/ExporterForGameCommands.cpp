// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ExporterForGamePrivatePCH.h"
#include "ExporterForGameCommands.h"

#define LOCTEXT_NAMESPACE "FExporterForGameModule"

void FExporterForGameCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "ExporterForGame", "Execute ExporterForGame action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
