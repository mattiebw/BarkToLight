// copyright lololol

#pragma once

#if WITH_EDITOR
	#include "MessageLogModule.h"
#endif

#if !UE_BUILD_SHIPPING
	#include "Engine/Console.h"
#endif

DECLARE_LOG_CATEGORY_EXTERN(LogBarkToLight, Log, All);

#define BTL_LOG(fmt, ...) BarkToLightLog(ELogVerbosity::Log, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOG_VERBOSE(fmt, ...) BarkToLightLog(ELogVerbosity::Verbose, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOG_WARN(fmt, ...) BarkToLightLog(ELogVerbosity::Warning, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOG_ERROR(fmt, ...) BarkToLightLog(ELogVerbosity::Error, false, true, true, true, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOG_FATAL(fmt, ...) BarkToLightLog(ELogVerbosity::Fatal, false, true, true, true, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)

#define BTL_LOGC(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Log, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGC_VERBOSE(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Verbose, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGC_WARN(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Warning, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGC_ERROR(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Error, true, true, true, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGC_FATAL(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Fatal, true, true, true, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)

#define BTL_LOGS(fmt, ...) BarkToLightLog(ELogVerbosity::Log, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGS_VERBOSE(fmt, ...) BarkToLightLog(ELogVerbosity::Verbose, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGS_WARN(fmt, ...) BarkToLightLog(ELogVerbosity::Warning, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGS_ERROR(fmt, ...) BarkToLightLog(ELogVerbosity::Error, false, true, true, true, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGS_FATAL(fmt, ...) BarkToLightLog(ELogVerbosity::Fatal, false, true, true, true, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)

#define BTL_LOGSC(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Log, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGSC_VERBOSE(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Verbose, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGSC_WARN(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Warning, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGSC_ERROR(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Error, true, true, true, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGSC_FATAL(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Fatal, true, true, true, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)

#define BTL_LOG_NOLOC(fmt, ...) BarkToLightLog(ELogVerbosity::Log, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOG_VERBOSE_NOLOC(fmt, ...) BarkToLightLog(ELogVerbosity::Verbose, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOG_WARN_NOLOC(fmt, ...) BarkToLightLog(ELogVerbosity::Warning, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOG_ERROR_NOLOC(fmt, ...) BarkToLightLog(ELogVerbosity::Error, false, true, false, true, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOG_FATAL_NOLOC(fmt, ...) BarkToLightLog(ELogVerbosity::Fatal, false, true, false, true, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)

#define BTL_LOGC_NOLOC(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Log, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGC_VERBOSE_NOLOC(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Verbose, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGC_WARN_NOLOC(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Warning, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGC_ERROR_NOLOC(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Error, true, true, false, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGC_FATAL_NOLOC(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Fatal, true, true, false, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)

#define BTL_LOGS_NOLOC(fmt, ...) BarkToLightLog(ELogVerbosity::Log, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGS_VERBOSE_NOLOC(fmt, ...) BarkToLightLog(ELogVerbosity::Verbose, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGS_WARN_NOLOC(fmt, ...) BarkToLightLog(ELogVerbosity::Warning, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGS_ERROR_NOLOC(fmt, ...) BarkToLightLog(ELogVerbosity::Error, false, true, false, true, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGS_FATAL_NOLOC(fmt, ...) BarkToLightLog(ELogVerbosity::Fatal, false, true, false, true, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)

#define BTL_LOGSC_NOLOC(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Log, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGSC_VERBOSE_NOLOC(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Verbose, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGSC_WARN_NOLOC(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Warning, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGSC_ERROR_NOLOC(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Error, true, true, false, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTL_LOGSC_FATAL_NOLOC(WorldContext, fmt, ...) BarkToLightLog(ELogVerbosity::Fatal, true, true, false, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)

void InitBarkToLightLog();
void DeinitBarkToLightLog();

template <typename FmtType, typename... Types>
FORCEINLINE void BarkToLightLog(const uint8 LogVerbosity, const bool bLogToConsole, const bool bLogToMsgLog, const bool bShowLocation,
                      const bool bShowMsgLog, const char* File, const int Line, const char* Function, UObject* WorldContext,
                      const FmtType& Fmt, Types... Args)
{
	static_assert(TIsArrayOrRefOfTypeByPredicate<FmtType, TIsCharEncodingCompatibleWithTCHAR>::Value, "Formatting string must be a TCHAR array.");
	static_assert(TAnd<TIsValidVariadicFunctionArg<Types>...>::Value, "Invalid argument(s) passed to BarkToLightLog");
	
	FString Msg = FString::Printf(Fmt, Args...);
	if (bShowLocation)
	{
		Msg = FString::Printf(TEXT("%s(%d): %s: %s"), ANSI_TO_TCHAR(File), Line, ANSI_TO_TCHAR(Function), *Msg);
	}

	switch (LogVerbosity)
	{
	case ELogVerbosity::Verbose:
		UE_LOG(LogBarkToLight, Verbose, TEXT("%s"), *Msg);
		break;
	case ELogVerbosity::Log:
	default:
		UE_LOG(LogBarkToLight, Log, TEXT("%s"), *Msg);
		break;
	case ELogVerbosity::Warning:
		UE_LOG(LogBarkToLight, Warning, TEXT("%s"), *Msg);
		break;
	case ELogVerbosity::Error:
		UE_LOG(LogBarkToLight, Error, TEXT("%s"), *Msg);
		break;
	case ELogVerbosity::Fatal:
		UE_LOG(LogBarkToLight, Fatal, TEXT("%s"), *Msg);
		break;
	}
	
#if !UE_BUILD_SHIPPING
	if (bLogToConsole)
	{
		GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::Assert)->GetGameViewport()->ViewportConsole->OutputText(Msg);
	}
#endif

#if WITH_EDITOR
	if (bLogToMsgLog)
	{
		switch (LogVerbosity)
		{
		case ELogVerbosity::Verbose:
		case ELogVerbosity::Log:
		default:
			FMessageLog("BarkToLight").Info(FText::FromString(Msg));
			break;
		case ELogVerbosity::Warning:
			FMessageLog("BarkToLight").Warning(FText::FromString(Msg));
			break;
		case ELogVerbosity::Error:
		case ELogVerbosity::Fatal:
			FMessageLog("BarkToLight").Error(FText::FromString(Msg));
			break;
		}

		if (bShowMsgLog)
		{
			FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
			MessageLogModule.OpenMessageLog("BarkToLight");
		}
	}
#endif
}
