// copyright lololol

#include "AssetFactories.h"

#include "Core/LayerData.h"

#define DEFINE_ASSET_FACTORY(Type, FullType) U##Type##Factory::U##Type##Factory()\
{\
	SupportedClass = ##FullType##::StaticClass();\
	bEditAfterNew = true;\
	bCreateNew = true;\
}\
\
UObject* U##Type##Factory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,\
	UObject* Context, FFeedbackContext* Warn)\
{\
	const auto Obj = NewObject<##FullType##>(InParent, Class, Name, Flags, Context);\
	Obj->Name = FText::FromString(FactoryStatics::HumanizeString(Name.ToString()));\
	return Obj;\
}

#define DEFINE_ASSET_FACTORY_NONAME(Type, FullType) U##Type##Factory::U##Type##Factory()\
{\
	SupportedClass = ##FullType##::StaticClass();\
	bEditAfterNew = true;\
	bCreateNew = true;\
}\
\
UObject* U##Type##Factory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,\
	UObject* Context, FFeedbackContext* Warn)\
{\
	return NewObject<##FullType##>(InParent, Class, Name, Flags, Context);\
}

FString FactoryStatics::GetAssetNameWithoutPrefix(FString Name)
{
	TArray<FString> Sections;
	const TCHAR     Sep = '_';
	Name.ParseIntoArray(Sections, &Sep);
	return Sections.Last();
}

FString FactoryStatics::HumanizeString(FString Input)
{
	Input = Input.Replace(TEXT("_"), TEXT(" "));
	Input = Input.Replace(TEXT("-"), TEXT(" "));

	TArray<FString> Words;
	Input.ParseIntoArray(Words, TEXT(" "), true);

	for (FString& Word : Words)
	{
		Word = Word.Left(1).ToUpper() + Word.Mid(1).ToLower();
	}

	return FString::Join(Words, TEXT(" "));
}

// The below are all generated via the DEFINE_ASSET_FACTORY macro, but expanded here via ReSharper as the macro doesn't compile.
// Example: DEFINE_ASSET_FACTORY(WeaponData, UWeaponData);
// DEFINE_ASSET_FACTORY_NONAME can be used if the asset doesn't have a Name property.

ULayerDataFactory::ULayerDataFactory()
{
	SupportedClass = ULayerData::StaticClass();
	bEditAfterNew  = true;
	bCreateNew     = true;
}

UObject* ULayerDataFactory::FactoryCreateNew(UClass*  Class, UObject* InParent, FName Name, EObjectFlags Flags,
                                             UObject* Context, FFeedbackContext* Warn)
{
	const auto Obj = NewObject<ULayerData>(InParent, Class, Name, Flags, Context);
	Obj->Name      = FText::FromString(FactoryStatics::HumanizeString(Name.ToString()));
	return Obj;
};
