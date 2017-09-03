#pragma once

//----------------------------------------------------------------------------------------------------
// FootballApp - Application class contains all necessary components in order to run program correctly.
//----------------------------------------------------------------------------------------------------

class FootballApp : public GameEngineApp
{
public:
	// Constructors
	FootballApp(Context* context);
	virtual ~FootballApp();

	// Application class overrided functions
	virtual void Setup();						// Called before engine initialization. Logging system and
	// user specific game options init here

	virtual void Start();						// Called after engine initialization. Setup application & subscribe to events here
	virtual void Stop();						// Perform optional cleanup after main loop has terminated

	// Create game logic and game views
	bool virtual VCreateViewLogic();			// Create specific game logic and game view.

	virtual String GetWindowIcon() { return ""; }
	virtual String GetWindowTitle() { return "2DFootball"; }
};
