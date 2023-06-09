#pragma once
#include "UITab.h"
#include <UI/UIButton.h>
#include <UI/UIBackground.h>
#include <UI/UIText.h>
#include <UI/UIScrollBox.h>
#include <UI/UITextField.h>
#include <atomic>

class ModsTab : public UITab
{
	static ModsTab* CurrentModsTab;
	static std::vector<UIButton*> ModButtons;
	static std::vector<UIButton*> PageButtons;
	static std::vector<UIButton*> CategoryButtons;
	UIBackground* ModsBackground = nullptr;
	UIScrollBox* ModsScrollBox = nullptr;
	std::vector<unsigned int> ModTextures;
	unsigned int ModPreviewTexture = 0;
	float PrevAspectRatio = 0;
	std::vector<UIBackground*> ModImages;
public:
	static std::atomic<unsigned int> ModsPerPage;
	UITextField* SearchBar = nullptr;
	std::string Filter = "";
	bool DownloadingPage = false;
	bool LoadedModList = false;
	size_t SelectedPage = 0;
	void GenerateModInfo();
	void GenerateModPage();
	void GenerateModImages();
	void ShowLoadingText();
	void UpdateClickedCategoryButton();

	static int GetModsPerPage(float Aspect);
	void ClearLoadedTextures();
	static void CheckForModUpdates();

	ModsTab();
	void Tick() override;
	virtual ~ModsTab();
};