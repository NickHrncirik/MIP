#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KATEGORIA_DLZKA 52
#define MIESTO_DLZKA 52
#define ULICA_DLZKA 102
#define POPIS_DLZKA 202

int main(void) {
	SpajanyZoznam ponuky;
	inicializujZoznam(&ponuky);
	
	int volba = '-';
	do {
		volba = getchar();
		while (getchar() != '\n') {
			;
		}

		switch (volba) {
		case 'n':
			nacitajPonuky(&ponuky);
			break;

		case 'v':
			vypisPonuky(&ponuky);
			break;

		case 'h':
			najdiVhodnePonuky(&ponuky);
			break;
		
		case 'k':
			break;

		default:
			printf("Neznama moznost!\n");
		}
	} while (volba != 'k');

	uvolniZoznam(&ponuky);

	return 0;
}
