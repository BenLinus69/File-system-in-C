/*BLENDEA ANDREI-IONUT grupa 312CB*/
typedef struct Director
{
	char* nume;
	struct Director* parinte;
	struct Fisier* files;
	struct Director* directories;
	struct Director* st;
	struct Director* dr;
}Director;
typedef struct Fisier
{
	char* nume;
	struct Director* parinte;
	struct Fisier* st;
	struct Fisier* dr;
}Fisier;


Director* constrD(char* name, Director* parinte);
Director* insertD(Director* director_root, Director* parinte, char* nume, FILE* stdout);
Fisier* constrF(char* name, Director* parinte);
Fisier* insertF(Fisier* fisier_root, Director* parinte, char* nume, FILE* stdout);
Director* stergere_nod_dir(Director* director_root, char* nume, int* ok);
Director* nod_min_d(Director* node);
Fisier* stergere_nod_f(Fisier* director_root, char* nume, int* ok);
Fisier* nod_min_f(Fisier* node);
Director* changeDir(Director* director_root, Director* director_curent, char* nume, int* ok);
int Check_Files(Fisier* root, char* nume);
int Check_Dir(Director* root, char* nume);
void PrintWorkingDir(Director* current_Dir, FILE* stdout);
void printareF(Fisier* root, FILE* stdout);
void printareD(Director* root, FILE* stdout);
void search_dir(Director* root, char* nume, int* found, FILE* stdout);
void find_dir(Director* root, char* nume, int* ok, int* found, FILE* stdout);
void search_file(Fisier* root, char* nume, int* found, FILE* stdout);
void find_file(Director* root_dir, char* nume, int* ok, int* found, FILE* stdout);
void memory_free(Director* root);
void memory_free_files(Fisier* root);
