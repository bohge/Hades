// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


class FExporterUtil
{
public:
	static bool SaveStringTextToFile(FString SaveDirectory, FString JoyfulFileName, FString SaveText, bool AllowOverWriting = false);
	static bool SaveStringArrayToFile(FString SaveDirectory, FString JoyfulFileName, TArray<FString> SaveText, bool AllowOverWriting = false);
};