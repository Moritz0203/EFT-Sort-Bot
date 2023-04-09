//#include "StartUp.h"
#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <unordered_set>
#include <utility>
#include <functional>
#include "ItemMoving.h"
#include "InitializeMovPrefix.h"
using namespace std;

//external controllers for applications / ECFA
//Rick and Morty Staffel 4 - Folge 4

struct pair_hash {
	template <class T1, class T2>
	size_t operator()(const pair<T1, T2>& p) const {
		return hash<T1>()(p.first) ^ hash<T2>()(p.second);
	}
};

struct SpecsForItem {
	int columns;
	int rows;
};

bool comparePoints(const POINT& a, const POINT& b);
bool Vertical_Horizontal(shared_ptr<vector<vector<POINT>>> ptr_vector, SpecsForItem specsForItem);
void remove_duplicates(std::vector<std::shared_ptr<std::vector<POINT>>>& points, std::shared_ptr<std::vector<int>> values);
bool check_Column(std::vector<std::vector<int>>& input, std::shared_ptr<std::vector<POINT>>& points);
bool Check_for_Space(shared_ptr<vector<vector<POINT>>> ptr_vector, int ItemSize);



class findFreeSlots_test{
	vector<vector<POINT>> FinalResults;
	vector<POINT> Clean_ReturnPoints;

	vector<vector<POINT>> SortINrows(vector<POINT> vector_Input) {
		vector<vector<POINT>> vector_return;
		vector<POINT> temp;
		unordered_set<int> unset;

		for (POINT point : vector_Input) {
			if (unset.find(point.y) == unset.end()) {
				unset.insert(point.y);

				for (POINT pointIN : vector_Input) {

					if (point.y == pointIN.y) {
						temp.push_back(pointIN);
					}
				}

				std::sort(temp.begin(), temp.end(), comparePoints);
				vector_return.push_back(temp);
				temp.clear();
			}
		}

		return vector_return;
	}

	static bool comparePoints(const POINT& a, const POINT& b) {
		return a.x < b.x;
	}


public:
	vector<vector<POINT>> findeSlots(vector<POINT> ReturnPoints) { //parent case must be open to use this function
		//Mat MatScreen;
		//Mat templ = imread("ObjectImages/EmptySquare.png");
		//int freeSlots = {};

		//Mouse::MoverPOINTandPressTwoTimes(case_shared_ptr->point);

		//HWND hWND = FindeWindow();
		//SetForegroundWindow(hWND);
		//Sleep(5);//Delete later
		//MatScreen = getMat(hWND);

		/*vector<POINT> ReturnPoints = TemplateMatching::templateMatchingObjects_Vector(MatScreen, templ, 0.99999);*/

		Clean_ReturnPoints = Matching::removeDuplicates(ReturnPoints);

		return FinalResults = SortINrows(Clean_ReturnPoints);

		/*case_shared_ptr->freeSlots = FinalResults;*/
	}

	void Print_Out_Case_EmptySlots() {
		if (!FinalResults.empty()) {
			for (int i = 0; i < FinalResults.size(); i++) {
				if (FinalResults[i].size() == 0) {
					cout << "       |->";
					cout << " Lines: " << std::to_string(i) << " Total slots: " << Clean_ReturnPoints.size() - 1 << endl;
					break;
				}

				int count = 0;
				int length = 0;
				string str = {};
				string strIE = {};
				string strER = {};

				for (POINT point : FinalResults[i]) {
					string strY = "Y:  " + std::to_string(point.y);
					string strX = "  ---  X:  " + std::to_string(point.x);
					strER = "       |-> ";
					str = strY + strX;

					cout << strER + str;

					length = 30 - str.length();

					for (int i2 = 0; i2 < length; i2++) {
						cout << " ";
					}

					strIE = "Index: " + std::to_string(i) + "  ---  Empty per line: " + std::to_string(++count);
					cout << strIE << "\n";
				}

				cout << "       |";
				for (int i2 = 0; i2 < ((strER.length() - 8) + length + str.length() + strIE.length()); i2++) {
					cout << "-";
				}
				cout << " " << strIE << "\n";
			}
			FinalResults.clear();
		}
		else {
			cout << "You must first call the function: findFreeSlots::findSlots()." << endl;
		}
	}
};



