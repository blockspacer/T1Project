// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Views/SHeaderRow.h"
#include "T1ContentBrowserEditor/Public/T1ContentBrowserDelegates.h"

struct FT1AssetViewItem;

class FT1AssetViewSortManager
{
public:
	/** Constructor */
	FT1AssetViewSortManager();

	/** Reset the sort mode back to default */
	void ResetSort();

	/** Sorts a list using the current ColumnId and Mode. Supply a MajorityAssetType to help discover sorting type (numerical vs alphabetical) */
	void SortList(TArray<TSharedPtr<struct FT1AssetViewItem>>& AssetItems, const FName& MajorityAssetType, const TArray<FT1AssetViewCustomColumn>& CustomColumns) const;

	/** Exports the list of asset items to CSV, in order and with the listed columns */
	void ExportColumnsToCSV(TArray<TSharedPtr<struct FT1AssetViewItem>>& AssetItems, TArray<FName>& ColumnList, const TArray<FT1AssetViewCustomColumn>& CustomColumns, FString& OutString) const;

	/** Sets the sort mode based on the column that was clicked, returns true if newly assigned */
	bool SetOrToggleSortColumn(const EColumnSortPriority::Type InSortPriority, const FName& InColumnId);

	/**	Sets the column to sort */
	void SetSortColumnId(const EColumnSortPriority::Type InSortPriority, const FName& InColumnId);

	/**	Sets the current sort mode */
	void SetSortMode(const EColumnSortPriority::Type InSortPriority, const EColumnSortMode::Type InSortMode);

	/** Gets the current sort mode */
	EColumnSortMode::Type GetSortMode(const EColumnSortPriority::Type InSortPriority) const { check(InSortPriority < EColumnSortPriority::Max); return SortMode[InSortPriority]; }

	/** Gets the current sort column id */
	FName GetSortColumnId(const EColumnSortPriority::Type InSortPriority) const { check(InSortPriority < EColumnSortPriority::Max); return SortColumnId[InSortPriority]; }

	/** Refresh a custom column if needed.  If found, returns true with TagType parameter set */
	bool FindAndRefreshCustomColumn(TArray<TSharedPtr<FT1AssetViewItem>>& AssetItems, FName ColumnName, const TArray<FT1AssetViewCustomColumn>& CustomColumns, UObject::FAssetRegistryTag::ETagType& TagType) const;

public:
	/** The names of non-type specific columns in the columns view. */
	static const FName NameColumnId;
	static const FName ClassColumnId;
	static const FName PathColumnId;

private:
	/** The name of the column that is currently used for sorting. */
	FName SortColumnId[EColumnSortPriority::Max];

	/** Whether the sort is ascending or descending. */
	EColumnSortMode::Type SortMode[EColumnSortPriority::Max];
};
