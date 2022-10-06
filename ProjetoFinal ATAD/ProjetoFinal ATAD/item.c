#include "item.h"
#include <stdio.h>
#include <string.h>

Item itemCreate(float avgAgeDistrict, float avgBMIDistrict, float avgGlucoseDistrict, float avgInsulinDistrict, float avgMcp1District) {
	Item it;

	it.avgAgeDistrict = avgAgeDistrict;
	it.avgBMIDistrict = avgBMIDistrict;
	it.avgGlucoseDistrict = avgGlucoseDistrict;
	it.avgInsulinDistrict = avgInsulinDistrict;
	it.avgMcp1District = avgMcp1District;

	return it;
}

void itemPrint(Item *_this) {
	printf("Average Age: %f | Average BMI: %f | Average Glucose: %f | Average Insulin: %f | Average MCP1: %f \n\n", 
		_this->avgAgeDistrict, _this->avgBMIDistrict, _this->avgGlucoseDistrict, _this->avgInsulinDistrict, _this->avgMcp1District);
}