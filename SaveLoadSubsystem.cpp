// SaveLoadSubsystem.cpp

#include "SaveLoadSubsystem.h"
#include "EconomySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "MallSimSaveGame.h"
#include "StoreZoneActor.h"
#include "StoreZoneData.h"

void USaveLoadSubsystem::SaveGame() {
  // Создаем или получаем существующий объект сохранения
  UMallSimSaveGame *SaveGameInstance = Cast<UMallSimSaveGame>(
      UGameplayStatics::CreateSaveGameObject(UMallSimSaveGame::StaticClass()));
  if (!SaveGameInstance)
    return;

  // 1. Сохраняем деньги
  UEconomySubsystem *EconomySubsystem =
      GetGameInstance()->GetSubsystem<UEconomySubsystem>();
  if (EconomySubsystem) {
    SaveGameInstance->PlayerMoney = EconomySubsystem->GetCurrentBalance();
  }

  // 2. Сохраняем все размещенные зоны
  TArray<AActor *> FoundZones;
  UGameplayStatics::GetAllActorsOfClass(
      GetWorld(), AStoreZoneActor::StaticClass(), FoundZones);
  SaveGameInstance->PlacedZones.Empty(); // Очищаем старые данные

  for (AActor *Actor : FoundZones) {
    AStoreZoneActor *Zone = Cast<AStoreZoneActor>(Actor);
    if (Zone && Zone->ZoneData) {
      FZoneSaveData ZoneDataToSave;
      ZoneDataToSave.ZoneDataPath = FSoftObjectPath(Zone->ZoneData);
      ZoneDataToSave.ZoneTransform = Zone->GetActorTransform();
      SaveGameInstance->PlacedZones.Add(ZoneDataToSave);
    }
  }

  // Сохраняем на диск
  UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);
  UE_LOG(LogTemp, Warning, TEXT("Game Saved!"));
}

void USaveLoadSubsystem::LoadGame() {
  // Загружаем с диска
  UMallSimSaveGame *LoadedGame = Cast<UMallSimSaveGame>(
      UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
  if (!LoadedGame) {
    UE_LOG(LogTemp, Warning, TEXT("No save game found to load."));
    return;
  }

  // 1. Восстанавливаем деньги
  UEconomySubsystem *EconomySubsystem =
      GetGameInstance()->GetSubsystem<UEconomySubsystem>();
  if (EconomySubsystem) {
    // Чтобы избежать двойного начисления, сначала обнуляем баланс
    EconomySubsystem->TrySpendMoney(EconomySubsystem->GetCurrentBalance());
    EconomySubsystem->AddMoney(LoadedGame->PlayerMoney);
  }

  // 2. Удаляем старые зоны перед созданием новых
  TArray<AActor *> ExistingZones;
  UGameplayStatics::GetAllActorsOfClass(
      GetWorld(), AStoreZoneActor::StaticClass(), ExistingZones);
  for (AActor *Actor : ExistingZones) {
    Actor->Destroy();
  }

  // 3. Восстанавливаем размещенные зоны
  for (const FZoneSaveData &SavedZoneData : LoadedGame->PlacedZones) {
    UStoreZoneData *ZoneDataAsset =
        Cast<UStoreZoneData>(SavedZoneData.ZoneDataPath.TryLoad());
    if (ZoneDataAsset) {
      FActorSpawnParameters SpawnParams;
      AStoreZoneActor *NewZone = GetWorld()->SpawnActor<AStoreZoneActor>(
          AStoreZoneActor::StaticClass(), SavedZoneData.ZoneTransform,
          SpawnParams);
      if (NewZone) {
        NewZone->ZoneData = ZoneDataAsset;
        NewZone->PostInitializeComponents();
      }
    }
  }

  UE_LOG(LogTemp, Warning, TEXT("Game Loaded!"));
}