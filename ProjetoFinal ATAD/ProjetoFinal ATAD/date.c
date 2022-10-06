#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "date.h"
#include "split.h"

#include <stdio.h>


Date dateCreate(char date[]) {
	unsigned int day, month, year;
	char **tokens = split(date, 3, "/");
	day = atoi(tokens[0]);
	month = atoi(tokens[1]);
	year = atoi(tokens[2]);
	Date tempDate = { day, month, year };
	free(tokens);
	return tempDate;
}