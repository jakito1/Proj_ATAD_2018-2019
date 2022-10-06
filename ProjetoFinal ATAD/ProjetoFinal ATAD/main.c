/* PROJETO  ATAD 2018-19
* Identificacao dos Alunos:
*
*      Numero: 180221015 | Nome: Francisco Moura
*      Numero: 180221026 | Nome: António Pinto
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <io.h>
#include <time.h>
#include "list.h"
#include "queue.h"
#include "map.h"
#include "split.h"
#include "patient.h"

/* definicao de prototipos de funcoes, definidas depois do main() */
int equalsStringIgnoreCase(char str1[], char str2[]);
void printCommandsMenu();
int calculateAge(Date startingDate, Date endingDate);
void importData(char *filename, PtList patients, int *counter);
int getIdIndex(PtList patients, int id);
PtList cloneList(PtList patients);
void getMinimumMaximum(PtList patients, float *ageMin, float *ageMax, float *bmiMin, float *bmiMax,
	float *gluMin, float *gluMax, float *insMin, float *insMax, float *mcpMin, float *mcpMax);
PtList normalizeClinicalData(PtList patients);
void switchPatients(PtList patients, Patient *tempPatient1, Patient *tempPatient2, int firstPos, int secondPos);
PtList sortByBirthDate(PtList patients);
PtList sortByHospital(PtList patients);
PtList sortByDistrict(PtList patients);
PtQueue getQueue(PtList patients);
PtMap averageClinicalData(PtList patients);

