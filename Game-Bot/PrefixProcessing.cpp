#include "PrefixProcessing.h"
#include "MovPrefix.h"

void PrefixProcessing::PrefixOperator() {
	AssignPrefix assingPrefix;
	MovPrefix movPrefix_temp;
	MovPrefix movPrefix;

	for (AssignPrefix prefix : assingPrefix.assignPrefix) {
		bool found = false;
		for (int i = 0; i < PointMedical::pointMedical_C.size(); i++) {
			if(found)
				break;

			for (PointMedical medical : PointMedical::pointMedical_C[i]) {
				if (medical.nameOfItem != prefix.NameOfItem) 
					continue;

				if (!medical.hpItem < prefix.MinHp) 
					continue;

				movPrefix_temp.NameOfItem = medical.nameOfItem;
				movPrefix_temp.IdMov = prefix.IdMov;
				movPrefix_temp.pointCase = nullptr;
				movPrefix_temp.pointItem = std::make_shared<PointMedical>(medical);

				movPrefix.movPrefix.push_back(movPrefix_temp);

				found = true;
				break;
			}
		}
		
		for (int i = 0; i < PointCaseInStashMedical::pointCaseInStashMedical_C.size(); i++) {
			if (found)
				break;

			for (PointCaseInStashMedical pointCaseMedical : PointCaseInStashMedical::pointCaseInStashMedical_C[i]) {
				if (found)
					break;

				for (PointMedical medical : pointCaseMedical.ItemsInCase) {
					if (medical.nameOfItem != prefix.NameOfItem)
						continue;

					if (!medical.hpItem < prefix.MinHp)
						continue;

					movPrefix_temp.NameOfItem = medical.nameOfItem;
					movPrefix_temp.IdMov = prefix.IdMov;
					movPrefix_temp.pointCase = std::make_shared<PointCaseInStashMedical>(pointCaseMedical);
					movPrefix_temp.pointItem = std::make_shared<PointMedical>(medical);

					movPrefix.movPrefix.push_back(movPrefix_temp);

					found = true;
					break;
				}
			}
		}
		// TODO: Add other items
	}
}
