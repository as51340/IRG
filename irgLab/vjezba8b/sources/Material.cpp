#include "Material.h"

void Material::print() {
	std::cout << glm::to_string(amb) << std::endl << glm::to_string(diff) << std::endl << glm::to_string(ref) << std::endl;
}