/*
* Este programa irá fazer um diagnostico de saude com base em dados previamente obtidos.
*/
int main(int argc, char** argv) {

	/* declaracao de variaveis */
	PtList normalizedPatientList = NULL, normalizedTrainedPatientList = NULL, trainedPatientList = listCreate(1000), patientList = listCreate(1000);
	String patientsFile, clinicalDataFile;


	/* interpretador de comandos */
	String command;
	int quit = 0;

	setlocale(LC_ALL, "PT");
	while (!quit) {
		
		printCommandsMenu();
		fgets(command, sizeof(command), stdin);
		/* descartar 'newline'. Utilizar esta técnica sempre que for lida uma
		* string para ser utilizada, e.g., nome de ficheiro, chave, etc.. */
		command[strlen(command) - 1] = '\0';

		if (equalsStringIgnoreCase(command, "QUIT")) {
			quit = 1; /* vai provocar a saída do interpretador */	
		}
		
		/*Carregar os dados não treinados, caso um dos ficheiros não exista, dará erro e sairá para o menu*/
		else if (equalsStringIgnoreCase(command, "LOAD")) {
			printf("Nome do Ficheiro dos Pacientes> ");
			fgets(patientsFile, sizeof(patientsFile), stdin);
			patientsFile[strlen(patientsFile) - 1] = '\0';

			/*Verificar se o ficheiro existe*/
			if ((_access(patientsFile, 0)) != -1) {
				printf("Nome do Ficheiro com os dados Clinicos dos Pacientes> ");
				fgets(clinicalDataFile, sizeof(clinicalDataFile), stdin);
				clinicalDataFile[strlen(clinicalDataFile) - 1] = '\0';
				if ((_access(clinicalDataFile, 0)) != -1) {
					int numberPatients, numberClinicalData;
					importData(patientsFile, patientList, &numberPatients);
					importData(clinicalDataFile, patientList, &numberClinicalData);
					Patient patientCheck;
					listGet(patientList, 0, &patientCheck);

					/*Verificar se ocorreu algum erro, como por exemplo inserir os ficheiros pela ordem errada ou tentar inserir os 
					ficheiros com dados treinados. Caso seja verificado um erro, a lista é recriada apagando todos os dados.*/
					if ((patientCheck.hiddenInfo.c1 != 0 && patientCheck.hiddenInfo.c2 != 0 &&
						patientCheck.hiddenInfo.c3 != 0 && patientCheck.hiddenInfo.c4 != 0) 
						|| numberPatients == 0 || numberClinicalData == 0) {
						listDestroy(&patientList);
						patientList = listCreate(1000);
						numberPatients = 0, numberClinicalData = 0;
					}					
					printf("Foram lidos %d pacientes e informação sobre %d dados clinicos.\n", numberPatients, numberClinicalData);
				}
				else printf("Erro: Ficheiro Inexistente.\n");
			}
			else printf("Erro: Ficheiro Inexistente.\n");
		}

		/*Limpar o conteudo da lista não treinada*/
		else if (equalsStringIgnoreCase(command, "CLEAR")) {
			int size;
			listSize(patientList, &size);
			if (size == 0) {
				printf("SEM DADOS CARREGADOS.\n");
			}
			else {
				listClear(patientList);
				printf("Foram apagados %d registos de pacientes.\n", size);
			}
		}

		/*Imprimir toda a informação presente na lista não treinada*/
		else if (equalsStringIgnoreCase(command, "SHOW")) {
			if (listIsEmpty(patientList)) {
				printf("SEM DADOS CARREGADOS.\n");
			}
			else {
				listPrint(patientList);
			}
		}

		/*Ordenar todos os pacientes por um dos 3 critérios existentes: "data de nascimento", 
		"hospital" e "distrito"*/
		else if (equalsStringIgnoreCase(command, "SORT")) {
			if (listIsEmpty(patientList)) {
				printf("SEM DADOS CARREGADOS.\n");
			}
			else {
				PtList ordered = NULL;
				printf("Critério de Ordenação > ");
				String input;
				fgets(input, sizeof(input), stdin);
				input[strlen(input) - 1] = '\0';
				if (equalsStringIgnoreCase(input, "DATA DE NASCIMENTO")) {
					ordered = sortByBirthDate(patientList);
					listPrint(ordered);
				}else if (equalsStringIgnoreCase(input, "HOSPITAL")) {
					ordered = sortByHospital(patientList);
					listPrint(ordered);
				}
				else if (equalsStringIgnoreCase(input, "DISTRITO")) {
					ordered = sortByDistrict(patientList);
					listPrint(ordered);
				} else printf("Opção não encontrada.\n");
				listDestroy(&ordered);
			}
		}

		/*Mostra a média dos dados clinicos de cada paciente 
		(age, bmi, glucose, insulin e mcp1) em cada distrito, e ordenados por distrito. */
		else if (equalsStringIgnoreCase(command, "AVG")) {
			if (listIsEmpty(patientList)) {
				printf("SEM DADOS CARREGADOS.\n");
			}
			else {
				PtList sorted = sortByDistrict(patientList);
				PtMap tempMap = averageClinicalData(sorted);
				mapPrint(tempMap);
				listDestroy(&sorted);
				mapDestroy(&tempMap);
			}
		}

		/*Mostra, para cada paciente, os seus dados clínicos normalizados 
		entre -k e k, segundo a normalização min-max.*/
		else if (equalsStringIgnoreCase(command, "NORM")) {
			if (listIsEmpty(patientList)) {
				printf("SEM DADOS CARREGADOS.\n");
			}
			else {
				normalizedPatientList = normalizeClinicalData(patientList);
				listPrint(normalizedPatientList);
				listDestroy(&normalizedPatientList);
			}
		}

		/*Copiar os pacientes que cumpram os requisitos especificados em getQueue(PtList patients) para
		uma instancia de uma fila*/
		else if (equalsStringIgnoreCase(command, "QUEUE")) {
			if (listIsEmpty(patientList)) {
				printf("SEM DADOS CARREGADOS.\n");
			}
			else {
				int stop = 0;
				PtQueue tempQueue = getQueue(patientList);
				while (!stop) {
					printf("Comando da Fila> ");
					fgets(command, sizeof(command), stdin);
					command[strlen(command) - 1] = '\0';
					if (equalsStringIgnoreCase(command, "NEXT")) {
						if (queueIsEmpty(tempQueue)) {
							printf("A fila encontra-se vazia.\n");
						}
						else {
							Patient tempPacient;
							queueDequeue(tempQueue, &tempPacient);
							printPacientInfo(tempPacient);
						}
					}
					else if (equalsStringIgnoreCase(command, "QUIT")) {
						queueDestroy(&tempQueue);
						stop = 1;
					}
					else printf("Comando não encontrado.\n");
				}
			}
		}

		/*Segmenta os pacientes de acordo com o seu distrito de residência (fornecido pelo utilizador)*/
		else if (equalsStringIgnoreCase(command, "CHECKDISTRICT")) {
			if (listIsEmpty(patientList)) {
				printf("SEM DADOS CARREGADOS.\n");
			}
			else {
				int stop = 0;
				PtList sorted = sortByDistrict(patientList);
				PtMap tempMap = averageClinicalData(sorted);
				while (!stop) {
					printf("Nome do Distrito> ");
					fgets(command, sizeof(command), stdin);
					command[strlen(command) - 1] = '\0';
					StringKey tempKey = stringCodeCreate(command);
					if (mapContains(tempMap, tempKey)) {
						Item tempItem;
						mapGet(tempMap, tempKey, &tempItem);
						mapKeyPrint(tempKey);
						printf(" : ");
						mapValuePrint(tempItem);
					}
					else if (equalsStringIgnoreCase(command, "")) {
						listDestroy(&sorted);
						mapDestroy(&tempMap);
						stop = 1;
					}
					else printf("Distrito não encontrado.\n");
				}
			}
		}

		/*Carregar os dados treinados, caso um dos ficheiros não exista, dará erro e sairá para o menu.
		Funciona exatamente da mesma maneira que o método "LOAD"*/
		else if (equalsStringIgnoreCase(command, "LOADT")) {		
			printf("Nome do Ficheiro dos Pacientes> ");
			fgets(patientsFile, sizeof(patientsFile), stdin);
			patientsFile[strlen(patientsFile) - 1] = '\0';
			if ((_access(patientsFile, 0)) != -1) {
					printf("Nome do Ficheiro com os dados Clinicos dos Pacientes> ");
					fgets(clinicalDataFile, sizeof(clinicalDataFile), stdin);
					clinicalDataFile[strlen(clinicalDataFile) - 1] = '\0';
					if ((_access(clinicalDataFile, 0)) != -1) {
						int numberPatients, numberClinicalData;
						importData(patientsFile, trainedPatientList, &numberPatients);
						importData(clinicalDataFile, trainedPatientList, &numberClinicalData);
						Patient patientCheck;
						listGet(trainedPatientList, 0, &patientCheck);
						if ((patientCheck.hiddenInfo.c1 == 0 && patientCheck.hiddenInfo.c2 == 0 &&
							patientCheck.hiddenInfo.c3 == 0 && patientCheck.hiddenInfo.c4 == 0)
							|| numberPatients == 0 || numberClinicalData == 0) {
							listDestroy(&trainedPatientList);
							trainedPatientList = listCreate(1000);
							numberPatients = 0, numberClinicalData = 0;
						}
						printf("Foram lidos %d pacientes e informação sobre %d dados clinicos.\n", numberPatients, numberClinicalData);
					}
				else printf("Erro: Ficheiro Inexistente.\n");
			}
			else printf("Erro: Ficheiro Inexistente.\n");
		}

		/*Mostra, para cada paciente, os seus dados clínicos treinados normalizados
		entre -5 e 5, segundo a normalização min-max.*/
		else if (equalsStringIgnoreCase(command, "NORMT")) {
			if (listIsEmpty(trainedPatientList)) {
				printf("SEM DADOS CARREGADOS.\n");
			}
			else {
				normalizedTrainedPatientList = normalizeClinicalData(trainedPatientList);
				listPrint(normalizedTrainedPatientList);
			}
		}
		else if (equalsStringIgnoreCase(command, "NEURALNET")) {
			printf("Comando NEURALNET nao implementado.\n");
		}
		else {
			printf("Comando não encontrado.\n");
		}
	}

	/*Limpeza da memória*/
	listDestroy(&normalizedPatientList);
	listDestroy(&normalizedTrainedPatientList);
	listDestroy(&trainedPatientList);
	listDestroy(&patientList);
	printf("Adeus!\n");

	return (EXIT_SUCCESS);
}

