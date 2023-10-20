#include "ProgrammScheduler.h"

#include "DistributorForMatching.h"
#include "TemplateMatching.h"
#include "getMat.h"
#include "Utility.h"
#include "CaseProcessing.h"
#include "PouchProcessing.h"
#include "ReadConfigFile.h"
#include "c_log.h"
#include "BuyItemsFlea.h"
#include "LobbyControler.h"
#include "ItemMoving.h"
#include "StashObject.h"
#include "InputMK.h"

#include "human_mouse.h"
#include <cmath>
#include <random>

#define M_PI 3.14159265358979323846


// HighCard | High T
// SecurityCheck Items Barter / Medical / Food 


//void SetPosition(float x, float y) {
//	double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
//	double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
//	double fx = x * (65535.0f / fScreenWidth);
//	double fy = y * (65535.0f / fScreenHeight);
//
//	INPUT  Input = { 0 };
//	Input.type = INPUT_MOUSE;
//	Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
//	Input.mi.dx = (long)fx;
//	Input.mi.dy = (long)fy;
//	::SendInput(1, &Input, sizeof(INPUT));
//}

//void SetPosition(std::int32_t x, std::int32_t y)
//{
//	mouse_event(MOUSEEVENTF_MOVE, (DWORD)x, (DWORD)y, 0, 0);
//}

//void SetPosition(std::int32_t x, std::int32_t y)
//{
//	SetCursorPos(x, y);
//}




typedef enum e_rotation_x {
	Left = -900,
	Right = 900,
	HalfLeft = -400,
	HalfRight = 400,

	TurnAround = 1,
	AutoRotationX = 2,
}RotationX;

typedef enum e_rotation_y {
	Up = -800,
	Down = 800,
	HalfUp = -400,
	HalfDown = 400,

	AutoRotationY = 1,
}RotationY;

class HumanizedMouse
{
	void moveMouse(int x, int y) {
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		input.mi.dx = x;
		input.mi.dy = y;
		input.mi.mouseData = 0;
		input.mi.time = 0;
		input.mi.dwExtraInfo = 0;

		SendInput(1, &input, sizeof(INPUT));
	}

	bool shouldAddPrefix() {
		std::srand(static_cast<unsigned>(std::time(0))); 

		return (std::rand() % 2 == 0);
	}

	int getRandomValueForAutoRotation(int lower, int uper) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> distribution(-lower, uper);

		return distribution(gen);
	}


public:
	
	void MoveToExactPoint(int x, int y, uint speedIn_NS) {

	}

	void MoveViaRotation(float xRotation, float yRotation, uint speedIn_NS) {

	}

	void MoveToDirection(RotationX rotationX = AutoRotationX, RotationY rotationY = AutoRotationY, uint speedIn_NS = 900) {
		int XRotation = 0;
		int YRotation = 0;
		vector<pair<int, int>> mousePositions;
		
		/// X
		if (rotationX == TurnAround) {
			if (shouldAddPrefix())
				XRotation = -1800;
			else
				XRotation = 1800;
		}
		else if (rotationX == AutoRotationX) {
			XRotation = getRandomValueForAutoRotation(-100, 100);
		}
		else {
			XRotation = static_cast<int>(rotationX);
		}
		
		/// Y
		if (rotationY == AutoRotationY) {
			YRotation = getRandomValueForAutoRotation(-50, 50);
		}
		else {
			YRotation = static_cast<int>(rotationY);
		}


	}
};


typedef enum e_direction {
	Forward = 0,
	Backwards = 1,
	AutoForward = 2,
	SprintForward = 3,
	AutoSprintForward = 4,

	NoDirection = 10,
}Direction;

class HumanizedKeyboard
{



public:

	void MoveToExactPosition() {

	}

	void MoveToDirection(Direction direction = NoDirection, uint speedIn_NS = 900) {

	}
};




class HumanizedMovement : public HumanizedMouse, public HumanizedKeyboard
{
	
};





void moveMouse_testing(int x, int y) {
    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = x;
    input.mi.dy = y;
    input.mi.mouseData = 0;
    input.mi.time = 0;
    input.mi.dwExtraInfo = 0;

	cout << "x: " << x << " y: " << y << endl;

    SendInput(1, &input, sizeof(INPUT));
}




