#pragma once

/*Definição do item*/
typedef struct item {
	float avgAgeDistrict; /* media das idades no distrito */
	float avgBMIDistrict; /* media dos valores de IMC no distrito */
	float avgGlucoseDistrict; /* media dos valores de glicose no distrito */
	float avgInsulinDistrict; /* media dos valores de insulina no distrito */
	float avgMcp1District; /* media dos valores da proteina MCP1 no distrito */
} Item;

Item itemCreate(float avgAgeDistrict, float avgBMIDistrict, float avgGlucoseDistrict, float avgInsulinDistrict, float avgMcp1District);
void itemPrint(Item *ptItem);
