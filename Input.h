#pragma once
#include <stdio.h>
#include <string.h>
#include "Structures.h"
#include "Company.h"

void readCompany(struct Company* company)
{
	char name[16];
	int status;

	name[0] = '\0';

	printf("Enter company\'s name: ");
	scanf("%s", name);

	strcpy(company->name, name);

	printf("Enter company\'s status: ");
	scanf("%d", &status);

	company->status = status;
}

void readSupplier(struct Supplier* supplier)
{
	int x;

	printf("Enter price: ");
	scanf("%d", &x);

	supplier->price = x;

	printf("Enter amount: ");
	scanf("%d", &x);

	supplier->amount = x;
}