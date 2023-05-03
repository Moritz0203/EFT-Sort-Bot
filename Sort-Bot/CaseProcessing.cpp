#pragma once
#include "CaseProcessing.h"
#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include "TemplateMatching.h"
#include "PointGlobalVector.h"
#include "DistributorForMatching.h"
#include "getMat.h"
#include "InputMK.h"
#include "Checks.h"
using namespace std;
using namespace cv;

std::array<std::string, 6> CasesInCase{
	"itemImages/CaseImages/AmmoCase.png",
	"itemImages/CaseImages/GrenadCase.png",
	"itemImages/CaseImages/HolodilnickCase.png",
	"itemImages/CaseImages/MagCase.png",
	"itemImages/CaseImages/MedCase.png",
	"itemImages/CaseImages/MoneyCase.png",
};

std::array<std::string, 6> NameOfItemCasesInCase{
	"AmmoCase",
	"GrenadCase",
	"HolodilnickCase",
	"MagCase",
	"MedCase",
	"MoneyCase",
};

std::array<double, 6> CasesInCaseThreshold{
	0.90,//AmmoCase
	0.90,//GrenadCase
	0.90,//HolodilnickCase
	0.90,//MagCase
	0.90,//MedCase
	0.90,//MoneyCase
};


void CaseProcessor::caseProcess() {
	Matching matching;
	std::shared_ptr<vector<vector<POINT>>> ptr_FreeSlots;
	findFreeSlots FindFreeSlots;
	checksPublic ChecksPublic;

	matching.CaseMatching();
	cout << PointCaseInStash::pointCaseInStash_NC.size() << endl;
	cleanUpVectorCase();

	std::shared_ptr<PointCaseInStash> ptr_PCIS;
	ChecksPublic.CheckScrollbarPositions();
	for (int i = 0; i < PointCaseInStash::pointCaseInStash_C.size(); i++) {
		for (PointCaseInStash INpointCase : PointCaseInStash::pointCaseInStash_C[i]) {
			ptr_PCIS = std::make_shared<PointCaseInStash>(INpointCase);

			if (INpointCase.nameOfCase == "THICCcase" || INpointCase.nameOfCase == "ItemsCase") {
				//OpenCaseAndTakeScreen(ptr_PCIS);
			}
			else {
				//ptr_FreeSlots = make_shared<vector<vector<POINT>>>(INpointCase.freeSlots);
				//FindFreeSlots.findeSlots(INpointCase.point, ptr_FreeSlots);
			}
		}
		int keyforInput = 0x28;// virtual-key code for the "DOWN ARROW" key
		Keyboard::KeyboardInput(keyforInput);
		Sleep(500);
	}
}

void CaseProcessor::MoveTopBarTHICCcase() {
	Mat MatScreen;
	Mat templ;

	HWND hWND = GetMat::FindeWindow();
	SetForegroundWindow(hWND);
	MatScreen = GetMat::getMat(hWND);

	templ = imread("ObjectImages/THICCtopBar.png");
	POINT pointA = TemplateMatching::templateMatchingObjects(MatScreen, templ, 0.70);
	pointA.y = (templ.rows / 2) + pointA.y;
	pointA.x = (templ.cols / 2) + pointA.x;

	POINT pointB{};
	pointB.x = pointA.x - 200;
	Mouse::MouseMoveAtoB(pointA, pointB);
}


void CaseProcessor::OpenCaseAndTakeScreen(std::shared_ptr<PointCaseInStash> ptr_PCIS) {
	Mat MatScreen;
	POINT point{};
	GetMat getMat;

	point.y = (ptr_PCIS->heightTempl / 2) + ptr_PCIS->point.y;
	point.x = (ptr_PCIS->widthTempl / 2) + ptr_PCIS->point.x;
	Mouse::MoverPOINTandPressTwoTimes(point);

	if (ptr_PCIS->nameOfCase == "THICCcase") {
		MoveTopBarTHICCcase();
	}

	HWND hWND = GetMat::FindeWindow();
	SetForegroundWindow(hWND);
	MatScreen = GetMat::getMat(hWND);

	int keyforInput = 0x1B;// virtual-key code for the "ESC button" key
	Keyboard::KeyboardInput(keyforInput);

	MatchingCaseInCase(MatScreen, ptr_PCIS->page, ptr_PCIS->point);
}

