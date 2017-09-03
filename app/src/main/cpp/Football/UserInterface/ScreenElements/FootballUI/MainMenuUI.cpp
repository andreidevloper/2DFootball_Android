#include "FootballStd.h"

#include "GameLogic/FootballLogic.h"
#include "Events/FootballEvents.h"
#include "MainMenuUI.h"

MainMenuUI::MainMenuUI(Context* context) : BaseUI(context)
{
	m_pUI = g_pApp->GetSubsystem<UI>();
	m_bIsInitialized = false;
	m_pMainWindow = nullptr;
	m_bIsVisible = false;
	m_pFootballLogic = (FootballLogic*)g_pApp->GetGameLogic();
}

void MainMenuUI::VOnUpdate(float timeStep)
{
	float leftTeamEnergy = 0.0f;
	float rightTeamEnergy = 0.0f;

	if (m_pFootballLogic->GetGameState() == GS_GAMEPLAY || m_pFootballLogic->GetGameState() == GS_FIRST_KICK)
	{
		leftTeamEnergy = m_pFootballLogic->CalcLeftTeamEnergy();
		rightTeamEnergy = m_pFootballLogic->CalcRightTeamEnergy();
		m_pEnergyLeftLineEdit->SetText(String(leftTeamEnergy));
		m_pEnergyRightLineEdit->SetText(String(rightTeamEnergy));
		m_pFootballLogic->SetFalseETDFlag();
	}
	else if (m_pFootballLogic->IsEnergyTextDirty())
	{
		leftTeamEnergy = m_pFootballLogic->GetLeftTeamEnergy();
		rightTeamEnergy = m_pFootballLogic->GetRightTeamEnergy();
		m_pEnergyLeftLineEdit->SetText(String(leftTeamEnergy));
		m_pEnergyRightLineEdit->SetText(String(rightTeamEnergy));
		m_pFootballLogic->SetFalseETDFlag();
	}

	if(m_pFootballLogic->IsTimeTextDirty())
	{
		float time = m_pFootballLogic->GetTime();
		m_pTimeText->SetText(String(roundf(time)));
		m_pFootballLogic->SetFalseTTDFlag();
	}

	if (m_pFootballLogic->IsScoreTextDirty())
	{
		int left = m_pFootballLogic->GetScore(TeamNumber::Left);
		int right = m_pFootballLogic->GetScore(TeamNumber::Right);
		m_pLeftScoreText->SetText(String(left));
		m_pRightScoreText->SetText(String(right));
		m_pFootballLogic->SetFalseSTDFlag();
	}

}

bool MainMenuUI::VOnRestore()
{
	if (!m_bIsInitialized)
	{
		MainMenuUI::CreateMainMenuWindow();
		return true;
	}
	else
	{
		if (m_pMainWindow)
		{
			m_pMainWindow->SetVisible(true);
		}
	}

	return false;
}

void MainMenuUI::VSetVisible(bool value)
{
	if (m_pMainWindow)
	{
		m_pMainWindow->SetVisible(value);
	}
}

bool MainMenuUI::VOnLostDevice()
{
	m_pMainWindow->SetVisible(false);
	return false;
}

