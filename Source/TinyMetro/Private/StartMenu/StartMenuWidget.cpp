// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenu/StartMenuWidget.h"
#include "GenericPlatform/GenericPlatformFile.h"

bool UStartMenuWidget::DeleteDirectory(const FString& DirectoryPath)
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    // Check if the directory exists
    if (!PlatformFile.DirectoryExists(*DirectoryPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Directory does not exist: %s"), *DirectoryPath);
        return false;
    }

    // Delete the directory and its contents recursively
    if (PlatformFile.DeleteDirectoryRecursively(*DirectoryPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Deleted directory and its contents: %s"), *DirectoryPath);
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to delete directory: %s"), *DirectoryPath);
        return false;
    }
}