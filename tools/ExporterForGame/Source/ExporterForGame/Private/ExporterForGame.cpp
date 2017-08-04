// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ExporterForGamePrivatePCH.h"

#include "SlateBasics.h"
#include "SlateExtras.h"

#include "ExporterForGameStyle.h"
#include "ExporterForGameCommands.h"

#include "LevelEditor.h"
#include "ExporterUtil.h"

static const FName ExporterForGameTabName("ExporterForGame");

#define LOCTEXT_NAMESPACE "FExporterForGameModule"

void FExporterForGameModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FExporterForGameStyle::Initialize();
	FExporterForGameStyle::ReloadTextures();

	FExporterForGameCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FExporterForGameCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FExporterForGameModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FExporterForGameModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FExporterForGameModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
}

void FExporterForGameModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FExporterForGameStyle::Shutdown();

	FExporterForGameCommands::Unregister();
}

void FExporterForGameModule::PluginButtonClicked()
{
	if (GEditor->GetSelectedActorCount() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Please Select Something"));
		return;
	}
	UWorld* World = GEngine->GetWorldFromContextObject(GEditor->GetSelectedActors()->GetSelectedObject(0));
	FString LevelName = World->GetName();
	FString SaveDirectory = FString("D:/GameData");
	FString FileName = LevelName + FString(".txt");

	TArray<FString>MonsterID;
	if (!FFileHelper::LoadANSITextFileToStrings(TEXT("D:/GameData/MonsterID.txt"), NULL, MonsterID))
	{
		UE_LOG(LogTemp, Error, TEXT("Read MonsterID failed"));
		return;
	}

	TMap<FString, FString>MonsterIDMap;
	for (FString& Each : MonsterID)
	{
		FString Name;
		FString ID;
		if (Each.Split(" ", &Name, &ID) || Each.Split("\t", &Name, &ID))
		{
			//UE_LOG(LogTemp, Error, TEXT("Name is %s, ID is %s"), *Name, *ID);
			MonsterIDMap.Add(Name, ID);
		}
	}

	TArray<FString> SaveText;
	TArray<FString> PlayerStarts;
	TArray<FString> ProducePropPoses;
	USelection* SelectedActor = GEditor->GetSelectedActors();
	for (FSelectionIterator LvlActorIt(*SelectedActor); LvlActorIt; ++LvlActorIt)
	{
		AActor* Actor = Cast<AActor>(*LvlActorIt);
		FVector Position = Actor->GetActorLocation();
		FString ID("0");
		for (auto& Each : MonsterIDMap)
		{
			if (Actor->GetName().Contains(Each.Value))
			{
				ID = Each.Key;
			}
		}

		if (ID != "0")
		{
			SaveText.Add(FString::Printf(TEXT("%s	(%f,%f,%f)"), *ID, Position.X, Position.Y, Position.Z));
		}
		else
		{
			if (Actor->GetName().Contains("PlayerStart"))
			{
				PlayerStarts.Add(FString::Printf(TEXT("(%f,%f,%f)"), Position.X, Position.Y, Position.Z));
			}
			else if (LvlActorIt->GetName().Contains("ProducePropPos"))
			{
				ProducePropPoses.Add(FString::Printf(TEXT("(%f,%f,%f)"), Position.X, Position.Y, Position.Z));
			}
		}
	}

	FString Temp;
	for (auto& Each : PlayerStarts)
	{
		Temp += "\"" + Each + "\",";
	}
	Temp.RemoveFromEnd(",");
	SaveText.Add(FString::Printf(TEXT("\"PlayerStart\" : [%s]"), *Temp));
	Temp = "";
	for (auto& Each : ProducePropPoses)
	{
		Temp += "\"" + Each + "\",";
	}
	Temp.RemoveFromEnd(",");
	SaveText.Add(FString::Printf(TEXT("\"ProducePropPos\" : [%s]"), *Temp));


	FExporterUtil::SaveStringArrayToFile(SaveDirectory, FileName, SaveText, true);
	
}

void FExporterForGameModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FExporterForGameCommands::Get().PluginAction);
}

void FExporterForGameModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FExporterForGameCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FExporterForGameModule, ExporterForGame)