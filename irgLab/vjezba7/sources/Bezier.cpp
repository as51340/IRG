#include "Bezier.h"

Bezier::Bezier() {};

Bezier::~Bezier() {};

const glm::vec4& Bezier::parVector(float t) {
	glm::vec4 vec = glm::vec4(glm::pow(t, 3), glm::pow(t, 2), glm::pow(t, 1), 1);
	return vec;
}

const glm::mat4& Bezier::getCoeffs() {
	return this->coeffs;
}

std::vector<int> Bezier::computeFactors(int n) {
	int i, a = 1;
	std::vector<int> factors(n + 1, 0);
	for (i = 1; i <= n + 1; i++) {
		factors[i - 1] = a;
		a = a * (n - i + 1) / i;
	}
	return factors;
}

glm::vec3 Bezier::getPoint(float t, std::vector<glm::vec3>& controls) {
	int n = controls.size() - 1;
	std::vector<int> factors = computeFactors(n);
	double b, px, py, pz;
	px = 0.0; py = 0.0, pz = 0.0;
	for (int j = 0; j <= n; j++) {
		if (j == 0) {
			b = factors[j] * glm::pow(1 - t, n);
		}
		else if (j == n) {
			b = factors[j] * glm::pow(t, n);
		}
		else {
			b = factors[j] * glm::pow(t, j) * glm::pow(1 - t, n - j);
		}
		double factor = b * controls[j].x;
		//std::cout << "T " << t << " " << factor << std::endl;
		px += b * controls[j].x;
		py += b * controls[j].y;
		pz += b * controls[j].z;
	}
	return glm::vec3(px, py, pz);
}

void Bezier::draw_bezier(Curve& curve, int divs) {
	std::vector<glm::vec3> vertices = curve.getVertices();
	int n = vertices.size() - 1;
	std::vector<int> factors = computeFactors(n);
	double t, b, px, py, pz;
	for (int i = 0; i <= divs; i++) {
		t = 1.0 / (double)divs * i;
		//std::cout << t << std::endl;
		px = 0.0; py = 0.0, pz = 0.0;
		for (int j = 0; j <= n; j++) {
			if (j == 0) {
				b = factors[j] * glm::pow(1 - t, n);
			}
			else if (j == n) {
				b = factors[j] * glm::pow(t, n);
			}
			else {
				b = factors[j] * glm::pow(t, j) * glm::pow(1 - t, n - j);
			}
			double factor = b * curve.getVertices()[j].x;
			//std::cout << "T " << t << " " << factor << std::endl;
			px += b * curve.getVertices()[j].x;
			py += b * curve.getVertices()[j].y;
			pz += b * curve.getVertices()[j].z;
		}
		//std::cout << std::endl;
		curve.addPoint(glm::vec3(px, py, pz));
	}
}

std::vector<glm::vec3> Bezier::interPoints(std::vector<glm::vec3>& points) {
	glm::mat4 matPoints = glm::mat4(0);;

	if (points.size() != 4) {
		throw "Wrong number of points";
	}
	for (int i = 0; i < points.size(); i++) {
		matPoints[0][i] = points[i].x;
		matPoints[1][i] = points[i].y;
		matPoints[2][i] = points[i].z;
		matPoints[3][i] = 0;
	}
	//std::cout << glm::to_string(matPoints) << std::endl;
	const glm::mat4 invCoeffs = glm::mat4(
		0.0f, 1.0f / 27.0f, 8.0f / 27.0f, 1.0f,
		0.0f, 1.0f / 9.0f, 4.0f / 9.0f, 1.0f,
		0.0f, 1.0f / 3.0f, 2.0f / 3.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 sol = glm::inverse(coeffs) * glm::inverse(invCoeffs) * matPoints;
	//std::cout << glm::to_string(glm::inverse(invCoeffs)) << std::endl;
	std::vector<glm::vec3> vecSol;
	for (int i = 0; i < 4; i++) {
		vecSol.push_back(glm::vec3(sol[0][i], sol[1][i], sol[2][i]));
	}
	return vecSol;
}