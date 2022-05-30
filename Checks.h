#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "Supplier.h"

int checkFileExistence(FILE* indexTable, FILE* database, char* error)
{
	// DB files do not exist yet
	if (indexTable == NULL || database == NULL)
	{
		strcpy(error, "database files are not created yet");
		return 0;
	}

	return 1;
}

int checkIndexExistence(FILE* indexTable, char* error, int id)
{
	fseek(indexTable, 0, SEEK_END);

	long indexTableSize = ftell(indexTable);

	if (indexTableSize == 0 || id * sizeof(struct Indexer) > indexTableSize)
	{
		strcpy(error, "no such ID in table");
		return 0;
	}

	return 1;
}

int checkRecordExistence(struct Indexer indexer, char* error)
{
	// Record's been removed
	if (!indexer.exists)
	{
		strcpy(error, "the record you\'re looking for has been removed");
		return 0;
	}

	return 1;
}

void info()
{
	FILE* indexTable = fopen("company.ind", "rb");

	if (indexTable == NULL)
	{
		printf("Error: database files are not created yet\n");
		return;
	}

	int companyCount = 0;
	int supplierCount = 0;

	fseek(indexTable, 0, SEEK_END);
	int indAmount = ftell(indexTable) / sizeof(struct Indexer);

	struct Company company;

	char dummy[51];

	for (int i = 1; i <= indAmount; i++)
	{
		if (getCompany(&company, i, dummy))
		{
			companyCount++;
			supplierCount += company.suppliersCount;

			printf("Company #%d has %d supplier(s)\n", i, company.suppliersCount);
		}
	}

	fclose(indexTable);

	printf("Total companies: %d\n", companyCount);
	printf("Total suppliers: %d\n", supplierCount);
}
