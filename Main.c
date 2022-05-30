#include <stdio.h>
#include "Structures.h"
#include "Company.h"
#include "Supplier.h"
#include "Input.h"
#include "Output.h"


int main()
{
	struct Company company;
	struct Supplier supplier;

	while (1)
	{  
		int choice;
		int id;
		char error[51];

		printf("Choose option:\n0 - Quit\n1 - Insert Company\n2 - Get Company\n3 - Update Company\n4 - Delete Company\n5 - Insert Supplier\n6 - Get Supplier\n7 - Update Supplier\n8 - Delete Supplier\n9 - Info\n");
		scanf("%d", &choice);

		switch (choice)
		{
			case 0:
				return 0;

			case 1:
				readCompany(&company);
				insertCompany(company);
				break;
				
			case 2:
				printf("Enter ID: ");
				scanf("%d", &id);
				getCompany(&company, id, error) ? printCompany(company) : printf("Error: %s\n", error);
				break;

			case 3:
				printf("Enter ID: ");
				scanf("%d", &id);

				company.id = id;
				
				readCompany(&company);
				updateCompany(company, error) ? printf("Updated successfully\n") : printf("Error: %s\n", error);
				break;

			case 4:
				printf("Enter ID: ");
				scanf("%d", &id);
				deleteCompany(id, error) ? printf("Deleted successfully\n") : printf("Error: %s\n", error);
				break;

			case 5:
				printf("Enter company\'s ID: ");
				scanf("%d", &id);

				if (getCompany(&company, id, error))
				{
					supplier.companyId = id;
					printf("Enter product ID: ");
					scanf("%d", &id);

					supplier.productId = id;
					readSupplier(&supplier);
					insertSupplier(company, supplier, error);
					printf("Inserted successfully. To access, use company\'s and product\'s IDs\n");
				}
				else
				{
					printf("Error: %s\n", error);
				}
				break;

			case 6:
				printf("Enter company\'s ID: ");
				scanf("%d", &id);

				if (getCompany(&company, id, error))
				{
					printf("Enter product ID: ");
					scanf("%d", &id);
					getSupplier(company, &supplier, id, error) ? printSupplier(supplier, company) : printf("Error: %s\n", error);
				}
				else
				{
					printf("Error: %s\n", error);
				}
				break;

			case 7:
				printf("Enter company\'s ID: ");
				scanf("%d", &id);

				if (getCompany(&company, id, error))
				{
					printf("Enter product ID: ");
					scanf("%d", &id);
					
					if (getSupplier(company, &supplier, id, error))
					{
						readSupplier(&supplier);
						updateSupplier(supplier, id, error);
						printf("Updated successfully\n");
					}
					else
					{
						printf("Error: %s\n", error);
					}
				}
				else
				{
					printf("Error: %s\n", error);
				}
				break;

			case 8:
				printf("Enter company\'s ID: ");
				scanf("%d", &id);

				if (getCompany(&company, id, error))
				{
					printf("Enter product ID: ");
					scanf("%d", &id);

					if (getSupplier(company, &supplier, id, error))
					{
						deleteSupplier(company, supplier, id, error);
						printf("Deleted successfully\n");
					}
					else
					{
						printf("Error: %s\n", error);
					}
				}
				else
				{
					printf("Error: %s\n", error);
				}
				break;

			case 9:
				info();
				break;

			default:
				printf("Invalid input, please try again\n");
		}

		printf("---------\n");
	}

	return 0;
}