/*Compara duas strings de forma case insensitive
Devolve 0 caso sejam iguais.*/
int equalsStringIgnoreCase(char str1[], char str2[]) {
	return (_stricmp(str1, str2) == 0);
}

void printCommandsMenu() {
	printf("\n===================================================================================");
	printf("\n                          PROJECT: Patients and Clinical Data                      ");
	printf("\n===================================================================================");
	printf("\nA. Info about patients (LOAD, SHOW, CLEAR).");
	printf("\nB. Aggregated info about patients and clinical data (SORT, AVG, NORM, QUEUE, CHECKDISTRICT).");
	printf("\nC. Advanced indicator (LOADT, NORMT, NEURALNET)");
	printf("\nD. Exit (QUIT)\n\n");
	printf("COMMAND> ");
}

/*Calcula e devolve os anos entre duas datas.*/
int calculateAge(Date startingDate, Date endingDate) {
	int cmp = endingDate.month - startingDate.month;
	int age = endingDate.year - startingDate.year;

	if (cmp > 0) {
		return age;
	}
	else if (cmp < 0) {
		return age - 1;
	}
	else { 
		if (startingDate.day > endingDate.day) {
			return age - 1;
		}
		else {
			return age;
		}
	}
}

/*Função responsável por ler os ficheiros e carregar a informação*/
void importData(char *filename, PtList patients, int *counter) {
	FILE *f;
	int error = fopen_s(&f, filename, "r");

	if (error) {
		printf("An error ocurred...It was not possible to open the file %s...\n", filename);
		return;
	}

	char nextLine[1024];
	*counter = 0;

	while (fgets(nextLine, sizeof(nextLine), f)) {
		if (strlen(nextLine) < 1)
			continue;
		
		int i, count, tempId;
		for (i = 0, count = 1; nextLine[i]; i++)
			count += (nextLine[i] == ';');


		char **tokens = split(nextLine, count, ";");
		if (count == 5) {
			Patient tempPatient;
			tempPatient.id = atoi(tokens[0]);
			tempPatient.birthdate = dateCreate(tokens[1]);
			tempPatient.gender = *tokens[2];
			strcpy_s(tempPatient.hospital, sizeof(tempPatient.hospital), tokens[3]);
			strcpy_s(tempPatient.district, sizeof(tempPatient.district), tokens[4]);
			int size, location = getIdIndex(patients, tempPatient.id);
			listSize(patients, &size);

			/*Iniciar os dados ainda não carregados para prevenir comportamentos inadequados*/
			tempPatient.clinicalData.clinicalDataCount = 0;
			tempPatient.clinicalData.age = 0;
			tempPatient.clinicalData.bmi = 0;
			tempPatient.clinicalData.glucose = 0;
			tempPatient.clinicalData.insulin = 0;
			tempPatient.clinicalData.mcp1 = 0;
			tempPatient.clinicalData.disease_type = 0;
			tempPatient.hiddenInfo.c1 = 0;
			tempPatient.hiddenInfo.c2 = 0;
			tempPatient.hiddenInfo.c3 = 0;
			tempPatient.hiddenInfo.c4 = 0;

			/*Caso o paciente ainda não exista na lista este será colocado na ultima posição
			do arraylist de forma a poupar tempo de execução. Caso já exista, este será substituido pelo novo paciente com o mesmo id.*/
			if (location == -1) {
				location = size;
				listAdd(patients, location, tempPatient);
			}
			else {
				Patient delete;
				listSet(patients, location, tempPatient, &delete);
			}
			*counter += 1;
		}

		/*O count será igual a 6 se forem os dados não treinados e igual a 11 se forem os treinados.*/
		else if (count == 6 || count == 11) {
			tempId = atoi(tokens[0]);
			int size, location = getIdIndex(patients, tempId);
			listSize(patients, &size);
			if (location != -1) {
				Patient tempPatient, old;
				listGet(patients, location, &tempPatient);
				Date tempDate = dateCreate(tokens[1]);
				float age = calculateAge(tempPatient.birthdate, tempDate);
				float bmi = atof(tokens[2]);
				float glucose = atof(tokens[3]);
				float insulin = atof(tokens[4]);
				float mcp1 = atof(tokens[5]);
				int tempClinicalDataCount = ++tempPatient.clinicalData.clinicalDataCount;
				tempPatient.clinicalData.age = (float)(tempPatient.clinicalData.age * (tempClinicalDataCount - 1) + age) / tempClinicalDataCount;
				tempPatient.clinicalData.bmi = (float)(tempPatient.clinicalData.bmi * (tempClinicalDataCount - 1) + bmi) / tempClinicalDataCount;
				tempPatient.clinicalData.glucose = (float)(tempPatient.clinicalData.glucose * (tempClinicalDataCount - 1) + glucose) / tempClinicalDataCount;
				tempPatient.clinicalData.insulin = (float)(tempPatient.clinicalData.insulin * (tempClinicalDataCount - 1) + insulin) / tempClinicalDataCount;
				tempPatient.clinicalData.mcp1 = (float)(tempPatient.clinicalData.mcp1 * (tempClinicalDataCount - 1) + mcp1) / tempClinicalDataCount;
				if (count == 11) {
					tempPatient.clinicalData.disease_type = atoi(tokens[6]);
					tempPatient.hiddenInfo.c1 = atof(tokens[7]);
					tempPatient.hiddenInfo.c2 = atof(tokens[8]);
					tempPatient.hiddenInfo.c3 = atof(tokens[9]);
					tempPatient.hiddenInfo.c4 = atof(tokens[10]);
				}
				listSet(patients, location, tempPatient, &old);
				*counter += 1;
			}
		}

		free(tokens);
	}
	fclose(f);
}

