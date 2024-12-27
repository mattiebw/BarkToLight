// copyright lololol

#pragma once

#if WITH_EDITOR
	#include "MessageLogModule.h"
#endif

#if !UE_BUILD_SHIPPING
	#include "Engine/Console.h"
#endif

DECLARE_LOG_CATEGORY_EXTERN(LogBarkToLightEditor, Log, All);

#define BTLED_LOG(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Log, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOG_VERBOSE(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Verbose, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOG_WARN(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Warning, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOG_ERROR(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Error, false, true, true, true, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOG_FATAL(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Fatal, false, true, true, true, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)

#define BTLED_LOGC(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Log, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGC_VERBOSE(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Verbose, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGC_WARN(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Warning, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGC_ERROR(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Error, true, true, true, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGC_FATAL(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Fatal, true, true, true, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)

#define BTLED_LOGS(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Log, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGS_VERBOSE(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Verbose, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGS_WARN(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Warning, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGS_ERROR(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Error, false, true, true, true, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGS_FATAL(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Fatal, false, true, true, true, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)

#define BTLED_LOGSC(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Log, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGSC_VERBOSE(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Verbose, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGSC_WARN(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Warning, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGSC_ERROR(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Error, true, true, true, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGSC_FATAL(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Fatal, true, true, true, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)

#define BTLED_LOG_NOLOC(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Log, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOG_VERBOSE_NOLOC(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Verbose, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOG_WARN_NOLOC(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Warning, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOG_ERROR_NOLOC(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Error, false, true, false, true, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOG_FATAL_NOLOC(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Fatal, false, true, false, true, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)

#define BTLED_LOGC_NOLOC(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Log, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGC_VERBOSE_NOLOC(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Verbose, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGC_WARN_NOLOC(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Warning, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGC_ERROR_NOLOC(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Error, true, true, false, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGC_FATAL_NOLOC(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Fatal, true, true, false, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt) __VA_OPT__(,) __VA_ARGS__)

#define BTLED_LOGS_NOLOC(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Log, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGS_VERBOSE_NOLOC(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Verbose, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGS_WARN_NOLOC(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Warning, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGS_ERROR_NOLOC(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Error, false, true, false, true, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGS_FATAL_NOLOC(fmt, ...) BarkToLightEditorLog(ELogVerbosity::Fatal, false, true, false, true, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt __VA_OPT__(,) __VA_ARGS__)

#define BTLED_LOGSC_NOLOC(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Log, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGSC_VERBOSE_NOLOC(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Verbose, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGSC_WARN_NOLOC(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Warning, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGSC_ERROR_NOLOC(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Error, true, true, false, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)
#define BTLED_LOGSC_FATAL_NOLOC(WorldContext, fmt, ...) BarkToLightEditorLog(ELogVerbosity::Fatal, true, true, false, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt __VA_OPT__(,) __VA_ARGS__)

void InitBarkToLightEditorLog();
void DeinitBarkToLightEditorLog();

template <typename FmtType, typename... Types>
FORCEINLINE void BarkToLightEditorLog(const uint8 LogVerbosity, const bool bLogToConsole, const bool bLogToMsgLog, const bool bShowLocation,
                      const bool bShowMsgLog, const char* File, const int Line, const char* Function, UObject* WorldContext,
                      const FmtType& Fmt, Types... Args)
{
	static_assert(TIsArrayOrRefOfTypeByPredicate<FmtType, TIsCharEncodingCompatibleWithTCHAR>::Value, "Formatting string must be a TCHAR array.");
	static_assert(TAnd<TIsValidVariadicFunctionArg<Types>...>::Value, "Invalid argument(s) passed to BarkToLightEditorLog");
	
	FString Msg = FString::Printf(Fmt, Args...);
	if (bShowLocation)
	{
		Msg = FString::Printf(TEXT("%s(%d): %s: %s"), ANSI_TO_TCHAR(File), Line, ANSI_TO_TCHAR(Function), *Msg);
	}

	switch (LogVerbosity)
	{
	case ELogVerbosity::Verbose:
		UE_LOG(LogBarkToLightEditor, Verbose, TEXT("%s"), *Msg);
		break;
	case ELogVerbosity::Log:
	default:
		UE_LOG(LogBarkToLightEditor, Log, TEXT("%s"), *Msg);
		break;
	case ELogVerbosity::Warning:
		UE_LOG(LogBarkToLightEditor, Warning, TEXT("%s"), *Msg);
		break;
	case ELogVerbosity::Error:
		UE_LOG(LogBarkToLightEditor, Error, TEXT("%s"), *Msg);
		break;
	case ELogVerbosity::Fatal:
		UE_LOG(LogBarkToLightEditor, Fatal, TEXT("%s"), *Msg);
		break;
	}
	
#if !UE_BUILD_SHIPPING
	if (bLogToConsole)
	{
		WorldContext->GetWorld()->GetGameViewport()->ViewportConsole->OutputText(Msg);
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
			FMessageLog("BarkToLightEditor").Info(FText::FromString(Msg));
			break;
		case ELogVerbosity::Warning:
			FMessageLog("BarkToLightEditor").Warning(FText::FromString(Msg));
			break;
		case ELogVerbosity::Error:
		case ELogVerbosity::Fatal:
			FMessageLog("BarkToLightEditor").Error(FText::FromString(Msg));
			break;
		}

		if (bShowMsgLog)
		{
			FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
			MessageLogModule.OpenMessageLog("BarkToLightEditor");
		}
	}
#endif
}
