#include "FootballStd.h"

#include "Events/FootballEvents.h"
#include "GameLogic/FootballLogic.h"
#include "Components/Objects/CameraComponent/CameraComponent.h"

#include "UserInterface/ScreenElements/FootballUI/MainMenuUI.h"
#include "UserInterface/ScreenElements/FootballUI/CommentUI.h"

#include "FootballView.h"

FootballView::FootballView(Context *context, Renderer* renderer) : HumanView(context, renderer)
{
    m_pMainMenuUI = SharedPtr<IScreenElement>(new MainMenuUI(context));
    m_pCommentUI = SharedPtr<IScreenElement>(new CommentUI(context));

    VPushElement(m_pMainMenuUI);
    VPushElement(m_pCommentUI);

    m_pMainMenuUI->VOnRestore();
    m_pCommentUI->VOnRestore();

    m_pMainMenuUI->VSetVisible(true);
    m_pCommentUI->VSetVisible(true);

    m_pFootballLogic = (FootballLogic*)g_pApp->GetGameLogic();
    SubscribeToEvent(E_INTRO_SIMULATION, URHO3D_HANDLER(FootballView, HandleIntroSimulationDelegate));
    SubscribeToEvent(E_FIRST_KICK, URHO3D_HANDLER(FootballView, HandleFirstKickDelegate));
    SubscribeToEvent(E_RESTART_SIMULATION, URHO3D_HANDLER(FootballView, HandleRestartGameDelegate));
}


bool FootballView::VOnRestore()
{
	if (!HumanView::VOnRestore())
	{
		return false;
	}

	return true;
}

void FootballView::VOnUpdate(float timeStep)
{
	HumanView::VOnUpdate(timeStep);

}

void FootballView::VShutdown()
{
	HumanView::VShutdown();

}

bool FootballView::VOnMsgProc(AppMsg message)
{
	HumanView::VOnMsgProc(message);


	return true;
}

bool FootballView::VLoadGameDelegate(SharedPtr<File> level)
{
	//HumanView::VLoadGameDelegate(level);
    m_pScene = g_pApp->GetGameLogic()->GetScene();

    m_pControlledNode = m_pScene->CreateChild("Camera");
    Camera* pCamera = m_pControlledNode->CreateComponent<Camera>();
    m_pControlledNode->SetPosition(CAMERA_DEFAULT_POS);
    m_pControlledNode->CreateComponent<CameraComponent>();

    // Создаем ноду и прикрепляем к ней источник света (солнечный свет).
    Node* lightNode = m_pScene->CreateChild();
    Light* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
    light->SetColor(Color(0.8f, 0.8f, 0.8f, 1.0f));
    light->SetSpecularIntensity(1.0f);
    light->SetBrightness(0.8f);
    light->SetRange(10.0f);
    light->SetFov(30.0f);

    auto viewport = new Viewport(context_, m_pScene, pCamera);
    m_pRenderer->SetViewport(0, viewport);

    CreateLight();

    return true;
}

