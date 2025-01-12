// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "AssetFactories.generated.h"

#define DECLARE_ASSET_FACTORY(AssetName) UCLASS()\
class U##AssetName##Factory : public UFactory\
{\
	GENERATED_BODY()\
	\
public:\
	U##AssetName##Factory();\
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;\
}

namespace FactoryStatics
{
	static FString GetAssetNameWithoutPrefix(FString Name);
	static FString HumanizeString(FString Input);
};

// These are all generated via the DECLARE_ASSET_FACTORY macro, but expanded here via ReSharper as the macro doesn't compile.
// Example: DECLARE_ASSET_FACTORY(WeaponData);
UCLASS()
class ULayerDataFactory : public UFactory
{
	GENERATED_BODY()

public:
	ULayerDataFactory();
	virtual UObject* FactoryCreateNew(UClass*  Class, UObject*            InParent, FName Name, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn) override;
};

UCLASS()
class UEnemyDataFactory : public UFactory
{
	GENERATED_BODY()

public:
	UEnemyDataFactory();
	virtual UObject* FactoryCreateNew(UClass*  Class, UObject*            InParent, FName Name, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn) override;
};

UCLASS()
class UWeaponDataFactory : public UFactory
{
	GENERATED_BODY()

public:
	UWeaponDataFactory();
	virtual UObject* FactoryCreateNew(UClass*  Class, UObject*            InParent, FName Name, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn) override;
};

UCLASS()
class ULevelGeneratorSettingsFactory : public UFactory
{
	GENERATED_BODY()

public:
	ULevelGeneratorSettingsFactory();
	virtual UObject* FactoryCreateNew(UClass*  Class, UObject*            InParent, FName Name, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn) override;
};

UCLASS()
class UProjectileDataFactory : public UFactory
{
	GENERATED_BODY()

public:
	UProjectileDataFactory();
	virtual UObject* FactoryCreateNew(UClass*  Class, UObject*            InParent, FName Name, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn) override;
};
