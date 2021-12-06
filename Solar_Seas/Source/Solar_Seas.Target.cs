// Copyright © 2021 Kdean Games. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Solar_SeasTarget : TargetRules
{
	public Solar_SeasTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "Solar_Seas" } );
	}
}
