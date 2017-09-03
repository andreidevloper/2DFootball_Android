#include "FootballStd.h"

#include "GameEngine/Events/GameEngineEvents.h"
#include "GameLogic/FootballLogic.h"

#include "FootballApp.h"

// Application entry-point
URHO3D_DEFINE_APPLICATION_MAIN(FootballApp)

FootballApp::FootballApp(Context* context) : GameEngineApp(context)
{
	g_pApp = this;
	m_bIsInit = false;
}

FootballApp::~FootballApp()
{

}


void FootballApp::Setup()
{
    GameEngineApp::Setup();


}


void FootballApp::Start()
{
    GameEngineApp::Start();

    SendEvent(EVENT_DATA_START_GAME);
	m_bIsInit = true;
}

void FootballApp::Stop()
{
    GameEngineApp::Stop();

}


bool FootballApp::VCreateViewLogic()
{
	m_pGameLogic = new FootballLogic(context_);
	context_->RegisterSubsystem(m_pGameLogic);

	if (!m_pGameLogic->VInitialize())
	{
		URHO3D_LOGERROR("Game logic init failed");
		m_bIsInit = false;
	}
	URHO3D_LOGDEBUG("Game logic successfully initialized");

	return true;
}

