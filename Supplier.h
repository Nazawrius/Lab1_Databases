#pragma once
#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include "Structures.h"
#include "Checks.h"
#include "Company.h"

#define SUPPLIER_DATA "supplier.fl"
#define SUPPLIER_GARBAGE "supplier_garbage.txt"
#define SUPPLIER_SIZE sizeof(struct Supplier)

/*
Functions performing all the needed operations on the suppliers.
*/

void reopenDatabase(FILE* database)
{
	fclose(database);
	database = fopen(SUPPLIER_DATA, "r+b");
}

void linkAddresses(FILE* database, struct Company company, struct Supplier supplier)
{
	reopenDatabase(database);								// Змінюємо режим на "читання з та запис у будь-яке місце"

	struct Supplier previous;

	fseek(database, company.firstSupplierAddress, SEEK_SET);

	for (int i = 0; i < company.suppliersCount; i++)		    // Пробігаємомо зв'язаний список до останньої поставки
	{
		fread(&previous, SUPPLIER_SIZE, 1, database);
		fseek(database, previous.nextAddress, SEEK_SET);
	}

	previous.nextAddress = supplier.selfAddress;				// Зв'язуємо адреси
	fwrite(&previous, SUPPLIER_SIZE, 1, database);				// Заносимо оновлений запис назад до файлу
}

void relinkAddresses(FILE* database, struct Supplier previous, struct Supplier supplier, struct Company* company)
{
	if (supplier.selfAddress == company->firstSupplierAddress)		// Немає попередника (перший запис)...
	{
		if (supplier.selfAddress == supplier.nextAddress)			// ...ще й немає наступника (запис лише один)
		{
			company->firstSupplierAddress = -1;					// Неможлива адреса для безпеки
		}
		else                                                // ...а наступник є,
		{
			company->firstSupplierAddress = supplier.nextAddress;  // робимо його першим
		}
	}
	else                                                    // Попередник є...
	{
		if (supplier.selfAddress == supplier.nextAddress)			// ...але немає наступника (останній запис)
		{
			previous.nextAddress = previous.selfAddress;    // Робимо попередник останнім
		}
		else                                                // ... а разом з ним і наступник
		{
			previous.nextAddress = supplier.nextAddress;		// Робимо наступник наступником попередника
		}

		fseek(database, previous.selfAddress, SEEK_SET);	// Записуємо оновлений попередник
		fwrite(&previous, SUPPLIER_SIZE, 1, database);			// назад до таблички
	}
}

void noteDeletedSupplier(long address)
{
	FILE* garbageZone = fopen(SUPPLIER_GARBAGE, "rb");			// "rb": відкриваємо бінарний файл для читання

	int garbageCount;
	fscanf(garbageZone, "%d", &garbageCount);

	long* delAddresses = malloc(garbageCount * sizeof(long)); // Виділяємо місце під список "сміттєвих" адрес

	for (int i = 0; i < garbageCount; i++)
	{
		fscanf(garbageZone, "%ld", delAddresses + i);		// Заповнюємо його
	}

	fclose(garbageZone);									// За допомогою цих двох команд
	garbageZone = fopen(SUPPLIER_GARBAGE, "wb");				// повністю очищуємо файл зі "сміттям"
	fprintf(garbageZone, "%ld", garbageCount + 1);			// Записуємо нову кількість "сміттєвих" адрес

	for (int i = 0; i < garbageCount; i++)
	{
		fprintf(garbageZone, " %ld", delAddresses[i]);		// Заносимо "сміттєві" адреси назад...
	}

	fprintf(garbageZone, " %d", address);					// ...і дописуємо до них адресу щойно видаленого запису
	free(delAddresses);										// Звільняємо виділену під масив пам'ять
	fclose(garbageZone);									// Закриваємо файл
}

void overwriteGarbageAddress(int garbageCount, FILE* garbageZone, struct Supplier* record)
{
	long* delIds = malloc(garbageCount * sizeof(long));		// Виділяємо місце під список "сміттєвих" адрес

	for (int i = 0; i < garbageCount; i++)
	{
		fscanf(garbageZone, "%d", delIds + i);				// Заповнюємо його
	}

	record->selfAddress = delIds[0];						// Для запису замість логічно видаленої "сміттєвої"
	record->nextAddress = delIds[0];

	fclose(garbageZone);									// За допомогою цих двох команд
	fopen(SUPPLIER_GARBAGE, "wb");							    // повністю очищуємо файл зі "сміттям"
	fprintf(garbageZone, "%d", garbageCount - 1);			// Записуємо нову кількість "сміттєвих" адрес

	for (int i = 1; i < garbageCount; i++)
	{
		fprintf(garbageZone, " %d", delIds[i]);				// Записуємо решту "сміттєвих" адрес
	}

	free(delIds);											// Звільняємо виділену під масив пам'ять
	fclose(garbageZone);									// Закриваємо файл
}