void CaseProcessor::MatchingCaseInCase(Mat& MatScreen, int page, POINT parentCasePoints) {
	string filename, templatename;
	double threshold;
	int sizeString = sizeof(CasesInCase) / sizeof(string);
	Mat templ;
	shared_ptr<Prefix> prefix{};
	std::shared_ptr<vector<vector<POINT>>> ptr_FreeSlots;
	findFreeSlots FindFreeSlots;
	vector<vector<POINT>> freeSlots_empty{};
	


	vector<POINT> ReturnDataCase;
	for (int i2 = 0; i2 < sizeString; i2++) {
		threshold = CasesInCaseThreshold[i2];
		templatename = CasesInCase[i2];
		templ = imread(CasesInCase[i2]);

		ReturnDataCase = TemplateMatching::templateMatchingItems(templatename, threshold, false, true, NameOfItemCasesInCase[i2], MatScreen);

		for (int i3 = 0; i3 < ReturnDataCase.size(); i3++) {
			Rect Rec(ReturnDataCase[i3].x, ReturnDataCase[i3].y, 13/*templ.cols*/, templ.rows);

			const string tagCase = TextMatching::textMatching(MatScreen, Rec);
			if (Matching::checkSecondLastChar(tagCase)) {
				PointCaseInCase::pointCaseInCase[page].emplace_back(ReturnDataCase[i3], parentCasePoints, NameOfItemCasesInCase[i2], tagCase, templ.rows, templ.cols, page, 0x0, freeSlots_empty, prefix);
				ptr_FreeSlots = make_shared<vector<vector<POINT>>>(PointCaseInCase::pointCaseInCase[page].back().freeSlots);
				FindFreeSlots.findeSlots(PointCaseInCase::pointCaseInCase[page].back().point, ptr_FreeSlots);
			}
		}
	}
}

void CaseProcessor::cleanUpVectorCase() {
	vector<PointCaseInStash> temp;

	temp = PointCaseInStash::pointCaseInStash_NC[0];
	PointCaseInStash::pointCaseInStash_C.emplace_back(temp);
	temp.clear();

	int iTemp = 1;
	for (int i = 1; i < PointCaseInStash::pointCaseInStash_NC.size(); i++) {
		iTemp++;

		if (iTemp == PointCaseInStash::pointCaseInStash_NC.size())
			break;

		for (PointCaseInStash pointCase : PointCaseInStash::pointCaseInStash_NC[i]) {
			for (PointCaseInStash inPointCase : PointCaseInStash::pointCaseInStash_NC[iTemp]) {
				PointCaseInStash tempPointCase = inPointCase;
				tempPointCase.point.y = tempPointCase.point.y + 343;

				int x_minus_1 = tempPointCase.point.x - 1;
				int x_plus_1 = tempPointCase.point.x + 1;

				if (tempPointCase.point.y == pointCase.point.y) {
					if (tempPointCase.point.x == pointCase.point.x || x_minus_1 == pointCase.point.x || x_plus_1 == pointCase.point.x) {
						temp.emplace_back(pointCase);
					}
				}
			}
		}
		if (iTemp == 10) {
			for (PointCaseInStash pointCase : PointCaseInStash::pointCaseInStash_NC[iTemp]) {
				if (pointCase.point.y >= 618) {
					temp.emplace_back(pointCase);
				}
			}
		}
		PointCaseInStash::pointCaseInStash_C.emplace_back(temp);
		temp.clear();
	}
}