bool Vertical_Horizontal(shared_ptr<vector<vector<POINT>>> ptr_vector, SpecsForItem specsForItem) {
	vector<vector<POINT>> vector_row = (*ptr_vector);
	vector<vector<int>> points_for_LookUp{};
	vector<int> IN_temp_Pairs_for_LookUp{};
	shared_ptr<vector<POINT>> ptr_vector_row{};
	vector<shared_ptr<vector<POINT>>> vector_ptr_vector_row{};
	shared_ptr<vector<int>> ptr_vector_for_clean{};

	for (int i = 0; i < vector_row.size(); i++) {
		for (int i2 = 0; i2 < vector_row[i].size(); i2++) {
			int index = i2 + 1;
			int temp_LookUp = vector_row[i][i2].x + 63;
			bool break_tryNew = false;

			IN_temp_Pairs_for_LookUp.push_back(vector_row[i][i2].x);
			for (int column = 1; column < specsForItem.columns; column) {

				if (temp_LookUp == vector_row[i][index].x) {
					IN_temp_Pairs_for_LookUp.push_back(vector_row[i][index].x);
					index++;
					temp_LookUp += 63;
				} else {
					break_tryNew = true;
					IN_temp_Pairs_for_LookUp.clear();
				}
			}

			if (break_tryNew != true)
				points_for_LookUp.push_back(IN_temp_Pairs_for_LookUp); IN_temp_Pairs_for_LookUp.clear();
		}

		if (points_for_LookUp.size() != 0) {
			bool space_is_free = true;
			int index = i;

			for (int rows = 1; rows < specsForItem.rows; rows++) {
				ptr_vector_row = make_shared<vector<POINT>>(vector_row[++index]);
				vector_ptr_vector_row.push_back(ptr_vector_row);

				if (!check_Column(points_for_LookUp, ptr_vector_row)) {
					space_is_free = false;
					break;
				}
			}

			if (space_is_free) {
				ptr_vector_for_clean = make_shared<vector<int>>(points_for_LookUp[0]);
				remove_duplicates(vector_ptr_vector_row, ptr_vector_for_clean);
				return true;
			} else {
				vector_ptr_vector_row.clear();
			}
			
			points_for_LookUp.clear();
		}
	}

	return false;
}

void remove_duplicates(std::vector<std::shared_ptr<std::vector<POINT>>>& points, std::shared_ptr<std::vector<int>> values) {
	for (auto& vec : points) {
		vec->erase(std::remove_if(vec->begin(), vec->end(),
			[&](const POINT& p) {
				return std::find(values->begin(), values->end(), p.x) != values->end();
			}), vec->end());
	}
}

//bool check_Column_Lambda(std::vector<std::vector<int>>& input, std::shared_ptr<std::vector<POINT>> points) {
//	bool found = false;
//	input.erase(std::remove_if(input.begin(), input.end(), [&](const std::vector<int>& vec) {
//		for (const auto& val : vec) {
//			if (std::find_if(points->begin(), points->end(), [&](const POINT& point) { return point.x == val; }) == points->end()) {
//				return true;
//			}
//		}
//		found = true;
//		return false;
//	}), input.end());
//
//	return found;
//}

bool check_Column(std::vector<std::vector<int>>& input, std::shared_ptr<std::vector<POINT>>& points) {
	bool found = false;
	std::vector<std::vector<int>> result;
	for (const auto& vec : input) {
		bool allMatch = true;
		for (const auto& val : vec) {
			bool match = false;
			for (const auto& point : *points) {
				if (point.x == val) {
					match = true;
					break;
				}
			}
			if (!match) {
				allMatch = false;
				break;
			}
		}
		if (allMatch) {
			found = true;
			result.push_back(vec);
		}
	}
	input = result;
	return found;
}