/*Descobrir o rank de um elemento na lista dos pacientes.
Devolve o rank caso exista ou -1 caso não exista.*/
int getIdIndex(PtList patients, int id) {
	int size, i;
	listSize(patients, &size);
	for (i = 0; i < size; ++i) {
		Patient temp;
		listGet(patients, i, &temp);
		if (temp.id == id) return i;
	}
	return -1;
}

/*Clonar uma lista, incluindo todos os elementos dentro dela.
Devolve a referência da nova lista.*/
PtList cloneList(PtList patients) {
	int size;
	listSize(patients, &size);
	PtList returnList = listCreate(size);
	for (int i = 0; i < size; i++) {
		Patient tempPatient;
		listGet(patients, i, &tempPatient);
		listAdd(returnList, i, tempPatient);
	}
	return returnList;
}

/*Procurar pelos valores máximos e minimos dentro de todos os dados clinicos de todos os pacientes*/
void getMinimumMaximum(PtList patients, float *ageMin, float *ageMax, float *bmiMin, float *bmiMax,
	float *gluMin, float *gluMax, float *insMin, float *insMax, float *mcpMin, float *mcpMax) {

	int size;
	listSize(patients, &size);
	Patient tempPatient;
	listGet(patients, 0, &tempPatient);

	*ageMin = tempPatient.clinicalData.age,
		*ageMax = tempPatient.clinicalData.age,
		*bmiMin = tempPatient.clinicalData.bmi,
		*bmiMax = tempPatient.clinicalData.bmi,
		*gluMin = tempPatient.clinicalData.glucose,
		*gluMax = tempPatient.clinicalData.glucose,
		*insMin = tempPatient.clinicalData.insulin,
		*insMax = tempPatient.clinicalData.insulin,
		*mcpMin = tempPatient.clinicalData.mcp1,
		*mcpMax = tempPatient.clinicalData.mcp1;

	for (unsigned int i = 0; i < size; i++) {
		listGet(patients, i, &tempPatient);

		if (tempPatient.clinicalData.age < *ageMin)
			*ageMin = tempPatient.clinicalData.age;
		if (tempPatient.clinicalData.age > *ageMax)
			*ageMax = tempPatient.clinicalData.age;

		if (tempPatient.clinicalData.bmi < *bmiMin)
			*bmiMin = tempPatient.clinicalData.bmi;
		if (tempPatient.clinicalData.bmi > *bmiMax)
			*bmiMax = tempPatient.clinicalData.bmi;

		if (tempPatient.clinicalData.glucose < *gluMin)
			*gluMin = tempPatient.clinicalData.glucose;
		if (tempPatient.clinicalData.glucose > *gluMax)
			*gluMax = tempPatient.clinicalData.glucose;

		if (tempPatient.clinicalData.insulin < *insMin)
			*insMin = tempPatient.clinicalData.insulin;
		if (tempPatient.clinicalData.insulin > *insMax)
			*insMax = tempPatient.clinicalData.insulin;

		if (tempPatient.clinicalData.mcp1 < *mcpMin)
			*mcpMin = tempPatient.clinicalData.mcp1;
		if (tempPatient.clinicalData.mcp1 > *mcpMax)
			*mcpMax = tempPatient.clinicalData.mcp1;
	}
}

