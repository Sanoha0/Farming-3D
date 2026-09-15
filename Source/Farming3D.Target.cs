using UnrealBuildTool;
using System.Collections.Generic;

public class Farming3DTarget : TargetRules
{
    public Farming3DTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        ExtraModuleNames.Add("Farming3D");
    }
}
