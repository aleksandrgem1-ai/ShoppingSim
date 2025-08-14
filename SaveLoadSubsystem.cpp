// SaveLoadSubsystem.cpp

#include "SaveLoadSubsystem.h"
#include "EconomySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "MallSimSaveGame.h"
#include "StoreZoneActor.h"
#include "StoreZoneData.h"

void USaveLoadSubsystem::SaveGame() {
  // ������� ��� �������� ������������ ������ ����������
  UMallSimSaveGame *SaveGameInstance = Cast<UMallSimSaveGame>(
      UGameplayStatics::CreateSaveGameObject(UMallSimSaveGame::StaticClass()));
  if (!SaveGameInstance)
    return;

  // 1. ��������� ������
  UEconomySubsystem *EconomySubsystem =
      GetGameInstance()->GetSubsystem<UEconomySubsystem>();
  if (EconomySubsystem) {
    SaveGameInstance->PlayerMoney = EconomySubsystem->GetCurrentBalance();
  }

  // 2. ��������� ��� ����������� ����
  TArray<AActor *> FoundZones;
  UGameplayStatics::GetAllActorsOfClass(
      GetWorld(), AStoreZoneActor::StaticClass(), FoundZones);
  SaveGameInstance->PlacedZones.Empty(); // ������� ������ ������

  for (AActor *Actor : FoundZones) {
    AStoreZoneActor *Zone = Cast<AStoreZoneActor>(Actor);
    if (Zone && Zone->ZoneData) {
      FZoneSaveData ZoneDataToSave;
      ZoneDataToSave.ZoneDataPath = FSoftObjectPath(Zone->ZoneData);
      ZoneDataToSave.ZoneTransform = Zone->GetActorTransform();
      SaveGameInstance->PlacedZones.Add(ZoneDataToSave);
    }
  }

  // ��������� �� ����
  UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);
  UE_LOG(LogTemp, Warning, TEXT("Game Saved!"));
}

void USaveLoadSubsystem::LoadGame() {
  // ��������� � �����
  UMallSimSaveGame *LoadedGame = Cast<UMallSimSaveGame>(
      UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
  if (!LoadedGame) {
    UE_LOG(LogTemp, Warning, TEXT("No save game found to load."));
    return;
  }

  // 1. ��������������� ������
  UEconomySubsystem *EconomySubsystem =
      GetGameInstance()->GetSubsystem<UEconomySubsystem>();
  if (EconomySubsystem) {
    // ����� �������� �������� ����������, ������� �������� ������
    EconomySubsystem->TrySpendMoney(EconomySubsystem->GetCurrentBalance());
    EconomySubsystem->AddMoney(LoadedGame->PlayerMoney);
  }

  // 2. ������� ������ ���� ����� ��������� �����
  TArray<AActor *> ExistingZones;
  UGameplayStatics::GetAllActorsOfClass(
      GetWorld(), AStoreZoneActor::StaticClass(), ExistingZones);
  for (AActor *Actor : ExistingZones) {
    Actor->Destroy();
  }

  // 3. ��������������� ����������� ����
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