//std::vector<std::pair<int, int>> splitDistance(int x, int y) {
//	std::vector<std::pair<int, int>> result;
//
//	std::random_device rd;
//	std::mt19937 gen(rd());
//	std::uniform_int_distribution<int> distX(2, 8);
//	std::uniform_int_distribution<int> distY(2, 8);
//
//	int initialStepX = distX(gen);
//	int initialStepY = distY(gen);
//
//	while (x > 0 || y > 0) {
//		int stepX = std::min(initialStepX, x);
//		int stepY = std::min(initialStepY, y);
//
//		result.push_back(std::make_pair(stepX, stepY));
//
//		x -= stepX;
//		y -= stepY;
//
//		// Reduziere die Schrittgr��e
//		initialStepX = std::max(1, initialStepX - 1);
//		initialStepY = std::max(1, initialStepY - 1);
//	}
//
//	return result;
//}

std::vector<std::pair<int, int>> splitDistance(int x, int y) {
	std::vector<std::pair<int, int>> result;
	int progressTrackerX = x;
	int progressTrackerY = y;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distX_Default(6, 9);
	std::uniform_int_distribution<int> distY_Default(4, 6);

	std::uniform_int_distribution<int> distX_13(4, 7);
	std::uniform_int_distribution<int> distY_13(3, 4);

	std::uniform_int_distribution<int> distX_14(2, 5);
	std::uniform_int_distribution<int> distY_14(1, 2);

	std::uniform_int_distribution<int> distX_15(1, 3);


	//cout << "x: " << 1.0 / 80 * x << " y: " << 1.0 / 80 * y << endl;

	int stepX = 0;
	int stepY = 0;
	while (x > 0 || y > 0) {
		

			//cout << x << " 1/8 " <<  1.0 / 12 * progressTracker << " 1/6 " <<  1.0 / 5 * progressTracker << " 1/2 " <<  1.0 / 2 * progressTracker << endl;

			if (x <= 1.0 / 12 * progressTrackerX) {
				stepX = distX_15(gen);
			}
			else if (x <= 1.0 / 5 * progressTrackerX) {
				stepX = distX_14(gen);
			}
			else if (x <= 1.0 / 2 * progressTrackerX) {
				stepX = distX_13(gen);
			}
			else {
				stepX = distX_Default(gen);
			}
		
			if (y <= 1.0 / 4 * progressTrackerY) {
				// F�hre Aktionen f�r progressTracker >= 1/4 von y aus
				stepY = distY_14(gen);
			}
			else if (y <= 1.0 / 2 * progressTrackerY) {
				// F�hre Aktionen f�r progressTracker >= 1/3 von y aus
				stepY = distY_13(gen);
			}
			else {
				stepY = distY_Default(gen);
			}
		


		

		if (stepX > x) stepX = x;
		if (stepY > y) stepY = y;

		result.push_back(std::make_pair(stepX, stepY));

		x -= stepX;
		y -= stepY;

	}

	return result;
}



/// 180 degree turn = ~1800 pixels
/// 90  degree turn = ~900  pixels
/// 45  degree turn = ~400  pixels

/// 90  degree up   = ~-800 pixels
/// 45  degree up   = ~-400 pixels


int main() {
	//c_log::add_out(new c_log::c_log_consolestream);


	//const HWND hWND = GetMat::FindeWindow();
	//SetForegroundWindow(hWND);
	//Sleep(1000);//Delete later


	int endX = 900; // Endpunkt
	int endY = 120;

	std::vector<std::pair<int, int>> steps = splitDistance(endX, endY);
	int currentX = 0, currentY = 0, count = 0;

	for (const auto& step : steps) {
		currentX += step.first;
		currentY += step.second;
		count++;
		std::cout << "(" << step.first << ", " << step.second << ") -> Aktuelle Position: (" << currentX << ", " << currentY << ")" << std::endl;
	}

	std::cout << "Endpunkt erreicht: (" << currentX << ", " << currentY << ")\nPoints to Endpoint: " << count << std::endl;




	//int y = 0;
	//int x = 0;
	//while (true)
	//{
	//	/*while (y > -100)
	//	{
	//		moveMouse(2, -1);
	//		y--;
	//		Sleep(1);
	//	}
	//	while (y < 100)
	//	{
	//		moveMouse(2, 1);
	//		y++;
	//		Sleep(1);
	//	}*/

	//	/*while (x > -1800)
	//	{
	//		moveMouse(-1, 0);
	//		x--;
	//		Sleep(1);
	//	}*/

	//	while (y > -400) {
	//		moveMouse(0, -1);
	//		y--;
	//		Sleep(1);
	//	}
	//}
}