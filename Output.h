#pragma once

#include <stdio.h>
#include "Company.h"
#include "Structures.h"

void printCompany(struct Company company)
{
	printf("Company\'s name: %s\n", company.name);
	printf("Company\'s status: %d\n", company.status);
}

void printSupplier(struct Supplier supplier, struct Company company)
{
	printf("Company: %s, %d scores\n", company.name, company.status);
	printf("Price: %d\n", supplier.price);
	printf("Amount: %d\n", supplier.amount);
}