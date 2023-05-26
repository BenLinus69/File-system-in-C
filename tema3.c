/*BLENDEA ANDREI-IONUT grupa 312CB*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "structs.h"


int main()
{
	Director* root = malloc(sizeof(Director));
	Director* currentD;//pointer catre directorul curent
	root->nume = malloc(5 * sizeof(char));
	strcpy(root->nume, "root");
	root->parinte = NULL;
	root->directories = NULL;
	root->dr = NULL;
	root->st = NULL;
	root->files = NULL;

	currentD = root;
	int max = 256;
	char* line = (char*)malloc(sizeof(char) * max);


	while (fgets(line, 256, stdin))
	{
		char* tip_actiune = strtok(line, " ");
		if (tip_actiune[strlen(tip_actiune) - 1] == '\n')//elimin \n
		{
			tip_actiune[strlen(tip_actiune) - 1] = '\0';
		}
		if (strcmp(tip_actiune, "touch") == 0)
		{
			char* nume_fisier = strtok(NULL, " ");
			if (nume_fisier[strlen(nume_fisier) - 1] == '\n')//elimin \n
			{
				nume_fisier[strlen(nume_fisier) - 1] = '\0';
			}
			currentD->files = insertF(currentD->files, currentD, nume_fisier, stdout);

		}
		if (strcmp(tip_actiune, "mkdir") == 0)
		{
			char* nume_director = strtok(NULL, " ");
			if (nume_director[strlen(nume_director) - 1] == '\n')//elimin \n
			{
				nume_director[strlen(nume_director) - 1] = '\0';
			}
			currentD->directories = insertD(currentD->directories, currentD, nume_director, stdout);

		}
		if (strcmp(tip_actiune, "ls") == 0)
		{
			printareD(currentD->directories, stdout);//printez dir si fisierele
			printareF(currentD->files, stdout);
			fprintf(stdout, "\n");
		}
		if (strcmp(tip_actiune, "rmdir") == 0)
		{
			char* nume_director_d = strtok(NULL, " ");
			if (nume_director_d[strlen(nume_director_d) - 1] == '\n')//elimin \n
			{
				nume_director_d[strlen(nume_director_d) - 1] = '\0';
			}
			int ok = 0;
			currentD->directories = stergere_nod_dir(currentD->directories, nume_director_d, &ok);
			if (ok == 0)
			{
				fprintf(stdout, "Directory %s doesn't exist!\n", nume_director_d);
			}
		}
		if (strcmp(tip_actiune, "rm") == 0)
		{
			char* nume_fisier_d = strtok(NULL, " ");
			if (nume_fisier_d[strlen(nume_fisier_d) - 1] == '\n')//elimin \n
			{
				nume_fisier_d[strlen(nume_fisier_d) - 1] = '\0';
			}
			int ok = 0;
			currentD->files = stergere_nod_f(currentD->files, nume_fisier_d, &ok);
			if (ok == 0)
			{
				fprintf(stdout, "File %s doesn't exist!\n", nume_fisier_d);
			}
		}
		if (strcmp(tip_actiune, "cd") == 0)
		{
			char* nume_dir = strtok(NULL, " ");
			int ok = 0;
			if (nume_dir[strlen(nume_dir) - 1] == '\n')//elimin \n
			{
				nume_dir[strlen(nume_dir) - 1] = '\0';
			}
			Director* copie;//declar un pointer care sa pointeze la currentD
			copie = currentD;
			currentD = changeDir(currentD->directories, currentD, nume_dir, &ok);
			if (ok == 0)//daca currentD pointeaza spre NULL,ii dau inapoi adresa din copie
			{
				currentD = copie;
				fprintf(stdout, "Directory not found!\n");
			}
			
		}
		if (strcmp(tip_actiune, "pwd") == 0)
		{
			PrintWorkingDir(currentD, stdout);
			fprintf(stdout, "\n");
		}
		if (strcmp(tip_actiune, "find") == 0)
		{
			char* tip_cautare = strtok(NULL, " ");
			if (strcmp(tip_cautare, "-d") == 0)
			{
				char* nume_director = strtok(NULL, " ");
				if (nume_director[strlen(nume_director) - 1] == '\n')//elimin \n
				{
					nume_director[strlen(nume_director) - 1] = '\0';
				}
				int ok = 0;
				int found = 0;
				find_dir(root->directories, nume_director, &ok, &found, stdout);
				if (found == 0)
				{
					fprintf(stdout, "Directory %s not found!\n", nume_director);
				}
			}
			if (strcmp(tip_cautare, "-f") == 0)
			{
				char* nume_fisier = strtok(NULL, " ");
				if (nume_fisier[strlen(nume_fisier) - 1] == '\n')//elimin \n
				{
					nume_fisier[strlen(nume_fisier) - 1] = '\0';
				}
				int ok = 0;
				int found = 0;
				find_file(root->directories, nume_fisier, &ok, &found, stdout);
				if (found == 0)
				{
					fprintf(stdout, "File %s not found!\n", nume_fisier);
				}


			}
		}
		if (strcmp(tip_actiune, "quit") == 0)
		{
			free(line);
			memory_free(root);//dezaloc intregul arbore
		}

	}

	return 0;
}