void FootballView::CreateLight()
{
    // Create a point light to the world so that we can see something.
    Node* lightNode = m_pScene->CreateChild("PointLight_1");
    Light* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_POINT);
    light->SetSpecularIntensity(1.0f);
    light->SetBrightness(2.0f);
    light->SetRange(11.0f);
    light->SetFov(30.0f);
    lightNode->SetEnabled(false);
    m_Light.Push(lightNode);

    // Create light animation
    SharedPtr<ObjectAnimation> lightAnimation(new ObjectAnimation(context_));
    // Create light position animation
    SharedPtr<ValueAnimation> positionAnimation(new ValueAnimation(context_));
    // Use spline interpolation method
    positionAnimation->SetInterpolationMethod(IM_SPLINE);
    // Set spline tension
    positionAnimation->SetSplineTension(0.7f);
    positionAnimation->SetKeyFrame(0.0f, Vector3(15.0f, -15.0f, -10.0f));
    positionAnimation->SetKeyFrame(1.0f, Vector3(-15.0f, -15.0f, -10.0f));
    positionAnimation->SetKeyFrame(2.0f, Vector3(-15.0f, 15.0f, -10.0f));
    positionAnimation->SetKeyFrame(3.0f, Vector3(15.0f, 15.0f, -10.0f));
    positionAnimation->SetKeyFrame(4.0f, Vector3(15.0f, -15.0f, -10.0f));
    // Set position animation
    lightAnimation->AddAttributeAnimation("Position", positionAnimation);

    // Create light color animation
    SharedPtr<ValueAnimation> colorAnimation(new ValueAnimation(context_));
    colorAnimation->SetKeyFrame(0.0f, Color::WHITE);
    colorAnimation->SetKeyFrame(1.0f, Color::RED);
    colorAnimation->SetKeyFrame(2.0f, Color::YELLOW);
    colorAnimation->SetKeyFrame(3.0f, Color::GREEN);
    colorAnimation->SetKeyFrame(4.0f, Color::WHITE);
    // Set Light component's color animation
    lightAnimation->AddAttributeAnimation("@Light/Color", colorAnimation);

    // Apply light animation to light node
    lightNode->SetObjectAnimation(lightAnimation);

    lightNode = m_pScene->CreateChild("PointLight_2");
    light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_POINT);
    light->SetSpecularIntensity(1.0f);
    light->SetBrightness(2.0f);
    light->SetRange(11.0f);
    light->SetFov(30.0f);
    lightNode->SetEnabled(false);
    m_Light.Push(lightNode);

    // Create light animation
    lightAnimation = new ObjectAnimation(context_);

    // Create light position animation
    positionAnimation = new ValueAnimation(context_);
    // Use spline interpolation method
    positionAnimation->SetInterpolationMethod(IM_SPLINE);
    // Set spline tension
    positionAnimation->SetSplineTension(0.7f);
    positionAnimation->SetKeyFrame(0.0f, Vector3(-15.0f, 15.0f, -10.0f));
    positionAnimation->SetKeyFrame(1.0f, Vector3(15.0f, 15.0f, -10.0f));
    positionAnimation->SetKeyFrame(2.0f, Vector3(15.0f, -15.0f, -10.0f));
    positionAnimation->SetKeyFrame(3.0f, Vector3(-15.0f, -15.0f, -10.0f));
    positionAnimation->SetKeyFrame(4.0f, Vector3(-15.0f, 15.0f, -10.0f));
    // Set position animation
    lightAnimation->AddAttributeAnimation("Position", positionAnimation);

    // Create light color animation
    colorAnimation = new ValueAnimation(context_);
    colorAnimation->SetKeyFrame(0.0f, Color::WHITE);
    colorAnimation->SetKeyFrame(1.0f, Color::BLUE);
    colorAnimation->SetKeyFrame(2.0f, Color::GREEN);
    colorAnimation->SetKeyFrame(3.0f, Color::RED);
    colorAnimation->SetKeyFrame(4.0f, Color::WHITE);
    // Set Light component's color animation
    lightAnimation->AddAttributeAnimation("@Light/Color", colorAnimation);

    // Apply light animation to light node
    lightNode->SetObjectAnimation(lightAnimation);
}

void FootballView::HandleIntroSimulationDelegate(StringHash eventType, VariantMap& eventData)
{
    // Сбрасываем счётчик очков
    m_Light[0]->SetEnabled(false);
    m_Light[1]->SetEnabled(false);
}

void FootballView::HandleFirstKickDelegate(StringHash eventType, VariantMap& eventData)
{
    // Сбрасываем счётчик очков
    m_Light[0]->SetEnabled(false);
    m_Light[1]->SetEnabled(false);
}

void FootballView::HandleRestartGameDelegate(StringHash eventType, VariantMap& eventData)
{
    m_Light[0]->SetEnabled(true);
    m_Light[1]->SetEnabled(true);

    auto soundSource = m_pFootballLogic->GetReferee()->GetOrCreateComponent<SoundSource>();
    soundSource->Play(g_pApp->GetResCache()->GetResource<Sound>("Audio/whistle.wav"));
}

