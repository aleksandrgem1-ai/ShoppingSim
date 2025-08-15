// SaveLoadSubsystem.cpp (ÈÑÏÐÀÂËÅÍÍÛÉ)

#include "SaveLoadSubsystem.h"
#include "EconomySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "MallSimSaveGame.h"
#include "StoreZoneActor.h"
#include "StoreZoneData.h"

void USaveLoadSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
  Super::Initialize(Collection);

  const float AutosaveInterval = 300.0f; // 5 ìèíóò
  GetGameInstance()->GetTimerManager().SetTimer(AutosaveTimerHandle, this,
                                                &USaveLoadSubsystem::SaveGame,
                                                AutosaveInterval, true);
  UE_LOG(LogTemp, Log, TEXT("Autosave timer started. Interval: %f seconds."),
         AutosaveInterval);
}

void USaveLoadSubsystem::SaveGame() {
  UMallSimSaveGame *SaveGameInstance = Cast<UMallSimSaveGame>(
      UGameplayStatics::CreateSaveGameObject(UMallSimSaveGame::StaticClass()));
  if (!SaveGameInstance)
    return;

  UEconomySubsystem *EconomySubsystem =
      GetGameInstance()->GetSubsystem<UEconomySubsystem>();
  if (EconomySubsystem) {
    SaveGameInstance->PlayerMoney = EconomySubsystem->GetCurrentBalance();
  }

  TArray<AActor *> FoundZones;
  if (UWorld *World = GetGameInstance()->GetWorld()) {
    UGameplayStatics::GetAllActorsOfClass(World, AStoreZoneActor::StaticClass(),
                                          FoundZones);
  }
  SaveGameInstance->PlacedZones.Empty();

  for (AActor *Actor : FoundZones) {
    AStoreZoneActor *Zone = Cast<AStoreZoneActor>(Actor);
    if (Zone && Zone->ZoneData) {
      FZoneSaveData ZoneDataToSave;
      ZoneDataToSave.ZoneDataPath = FSoftObjectPath(Zone->ZoneData);
      ZoneDataToSave.ZoneTransform = Zone->GetActorTransform();
      SaveGameInstance->PlacedZones.Add(ZoneDataToSave);
    }
  }

  UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);
  UE_LOG(LogTemp, Warning, TEXT("Game Saved! (Autosave or manual)"));
}

void USaveLoadSubsystem::LoadGame() {
  UMallSimSaveGame *LoadedGame = Cast<UMallSimSaveGame>(
      UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
  if (!LoadedGame) {
    UE_LOG(LogTemp, Warning, TEXT("No save game found to load."));
    return;
  }

  UEconomySubsystem *EconomySubsystem =
      GetGameInstance()->GetSubsystem<UEconomySubsystem>();
  if (EconomySubsystem) {
    EconomySubsystem->TrySpendMoney(EconomySubsystem->GetCurrentBalance());
    EconomySubsystem->AddMoney(LoadedGame->PlayerMoney);
  }

  if (UWorld *World = GetGameInstance()->GetWorld()) {
    TArray<AActor *> ExistingZones;
    UGameplayStatics::GetAllActorsOfClass(World, AStoreZoneActor::StaticClass(),
                                          ExistingZones);
    for (AActor *Actor : ExistingZones) {
      Actor->Destroy();
    }

    for (const FZoneSaveData &SavedZoneData : LoadedGame->PlacedZones) {
      UStoreZoneData *ZoneDataAsset =
          Cast<UStoreZoneData>(SavedZoneData.ZoneDataPath.TryLoad());
      if (ZoneDataAsset) {
        FActorSpawnParameters SpawnParams;
        AStoreZoneActor *NewZone = World->SpawnActor<AStoreZoneActor>(
            AStoreZoneActor::StaticClass(), SavedZoneData.ZoneTransform,
            SpawnParams);
        if (NewZone) {
          NewZone->ZoneData = ZoneDataAsset;
          NewZone->PostInitializeComponents();
        }
      }
    }
  }

  UE_LOG(LogTemp, Warning, TEXT("Game Loaded!"));
}