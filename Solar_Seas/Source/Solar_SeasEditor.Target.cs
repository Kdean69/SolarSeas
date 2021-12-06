// Copyright © 2021 Kdean Games. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Solar_SeasEditorTarget : TargetRules
{
	public Solar_SeasEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "Solar_Seas" } );
	}
}
