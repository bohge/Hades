// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SlateBasics.h"
#include "ExportNavigationStyle.h"

class FExportNavigationCommands : public TCommands<FExportNavigationCommands>
{
public:

	FExportNavigationCommands()
		: TCommands<FExportNavigationCommands>(TEXT("ExportNavigation"), NSLOCTEXT("Contexts", "ExportNavigation", "ExportNavigation Plugin"), NAME_None, FExportNavigationStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
