// Local Headers

// System Headers
#include "Grafika.h"

// Standard Headers
#include <cstdio>
#include <cstdlib>

#include <iostream>

std::vector<std::pair<int, int> >klikovi;


void iscrtajLiniju(Grafika &grafika, int x0, int y0, int x1, int y1) {
	grafika.osvijetliFragment(x0, y0, glm::vec3(0, 0.8, 0));

	//odabir varijante bresenhama i algoritam

}

void klikMisa(int x, int y, int vrsta) {

	if (vrsta == 0)
		std::cout << "lijevi:";
	if (vrsta == 1)
		std::cout << "desni:";

	std::cout << x << " " << y<< std::endl;
	klikovi.push_back(std::make_pair(x, y));
}

int main(int argc, char * argv[]) {

	int width = 97, height = 97;
	std::cout << argv[0] << std::endl;
	Grafika grafika(width, height, glm::vec3(0, 0, 0), argv[0]);

	//prosljeduje se pokazivac na funkciju. Ta funkcija ce se pozvati kada korisnik pritisne misem
	grafika.registrirajFunkcijuZaKlikMisa(klikMisa);

	while (grafika.trebaZatvoriti()) {

		//osvjezavanje pozicije razlicitih objekata i fizikalna simulacija bi se izvrsavala u ovom dijelu

		grafika.pobrisiProzor();

		//iscrtavanje uzorka sahovnice
		for (int i = 0; i < height; i += 1)
			for (int j = 0; j < width; j += 1) {
				if ((i + j) % 2 == 0)
					grafika.osvijetliFragment(i, j, glm::vec3(0.1, 0.1, 0.1));
				if (i % 10 == 0 && j % 10 == 0)
					grafika.osvijetliFragment(i, j, glm::vec3(0.15, 0.15, 0.2));
			}


		//iscrtavanje pritisnutih fragmenata
		//ishodiste koordinatnog sustava za operacijski sustav je u gornjem lijevom kutu, a za OpenGL je u donjem lijevom, pa je potrebno okrenuti predznak
		for (int i = 0; i < klikovi.size(); i++)
			grafika.osvijetliFragment(klikovi[i].first, height - klikovi[i].second -1, glm::vec3(0.6, 0.2, 0));

		grafika.iscrtajRaster();

		//kako program ne bi trosio previse resursa, na kraj petlje se moze dodati poziv funkcije za cekanje
		//kao npr Sleep ili od c++11 na dalje this_thread::sleep_for(chrono::milliseconds(16));
	}   
	

    return EXIT_SUCCESS;
}
