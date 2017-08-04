// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ExportNavigationPrivatePCH.h"
#include "ExportNavigationCommands.h"

#define LOCTEXT_NAMESPACE "FExportNavigationModule"

void FExportNavigationCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Exnavigation", "Export navigation mesh data", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