void MainMenuUI::CreateMainMenuWindow()
{
	if (!m_pMainWindow)
	{
		UI* ui = GetSubsystem<UI>();

		XMLFile* file = g_pApp->GetResCache()->GetResource<XMLFile>("UI/PanelUI.xml");
		SharedPtr<UIElement> element = ui->LoadLayout(file);
		m_pMainWindow = StaticCast<Window>(element);

		m_pCloseButton = (Button*)element->GetChild(String("Close_Button"), true);
		SubscribeToEvent(m_pCloseButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, HandleCloseButton));

		// Получить указатель на время текст бокс
		m_pTimeText = (Text*)element->GetChild(String("Text_Time"), true);
		m_pLeftScoreText = (Text*)element->GetChild(String("Text_Score_Left"), true);
		m_pRightScoreText = (Text*)element->GetChild(String("Text_Score_Right"), true);

		// Получить указатель на энергию лине эдит
		m_pEnergyLeftLineEdit = (LineEdit*)element->GetChild(String("Energy_Left"), true);
		SubscribeToEvent(m_pEnergyLeftLineEdit, E_TEXTFINISHED, URHO3D_HANDLER(MainMenuUI, HandleEnergyLineEdit));
		m_pEnergyRightLineEdit = (LineEdit*)element->GetChild(String("Energy_Right"), true);
		SubscribeToEvent(m_pEnergyRightLineEdit, E_TEXTFINISHED, URHO3D_HANDLER(MainMenuUI, HandleEnergyLineEdit));

		// Получить указатель на изменение энерегии кнопки
		m_pEnergyLeftUpButton = (Button*)element->GetChild(String("Energy_Left_Up"), true);
		SubscribeToEvent(m_pEnergyLeftUpButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, HandleEnergyUpDownButton));
		m_pEnergyLeftDownButton = (Button*)element->GetChild(String("Energy_Left_Down"), true);
		SubscribeToEvent(m_pEnergyLeftDownButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, HandleEnergyUpDownButton));
		m_pEnergyRightUpButton = (Button*)element->GetChild(String("Energy_Right_Up"), true);
		SubscribeToEvent(m_pEnergyRightUpButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, HandleEnergyUpDownButton));
		m_pEnergyRightDownButton = (Button*)element->GetChild(String("Energy_Right_Down"), true);
		SubscribeToEvent(m_pEnergyRightDownButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, HandleEnergyUpDownButton));

		// Получить указатель на чекбоксы левой комманды
		CheckBox* pCheckboxLeft = (CheckBox*)element->GetChild(String("Checkbox_Left_2"), true);
		m_LeftTeamCheckbox.Push(pCheckboxLeft);
		SubscribeToEvent(pCheckboxLeft, E_TOGGLED, URHO3D_HANDLER(MainMenuUI, HandlePlayerCheckbox));

		pCheckboxLeft = (CheckBox*)element->GetChild(String("Checkbox_Left_3"), true);
		m_LeftTeamCheckbox.Push(pCheckboxLeft);
		SubscribeToEvent(pCheckboxLeft, E_TOGGLED, URHO3D_HANDLER(MainMenuUI, HandlePlayerCheckbox));

		pCheckboxLeft = (CheckBox*)element->GetChild(String("Checkbox_Left_4"), true);
		m_LeftTeamCheckbox.Push(pCheckboxLeft);
		SubscribeToEvent(pCheckboxLeft, E_TOGGLED, URHO3D_HANDLER(MainMenuUI, HandlePlayerCheckbox));

		pCheckboxLeft = (CheckBox*)element->GetChild(String("Checkbox_Left_5"), true);
		m_LeftTeamCheckbox.Push(pCheckboxLeft);
		SubscribeToEvent(pCheckboxLeft, E_TOGGLED, URHO3D_HANDLER(MainMenuUI, HandlePlayerCheckbox));

		pCheckboxLeft = (CheckBox*)element->GetChild(String("Checkbox_Left_6"), true);
		m_LeftTeamCheckbox.Push(pCheckboxLeft);
		SubscribeToEvent(pCheckboxLeft, E_TOGGLED, URHO3D_HANDLER(MainMenuUI, HandlePlayerCheckbox));

		// Получить указатель на чекбоксы правой комманды
		CheckBox* pCheckboxRight = (CheckBox*)element->GetChild(String("Checkbox_Right_2"), true);
		m_RightTeamCheckbox.Push(pCheckboxRight);
		SubscribeToEvent(pCheckboxRight, E_TOGGLED, URHO3D_HANDLER(MainMenuUI, HandlePlayerCheckbox));

		pCheckboxRight = (CheckBox*)element->GetChild(String("Checkbox_Right_3"), true);
		m_RightTeamCheckbox.Push(pCheckboxRight);
		SubscribeToEvent(pCheckboxRight, E_TOGGLED, URHO3D_HANDLER(MainMenuUI, HandlePlayerCheckbox));

		pCheckboxRight = (CheckBox*)element->GetChild(String("Checkbox_Right_4"), true);
		m_RightTeamCheckbox.Push(pCheckboxRight);
		SubscribeToEvent(pCheckboxRight, E_TOGGLED, URHO3D_HANDLER(MainMenuUI, HandlePlayerCheckbox));

		pCheckboxRight = (CheckBox*)element->GetChild(String("Checkbox_Right_5"), true);
		m_RightTeamCheckbox.Push(pCheckboxRight);
		SubscribeToEvent(pCheckboxRight, E_TOGGLED, URHO3D_HANDLER(MainMenuUI, HandlePlayerCheckbox));

		pCheckboxRight = (CheckBox*)element->GetChild(String("Checkbox_Right_6"), true);
		m_RightTeamCheckbox.Push(pCheckboxRight);
		SubscribeToEvent(pCheckboxRight, E_TOGGLED, URHO3D_HANDLER(MainMenuUI, HandlePlayerCheckbox));

		// Получить указатель на чекбокс ЗВУК
		m_pSoundCheckbox = (CheckBox*)element->GetChild(String("Sound"), true);
		SubscribeToEvent(m_pSoundCheckbox, E_TOGGLED, URHO3D_HANDLER(MainMenuUI, HandleSoundCheckbox));

		m_pSimulationSpeedSlider = (Slider*)element->GetChild(String("Simulation_Speed"), true);
		SubscribeToEvent(m_pSimulationSpeedSlider, E_SLIDERCHANGED, URHO3D_HANDLER(MainMenuUI, HandleSimulationSlider));

		m_pStartButton = (Button*)element->GetChild(String("Start"), true);
		SubscribeToEvent(m_pStartButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, HandleStartButton));
		m_pStopButton = (Button*)element->GetChild(String("Stop"), true);
		SubscribeToEvent(m_pStopButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, HandleStopButton));

		m_pUI->GetRoot()->AddChild(element);

		SetStartupValues();

        m_bIsInitialized = true;
	}
}

