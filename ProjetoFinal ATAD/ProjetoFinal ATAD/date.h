#pragma once

//Defini��o de tipo Date
typedef struct date {
	unsigned int day, month, year;
}Date;

//Defini��o de fun��es
Date dateCreate(char date[]);
