/*BLENDEA ANDREI-IONUT grupa 312CB*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "structs.h"

Fisier* constrF(char* name, Director* parinte)
{
	Fisier* temp = (Fisier*)malloc(sizeof(Fisier));//aloc noul fisier
	temp->nume = malloc(strlen(name) * sizeof(name));//aloc spatiu pentru nume
	strcpy(temp->nume, name);//copiez numele in fisier
	temp->st = temp->dr = NULL;
	temp->parinte = parinte;//setez ca parinte directorul de care apartine fisierul
	return temp;
}

Fisier* insertF(Fisier* fisier_root, Director* parinte, char* nume, FILE* Output)
{

	if (Check_Dir(parinte->directories, nume))//daca exista un director cu acelasi nume,afisez eroare
	{

		fprintf(Output, "Directory %s already exists!\n", nume);
		return fisier_root;
	}
	if (fisier_root == NULL)//cand ajung la pozitia potrivita,inserez nodul
	{
		return constrF(nume, parinte);
	}

	if (strcmp(fisier_root->nume, nume) == 0)//daca exista o fila cu acelasi nume,afisez eroare
	{
		fprintf(Output, "File %s already exists!\n", nume);
		return fisier_root;
	}

	else if (strcmp(fisier_root->nume, nume) > 0)//parcurg sortat arborele
		fisier_root->st = insertF(fisier_root->st, parinte, nume, Output);
	else if (strcmp(fisier_root->nume, nume) < 0)
		fisier_root->dr = insertF(fisier_root->dr, parinte, nume, Output);

	return fisier_root;
}

Director* constrD(char* name, Director* parinte)
{
	Director* temp = (Director*)malloc(sizeof(Director));//aloc directorul - analog constrF
	temp->nume = malloc(strlen(name) * sizeof(name));
	strcpy(temp->nume, name);
	temp->st = temp->dr = NULL;
	temp->directories = NULL;
	temp->files = NULL;
	temp->parinte = parinte;
	return temp;
}

Director* insertD(Director* director_root, Director* parinte, char* nume, FILE* Output)
{

	if (director_root == NULL)//daca am ajuns la pozitia potrivita,introduc directorul
		return constrD(nume, parinte);

	if (strcmp(director_root->nume, nume) == 0)
	{
		fprintf(Output, "Directory %s already exists!\n", nume);
		return director_root;
	}
	else if (Check_Files(parinte->files, nume))
	{
		fprintf(Output, "File %s already exists!\n", nume);
	}

	else if (strcmp(director_root->nume, nume) > 0)
		director_root->st = insertD(director_root->st, parinte, nume, Output);
	else if (strcmp(director_root->nume, nume) < 0)
		director_root->dr = insertD(director_root->dr, parinte, nume, Output);

	return director_root;
}
int Check_Files(Fisier* root, char* nume)//verifica daca exista fisiere cu numele directrului
{
	if (root == NULL)
	{
		return 0;
	}
	else if (strcmp(root->nume, nume) > 0)
	{
		return Check_Files(root->st, nume);
	}
	else if (strcmp(root->nume, nume) < 0)
	{
		return Check_Files(root->dr, nume);
	}
	else if (strcmp(root->nume, nume) == 0)
	{
		return 1;
	}
	return 0;
}

int Check_Dir(Director* root, char* nume)//verifica daca exista directoare cu numele fisierului
{

	if (root == NULL)
	{
		return 0;//returnez 0 daca nu a gasit niciun director cu acelasi nume
	}
	else if (strcmp(root->nume, nume) > 0)
	{
		return Check_Dir(root->st, nume);
	}
	else if (strcmp(root->nume, nume) < 0)
	{
		return Check_Dir(root->dr, nume);
	}
	else if (strcmp(root->nume, nume) == 0)
	{
		return 1;//returnez 1 daca gaseste un director cu acelasi nume
	}
	return 0;
}
void printareF(Fisier* root, FILE* Output)
{
	if (root != NULL) {//printez arborele de fisiere ordonat
		printareF(root->st, Output);
		fprintf(Output, "%s ", root->nume);
		printareF(root->dr, Output);
	}
}

void printareD(Director* root, FILE* Output)
{
	if (root != NULL) {//afisez arborele de directoare ordonat
		printareD(root->st, Output);
		fprintf(Output, "%s ", root->nume);
		printareD(root->dr, Output);
	}
}
Director* nod_min_d(Director* node)
{
	Director* current = node;

	while (current && current->st != NULL)//caut cel mai mic nod pornind de la root
		current = current->st;

	return current;
}
Director* stergere_nod_dir(Director* director_root, char* nume, int* ok)
{
	if (director_root == NULL)
		return director_root;

	if (strcmp(director_root->nume, nume) > 0)//parcurg arborele
		director_root->st = stergere_nod_dir(director_root->st, nume, ok);


	else if (strcmp(director_root->nume, nume) < 0)
		director_root->dr = stergere_nod_dir(director_root->dr, nume, ok);


	else {
		*ok = 1;//counter pentru a afla daca am sters vreun dir
		if (director_root->st == NULL) {//daca elem are un singur copil:
			Director* temp = director_root->dr;
			memory_free(director_root->directories);//dezaloc toate subdir si fisierele lor
			free(director_root->nume);//dezaloc numele nodului
			memory_free_files(director_root->files);//dezaloc fisierele nodului
			free(director_root);//dezaloc nodul

			return temp;
		}
		else if (director_root->dr == NULL) {
			Director* temp = director_root->st;
			memory_free(director_root->directories);
			free(director_root->nume);
			memory_free_files(director_root->files);
			free(director_root);

			return temp;
		}

		Director* temp = nod_min_d(director_root->dr);

		strcpy(director_root->nume, temp->nume);//inlocuiesc numele cu "mostenitorul"

		director_root->dr = stergere_nod_dir(director_root->dr, temp->nume, ok);
	}
	return director_root;
}

Fisier* nod_min_f(Fisier* node)
{
	Fisier* current = node;

	while (current && current->st != NULL)//analog dir
		current = current->st;

	return current;
}

Fisier* stergere_nod_f(Fisier* director_root, char* nume, int* ok)//analog dir
{
	if (director_root == NULL)
		return director_root;


	if (strcmp(director_root->nume, nume) > 0)
		director_root->st = stergere_nod_f(director_root->st, nume, ok);


	else if (strcmp(director_root->nume, nume) < 0)
		director_root->dr = stergere_nod_f(director_root->dr, nume, ok);


	else {
		*ok = 1;

		if (director_root->st == NULL) {
			Fisier* temp = director_root->dr;
			free(director_root->nume);
			free(director_root);
			return temp;
		}
		else if (director_root->dr == NULL) {
			Fisier* temp = director_root->st;
			free(director_root->nume);
			free(director_root);
			return temp;
		}

		Fisier* temp = nod_min_f(director_root->dr);

		strcpy(director_root->nume, temp->nume);

		director_root->dr = stergere_nod_f(director_root->dr, temp->nume, ok);
	}
	return director_root;
}

Director* changeDir(Director* director_root, Director* director_curent, char* nume, int* ok)
{

	if (strcmp(nume, "..") == 0)
	{
		*ok = 1;//in cazul .. ,returnez parintele directorului
		return director_curent->parinte;

	}
	else if (director_root == NULL)
	{
		return director_root;
	}
	else if (strcmp(director_root->nume, nume) > 0)//parcurg 
	{
		return changeDir(director_root->st, director_curent, nume, ok);
	}
	else if (strcmp(director_root->nume, nume) < 0)
	{
		return changeDir(director_root->dr, director_curent, nume, ok);
	}
	else if (strcmp(director_root->nume, nume) == 0)
	{
		*ok = 1;//daca gasesc directorul,il returnez pt a-l inlocui pe currentD
		return director_root;
	}
	return director_root;
}

void PrintWorkingDir(Director* current_Dir, FILE* Output)
{
	Director* temp;
	temp = current_Dir;
	char* cale[20];//vector char pt a stora directoarele
	int nr_Dir = 0;
	while (temp != NULL)
	{
		cale[nr_Dir] = temp->nume;//copiez fiecare parinte in vector
		temp = temp->parinte;
		nr_Dir++;
	}
	for (int i = nr_Dir - 1; i >= 0; i--)
	{
		fprintf(Output, "/%s", cale[i]);//printez invers directoarele
	}
}

void search_dir(Director* root, char* nume, int* found, FILE* Output)
{

	if (root == NULL)
	{}
	else if (strcmp(root->nume, nume) > 0)
	{
		search_dir(root->st, nume, found, Output);
	}
	else if (strcmp(root->nume, nume) < 0)
	{
		search_dir(root->dr, nume, found, Output);
	}
	else if (strcmp(root->nume, nume) == 0)
	{
		*found = 1;
		fprintf(Output, "Directory %s found!\n", nume);
		PrintWorkingDir(root, Output);
		fprintf(Output, "\n");
	}

}
void find_dir(Director* root, char* nume, int* ok, int* found, FILE* Output)
{
	if (root == NULL)
		return;

	if (*ok == 0)
	{
		search_dir(root, nume, found, Output);
		*ok = 1;
	}
	if (root->directories != NULL)//daca directorul contine subdir,caut recursiv in ele;
	{
		search_dir(root->directories, nume, found, Output);
		find_dir(root->directories, nume, ok, found, Output);
	}

	find_dir(root->st, nume, ok, found, Output);//parcurg directorele
	find_dir(root->dr, nume, ok, found, Output);
}

void search_file(Fisier* root, char* nume, int* found, FILE* Output)
{

	if (root == NULL)
	{}
	else if (strcmp(root->nume, nume) > 0)
	{
		search_file(root->st, nume, found, Output);
	}
	else if (strcmp(root->nume, nume) < 0)
	{
		search_file(root->dr, nume, found, Output);
	}
	else if (strcmp(root->nume, nume) == 0)
	{
		*found = 1;
		fprintf(Output, "File %s found!\n", nume);
		PrintWorkingDir(root->parinte, Output);//afisez pwd daca am gasit fila
		fprintf(Output, "\n");
	}

}

void find_file(Director* root_dir, char* nume, int* ok, int* found, FILE* Output)
{
	if (root_dir == NULL)
		return;

	if (*ok == 0)
	{
		search_file(root_dir->parinte->files, nume, found, Output);//caut o singura data in fisierele din root
		*ok = 1;
	}
	search_file(root_dir->files, nume, found, Output);//caut in fisierele din dir curent

	if (root_dir->directories != NULL)//parcurg directoarele din subdirectories
	{
		find_file(root_dir->directories, nume, ok, found, Output);
	}
	find_file(root_dir->st, nume, ok, found, Output);//parcurg tree-ul de dir
	find_file(root_dir->dr, nume, ok, found, Output);
}
void memory_free(Director* root)
{
	if (root == NULL)
		return;
	if (root->directories != NULL)//intru in subdirectoare,daca exista
	{
		memory_free(root->directories);
	}

	memory_free(root->st);//parcurg arborele de dir
	memory_free(root->dr);
	memory_free_files(root->files);//dau free fisierelor din dir curent
	free(root->nume);//dau free directorului curent
	free(root);//dau free numelui dir curent
}
void memory_free_files(Fisier* root)
{
	if (root == NULL)
		return;

	memory_free_files(root->st);//parcurg un arbore de fisiere
	memory_free_files(root->dr);
	free(root->nume);
	free(root);//dau free fisierului curent
}