/*Função que normaliza os dados clinicos dos pacientes, dado um k fornecido pelo utilizador.
Devolve a referencia para uma lista com os dados dos pacientes normalizados*/
PtList normalizeClinicalData(PtList patients) {
	PtList clonedList = cloneList(patients);
	int size;
	float norm;
	listSize(clonedList, &size);
	Patient tempPatient, old;
	listGet(clonedList, 0, &tempPatient);

	/*Verificar se se tratam dos dados treinados. Se sim, então o k será igual a 5*/
	if (tempPatient.hiddenInfo.c1 != 0 && tempPatient.hiddenInfo.c2 != 0 &&
		tempPatient.hiddenInfo.c3 != 0 && tempPatient.hiddenInfo.c4 != 0) {
		norm = 5;
	}
	else {
		printf("\n\n [-value, value] \n\nK = ? > ");
		String input;
		fgets(input, sizeof(input), stdin);
		input[strlen(input) - 1] = '\0';
		norm = atof(input);
	}

	float ageMin, ageMax, bmiMin, bmiMax,
		gluMin, gluMax, insMin, insMax, mcpMin, mcpMax;

	getMinimumMaximum(clonedList, &ageMin, &ageMax, &bmiMin, &bmiMax, &gluMin, &gluMax, &insMin,
		&insMax, &mcpMin, &mcpMax);

	for (unsigned int i = 0; i < size; i++) {
		listGet(clonedList, i, &tempPatient);
		if (ageMax - ageMin != 0) tempPatient.clinicalData.age = ((tempPatient.clinicalData.age - ageMin) / (ageMax - ageMin)) * (2 * norm) - norm;
		if (bmiMax - bmiMin != 0) tempPatient.clinicalData.bmi = ((tempPatient.clinicalData.bmi - bmiMin) / (bmiMax - bmiMin)) * (2 * norm) - norm;
		if (gluMax - gluMin != 0) tempPatient.clinicalData.glucose = ((tempPatient.clinicalData.glucose - gluMin) / (gluMax - gluMin)) * (2 * norm) - norm;
		if (insMax - insMin != 0) tempPatient.clinicalData.insulin = ((tempPatient.clinicalData.insulin - insMin) / (insMax - insMin)) * (2 * norm) - norm;
		if (mcpMax - mcpMin != 0) tempPatient.clinicalData.mcp1 = ((tempPatient.clinicalData.mcp1 - mcpMin) / (mcpMax - mcpMin)) * (2 * norm) - norm;
		
		listSet(clonedList, i, tempPatient, &old);
	}
	return clonedList;
}

