#include <stdio.h>
#include <string.h>

/* макрос для удаления символа перевода строки */
#define REM_SN(fname) ((fname[strlen(fname)-1]=='\n')?('\0'):(fname[strlen(fname)-1]))

int main()
{
	printf("Kernel configuration program\n");

	/* считывание пути к файлу и таймера */
	char fname[200];
	size_t per;
	fgets(fname, 200, stdin);
	fname[strlen(fname) - 1] = REM_SN(fname);
	int ret = scanf("%ld", &per);
	if (!ret) {
		per = 0;
	}

	FILE *fd;
	/* открытие и запись информации в файл */
	fd = fopen("../name", "w");
	if (!fd) {
		perror("ERROR: ");
		return -1;
	}
	fprintf(fd, "%s\n", fname);
	fclose(fd);

	fd = fopen("../timer", "w");
	if (!fd) {
		perror("ERROR: ");
		return -1;
	}
	fprintf(fd, "%d\n", per);
	fclose(fd);

	return 0;
}
