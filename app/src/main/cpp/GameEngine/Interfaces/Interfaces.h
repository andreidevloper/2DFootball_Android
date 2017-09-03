#pragma once

#include "IGameLogic.h"

#include "IGameView.h"

typedef List<SharedPtr<IGameView>> GameViewList;

#include "IScreenElement.h"

typedef List<SharedPtr<IScreenElement>> ScreenElementList;

#include "Controllers/IPointerHandler.h"
#include "Controllers/IKeyboardHandler.h"