bool Check_for_Space(shared_ptr<vector<vector<POINT>>> ptr_vector, int ItemSize) {
	//switch (ItemSize)
	//{
	//case 6:
		SpecsForItem SixSlotsVertical(2, 3);
		SpecsForItem SixSlotsHorizontal(3, 2);

		if (Vertical_Horizontal(ptr_vector, SixSlotsVertical)) {
			cout << "es ist genug platz vor handen Vertical" << endl;
			return true; // genug Platz vor handen
		}
		else if (Vertical_Horizontal(ptr_vector, SixSlotsHorizontal)) {
			cout << "es ist genug platz vor handen Horizontal" << endl;
			return true; // genug Platz vor handen
		}
		else {
			//Do something : wenn beides fehlschlägt
			cout << "beides fehlgeschlagen" << endl;
			return false;
		}
	//}
	

}





int main() {
	
	
  /*cout << "//    ___ ___ _____         ___  ___  ___ _____         ___  ___ _____  " << endl;
	cout << "//   | __| __|_   _|  ___  / __|/ _ \| _ \_   _|  ___  | _ )/ _ \_   _| " << endl;
	cout << "//   | _|| _|  | |   |___| \__ \ (_) |   / | |   |___| | _ \ (_) || |   " << endl;
	cout << "//   |___|_|   |_|      _  |___/\___/|_|_\_|_|_ _      |___/\___/ |_|   " << endl;
	cout << "//                     | |__ _  _  | __/ __| __/_\                      " << endl;
	cout << "//                     | '_ \ || | | _| (__| _/ _ \                     " << endl;
	cout << "//                     |_.__/\_, | |___\___|_/_/ \_\                    " << endl;
	cout << "//                           |__/                                       " << endl;*/

	cout << "Welcome to EFT-SORT-BOT" << endl;
	cout << "by ECFA" << endl;
	cout << endl;
		
	
	/*InitializeMovPrefix::Initialize();*/



	/*HWND hWND = FindeWindow();
	SetForegroundWindow(hWND);
	Sleep(100);
	Mat MatScreen = getMat(hWND);*/
	

	string Start = "test";
	

	/*if (Start == "start") {
		StartUp::Entrance();
	}*/
	if (Start == "test") {
		/*ItemMoving::AmmunitionMoving();*/

		Mat templ;
		Mat templ1;

		Mat MatScreen = imread("C:/Users/morit/OneDrive/Desktop/EFT-Sort-Bot/Images/Screenshot_3.png");
		
		/*vector<POINT> ReturnPoints;*/
		//vector<POINT> ReturnPoints;
		/*templ = imread("CaseImages/HolodilnickCase.png");*/
		templ1 = imread("ObjectImages/EmptySquare.png");
		const char* image_window = "Source Image";
		namedWindow(image_window, WINDOW_AUTOSIZE);
		/*const char* image_window2 = "Source Image2222222222";
		namedWindow(image_window2, WINDOW_AUTOSIZE);*/

		//TemplateMatching::templateMatchingItems("ObjectImages/EmptySquare.png", 0.99999, false, false, "BP", ReturnPoints, MatScreen);

		vector<POINT> ReturnPoints = TemplateMatching::templateMatchingObjects_Vector(MatScreen, templ1, 0.99999);

		

		findFreeSlots_test findFreeSlots1;
		vector<vector<POINT>> FinalResult = findFreeSlots1.findeSlots(ReturnPoints);

		findFreeSlots1.Print_Out_Case_EmptySlots();

		shared_ptr<vector<vector<POINT>>> ptr_vector = make_shared<vector<vector<POINT>>>(FinalResult);

		cout << Check_for_Space(ptr_vector, 6) << endl;








		/*Mat MatScreen1 = imread("C:/Users/morit/OneDrive/Desktop/EFT-Sort-Bot/Images/Screenshot_3.png");

		vector<POINT> ReturnPoints1 = TemplateMatching::templateMatchingObjects_Vector(MatScreen1, templ1, 0.99999);


		findFreeSlots1.findeSlots(ReturnPoints1);

		findFreeSlots1.Print_Out_Case_EmptySlots();*/

		/*for (int i = 0; i < ReturnPoints.size(); i++) {
			cout << ReturnPoints[i].y << " " << ReturnPoints[i].x << "----2 - " << i << endl;
		}*/

		
		//cv::Rect rec; 

		//rec.height = templ.cols;
		//rec.width = templ.rows;
		//rec.x = ReturnPoints[0].x;
		//rec.y = ReturnPoints[0].y;

		/*ColorMatching::colorMatching(rec, MatScreen);*/


		//TemplateMatching::templateMatchingItems("CaseImages/AmmoCase.png", 0.90, false, false, "amo", ReturnPoints1, MatScreen);

		///*cout << ReturnPoints[0].y << " " << ReturnPoints[0].x << endl;
		//cout << ReturnPoints1[0].y << " " << ReturnPoints1[0].x << endl;*/

		//POINT pointA{};
		//POINT pointB{};

		//pointB.y = (templ1.rows / 2) + ReturnPoints1[0].y;
		//pointB.x = (templ1.cols / 2) + ReturnPoints1[0].x;



		///*unordered_set<int> unSet;
		//vector<POINT> result;
		//for (POINT point : ReturnPoints) {
		//	int x_minus_1 = point.x - 1;
		//	int x_plus_1 = point.x + 1;
		//	if (unSet.find(point.x) == unSet.end() && unSet.find(x_minus_1) == unSet.end() && unSet.find(x_plus_1) == unSet.end()) {
		//		result.push_back(point);
		//		unSet.insert(point.x);
		//		unSet.insert(x_minus_1);
		//		unSet.insert(x_plus_1);
		//	}
		//}*/


		/*unordered_set<pair<int, int>, pair_hash> unSet;
		vector<POINT> result;
		for (POINT& point : ReturnPoints) {
			int x_minus_1 = point.x - 1;
			int x_plus_1 = point.x + 1;
			int y_minus_1 = point.y - 1;
			int y_plus_1 = point.y + 1;
			pair<int, int> point_x = make_pair(point.x, point.y);
			pair<int, int> point_x_minus_1 = make_pair(x_minus_1, point.y);
			pair<int, int> point_x_plus_1 = make_pair(x_plus_1, point.y);
			pair<int, int> point_y = make_pair(point.x, point.y);
			pair<int, int> point_y_minus_1 = make_pair(point.x, y_minus_1);
			pair<int, int> point_y_plus_1 = make_pair(point.x, y_plus_1);
			if (unSet.find(point_x) == unSet.end() && unSet.find(point_x_minus_1) == unSet.end() && unSet.find(point_x_plus_1) == unSet.end()
				&& unSet.find(point_y) == unSet.end() && unSet.find(point_y_minus_1) == unSet.end() && unSet.find(point_y_plus_1) == unSet.end()) {
				result.push_back(point);
				unSet.insert(point_x);
				unSet.insert(point_x_minus_1);
				unSet.insert(point_x_plus_1);
				unSet.insert(point_y);
				unSet.insert(point_y_minus_1);
				unSet.insert(point_y_plus_1);
			}
		}




		for (int i = 0; i < result.size(); i++) {
			cout << result[i].y << " " << result[i].x << " ---- " << i << endl;
		}

		vector<vector<POINT>> JunkCase;
		vector<POINT> temp;
		unordered_set<int> unset;

		for (POINT point : result) {
			if (unset.find(point.y) == unset.end()) {
				unset.insert(point.y);

				for (POINT pointIN : result) {

					if (point.y == pointIN.y) {
						temp.push_back(pointIN);
					}
				}
				
				std::sort(temp.begin(), temp.end(), comparePoints);
				JunkCase.push_back(temp);
				temp.clear();
			}
			
		}
			
		

		
		for (int i = 0; i < JunkCase.size(); i++) {
			int count = 0;
			int length = 0;
			string str = {};
			string strIE = {};
			string strER = {};
			
			if (JunkCase[i].size() == 0)
				break;

			for (POINT point : JunkCase[i]) {
				string strY = "Y:  " + std::to_string(point.y);
				string strX = "  ---  X:  " + std::to_string(point.x);
				strER = "       |-> ";
				str = strY + strX;
				
				cout << strER + str;

				length = 30 - str.length();
				
				for (int i2 = 0; i2 < length; i2++) {
					cout << " ";
				}

				strIE = "Index: " + std::to_string(i) + "  ---  Empty per line: " + std::to_string(++count);
				cout << strIE << "\n";
			}
			
			cout << "       |";
			for (int i2 = 0; i2 < ((strER.length() - 8) + length + str.length() + strIE.length()); i2++) {
				cout << "-";
			}
			cout << " " << strIE << "\n";
		}
		
		cout << ReturnPoints.size() << endl;*/

		//for (int i = 0; i < result.size(); i++) {
		//	pointA.y = (templ.rows / 2) + result[i].y;
		//	pointA.x = (templ.cols / 2) + result[i].x;

		//	Mouse::MouseMoveAtoB(pointA, pointB);
		//}
		
		//cout << "------" << endl;
		//
		//Rect Rec(ReturnPoints[0].x + 45, ReturnPoints[0].y + 46, templ.cols - 45, templ.rows - 46);
		////Rect Rec(ReturnPoints[0].x , ReturnPoints[0].y , templ.cols, templ.rows );

		//Mat Roi2;
		//Mat Roi = MatScreen(Rec);
	

		/*Mat temp = imread("ObjectImages/FoundInRaid/FoundInRaid-Blue.png");
		if (TemplateMatching::templateMatchingBool(Roi, temp, 0.99)) {
			cout << "found in raid" << endl;
		}*/

		/*Mat blacktext = imread("C:/Users/morit/OneDrive/Desktop/EFT-Sort-Bot/Images/Screenshot_5.png");

		cv::imshow(image_window, blacktext);

		TextMatching::txttest(blacktext);*/

		/*MatScreen = imread("C:/Users/morit/OneDrive/Desktop/EFT-Sort-Bot/Images/Screenshot_5.png");*/

		/*const string str = TextMatching::textMatching(MatScreen, Rec);

		cout << str << endl;*/


		//const char test = '-';
		///*if (str[9] == test) {
		//	cout << "test" << endl;
		//}*/

		//int i1 = -1;
		//for (int i = 0; i < str.length(); i++) {
		//	i1++;
		//	cout << str[i] << " --- " << i1 << endl;
		//}

		//cout << str[str.back() - 1] << "<------ " << i1 << endl;

		/*cout << "123" << endl;*/
		//imshow(image_window, Roi);
		///*imshow(image_window2, Roi2);*/
		//waitKey(0);
		
		
		//cv::cvtColor(Roi, Roi, cv::COLOR_BGR2HSV);

		//if (!ReturnPoints[0].y <= 500) {
		//	/*Roi2 = Roi + cv::Scalar(-17, -17, -17);*/
		//	Roi2 = Roi + cv::Scalar(-20, -20, -20);
		//	cout << "hey" << endl;
		//}

		//int i{}, i2{}, i3{};
		//while (true)
		//{
		//	i--;
		//	i2--;
		//	i3--;
		//	Roi2 = Roi + cv::Scalar(i, i2, i3);	

		//	color = TemplateMatching::ColorMatching(Rec, Roi2);

		//	system("cls");
		//	cout << color << " " << "color " << i << " " << i2 << " " << i3 <<  endl;
		//	if (color == 0)// Green -34 / Blue -64 / 
		//		break;
		//}

		/*color = TemplateMatching::ColorMatching(Rec, Roi);
		
		cout << color << " " << "color" << endl;*/

		/*CaseMatching::THICCcase();*/
	}

	/*for (int i = 0; i < Returner.size(); i++) {
			cout << Returner[i].y << " " << Returner[i].x << endl;
	}*/


	
}

bool comparePoints(const POINT& a, const POINT& b) {
	return a.x < b.x;
}