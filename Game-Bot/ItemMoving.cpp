#include "ItemMoving.h"
#include "Includes.h"
#include "MovPrefix.h"
#include "TemplateMatching.h"
#include "InputMK.h"
#include "Utility.h"
#include "getMat.h"
#include "PointCase.h"
#include "StashObject.h"
#include "BuyItemsFlea.h"
#include "c_log.h"


void ItemMoving::MovFromStash(PointerStack prefix) {
	checksPublic ChecksPublic;
	const int keyforInput = 0x28;// virtual-key code for the "DOWN ARROW" key

	/*if (prefix.pointItem.page == lastPage) {
		ChecksPublic.ClickScrollbarPositions();
	}
	else if (prefix.pointItem.page < lastPage) {
		ChecksPublic.ClickScrollbarPositions();
		for (int i = lastPage; i < prefix.pointItem.page; i++) {
			Sleep(400);
			Keyboard::KeyboardInput(keyforInput);
		}
	}
	else {*/
		ChecksPublic.CheckScrollbarPositions();
		for (int i = 0; i < prefix.pointItem.page; i++) {
			Sleep(400);
			Keyboard::KeyboardInput(keyforInput);
		}
	//}
	Sleep(100);
	
	POINT point_a {};
	POINT point_b {};

	point_a.x = prefix.pointItem.point.x + (prefix.pointItem.widthTempl / 2);
	point_a.y = prefix.pointItem.point.y + (prefix.pointItem.heightTempl / 2);
	point_b.x = Pouch::pouch.pointPouch.x + 10;
	point_b.y = Pouch::pouch.pointPouch.y + 63;

	lastPage = prefix.pointItem.page;
	Mouse::MouseMoveAtoB(point_a, point_b);
}





void ItemMoving::MovFromCase(PointerStack prefix) {
	checksPublic ChecksPublic;
	const int keyforInput = 0x28;// virtual-key code for the "DOWN ARROW" key

	/*if (prefix.pointCase.page == lastPage) {
		ChecksPublic.ClickScrollbarPositions();
	}
	else if (prefix.pointCase.page > lastPage) {
		ChecksPublic.ClickScrollbarPositions();
		for (int i = lastPage; i < prefix.pointCase.page; i++) {
			Sleep(400);
			Keyboard::KeyboardInput(keyforInput);
		}
	}
	else {*/
		ChecksPublic.CheckScrollbarPositions();
		for (int i = 0; i < prefix.pointCase.page; i++) {
			Sleep(400);
			Keyboard::KeyboardInput(keyforInput);
		}
	//}
	Sleep(100);	

	POINT point_a{};
	POINT point_b{};
	POINT point_case{};

	point_a.x = prefix.pointItem.point.x + (prefix.pointItem.widthTempl / 2);
	point_a.y = prefix.pointItem.point.y + (prefix.pointItem.heightTempl / 2);
	point_b.x = Pouch::pouch.pointPouch.x + 10;
	point_b.y = Pouch::pouch.pointPouch.y + 63;
	point_case.x = prefix.pointCase.point.x + (prefix.pointCase.widthTempl / 2);
	point_case.y = prefix.pointCase.point.y + (prefix.pointCase.heightTempl / 2);

	Mouse::MoverPOINTandPressTwoTimes(point_case);
	Sleep(100);
	Mouse::MouseMoveAtoB(point_a, point_b);

	lastPage = prefix.pointItem.page;
	Keyboard::KeyboardInput(0x1B);// virtual-key code for the "ESC" key
}


void ItemMoving::MovOneItem(MovPrefix& movPrefix) {
	BuyItemsFlea buyItemsFlea;
	uint16_t Mov = movPrefix.HowMuchToMove;
	checksPublic ChecksPublic;
	
	ChecksPublic.CheckScrollbarPositions();

	if (movPrefix.pointerStack_vec.size() == 0) {
		vector<PointItem> vec_Items = buyItemsFlea.EasyBuyItemsAPI(movPrefix.NameOfItem, movPrefix.BuyQuantity);

		for (PointItem item : vec_Items) {
			movPrefix.pointerStack_vec.push_back({ {}, item });
		}
	}

	for (PointerStack& pointerStack : movPrefix.pointerStack_vec) {

		if (Mov == 0)
			break;

		if (pointerStack.pointCase.nameOfCase == "") {
			MovFromStash(pointerStack);// Item is in Stash
			Mov--;
		}
		else {
			MovFromCase(pointerStack);// Item is in Case
			Mov--;
		}
	}
}


void ItemMoving::MovingOperator() {
	BuyItemsFlea buyItemsFlea;
	checksPublic ChecksPublic;

	c_log::Start("MovingOperator");

	for (MovPrefix& prefix : Pouch::pouch.Prefix) {
		cout << "Prefix Name of Item: " << prefix.NameOfItem << endl;
		for (PointerStack& pointerStack : prefix.pointerStack_vec) {
			cout << "| - PointerStack Name of Item: " << pointerStack.pointItem.nameOfItem << " " << pointerStack.pointItem.page << endl;
		}
	}


	ChecksPublic.CheckScrollbarPositions();

	Sleep(100);

	for (MovPrefix& prefix : Pouch::pouch.Prefix) {
		int Mov = prefix.HowMuchToMove;
		
		cout << prefix.NameOfItem << endl;
		cout << "First for: " << Mov << endl;

		if(prefix.pointerStack_vec.size() == 0){
			vector<PointItem> vec_Items = buyItemsFlea.EasyBuyItemsAPI(prefix.NameOfItem, prefix.BuyQuantity);
			
			for (PointItem item : vec_Items) {
				prefix.pointerStack_vec.push_back({ {}, item });
			}
		}

		for (PointerStack& pointerStack : prefix.pointerStack_vec) {
			

			if(Mov == 0)
				break;
			
			cout << "Second for: " << Mov << endl;


			if(pointerStack.pointCase.nameOfCase == "") {
				c_log::Info("MovingOperator: ", pointerStack.pointItem.nameOfItem, " ", pointerStack.pointItem.page);	
				MovFromStash(pointerStack);// Item is in Stash
				Mov--;
			}
			else {
				c_log::Info("MovingOperator: ", pointerStack.pointItem.nameOfItem, " ", pointerStack.pointCase.nameOfCase, " ", pointerStack.pointCase.page);
				MovFromCase(pointerStack);// Item is in Case
				Mov--;
			}
		}
	}

	c_log::End("MovingOperator");
}
