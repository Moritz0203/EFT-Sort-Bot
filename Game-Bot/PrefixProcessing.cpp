#pragma once	
#include "PrefixProcessing.h"
#include "BuyItemsFlea.h"
#include "InputMK.h"
#include "ItemVectors.h"
#include "LobbyControler.h"
#include "getMat.h"
#include "ItemProcessing.h"
#include "StashObject.h"
#include "c_log.h"

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

		c_log::Info("BuyOperator: " + Prefix.NameOfItem);

		while (buyItemsFlea.BuyItemsFleaOperator(Prefix.NameOfItem.c_str(), Prefix.BuyQuantity, IsMedical) != true);// later with more checks
	}

	buyItemsFlea.CloseFlea(hWND);
	
	lobbyControler.TakeScreenShots();	
	Sleep(10);

	for (AssignPrefix Prefix : BuyPrefix) {
		bool IsMedical = false;	

		movPrefix_temp.NameOfItem = Prefix.NameOfItem;
		movPrefix_temp.BuyQuantity = Prefix.BuyQuantity;
		movPrefix_temp.HowMuchToMove = Prefix.HowMuchToMove;
		movPrefix_temp.MinHp = Prefix.MinHp;
		
		for (PathNameThresholdItemSizeMaxHP ItemName : MedicalVector::Medical) {
			if (ItemName.Name == Prefix.NameOfItem) {
				IsMedical = true;
				break;
			}
		}

		if (IsMedical) {
			vector<vector<PointMedical>> pointMedicalOneItem_C = itemProcessing.OneItemMedicalMatching(Prefix.NameOfItem);

			for (int i = 0; i < pointMedicalOneItem_C.size(); i++) {
				for (PointMedical item : pointMedicalOneItem_C[i]) {
					
					if (item.hpItem < Prefix.MinHp)
						continue;

					movPrefix_temp.pointerStack_vec.push_back({ {}, item });
				}
			}
		}
		else {
			vector<vector<PointBarter>> pointBarterOneItem_C = itemProcessing.OneItemBarterMatching(Prefix.NameOfItem);

			for (int i = 0; i < pointBarterOneItem_C.size(); i++) {
				for (PointBarter item : pointBarterOneItem_C[i]) {
					movPrefix_temp.pointerStack_vec.push_back({ {}, item });
				}
			}
		}

		for(PointerStack& pointerStack : movPrefix_temp.pointerStack_vec)
			c_log::Info("BuyOperator: " , Prefix.NameOfItem , " " , pointerStack.pointItem.nameOfItem , " " , pointerStack.pointItem.page);

		Pouch::pouch.Prefix.push_back(movPrefix_temp);
	}
}




void PrefixProcessing::PrefixOperator() {//build check if item in poch has inove hp to be ther and if not move it out and new in 
	MovPrefix movPrefix;
	vector<AssignPrefix> BuyPrefix{};

	c_log::Start("PrefixOperator");
	
	for (AssignPrefix Prefix : AssignPrefix::assignPrefix_vec) {
		bool found = false;

		if (Prefix.IdMov == 01) {
			movPrefix.NameOfItem = Prefix.NameOfItem;
			movPrefix.BuyQuantity = Prefix.BuyQuantity;
			movPrefix.HowMuchToMove = Prefix.HowMuchToMove;
			movPrefix.MinHp = Prefix.MinHp;

			for (int i = 0; i < PointMedical::pointMedical_C.size(); i++) {

				for (PointMedical item : PointMedical::pointMedical_C[i]) {
					if(item.nameOfItem != Prefix.NameOfItem)
						continue;
					
					if(item.hpItem < Prefix.MinHp) 
						continue;

					//cout << item.nameOfItem << endl;

					movPrefix.pointerStack_vec.push_back({ {}, item });

					found = true;
				}
			}

			for (int i = 0; i < PointCaseInStashMedical::pointCaseInStashMedical_C.size(); i++) {
				
				for (PointCaseInStashMedical Case : PointCaseInStashMedical::pointCaseInStashMedical_C[i]) {
					
					for (PointMedical item : Case.ItemsInCase) {
						
						if(item.nameOfItem != Prefix.NameOfItem)
							continue;

						if (item.hpItem < Prefix.MinHp)
							continue;

						movPrefix.pointerStack_vec.push_back({ Case, item });

						found = true;
					}
				}	
			}

			if (found) {
				Pouch::pouch.Prefix.push_back(movPrefix);
				movPrefix.pointerStack_vec.clear();
			}
		}

		if(!found)
			BuyPrefix.push_back(Prefix);
	}
		
	if(!BuyPrefix.empty())
		BuyOperator(BuyPrefix);

	c_log::End("PrefixOperator");
}