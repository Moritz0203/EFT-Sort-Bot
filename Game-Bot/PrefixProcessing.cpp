#pragma once	
#include "PrefixProcessing.h"
#include "BuyItemsFlea.h"
#include "InputMK.h"
#include "ItemVectors.h"
#include "LobbyControler.h"
#include "getMat.h"
#include "ItemProcessing.h"
#include "StashObject.h"

void PrefixProcessing::BuyOperator(vector<AssignPrefix> BuyPrefix) {
	BuyItemsFlea buyItemsFlea(true);
	LobbyControler lobbyControler;
	MovPrefix movPrefix_temp;
	ItemProcessing itemProcessing;
	GetMat getMat;
	const HWND hWND = GetMat::FindeWindow();



	buyItemsFlea.OpenFlea(hWND);

	for (AssignPrefix Prefix : BuyPrefix) {
		bool IsMedical = false;

		for (PathNameThresholdItemSizeMaxHP ItemName : MedicalVector::Medical) {
			if (ItemName.Name == Prefix.NameOfItem) {
				IsMedical = true;
				break;
			}
		}

		while (buyItemsFlea.BuyItemsFleaOperator(Prefix.NameOfItem.c_str(), Prefix.BuyQuantity, IsMedical) != true);// later with more checks



	}

	buyItemsFlea.CloseFlea(hWND);
	


	//lobbyControler.TakeScreenShots();	
	//Sleep(10);

	/*if (IsMedical) {
		vector<vector<PointMedical>> pointMedicalOneItem_C = itemProcessing.OneItemMedicalMatching(Prefix.NameOfItem);

		for (int i = 0; i < pointMedicalOneItem_C.size(); i++) {
			for (PointMedical item : pointMedicalOneItem_C[i]) {
				movPrefix_temp.NameOfItem = item.nameOfItem;
				movPrefix_temp.IdMov = Prefix.IdMov;
				movPrefix_temp.BuyQuantity = Prefix.BuyQuantity;
				movPrefix_temp.pointCase = nullptr;
				movPrefix_temp.pointItem = std::make_shared<PointMedical>(item);

				movPrefix.movPrefix.push_back(movPrefix_temp); 
			}
		}
	}
	else {
		vector<vector<PointBarter>> pointBarterOneItem_C = itemProcessing.OneItemBarterMatching(Prefix.NameOfItem);

		for (int i = 0; i < pointBarterOneItem_C.size(); i++) {
			for (PointBarter item : pointBarterOneItem_C[i]) {
				movPrefix_temp.NameOfItem = item.nameOfItem;
				movPrefix_temp.IdMov = Prefix.IdMov;
				movPrefix_temp.BuyQuantity = Prefix.BuyQuantity;
				movPrefix_temp.pointCase = nullptr;
				movPrefix_temp.pointItem = std::make_shared<PointBarter>(item);

				movPrefix.movPrefix.push_back(movPrefix_temp);
			}
		}
	}*/
}




void PrefixProcessing::PrefixOperator() {//build check if item in poch has inove hp to be ther and if not move it out and new in 
	MovPrefix movPrefix;
	vector<AssignPrefix> BuyPrefix{};
	
	for (AssignPrefix Prefix : AssignPrefix::assignPrefix_vec) {

		if (Prefix.IdMov == 01) {
			movPrefix.NameOfItem = Prefix.NameOfItem;
			movPrefix.BuyQuantity = Prefix.BuyQuantity;

			for (int i = 0; i < PointMedical::pointMedical_C.size(); i++) {

				for (PointMedical item : PointMedical::pointMedical_C[i]) {
					if(item.nameOfItem != Prefix.NameOfItem)
						continue;
					
					if(item.hpItem < Prefix.MinHp) 
						continue;

					
					movPrefix.pointerStack_vec.push_back({ nullptr, std::make_shared<PointMedical>(item) });
				}
			}

			for (int i = 0; i < PointCaseInStashMedical::pointCaseInStashMedical_C.size(); i++) {
				
				for (PointCaseInStashMedical Case : PointCaseInStashMedical::pointCaseInStashMedical_C[i]) {
					
					for (PointMedical item : Case.ItemsInCase) {
						
						if(item.nameOfItem != Prefix.NameOfItem)
							continue;

						if (item.hpItem < Prefix.MinHp)
							continue;

						movPrefix.pointerStack_vec.push_back({ std::make_shared<PointCase>(Case), std::make_shared<PointMedical>(item) });
					}
				}	
			}

			Pouch::pouch.Prefix.push_back(movPrefix);
		}



		BuyPrefix.push_back(Prefix);
	}
		

	BuyOperator(BuyPrefix);
}