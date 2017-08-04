// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SlateBasics.h"
#include "ExporterForGameStyle.h"

class FExporterForGameCommands : public TCommands<FExporterForGameCommands>
{
public:

	FExporterForGameCommands()
		: TCommands<FExporterForGameCommands>(TEXT("ExporterForGame"), NSLOCTEXT("Contexts", "ExporterForGame", "ExporterForGame Plugin"), NAME_None, FExporterForGameStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
