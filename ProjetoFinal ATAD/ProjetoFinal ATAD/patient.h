#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "date.h"

#define DOENCA1 1
#define DOENCA2 2
#define DOENCA3 3
#define SEM_DOENCA 4

typedef struct trainingData {
	float c1, c2, c3, c4;
}TrainingData;

typedef struct clinicalData {
	float age; /* Média da idade em anos quando os dados foram obtidos */
	float bmi; /* Valores médios do Imc- Indice Massa corporal médio (Kg/m2) */
	float glucose; /* Valores médios da Glicose (mg/dl) */
	float insulin; /* Valores médios da Insulina (µU/ml) */
	float mcp1; /* Valores médios da Proteina MCP-1 (pg/ml)*/
	int clinicalDataCount; /* Numero de vezes que os dados foram obtidos */
	int disease_type; /* Classificacao do Doente 1 – Doença1 2 – Doença2 3 – Doença3 4 – Sem Doença Usado apenas na opcao NEURALNET*/
} ClinicalData;

typedef struct patient {
	int id; /*Número que identifica um paciente */
	Date birthdate; /*Data de nascimento*/
	char gender; /*Género (M ou F)*/
	char hospital[256]; /*Nome do hospital de referencia do paciente*/
	char district[256]; /*Distrito onde pertence o hospital*/
	ClinicalData clinicalData; /*Dados clínicos do doente (médias)*/
	TrainingData hiddenInfo;
} Patient;

void printPacientInfo(Patient c);