#include "SettingsTab.h"
#include <UI/UIText.h>
#include <UI/UIButton.h>

#include <thread>
#include <sstream>
#include <zip.h>
#include <fstream>
#include <stdlib.h>
#include <filesystem>

#include "../Log.h"
#include "../UIDef.h"
#include "../Game.h"
#include "../Installer.h"
#include "../BackgroundTask.h"
#include "../WindowFunctions.h"

SettingsTab* CurrentSettingsTab = nullptr;

SettingsTab::SettingsTab()
{
	CurrentSettingsTab = this;
	Name = "Settings";

	Background->Align = UIBox::E_CENTERED;
	Background->SetHorizontal(true);

	SettingsBackground = new UIBackground(false, 0, 0, Vector2f(0.8, 1.85));
	Background->AddChild(SettingsBackground
		->SetOpacity(0.5)
		->AddChild((new UIBackground(true, 0, 1, Vector2f(0.8, 0.005)))
			->SetPadding(0))
		->SetPadding(0));
	SettingsBackground->Align = UIBox::E_REVERSE;
	SettingsBackground->AddChild(new UIText(0.8, 1, "Settings", UI::Text));
	SettingsBox = new UIScrollBox(false, 0, 10);
	SettingsBox->Align = UIBox::E_REVERSE;
	SettingsBackground->AddChild(SettingsBox);
	GenerateSettings();
}

void DeleteAllMods()
{
	// Do not uninstall the core mods. That's a very bad idea.
	std::set<std::string> CoreModNames =
	{
		"Northstar.CustomServers",
		"Northstar.Custom",
		"Northstar.Client",
		"Northstar.Coop", // soooooon�

	};

	for (const auto& m : std::filesystem::directory_iterator(Game::GamePath + "/R2Northstar/mods/"))
	{
		if (CoreModNames.find(m.path().filename().string()) == CoreModNames.end() && std::filesystem::is_directory(m))
		{
			std::filesystem::remove_all(m);
			Log::Print("Removing mod: " + m.path().filename().string(),  Log::Warning);
		}
	}
	std::filesystem::remove_all("Data/var/modinfo");
}

void LocateTitanfall()
{
	std::string NewPath = Window::ShowSelectFolderDialog();
	if (Game::IsValidTitanfallLocation(NewPath))
	{
		Game::SaveGameDir(NewPath);
		Game::GamePath = Game::GetTitanfallLocation();
	}
	CurrentSettingsTab->GenerateSettings();
}

constexpr uint16_t MAX_GAMEPATH_SIZE = 30;

void AddCategoryHeader(std::string Text, UIBox* Parent)
{
	Parent->AddChild((new UIText(0.5, 1, Text, UI::Text))
		->SetPadding(0.05, 0.01, 0.01, 0.01));
	Parent->AddChild((new UIBackground(true, 0, 1, Vector2f(0.76, 0.005)))
		->SetPadding(0.0, 0.02, 0, 0));
}

void SettingsTab::GenerateSettings()
{
	SettingsBox->DeleteChildren();

	std::string ShortGamePath = Game::GamePath;
	if (ShortGamePath.size() > MAX_GAMEPATH_SIZE)
	{
		ShortGamePath = ShortGamePath.substr(0, MAX_GAMEPATH_SIZE - 3) + "...";
	}

	AddCategoryHeader("General", SettingsBox);
	SettingsBox->AddChild((new UIButton(true, 0, 1, LocateTitanfall))
		->AddChild(new UIText(0.4, 0, Game::GamePath.empty() ? "Locate Titanfall 2 (No path!)" : "Locate Titanfall (" + ShortGamePath + ")", UI::Text)));

	if (Game::IsValidTitanfallLocation(Game::GamePath))
	{
		SettingsBox->AddChild((new UIButton(true, 0, 1, []() {
			new BackgroundTask(Installer::CheckForUpdates);
			}))
			->AddChild(new UIText(0.4, 0, "Re-check for updates", UI::Text)));

		SettingsBox->AddChild((new UIButton(true, 0, 1, Game::UpdateGameAsync))
			->AddChild(new UIText(0.4, 0, "Force reinstall Northstar", UI::Text)));


		AddCategoryHeader("Danger zone", SettingsBox);
		SettingsBox->AddChild((new UIButton(true, 0, 1, DeleteAllMods))
			->AddChild(new UIText(0.4, 0, "Delete all mods", UI::Text)));

		SettingsBox->AddChild((new UIButton(true, 0, 1, []() {
				Log::Print("Un-fucking installation...", Log::Warning);
				std::filesystem::remove_all("temp");
				Log::Print("Deleted ./Data/temp/", Log::Warning);
				DeleteAllMods();
				Game::UpdateGameAsync();
			}))
			->AddChild(new UIText(0.4, 0, "Try to unfuck installation", UI::Text)));
	}

	AddCategoryHeader("About", SettingsBox);
	SettingsBox->AddChild(new UIText(0.35, 1, "Installed Northstar version: " + Game::GetCurrentVersion(), UI::Text));
	SettingsBox->AddChild(new UIText(0.35, 1, "Installer version: " + Installer::InstallerVersion, UI::Text));
}
