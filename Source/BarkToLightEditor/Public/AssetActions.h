// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "AssetDefinitionDefault.h"
#include "AssetTypeActions_Base.h"

#include "Core/LayerData.h"
#include "Enemy/EnemyData.h"
#include "Weapon/WeaponData.h"
#include "WorldGen/RoomsLevelGeneratorSettings.h"

#include "AssetActions.generated.h"

#define LOCTEXT_NAMESPACE "BarkToLightEditorModule"

#define DECLARE_ASSET_DEFINITION(Type, KeyName, TypeName, Color) UCLASS()\
class UAssetDefinition_##KeyName : public UAssetDefinitionDefault\
{\
	GENERATED_BODY()\
\
public:\
	virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_"#KeyName, ##TypeName##); }\
	virtual FLinearColor GetAssetColor() const override { return Color; };\
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return Type::StaticClass(); }\
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override\
	{\
		static const auto Categories = {\
			EAssetCategoryPaths::Basic / LOCTEXT("BTLAssetSubMenu", "Bark To Light")\
		};\
		\
		return Categories;\
	}\
}

// These are all generated via the DECLARE_ASSET_DEFINITION macro, but expanded here via ReSharper as the macro doesn't compile.
// Example: DECLARE_ASSET_DEFINITION(UWeaponData, WeaponData, "Weapon Data", FLinearColor::Blue);
UCLASS() class UAssetDefinition_LayerData : public UAssetDefinitionDefault { GENERATED_BODY() public: virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_""LayerData","Layer Data"); } virtual FLinearColor GetAssetColor() const override { return FLinearColor::Blue; }; virtual TSoftClassPtr<UObject> GetAssetClass() const override { return ULayerData::StaticClass(); } virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override { static const auto Categories = { EAssetCategoryPaths::Basic / LOCTEXT("BTLAssetSubMenu", "Bark To Light") }; return Categories; } };
UCLASS() class UAssetDefinition_EnemyData : public UAssetDefinitionDefault { GENERATED_BODY() public: virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_""EnemyData","Enemy Data"); } virtual FLinearColor GetAssetColor() const override { return FLinearColor::Red; }; virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UEnemyData::StaticClass(); } virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override { static const auto Categories = { EAssetCategoryPaths::Basic / LOCTEXT("BTLAssetSubMenu", "Bark To Light") }; return Categories; } };
UCLASS() class UAssetDefinition_WeaponData : public UAssetDefinitionDefault { GENERATED_BODY() public: virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_""WeaponData","Weapon Data"); } virtual FLinearColor GetAssetColor() const override { return FLinearColor::Yellow; }; virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UWeaponData::StaticClass(); } virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override { static const auto Categories = { EAssetCategoryPaths::Basic / LOCTEXT("BTLAssetSubMenu", "Bark To Light") }; return Categories; } };
UCLASS() class UAssetDefinition_LevelGeneratorSettings : public UAssetDefinitionDefault { GENERATED_BODY() public: virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_""RoomsLevelGeneratorSettings","Rooms Level Generator Settings"); } virtual FLinearColor GetAssetColor() const override { return FLinearColor::Yellow; }; virtual TSoftClassPtr<UObject> GetAssetClass() const override { return URoomsLevelGeneratorSettings::StaticClass(); } virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override { static const auto Categories = { EAssetCategoryPaths::Basic / LOCTEXT("BTLAssetSubMenu", "Bark To Light") }; return Categories; } };

#undef LOCTEXT_NAMESPACE
