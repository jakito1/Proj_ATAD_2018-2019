#pragma once

//Definição de tipo Date
typedef struct date {
	unsigned int day, month, year;
}Date;

//Definição de funções
Date dateCreate(char date[]);
