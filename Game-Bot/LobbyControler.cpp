#pragma once
#include "LobbyControler.h"
#include "Checks.h"
#include "getMat.h"
#include "LobbyControler.h"
#include "Includes.h"
#include "TemplateMatching.h"
#include "InputMK.h"
#include "ReadConfigFile.h"
#include "PrefixProcessing.h"
#include "ItemProcessing.h"
#include <queue>
#include "CaseProcessing.h"
#include "PouchProcessing.h"
#include "ItemMoving.h"


std::queue<std::function<void()>> q;
std::mutex m;
std::condition_variable c_v;

void LobbyControler::SchedulerWorker() {
	while (true) {
		std::unique_lock<std::mutex> lock(m);
		c_v.wait(lock, [] {
			return !q.empty(); });

		auto func = q.front();
		q.pop();
		lock.unlock();
		func();

		if (q.size() == 0)
			break;
	}
}



void LobbyControler::TakeScreenShots() {
	checksPublic ChecksPublic;
	GetMat getMat;

	ChecksPublic.CheckScrollbarPositions();
	Sleep(300);
	getMat.TakeScreenshots(Stash_Version);
}

void LobbyControler::OpenStashView() {
	const HWND hWND = GetMat::FindeWindow();
	SetForegroundWindow(hWND);
	Sleep(5);//Delete later
	const Mat MatScreen = GetMat::getMat(hWND);
	const Mat templ_CharacterButton = imread("ObjectImages/CharacterButton.png");

	POINT point = TemplateMatching::templateMatchingObjects(MatScreen, templ_CharacterButton, 0.90);
	point.y = (templ_CharacterButton.rows / 2) + point.y;
	point.x = (templ_CharacterButton.cols / 2) + point.x;

	Mouse::MoverPOINTandPress(point);

	Sleep(2500);
}

void LobbyControler::FirstStartGetData() {
	ReadPrefixConfigFile readPrefixConfigFile("ConfigFile.txt");

	readPrefixConfigFile.ParseConfig();
	readPrefixConfigFile.PrintData();

}

void LobbyControler::FirstStartStashMatching() {
	ItemProcessing itemProcessing;
	LobbyControler lobbyControler;
	CaseProcessing caseProcessing;
	PouchProcessing pouchProcessing(Gamma);//later get from config file or form server 
	PrefixProcessing prefixProcessing;
	ItemMoving itemMoving;

	TakeScreenShots();

	std::thread ItemProcessing_Thread1(&LobbyControler::SchedulerWorker, &lobbyControler);
	std::thread ItemProcessing_Thread2(&LobbyControler::SchedulerWorker, &lobbyControler);

	{//new scope to delete the mutex at the end
		std::lock_guard<std::mutex> lock(m);
		q.push(std::bind(&ItemProcessing::Init_Vectors, &itemProcessing));
		q.push(std::bind(&ItemProcessing::MedicalProcess, &itemProcessing));
		q.push(std::bind(&ItemProcessing::CaseMatching_Medical, &itemProcessing));
	}
	c_v.notify_all();

	ItemProcessing_Thread1.join();

	ItemProcessing_Thread2.join();


	caseProcessing.CaseOperator();

	pouchProcessing.PouchOperator();

	prefixProcessing.PrefixOperator();

	itemMoving.MovingOperator();
}




void LobbyControler::StashControler() {
	PouchProcessing pouchProcessing(Gamma);//later get from config file or form server 

	OpenStashView();

	if (FirstStart) {
		FirstStartStashMatching();
		FirstStart = false;
	}
	else {
		pouchProcessing.PouchOperator();
	}
}

void LobbyControler::ServerControler() {// Later 


}

void LobbyControler::QueueControler() {// next to do 


}



void LobbyControler::Controler() {
	FirstStartGetData();


}