/*Dadas as referencias de dois pacientes, trocar as suas posições dentro do arrayList*/
void switchPatients(PtList patients, Patient *tempPatient1, Patient *tempPatient2, int firstPos, int secondPos) {
	Patient old;
	listSet(patients, firstPos, *tempPatient1, &*tempPatient2);
	listSet(patients, secondPos, *tempPatient2, &old);
}

/*Função responsável por ordenar os pacientes de acordo com a data de nascimento
Devolve a referência para uma lista com os pacientes ordenados*/
PtList sortByBirthDate(PtList patients) {
	PtList clonedList = cloneList(patients);
	int size;
	listSize(clonedList, &size);
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size - 1; ++j) {
			Patient tempPatient1, tempPatient2, old;
			listGet(clonedList, j, &tempPatient1);
			listGet(clonedList, j + 1, &tempPatient2);
			Date tempDate1 = tempPatient1.birthdate;
			Date tempDate2 = tempPatient2.birthdate;
			if (tempDate1.year > tempDate2.year || (tempDate1.year == tempDate2.year && tempDate1.month > tempDate2.month) ||
				(tempDate1.year == tempDate2.year && tempDate1.month == tempDate2.month && tempDate1.day > tempDate2.day)) {
				switchPatients(clonedList, &tempPatient1, &tempPatient2, j + 1, j);
			}
		}
	}
	return clonedList;
}