void MainMenuUI::SetStartupValues()
{
	m_pTimeText->SetText(String(0.0f));
	m_pLeftScoreText->SetText(String(0));
	m_pRightScoreText->SetText(String(0));

	m_pSoundCheckbox->SetChecked(false);
	GetSubsystem<Audio>()->SetMasterGain(SOUND_MASTER, 0.0f);


	m_pSimulationSpeedSlider->SetValue(60.0f);


	m_pEnergyLeftLineEdit->SetText(String(0));
	m_pEnergyRightLineEdit->SetText(String(0));

	m_pStartButton->SetEnabled(true);
	m_pStopButton->SetEnabled(false);
}

void MainMenuUI::HandleCloseButton(StringHash eventType, VariantMap& eventData)
{
	if (m_pMainWindow)
	{
		m_pMainWindow->SetVisible(false);
	}
}

void MainMenuUI::HandleEnergyLineEdit(StringHash eventType, VariantMap& eventData)
{

}

void MainMenuUI::HandleEnergyUpDownButton(StringHash eventType, VariantMap& eventData)
{
	Button* button = (Button*)eventData[Released::P_ELEMENT].GetPtr();
	String name = button->GetName();
	VariantMap data;
	float value = 0;
	if (name.Contains("Left"))
	{
		data[ENERGY_VALUE_CHANGE::P_TEAM] = TeamNumber::Left;
		if (name.Contains("Up"))
		{
			value = m_pFootballLogic->GetLeftTeamEnergy() + 1000.0f;
		}
		else
		{
			value = m_pFootballLogic->GetLeftTeamEnergy() - 1000.0f;
		}
	}
	else
	{
		data[ENERGY_VALUE_CHANGE::P_TEAM] = TeamNumber::Right;
		if (name.Contains("Up"))
		{
			value = m_pFootballLogic->GetRightTeamEnergy() + 1000.0f;
		}
		else
		{
			value = m_pFootballLogic->GetRightTeamEnergy() - 1000.0f;
		}
	}

	data[ENERGY_VALUE_CHANGE::P_VALUE] = value;
	SendEvent(E_ENERGY_VALUE_CHANGE, data);
}

void MainMenuUI::HandlePlayerCheckbox(StringHash eventType, VariantMap& eventData)
{
	CheckBox* checkbox = (CheckBox*)eventData[Toggled::P_ELEMENT].GetPtr();
	bool state = eventData[Toggled::P_STATE].GetBool();
	VariantMap data;

	String name = checkbox->GetName();
	int number = ToInt(name.Substring(name.Length() - 1));

	if (name.Contains("Left"))
	{
		data[PLAYER_CHECKBOX_PRESSED::P_TEAM] = TeamNumber::Left;
	}
	else
	{
		data[PLAYER_CHECKBOX_PRESSED::P_TEAM] = TeamNumber::Right;
	}

	data[PLAYER_CHECKBOX_PRESSED::P_PLAYER_NUMBER] = number;
	data[PLAYER_CHECKBOX_PRESSED::P_VALUE] = state;

	SendEvent(E_PLAYER_CHECKBOX_PRESSED, data);
}

void MainMenuUI::HandleSoundCheckbox(StringHash eventType, VariantMap& eventData)
{
	CheckBox* checkbox = (CheckBox*)eventData[Toggled::P_ELEMENT].GetPtr();
	bool state = eventData[Toggled::P_STATE].GetBool();
	float value = state ? 1.0f : 0.0f;
	GetSubsystem<Audio>()->SetMasterGain(SOUND_MASTER, value);
}

void MainMenuUI::HandleSimulationSlider(StringHash eventType, VariantMap& eventData)
{
	float value = eventData[SliderChanged::P_VALUE].GetFloat();
	VariantMap data;
	data[SIMULATION_SLIDER_CHANGE::P_VALUE] = value;
	SendEvent(E_SIMULATION_SLIDER_CHANGE, data);
}

void MainMenuUI::HandleStartButton(StringHash eventType, VariantMap& eventData)
{
	m_pFootballLogic->SetNeededGameState(GS_FIRST_KICK);
	m_pStartButton->SetEnabled(false);
	m_pStopButton->SetEnabled(true);
}

void MainMenuUI::HandleStopButton(StringHash eventType, VariantMap& eventData)
{
	m_pFootballLogic->SetNeededGameState(GS_STOP);
	m_pStopButton->SetEnabled(false);
	m_pStartButton->SetEnabled(true);
}





