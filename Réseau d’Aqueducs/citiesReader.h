
#ifdef __cplusplus
extern "C"
{
#endif

#pragma warning(disable : 4996)

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct ListOfCities ListOfCities;

struct ListOfCities
{
  int    number;
  char** name;
  int*   pop;
  float* lon;
  float* lat;
};

void loadListOfCities(FILE* inputFile, int popMin, ListOfCities* cities);
ListOfCities* citiesReader(int popMin);
void freeListOfCities(ListOfCities * cities);


#ifdef __cplusplus
}
#endif