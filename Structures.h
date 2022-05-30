#pragma once

struct Company
{
	int id;
	char name[16];
	int status;
	long firstSupplierAddress;
	int suppliersCount;
};

struct Supplier
{
	int companyId;
	int productId;
	int price;
	int amount;
	int exists;
	long selfAddress;
	long nextAddress;
};

struct Indexer
{
	int id;	
	int address;
	int exists;
};