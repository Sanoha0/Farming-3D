using UnrealBuildTool;
using System.Collections.Generic;

public class Farming3DEditorTarget : TargetRules
{
    public Farming3DEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V7;
        ExtraModuleNames.Add("Farming3D");
    }
}
