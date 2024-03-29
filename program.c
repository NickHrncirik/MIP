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

// uzol jednosmerne spajaneho zoznamu
typedef struct UzolStruct {
	Ponuka ponuka;
	struct UzolStruct *dalsi;
} Uzol;


typedef struct {
	Uzol *zaciatok;
} SpajanyZoznam;

// pripravi strukturu pre zoznam
void inicializujZoznam(SpajanyZoznam *zoznam) {
	if (!zoznam) {
		return;
	}

	zoznam->zaciatok = NULL;
}

// uvolni zoznam (vsetky uzly)
void uvolniZoznam(SpajanyZoznam *zoznam) {
	if (!zoznam) {
		return;
	}

	Uzol *uzol = zoznam->zaciatok;
	while (uzol) {
		Uzol *dalsi = uzol->dalsi;
		free(uzol);
		uzol = dalsi;
	}

	zoznam->zaciatok = NULL;
}

// odstrani z konca retazca znak noveho riadka
void odstranKoniecRiadka(char retazec[]) {
	size_t dlzka = strlen(retazec);
	if (dlzka > 0 && retazec[dlzka - 1] == '\n') {
		retazec[dlzka - 1] = '\0';
	}
}

// odstrani zo vsetkych retazcov struktury ponuka znak noveho riadka z konca
void odstranKonceRiadkov(Ponuka *ponuka) {
	odstranKoniecRiadka(ponuka->kategoria);
	odstranKoniecRiadka(ponuka->miesto);
	odstranKoniecRiadka(ponuka->ulica);
	odstranKoniecRiadka(ponuka->popis);
}

// prida ponuku do spajaneho zoznamu
void pridajPonuku(SpajanyZoznam *ponuky, Ponuka ponuka) {
	if (!ponuky) {
		return;
	}

	Uzol *novy = malloc(sizeof(Uzol));
	novy->ponuka = ponuka;
	novy->dalsi = NULL;

	if (ponuky->zaciatok) {
		Uzol *posledny = ponuky->zaciatok;
		while (posledny->dalsi) {
			posledny = posledny->dalsi;
		}
		posledny->dalsi = novy;
	}
	else {
		ponuky->zaciatok = novy;
	}
}

// nacita ponuky zo suboru do zoznamu, ak zoznam existuje tak ho najprv uvolni
void n(SpajanyZoznam *ponuky) {
	FILE *subor;
	if ((subor = fopen("reality.txt", "r")) == NULL) {
		printf("Zaznamy neboli nacitane\n");
		return;
	}

	if (ponuky->zaciatok) {
		uvolniZoznam(ponuky);
	}

	int pocetNacitanych = 0;
	Ponuka ponuka;
	char oddelovac[3];
	while (fgets(oddelovac, 3, subor) != NULL) {
		fgets(ponuka.kategoria, KATEGORIA_DLZKA, subor);
		fgets(ponuka.miesto, MIESTO_DLZKA, subor);
		fgets(ponuka.ulica, ULICA_DLZKA, subor);
		fscanf(subor, "%d", &ponuka.rozloha);
		fscanf(subor, "%d", &ponuka.cena);
		fgetc(subor);
		fgets(ponuka.popis, POPIS_DLZKA, subor);
		odstranKonceRiadkov(&ponuka);
		pridajPonuku(ponuky, ponuka);
		pocetNacitanych++;
	}

	printf("Nacitalo sa %d zaznamov\n", pocetNacitanych);

	if (fclose(subor) == EOF) {
		printf("Nepodarilo sa zatvorit subor reality.txt\n");
	}
}

// vypise ponuku
void vypisPonuku(Ponuka *ponuka) {
	printf("kategoria ponuky: %s\n", ponuka->kategoria);
	printf("miesto ponuky: %s\n", ponuka->miesto);
	printf("ulica: %s\n", ponuka->ulica);
	printf("rozloha v m2: %d\n", ponuka->rozloha);
	printf("cena: %d\n", ponuka->cena);
	printf("popis: %s\n", ponuka->popis);
}

// vypisu vsetky ponuky v zozname
void v(SpajanyZoznam *ponuky) {
	int poradoveCislo = 1;
	for (Uzol *uzol = ponuky->zaciatok; uzol; uzol = uzol->dalsi) {
		printf("%d.\n", poradoveCislo++);
		vypisPonuku(&uzol->ponuka);
	}
}

// najde vhodne ponuky podla zadanej ceny od uzivatela
void h(SpajanyZoznam *ponuky) {
	int cenovyLimit;
	scanf("%d", &cenovyLimit);
	getchar();

	int pocetVhodnych = 0;
	for (Uzol *uzol = ponuky->zaciatok; uzol; uzol = uzol->dalsi) {
		if (uzol->ponuka.cena <= cenovyLimit) {
			printf("%d.\n", ++pocetVhodnych);
			vypisPonuku(&uzol->ponuka);
		}
	}

	if (pocetVhodnych == 0) {
		printf("V ponuke su len reality s vyssou cenou\n");
	}
}


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
		case 'n':		//príkaz na načítanie záznamov o realitách zo súboru do spájaného zoznamu štruktúr
			n(&ponuky);
			break;

		case 'v':		//príkaz na výpis celého spájaného zoznamu záznamov
			v(&ponuky);
			break;

		case 'h':		//umožní používateľovi vyhľadať a vypísať všetky položky záznamu podľa ceny ponuky
			h(&ponuky);
			break;
		
		case 'k':		//príkaz na ukončenie programu
			break;

		default:
			printf("Neznama moznost!\n");
		}
	} while (volba != 'k');

	uvolniZoznam(&ponuky);

	return 0;
}
