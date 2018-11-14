// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class T1ProjectEditorTarget : TargetRules
{
	public T1ProjectEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "T1Project", "T1ProjectEditor" } );

		//ExtraModuleNames.Add("T1Project");
		//ExtraModuleNames.Add("T1ProjectEditor");

		//ExtraModuleNames.AddRange( new string[] { "T1Project" } );
		//ExtraModuleNames.AddRange( new string[] { "T1ProjectEditor" } );
	}
}
