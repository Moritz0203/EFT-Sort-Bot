#include "TemplateMatching.h"
#include "globalvector.h"
#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
using namespace std;

namespace Matching {

	bool checkSecondLastChar(const string tagCase) {
		int length = tagCase.length();
		if (length >= 2) {
			return (tagCase[static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(length) - 2] != '-');
		}
		return false;
	}

	std::array<std::string, 12> Ammunition{
		//NATO 7.62
			"itemImages/AmmunitionImages/7.62NATO/M80.png",//M80
			"itemImages/AmmunitionImages/7.62NATO/M62.png",//M62
			"itemImages/AmmunitionImages/7.62NATO/M61.png",//M61
			"itemImages/AmmunitionImages/7.62NATO/M993.png",//M993
			"itemImages/AmmunitionImages/7.62NATO/BCPFMJ.png",//BCPFMJ
			"itemImages/AmmunitionImages/7.62NATO/TCWSP.png",//TCWSP
			"itemImages/AmmunitionImages/7.62NATO/UltraNosi.png",//UltraNosi
		//RUS 7.62
			"itemImages/AmmunitionImages/7.62RUS/BP.png",//BP
			"itemImages/AmmunitionImages/7.62RUS/HP.png",//HP
			//"itemImages/AmmunitionImages/7.62RUS/MAIAP.png",//MAIAP
			"itemImages/AmmunitionImages/7.62RUS/PS.png",//PS
			"itemImages/AmmunitionImages/7.62RUS/T45M1.png",//T45M1
			"itemImages/AmmunitionImages/7.62RUS/US.png",//US
	};

	std::array<std::string, 12> NameOfItemAmmunition{
		//NATO 7.62
			"M80",
			"M62",
			"M61",
			"M993",
			"BCPFMJ",
			"TCWSP",
			"UltraNosi",
		//RUS 7.62
			"BP",
			"HP",
			"PS",
			"T45M1",
			"US",
	};

	std::array<double, 12> AmmunitionThreshold{
		//NATO 7.62
			0.90,//M80
			0.88,//M62
			0.90,//M61
			0.90,//M993
			0.84,//BCPFMJ
			0.86,//TCWSP
			0.88,//UltraNosi
		//RUS 7.62
			0.88,//BP
			0.90,//HP
			//0.84,//MAIAP
			0.90,//PS
			0.90,//T45M1
			0.90,//US
	};

	void AmmunitionMatching(array<Mat, 11> arrayMatScreen) {
		int sizeString = sizeof(Ammunition) / sizeof(string);
		int sizeMat = sizeof(arrayMatScreen) / sizeof(Mat);
		Mat templ;

		vector<POINT> ReturnDataAM;
		vector<PointAmmunition> pointAmmunitionTemp;
		for (int i1 = 0; i1 < sizeMat; i1++) {
			for (int i = 0; i < sizeString; i++) {
				TemplateMatching::templateMatchingItems(Ammunition[i], AmmunitionThreshold[i], false, true, NameOfItemAmmunition[i], ReturnDataAM, arrayMatScreen[i1]);

				templ = imread(Ammunition[i]);
				if (!ReturnDataAM.empty()) {
					for (int i2 = 0; i2 < ReturnDataAM.size(); i2++) {
						Rect Rec(ReturnDataAM[i2].x + 44, ReturnDataAM[i2].y + 48, templ.cols - 44, templ.rows - 48);
						const string tagCase = TextMatching::textMatching(arrayMatScreen[i1], Rec);
						int tagCaseConvertet = stoi(tagCase);
						pointAmmunitionTemp.emplace_back(ReturnDataAM[i2], NameOfItemAmmunition[i], tagCaseConvertet, templ.rows, templ.cols, i1);
					}
					ReturnDataAM.clear();
				}
			}
			pointAmmunition_NC.emplace_back(pointAmmunitionTemp);
			pointAmmunitionTemp.clear();
		}
	}


	std::array<std::string, 10> Cases{
		"CaseImages/AmmoCase.png",
		"CaseImages/GrenadCase.png",
		"CaseImages/HolodilnickCase.png",
		"CaseImages/MagCase.png",
		"CaseImages/MedCase.png",
		"CaseImages/MoneyCase.png",
		"CaseImages/JunkCase.png",
		"CaseImages/WeaponsCase.png",
		"CaseImages/ItemsCase.png",
		"CaseImages/THICCcase.png",
	};

	std::array<std::string, 10> NameOfItemCases{
		"AmmoCase",
		"GrenadCase",
		"HolodilnickCase",
		"MagCase",
		"MedCase",
		"MoneyCase",
		"JunkCase",
		"WeaponsCase",
		"ItemsCase",
		"THICCcase",
	};

	std::array<double, 10> CasesThreshold{
		0.90,//AmmoCase
		0.90,//GrenadCase
		0.90,//HolodilnickCase
		0.90,//MagCase
		0.90,//MedCase
		0.90,//MoneyCase
		0.90,//JunkCase
		0.90,//WeaponsCase
		0.90,//ItemsCase
		0.90,//THICCcase
	};