/*Função responsável por ordenar os pacientes de acordo com o hospital alfabeticamente, caso seja o mesmo hospital, usa a data de nascimento como desampate
Devolve a referência para uma lista com os pacientes ordenados*/
PtList sortByHospital(PtList patients) {
	PtList clonedList = cloneList(patients);
	int size;
	listSize(clonedList, &size);
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size - 1; ++j) {
			Patient tempPatient1, tempPatient2, old;
			listGet(clonedList, j, &tempPatient1);
			listGet(clonedList, j + 1, &tempPatient2);
			int comp = strcmp(tempPatient1.hospital, tempPatient2.hospital);
			if (comp > 0) {
				switchPatients(clonedList, &tempPatient1, &tempPatient2, j + 1, j);
			}
			else if (comp == 0) {
				Date tempDate1 = tempPatient1.birthdate;
				Date tempDate2 = tempPatient2.birthdate;
				if (tempDate1.year > tempDate2.year || (tempDate1.year == tempDate2.year && tempDate1.month > tempDate2.month) ||
					(tempDate1.year == tempDate2.year && tempDate1.month == tempDate2.month && tempDate1.day > tempDate2.day)) {
					switchPatients(clonedList, &tempPatient1, &tempPatient2, j + 1, j);
				}
			}
		}
	}
	return clonedList;
}

/*Função responsável por ordenar os pacientes de acordo com o distrito alfabeticamente, caso seja o mesmo distrito, 
usa o nome do hospital como desempate e caso esse empate, usará a data de nascimento como desampate
Devolve a referência para uma lista com os pacientes ordenados*/
PtList sortByDistrict(PtList patients) {
	PtList clonedList = cloneList(patients);
	int size;
	listSize(clonedList, &size);
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size - 1; ++j) {
			Patient tempPatient1, tempPatient2, old;
			listGet(clonedList, j, &tempPatient1);
			listGet(clonedList, j + 1, &tempPatient2);
			int comp = strcmp(tempPatient1.district, tempPatient2.district);
			if (comp > 0) {
				switchPatients(clonedList, &tempPatient1, &tempPatient2, j + 1, j);
			}
			else if (comp == 0) {
				comp = strcmp(tempPatient1.hospital, tempPatient2.hospital);
				if (comp > 0) {
					switchPatients(clonedList, &tempPatient1, &tempPatient2, j + 1, j);
				}
				else if (comp == 0) {
					Date tempDate1 = tempPatient1.birthdate;
					Date tempDate2 = tempPatient2.birthdate;
					if (tempDate1.year > tempDate2.year || (tempDate1.year == tempDate2.year && tempDate1.month > tempDate2.month) ||
						(tempDate1.year == tempDate2.year && tempDate1.month == tempDate2.month && tempDate1.day > tempDate2.day)) {
						switchPatients(clonedList, &tempPatient1, &tempPatient2, j + 1, j);
					}
				}
			}
		}
	}
	return clonedList;
}