int insertSupplier(struct Company company, struct Supplier supplier, char* error)
{
	supplier.exists = 1;

	FILE* database = fopen(SUPPLIER_DATA, "a+b");
	FILE* garbageZone = fopen(SUPPLIER_GARBAGE, "rb");

	int garbageCount;

	fscanf(garbageZone, "%d", &garbageCount);

	if (garbageCount)											// Наявні видалені записи
	{
		overwriteGarbageAddress(garbageCount, garbageZone, &supplier);
		reopenDatabase(database);								// Змінюємо режим доступу файлу
		fseek(database, supplier.selfAddress, SEEK_SET);			// Ставимо курсор на "сміття" для подальшого перезапису
	}
	else                                                        // Видалених немає, пишемо в кінець файлу
	{
		fseek(database, 0, SEEK_END);

		int dbSize = ftell(database);

		supplier.selfAddress = dbSize;
		supplier.nextAddress = dbSize;
	}

	fwrite(&supplier, SUPPLIER_SIZE, 1, database);					// Записуємо поставку до свого файлу

	if (!company.suppliersCount)								    // Поставок ще немає, пишемо адресу першої
	{
		company.firstSupplierAddress = supplier.selfAddress;
	}
	else                                                        // Поставки вже є, оновлюємо "адресу наступника" останньої
	{
		linkAddresses(database, company, supplier);
	}

	fclose(database);											// Закриваємо файл

	company.suppliersCount++;										// Стало на одну поставку більше
	updateCompany(company, error);								// Оновлюємо запис постачальника

	return 1;
}

int getSupplier(struct Company company, struct Supplier* supplier, int productId, char* error)
{
	if (!company.suppliersCount)									// У постачальника немає поставок
	{
		strcpy(error, "This company has no suppliers yet");
		return 0;
	}

	FILE* database = fopen(SUPPLIER_DATA, "rb");


	fseek(database, company.firstSupplierAddress, SEEK_SET);		// Отримуємо перший запис
	fread(supplier, SUPPLIER_SIZE, 1, database);

	for (int i = 0; i < company.suppliersCount; i++)				// Шукаємо потрібний запис по коду деталі
	{
		if (supplier->productId == productId)						// Знайшли
		{
			fclose(database);
			return 1;
		}

		fseek(database, supplier->nextAddress, SEEK_SET);
		fread(supplier, SUPPLIER_SIZE, 1, database);
	}

	strcpy(error, "No such supplier in database");					// Не знайшли
	fclose(database);
	return 0;
}

// На вхід подається поставка з оновленими значеннями, яку треба записати у файл
int updateSupplier(struct Supplier supplier, int productId, char* error)
{
	FILE* database = fopen(SUPPLIER_DATA, "r+b");

	fseek(database, supplier.selfAddress, SEEK_SET);
	fwrite(&supplier, SUPPLIER_SIZE, 1, database);
	fclose(database);

	return 1;
}

int deleteSupplier(struct Company company, struct Supplier supplier, int productId, char* error)
{
	FILE* database = fopen(SUPPLIER_DATA, "r+b");
	struct Supplier previous;

	fseek(database, company.firstSupplierAddress, SEEK_SET);

	do		                                                    // Шукаємо попередника запису (його може й не бути,
	{															// тоді в попередника занесеться сам запис)
		fread(&previous, SUPPLIER_SIZE, 1, database);
		fseek(database, previous.nextAddress, SEEK_SET);
	} while (previous.nextAddress != supplier.selfAddress && supplier.selfAddress != company.firstSupplierAddress);

	relinkAddresses(database, previous, supplier, &company);
	noteDeletedSupplier(supplier.selfAddress);						// Заносимо адресу видаленого запису у "смітник"

	supplier.exists = 0;											// Логічно не існуватиме

	fseek(database, supplier.selfAddress, SEEK_SET);				// ...але фізично
	fwrite(&supplier, SUPPLIER_SIZE, 1, database);					// записуємо назад
	fclose(database);

	company.suppliersCount--;										// Однією поставкою менше
	updateCompany(company, error);

	return 1;
}