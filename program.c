#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KATEGORIA_DLZKA 52
#define MIESTO_DLZKA 52
#define ULICA_DLZKA 102
#define POPIS_DLZKA 202

typedef struct {
	char kategoria[KATEGORIA_DLZKA];
	char miesto[MIESTO_DLZKA];
	char ulica[ULICA_DLZKA];
	int rozloha;
	int cena;
	char popis[POPIS_DLZKA];
} Ponuka;

typedef struct UzolStruct {
	Ponuka ponuka;
	struct UzolStruct *dalsi;
} Uzol;

typedef struct {
	Uzol *zaciatok;
} SpajanyZoznam;

void inicializujZoznam(SpajanyZoznam *zoznam) {
	if (!zoznam) {
		return;	}
	zoznam->zaciatok = NULL;}
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