	void CaseMatching(array<Mat, 11> arrayMatScreen) {
		int sizeString = sizeof(Cases) / sizeof(string);
		int sizeMat = sizeof(arrayMatScreen) / sizeof(Mat);
		Mat templ;

		vector<POINT> ReturnDataCase;
		vector<PointCaseInStash> pointCasetempStashTemp;
		for (int i1 = 0; i1 < sizeMat; i1++) {
			for (int i = 0; i < sizeString; i++) {
				TemplateMatching::templateMatchingItems(Cases[i], CasesThreshold[i], false, false, NameOfItemCases[i], ReturnDataCase, arrayMatScreen[i1]);
				
				templ = imread(Cases[i]);
				if (!ReturnDataCase.empty()) {
					for (int i3 = 0; i3 < ReturnDataCase.size(); i3++) {
						Rect Rec(ReturnDataCase[i3].x, ReturnDataCase[i3].y, 13, templ.rows);
						const string tagCase = TextMatching::textMatching(arrayMatScreen[i1], Rec);
						if (checkSecondLastChar(tagCase)) {
							pointCasetempStashTemp.emplace_back(ReturnDataCase[i3], NameOfItemCases[i], tagCase, templ.rows, templ.cols, i);
						}
					}
					ReturnDataCase.clear();
				}
			}
			pointCaseInStash_NC.emplace_back(pointCasetempStashTemp);
			pointCasetempStashTemp.clear();
		}
	}










	std::array<std::string, 8> Magazine{
		//MP
			"itemImages/MagazineImgas/4.6HK/MP7.png",
		//NATO
			"itemImages/MagazineImgas/5.56NATO/MAG5-60.png",
			"itemImages/MagazineImgas/5.56NATO/PMAG-D60.png",
			"itemImages/MagazineImgas/7.62NARO/PMAG.png",
			"itemImages/MagazineImgas/7.62NARO/SLR.png",
		//RUS
			"itemImages/MagazineImgas/7.62RRUS/SVD.png",
			"itemImages/MagazineImgas/7.62RUS/AK30.png",
			"itemImages/MagazineImgas/7.62RUS/GEN-M3.png",

	};

	std::array<std::string, 8> NameOfItemMagazine{
		//MP
			"MP7",
		//NATO
			"MAG5-60",
			"PMAG-D60",
			"PMAG",
			"SLR",
		//RUS
			"SVD",
			"AK30",
			"GEN-M3",
	}; 

	std::array<double, 8> MagazineThreshold{
		//MP
			0.85,//MP7
		//NATO
			0.83,//MAG5-60
			0.83,//PMAG-D60
			0.83,//PMAG
			0.85,//SLR
		//RUS
			0.87,//SVD
			0.87,//AK30
			0.87,//GEN-M3
	};

	vector<POINT> MagazineMatching() {
		Mat Screen;
		string  filename, templatename;
		double	threshold;
		int size = sizeof(Magazine) / sizeof(string);

		vector<POINT> ReturnDataMA;

		for (int i = 0; i < size; i++) {
			threshold = MagazineThreshold[i];
			templatename = Magazine[i];

			TemplateMatching::templateMatchingItems(templatename, threshold, true, false, NameOfItemMagazine[i], ReturnDataMA, Screen);
		}
		return ReturnDataMA;
	}



	std::array<std::string, 7> Barter{
		//OneSlot
			"itemImages/BarterImages/OneSlot/Bolts.png",
			"itemImages/BarterImages/OneSlot/Bulb.png",
			"itemImages/BarterImages/OneSlot/Defibrillator.png",
			"itemImages/BarterImages/OneSlot/LEDX.png",
			"itemImages/BarterImages/OneSlot/SDiary.png",
		//TwoSlot
			"itemImages/BarterImages/TwoSlot/Diary.png",
		//SixSlot
			"itemImages/BarterImages/SixSlot/Lion.png"
	};

	std::array<std::string, 7> NameOfItemBarter{
		//OneSlot
			"Bolts",
			"Bulb",
			"Defibrillator",
			"LEDX",
			"SDiary",
		//TwoSlot
			"Diary",
		//SixSlot
			"Lion",
	};

	std::array<double, 7> BarterThreshold{
		//OneSlot
			0.86,//Bolts
			0.92,//Bulb
			0.95,//Defibrillator
			0.88,//LEDX
			0.86,//SDiary
		//TwoSlot
			0.88,//Diary
		//SixSlot
			0.96,//Lion
	};

	vector<POINT> BarterMatching() {
		Mat Screen;
		string  filename, templatename;
		double	threshold;
		int size = sizeof(Barter) / sizeof(string);

		vector<POINT> ReturnDataBA;

		for (int i = 0; i < size; i++) {
			threshold = BarterThreshold[i];
			templatename = Barter[i];

			TemplateMatching::templateMatchingItems(templatename, threshold, false, true, NameOfItemBarter[i], ReturnDataBA, Screen);
		}
		return ReturnDataBA;
	}
}