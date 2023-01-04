#pragma once

#include "CoreMinimal.h"

#include "WorkbenchMath.generated.h"

UCLASS(meta=(BlueprintThreadSafe))
class UEWORKBENCH_API UWorkbenchMath : public UBlueprintFunctionLibrary
{
    GENERATED_UCLASS_BODY()

    static float NormalizeAngle(float Angle);
    static float AngleDistance(float FromAngle, float ToAngle);
    static float WeightedMovingAverage_Angle(float CurrentSample, float PreviousSample, float Weight);
    static float DynamicWeightedMovingAverage_Angle(float CurrentSample, float PreviousSample, float MaxDistance, float MinWeight, float MaxWeight);

    UFUNCTION(BlueprintPure, Category="Math|Smoothing", meta=(DisplayName="Weighted Moving Average Rotator (Fixed)"))
    static FRotator WeightedMovingAverage_FRotator(FRotator CurrentSample, FRotator PreviousSample, float Weight);

    UFUNCTION(BlueprintPure, Category="Math|Smoothing", meta=(DisplayName="Dynamic Weighted Moving Average Rotator (Fixed)"))
    static FRotator DynamicWeightedMovingAverage_FRotator(FRotator CurrentSample, FRotator PreviousSample, float MaxDistance, float MinWeight, float MaxWeight);
};
