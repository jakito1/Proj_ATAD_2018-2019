#include <stdio.h>
#include <string.h>
#include "patient.h"

void printPacientInfo(Patient c) {
	printf(" ID: %d |Birth Date: %02d/%02d/%04d | Gender: %c | Hospital: %s | District: %s", c.id, c.birthdate.day, c.birthdate.month, c.birthdate.year, c.gender, c.hospital, c.district);
	printf("               Age: %.5f | BMI: %.5f | Glucose: %.5f | Insulin: %.5f | MCP1: %.5f \n",
		c.clinicalData.age, c.clinicalData.bmi, c.clinicalData.glucose, c.clinicalData.insulin, c.clinicalData.mcp1);
	/*if (c.hiddenInfo.c1 != 0 && c.hiddenInfo.c2 != 0 &&
		c.hiddenInfo.c3 != 0 && c.hiddenInfo.c4 != 0) {
		printf("               Disease Type: %d | C1: %.5f | C2: %.5f | C3: %.5f | C4: %.5f\n",
			c.clinicalData.disease_type, c.hiddenInfo.c1, c.hiddenInfo.c2, c.hiddenInfo.c3, c.hiddenInfo.c4);
	}*/
}