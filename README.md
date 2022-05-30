# DB-and-IS-Lab1
DB imitation

GUIDELINE

0. Legend: "masters" stands for suppliers, "slaves" stand for supplements. The project represents a many-to-many relation between suppliers and product (only represented with abstract IDs, not appearing anywhere in the storage) via indexed binary files.
1. Adding a new record normally pushes it to the end of the database.
2. Deleting the record doesn't remove it physically, just marks it as logically non-existing and available as free space for the oncoming insertions (address is being added to the garbage file). Hence, if you have deleted several records, they'll be overwritten with furtherly inserted ones.
3. Garbage files [should] initially have value 0 stored. It stands for no garbage.
4. To access master records, use IDs. To access slave records, use master's and product's ID.
5. If you're still wondering how to do certain things, try following console output hints :)

P.S. This project does not urge you to slave-trade. All the presented characters are fictitious and do have nothing to do with the real life.

~~~~~~~~~
(c) artandfi 2020