/*Função que copiará os pacientes que respeitarem as condições impostas em baixo para uma instancia de uma lista
Devolve um ponteiro para a lista em questão*/
PtQueue getQueue(PtList patients) {
	int size = 0, i = 0, age = 0;
	listSize(patients, &size);
	Date curDate;

	/*Atribuir data atual a uma estrutura e criar uma "Date" com essa informação. 
	Usa funções não exclusivas do windows para uma maior compatibilidade com outros SO*/
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	curDate.day = newtime.tm_mday;
	curDate.month = newtime.tm_mon + 1;
	curDate.year = newtime.tm_year + 1900;
	
	PtQueue tempQueue = queueCreate(size);

	float ageMin, ageMax, bmiMin, bmiMax,
		gluMin, gluMax, insMin, insMax, mcpMin, mcpMax;

	getMinimumMaximum(patients, &ageMin, &ageMax, &bmiMin, &bmiMax, &gluMin, &gluMax, &insMin,
		&insMax, &mcpMin, &mcpMax);

	/*Descobrir a idade da pessoa mais velha e mais nova da lista usando o dia atual como data final*/
	for (i = 0; i < size; i++) {
		Patient tempPatient;
		listGet(patients, i, &tempPatient);
		age = calculateAge(tempPatient.birthdate, curDate);

		if (age < ageMin)
			ageMin = age;
		if (age > ageMax)
			ageMax = age;
	}

	for (i = 0; i < size; i++) {
		Patient tempPatient;
		listGet(patients, i, &tempPatient);
		age = calculateAge(tempPatient.birthdate, curDate);

		if (((ageMin + ageMax) / 2) > age) {
			queueEnqueue(tempQueue, tempPatient);
		}
		else if (((ageMin + ageMax) / 2) < age && ((bmiMin + bmiMax) / 2) < tempPatient.clinicalData.bmi &&
			((gluMin + gluMax) / 2) < tempPatient.clinicalData.glucose && ((insMin + insMax) / 2) < tempPatient.clinicalData.insulin &&
			((mcpMin + mcpMax) / 2) < tempPatient.clinicalData.mcp1) {
			queueEnqueue(tempQueue, tempPatient);
		}
	}

	return tempQueue;
}

/*Função que tratará os dados dos pacientes e criará um mapa com as médias dos dados clinicos de todos os pacientes
residentes nesse distrito usando o distrito como chave
Devolve um ponteiro para o mapa com os dados organizados*/
PtMap averageClinicalData(PtList patients) {
	PtMap tempMap = mapCreate(100);
	Patient tempPatient;
	listGet(patients, 0, &tempPatient);
	StringKey tempStringKey = stringCodeCreate(tempPatient.district);
	Item tempItem = itemCreate(tempPatient.clinicalData.age, tempPatient.clinicalData.bmi, tempPatient.clinicalData.glucose,
		tempPatient.clinicalData.insulin, tempPatient.clinicalData.mcp1);
	int size = 0, i = 1, quantity = 1;
	listSize(patients, &size);
	for (i; i < size; i++) {
		listGet(patients, i, &tempPatient);
		if (strcmp(tempStringKey.code, tempPatient.district) == 0) {
			++quantity;
			tempItem.avgAgeDistrict = (tempItem.avgAgeDistrict * (quantity - 1) + tempPatient.clinicalData.age) / quantity;
			tempItem.avgBMIDistrict = (tempItem.avgBMIDistrict * (quantity - 1) + tempPatient.clinicalData.bmi) / quantity;
			tempItem.avgGlucoseDistrict = (tempItem.avgGlucoseDistrict * (quantity - 1) + tempPatient.clinicalData.glucose) / quantity;
			tempItem.avgInsulinDistrict = (tempItem.avgInsulinDistrict * (quantity - 1) + tempPatient.clinicalData.insulin) / quantity;
			tempItem.avgMcp1District = (tempItem.avgMcp1District * (quantity - 1) + tempPatient.clinicalData.mcp1) / quantity;
		}
		else {
			tempStringKey.code[strlen(tempStringKey.code) - 1] = '\0'; //função que retira o \n do fim da string
			mapPut(tempMap, tempStringKey, tempItem);
			quantity = 1;
			listGet(patients, i, &tempPatient);
			tempStringKey = stringCodeCreate(tempPatient.district);
			tempItem = itemCreate(tempPatient.clinicalData.age, tempPatient.clinicalData.bmi, tempPatient.clinicalData.glucose,
				tempPatient.clinicalData.insulin, tempPatient.clinicalData.mcp1);
		}
	}
	tempStringKey.code[strlen(tempStringKey.code) - 1] = '\0';
	mapPut(tempMap, tempStringKey, tempItem);
	return tempMap;
}