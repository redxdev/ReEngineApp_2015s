#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	std::vector<vector3> positions = {
		vector3(-4.f, -2.f, 5.f),
		vector3(1.f, -2.f, 5.f),
		vector3(-3.f, -1.f, 3.f),
		vector3(2.f, -1.f, 3.f),
		vector3(-2.f, 0.f, 0.f),
		vector3(3.f, 0.f, 0.f),
		vector3(-1.f, 1.f, -3.f),
		vector3(4.f, 1.f, -3.f),
		vector3(0.f, 2.f, -5.f),
		vector3(5.f, 2.f, -5.f),
		vector3(1.f, 3.f, -5.f)
	};

	for (auto& pos : positions)
	{
		m_pMeshMngr->AddSphereToQueue(glm::translate(pos) * glm::scale(vector3(0.1, 0.1, 0.1)), RERED);
	}

	// modular arithmetic to figure out current + next points to lerp based on run time

	float adjustedRunTime = fRunTime / fDuration; // slow down the time by fDuration
	int currentIdx = ((int)adjustedRunTime) % positions.size();
	vector3 current = positions[currentIdx];
	vector3 next = positions[(currentIdx + 1) % positions.size()];
	float val = fmod(adjustedRunTime, positions.size()) - currentIdx;

	m_pMeshMngr->SetModelMatrix(glm::translate(glm::lerp(current, next, val)), "WallEye");
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}