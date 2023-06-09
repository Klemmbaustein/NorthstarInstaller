#pragma once
#include <atomic>
#include <string>
#include <vector>

namespace Thunderstore
{
	// Describes a mod on thunderstore.
	struct Package
	{
		std::string Name;
		std::string Description;
		std::string Author;
		std::string Img;
		std::string Namespace;
		std::string DownloadUrl;
		std::string Version;
		std::string UUID;
		std::string PageUrl;
		std::string FileVersion;
		size_t Downloads = 0;
		size_t Rating = 0;
		bool IsDeprecated = false;
		bool IsUnknownLocalMod = false;
		bool IsNSFW = false;
	};

	bool IsMostRecentFileVersion(std::string VersionString);

	enum class Ordering
	{
		Newest,
		Last_Updated,
		Most_Downloaded,
		Top_Rated,
		Installed
	};

	struct InstalledModsResult
	{
		std::vector<Package> Managed;
		std::vector<Package> Unmanaged;

		std::vector<Package> Combined() const
		{
			std::vector<Package> comb = Managed;
			for (auto& i : Unmanaged)
			{
				comb.push_back(i);
			}
			return comb;
		}
	};

	struct Category
	{
		std::string Name;
		Ordering o;
	};

	extern Ordering SelectedOrdering;
	extern std::atomic<bool> IsDownloading;
	extern std::atomic<bool> LoadedImages;
	extern std::atomic<bool> ShouldStopLoadingImages;
	extern std::vector<Package> FoundMods;
	extern std::atomic<bool> LoadedSelectedMod;
	extern Package SelectedMod;

	InstalledModsResult GetInstalledMods();

	// Tries to return if a given package is installed locally.
	// Will not work if it's named differently.
	// TODO: Check UUID too. This should still return true if an installed mod has a different name but the same UUID.
	bool IsModInstalled(Package m);

	// Gets the thunderstore mod page with the given ordering, filter and page index.
	// The result will be put into 'FoundMods' because this function is meant to be run as a new thread.
	void AsyncDownloadThunderstoreInfo(Ordering ModOrdering, size_t Page, std::string Filter);

	// Downloads the given package into "Data/temp/net/{m.Author}.{m.Name}.zip,
	// extracts it's contents into "Data/temp/mod",
	// then extracts the content of the extracted zip file's "mods" Folder into the Titanfall 2
	// mods folder.
	// TODO: Extract other elements of the mod. (Plugins, ...)
	void InstallOrUninstallMod(Package m, bool Async = true);

	// Sets the Thunderstore::SelectedMod variable to a more detailed version of the given package.
	void AsyncGetModInfo(Package m);

}