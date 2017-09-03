#pragma once

#include <GameEngine/UserInterface/UserInterface.h>

class FootballLogic;

class MainMenuUI : public BaseUI
{
	URHO3D_OBJECT(MainMenuUI, BaseUI);

public:
	MainMenuUI(Context* context);
	virtual void VOnUpdate(float timeStep);
	virtual bool VOnRestore();
	virtual void VSetVisible(bool visible);
	virtual bool VOnLostDevice();

protected:
	void CreateMainMenuWindow();
    void SetStartupValues();

    void HandleCloseButton(StringHash eventType, VariantMap& eventData);
    void HandleEnergyLineEdit(StringHash eventType, VariantMap& eventData);
    void HandleEnergyUpDownButton(StringHash eventType, VariantMap& eventData);
    void HandlePlayerCheckbox(StringHash eventType, VariantMap& eventData);
    void HandleSoundCheckbox(StringHash eventType, VariantMap& eventData);
    void HandleSimulationSlider(StringHash eventType, VariantMap& eventData);
    void HandleStartButton(StringHash eventType, VariantMap& eventData);
    void HandleStopButton(StringHash eventType, VariantMap& eventData);

protected:
	FootballLogic* m_pFootballLogic;
	UI* m_pUI;

	bool m_bIsInitialized;

    // UI elements
	SharedPtr<Window> m_pMainWindow;
    Button* m_pCloseButton;
    Text* m_pTimeText;
    Text* m_pLeftScoreText;
    Text* m_pRightScoreText;
    LineEdit* m_pEnergyLeftLineEdit;
    LineEdit* m_pEnergyRightLineEdit;
    Button* m_pEnergyLeftUpButton;
    Button* m_pEnergyLeftDownButton;
    Button* m_pEnergyRightUpButton;
    Button* m_pEnergyRightDownButton;
    Vector<CheckBox*> m_LeftTeamCheckbox;
    Vector<CheckBox*> m_RightTeamCheckbox;
    CheckBox* m_pSoundCheckbox;
    Slider* m_pSimulationSpeedSlider;
    Button* m_pStartButton;
    Button* m_pStopButton;
};
