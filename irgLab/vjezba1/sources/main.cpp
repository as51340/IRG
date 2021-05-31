// Local Headers

// System Headers
#include "Grafika.h"
#include "Bresenham.h"

// Standard Headers
#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <thread>
#include <chrono>


int width = 97, height = 97;
std::vector<std::pair<glm::vec2, glm::vec2>> lines;
glm::vec2 last; //remembers last
bool createLine = false; // if we need to create new line or not
bool cut = false; // usage of Sutherland algorithm or not
glm::vec3 col_red = glm::vec3(255, 0, 0);
glm::vec3 col_green = glm::vec3(0, 255, 0);


void mouse_click(int x, int y, int vrsta) {
	if (vrsta == 0) {
		std::cout << "lijevi:";
		glm::vec2 clicked = glm::vec2(x, height - y - 1);
		if (createLine) {
			lines.push_back(std::make_pair(last, clicked));
			createLine = false;
		}
		else {
			last = clicked;
			createLine = true;
		}
	}
	else if (vrsta == 1) {
		std::cout << "desni:";
		cut = !cut;
	}

}

int main(int argc, char* argv[]) {

	std::cout << argv[0] << std::endl;
	Grafika graphics(width, height, glm::vec3(0, 0, 0), argv[0]);
	Bresenham bresenham(width, height);


	//prosljeduje se pokazivac na funkciju. Ta funkcija ce se pozvati kada korisnik pritisne misem
	graphics.registrirajFunkcijuZaKlikMisa(mouse_click);

	//answering to the first questions, why we cannot use simplest version of bresenham algorithm


	//std::this_thread::sleep_for(std::chrono::seconds(3));

	while (graphics.trebaZatvoriti()) {

		//osvjezavanje pozicije razlicitih objekata i fizikalna simulacija bi se izvrsavala u ovom dijelu

		graphics.pobrisiProzor();

		//iscrtavanje uzorka sahovnice
		for (int i = 0; i < height; i += 1)
			for (int j = 0; j < width; j += 1) {
				if ((i + j) % 2 == 0)
					graphics.osvijetliFragment(i, j, glm::vec3(0.1, 0.1, 0.1));
				if (i % 10 == 0 && j % 10 == 0)
					graphics.osvijetliFragment(i, j, glm::vec3(0.15, 0.15, 0.2));
			}

		//iscrtaj stisnuti fragment fragment

		if (createLine) {
			graphics.osvijetliFragment(last.x, last.y, glm::vec3(255, 0, 0));
		}

		int lines_size = lines.size();
		if (cut) { //draw central segment and its lines
			bresenham.draw_central_green_segment(graphics);
			for (int i = 0; i < lines_size; i++) {
				bresenham.draw_suther_line(graphics, lines[i].first.x, lines[i].first.y, lines[i].second.x, lines[i].second.y, col_red);
			}
		}
		else { // or draw whole lines
			for (int i = 0; i < lines_size; i++) {
				bresenham.draw_line(graphics, lines[i].first.x, lines[i].first.y, lines[i].second.x, lines[i].second.y, col_red);
			}
		}

		graphics.iscrtajRaster();

		//kako program ne bi trosio previse resursa, na kraj petlje se moze dodati poziv funkcije za cekanje
		//kao npr Sleep ili od c++11 na dalje this_thread::sleep_for(chrono::milliseconds(16));

	}


	return EXIT_SUCCESS;
}
