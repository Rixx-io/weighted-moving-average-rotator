#include "WorkbenchMath.h"

UWorkbenchMath::UWorkbenchMath(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}


// Make angle -180 ..< 180.
float UWorkbenchMath::NormalizeAngle(float Angle)
{
    return fmod(Angle + 180.f, 360.f) - 180.f;
}


// The shortest distance around the circle from one angle to another. The sign of the result
// indicates the direction, and the result can be added to FromAngle to get to ToAngle,
// be sure to normalize after.
float UWorkbenchMath::AngleDistance(float FromAngle, float ToAngle)
{
    FromAngle = NormalizeAngle(FromAngle);
    ToAngle = NormalizeAngle(ToAngle);

    float Dist = ToAngle - FromAngle;
    if (Dist >= 180.f)
    {
        return Dist - 360.f;
    }
    else if (Dist < -180.f)
    {
        return Dist + 360.f;
    }
    else
    {
        return Dist;
    }
}


// Weighted moving average for angles. Uses AngleDistance(), result is normalized.
float UWorkbenchMath::WeightedMovingAverage_Angle(float CurrentSample, float PreviousSample, float Weight)
{
    Weight = FMath::Clamp<float>(Weight, 0.f, 1.f);
    float WAvg = NormalizeAngle(PreviousSample + AngleDistance(PreviousSample, CurrentSample) * Weight);
    return WAvg;
}


// Dynamic weighted moving average for angles. Uses AngleDistance(), result is normalized.
float UWorkbenchMath::DynamicWeightedMovingAverage_Angle(float CurrentSample, float PreviousSample, float MaxDistance, float MinWeight, float MaxWeight)
{
    // We need the distance between samples to determine how much of each weight to use
    const float Distance = FMath::Abs<float>(AngleDistance(PreviousSample, CurrentSample));
    float Weight = MinWeight;
    if (MaxDistance > 0)
    {
        // Figure out the lerp value to use between the min/max weights
        const float LerpAlpha = FMath::Clamp<float>(Distance / MaxDistance, 0.f, 1.f);
        Weight = FMath::Lerp<float>(MinWeight, MaxWeight, LerpAlpha);
    }
    return WeightedMovingAverage_Angle(CurrentSample, PreviousSample, Weight);
}


// Blueprint version.
FRotator UWorkbenchMath::WeightedMovingAverage_FRotator(FRotator CurrentSample, FRotator PreviousSample, float Weight)
{
    FRotator OutRotator;
    OutRotator.Yaw = WeightedMovingAverage_Angle(CurrentSample.Yaw, PreviousSample.Yaw, Weight);
    OutRotator.Pitch = WeightedMovingAverage_Angle(CurrentSample.Pitch, PreviousSample.Pitch, Weight);
    OutRotator.Roll = WeightedMovingAverage_Angle(CurrentSample.Roll, PreviousSample.Roll, Weight);
    return OutRotator;
}


// Blueprint version.
FRotator UWorkbenchMath::DynamicWeightedMovingAverage_FRotator(FRotator CurrentSample, FRotator PreviousSample, float MaxDistance, float MinWeight, float MaxWeight)
{
    FRotator OutRotator;
    OutRotator.Yaw = DynamicWeightedMovingAverage_Angle(CurrentSample.Yaw, PreviousSample.Yaw, MaxDistance, MinWeight, MaxWeight);
    OutRotator.Pitch = DynamicWeightedMovingAverage_Angle(CurrentSample.Pitch, PreviousSample.Pitch, MaxDistance, MinWeight, MaxWeight);
    OutRotator.Roll = DynamicWeightedMovingAverage_Angle(CurrentSample.Roll, PreviousSample.Roll, MaxDistance, MinWeight, MaxWeight);
    return OutRotator;
}
