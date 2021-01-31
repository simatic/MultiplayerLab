#include <Application/States/GlobalSettingsState.h>
#include <Common/Utility/Utility.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>

GlobalSettingsState::GlobalSettingsState(StateStack& stack, Context context, sf::RenderWindow* window, Settings* settings) :
	State(stack, context),
	window(window),
	settings(settings)
{
	ImGui::SFML::Init(*window);
	imguiInit = true;
}

bool GlobalSettingsState::update(sf::Time dt) {
	return true;
}

void GlobalSettingsState::draw() {
	if (!imguiInit) {
		ImGui::SFML::Init(*window);

		ImGui::GetStyle().WindowRounding = 0.0f;
		ImGui::GetStyle().WindowBorderSize = 0.0f;
		imguiInit = true;
	}
	ImGui::GetStyle().WindowRounding = 0.0f;
	ImGui::GetStyle().WindowBorderSize = 0.0f;

	ImGui::SFML::Update(*window, deltaClock.restart());

	ImGui::SetNextWindowPos(ImVec2{ float(window->getSize().x / 2 - window->getSize().x / 6), float(window->getSize().y / 2 - window->getSize().y / 4) });
	ImGui::SetNextWindowSize(ImVec2{ float(window->getSize().x / 3), float(window->getSize().y / 2) });
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

	static std::string title = "2M3";
	static std::string description = "Educational tool to simulate a network environment for multiplayer games";

	ImGui::SameLine(ImGui::GetWindowSize().x / 2 - ImGui::CalcTextSize(title.c_str()).x / 2);
	ImGui::Text(title.c_str());
	ImGui::NewLine();
	ImGui::SameLine(ImGui::GetWindowSize().x / 2 - ImGui::CalcTextSize(description.c_str()).x / 2);
	ImGui::Text(description.c_str());
	ImGui::Text("\n");
	ImGui::Text("Settings");
	ImGui::Separator();
	

	static std::string playerKeyLabels[5] = { "Forward", "Backward", "Left", "Right", "Shoot" };

	static std::string playerAKeys[5];
	playerAKeys[0] = toString(settings->playerAKeyBinding.getAssignedKey(KeyBinding::Action::Accelerate));
	playerAKeys[1] = toString(settings->playerAKeyBinding.getAssignedKey(KeyBinding::Action::Brake));
	playerAKeys[2] = toString(settings->playerAKeyBinding.getAssignedKey(KeyBinding::Action::TurnLeft));
	playerAKeys[3] = toString(settings->playerAKeyBinding.getAssignedKey(KeyBinding::Action::TurnRight));
	playerAKeys[4] = toString(settings->playerAKeyBinding.getAssignedKey(KeyBinding::Action::DoAction));

	static std::string playerBKeys[5];
	playerBKeys[0] = toString(settings->playerBKeyBinding.getAssignedKey(KeyBinding::Action::Accelerate));
	playerBKeys[1] = toString(settings->playerBKeyBinding.getAssignedKey(KeyBinding::Action::Brake));
	playerBKeys[2] = toString(settings->playerBKeyBinding.getAssignedKey(KeyBinding::Action::TurnLeft));
	playerBKeys[3] = toString(settings->playerBKeyBinding.getAssignedKey(KeyBinding::Action::TurnRight));
	playerBKeys[4] = toString(settings->playerBKeyBinding.getAssignedKey(KeyBinding::Action::DoAction));

	ImGui::Columns(2);

	ImGui::Text("Player A Keymap");
	for (int i = 0; i < 5; ++i) {
		ImGui::AlignTextToFramePadding();
		if (ImGui::Button(playerAKeys[i].c_str(), ImVec2(70, 20)) && !keyMapInEditMode) {
			playerAKeyEdit[i] = true;
			keyMapInEditMode = true;
		}
		ImGui::SameLine();
		ImGui::Text(playerKeyLabels[i].c_str());
	}

	ImGui::NextColumn();

	ImGui::Text("Player B Keymap");
	for (int i = 0; i < 5; ++i) {
		ImGui::AlignTextToFramePadding();
		if (ImGui::Button(playerBKeys[i].c_str(), ImVec2(70, 20)) && !keyMapInEditMode) {
			playerBKeyEdit[i] = true;
			keyMapInEditMode = true;
		}
		ImGui::SameLine();
		ImGui::Text(playerKeyLabels[i].c_str());
	}

	ImGui::Separator();

	ImGui::End();

	ImGui::SFML::Render(*window);
	window->display();
}

bool GlobalSettingsState::handleEvent(const sf::Event& event) {
	ImGui::SFML::ProcessEvent(event);
	if (keyMapInEditMode && event.type == sf::Event::KeyReleased) {
		for (std::size_t i = 0; i < 5; ++i) {
			if (playerAKeyEdit[i]) {
				settings->playerAKeyBinding.assignKey(static_cast<PlayerAction::Type>(i), event.key.code);
				keyMapInEditMode = false;
				playerAKeyEdit[i] = false;
			}
			else if (playerBKeyEdit[i]) {
				settings->playerBKeyBinding.assignKey(static_cast<PlayerAction::Type>(i), event.key.code);
				keyMapInEditMode = false;
				playerBKeyEdit[i] = false;
			}
		}
	}

	return true;
}