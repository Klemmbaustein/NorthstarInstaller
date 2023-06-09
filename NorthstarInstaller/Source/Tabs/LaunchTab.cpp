#include "LaunchTab.h"
#include <UI/UIText.h>
#include <UI/UIBackground.h>

#include "../UIDef.h"
#include "../Log.h"
#include "../Game.h"
#include "../Installer.h"
#include "../BackgroundTask.h"

#include <thread>
#include <atomic>
#include <map>
#include "ModsTab.h"

void NorthstarLaunchTask()
{
	BackgroundTask::SetStatus("Northstar is running");
	Log::Print("Game has started");
	system((Game::GamePath + "/NorthstarLauncher.exe").c_str());
	Log::Print("Game has finished running");
}

std::map<void (*)(), std::string> LaunchStoppingTasks =
{
	std::pair(Game::UpdateGame, "Updating northstar"),
	std::pair(NorthstarLaunchTask, "Northstar is running"),
	std::pair(Installer::CheckForUpdates, "Checking for updates (1/3)"),
	std::pair(Installer::CheckForInstallerUpdate, "Checking for updates (2/3)"),
	std::pair(ModsTab::CheckForModUpdates, "Checking for updates (3/3)"),
	std::pair(Installer::UpdateInstaller, "Updating installer")
};

void LaunchNorthstar()
{
	for (auto& i : LaunchStoppingTasks)
	{
		if (BackgroundTask::IsFunctionRunningAsTask(i.first))
		{
			return;
		}
	}
	if (!Game::GamePath.empty() && !Game::RequiresUpdate)
	{
		Log::Print("Starting game...");

		new BackgroundTask(NorthstarLaunchTask);
	}
	if (Game::RequiresUpdate && !Game::GamePath.empty())
	{
		Game::UpdateGameAsync();
	}
}

LaunchTab::LaunchTab()
{
	Name = "Play";
	Log::Print("Loading launch tab...");
	Background->Align = UIBox::E_DEFAULT;

	auto TextBox = (new UIBackground(true, 0, 0, 0))->SetOpacity(0.3);
	TextBox->Align = UIBox::E_CENTERED;

	LaunchButton = new UIButton(true, 0, 1, LaunchNorthstar);
	LaunchText = new UIText(0.7, 0, "Launch", UI::Text);

	Background->AddChild(TextBox
		->SetMinSize(Vector2f(2, 0.2))
		->SetPadding(0)
		->AddChild(LaunchButton
			->SetPadding(0.03)
			->SetBorder(UIBox::E_ROUNDED, 1)
			->AddChild(LaunchText)));


}

void LaunchTab::Tick()
{
	if (Game::GamePath.empty())
	{
		LaunchText->SetText("Titanfall 2 not found");
		return;
	}
	for (auto& i : LaunchStoppingTasks)
	{
		if (BackgroundTask::IsFunctionRunningAsTask(i.first))
		{
			LaunchText->SetText(i.second);
			return;
		}
	}
	if (Game::RequiresUpdate)
	{
		LaunchText->SetText("Update northstar");
	}
	else
	{
		LaunchText->SetText("Launch Northstar");
	}
}

LaunchTab::~LaunchTab()
{
}
