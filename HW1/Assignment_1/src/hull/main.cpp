////////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <complex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
////////////////////////////////////////////////////////////////////////////////

typedef std::complex<double> Point;
typedef std::vector<Point> Polygon;

double inline det(const Point &u, const Point &v) {
	// TODO
	return u.real()*v.imag() - u.imag()*v.real();
}

struct Compare {
	Point p0; // Leftmost point of the poly
	bool operator ()(const Point &p1, const Point &p2) {
		// TODO
		Point v1 = p1 - p0;
		Point v2 = p2 - p0;
		double d = det(v1, v2);
		if (d > 0) {
			return true;
		}
		else if (d == 0 && pow(v1.real(), 2) + pow(v1.imag(), 2) < pow(v2.real(), 2) + pow(v2.imag(), 2)) {
			return true;
		}
		return false;
	}
};

bool inline salientAngle(Point &a, Point &b, Point &c) {
	// TODO
	Point v1 = b - a;
	Point v2 = c - a;
	double d = det(v1, v2);
	if (d < 0) {
		return true;
	}
	else if (d == 0 && pow(v1.real(), 2) + pow(v1.imag(), 2) > pow(v2.real(), 2) + pow(v2.imag(), 2)) {
		return true;
	}
	return false;
}

// Custom compare function
bool comp(std::complex<double> &x1, std::complex<double> &x2) {
	if (x1.real() == x2.real()) {
		return x1.imag() < x2.imag();
	}
	return x1.real() < x2.real();
}

////////////////////////////////////////////////////////////////////////////////

Polygon convex_hull(std::vector<Point> &points) {
	Compare order;
	// TODO

	std::sort(points.begin(), points.end(), comp);

	order.p0 = points[0];
	std::sort(points.begin()+1, points.end(), order);
	Polygon hull;
	// TODO
	// use salientAngle(a, b, c) here
	hull.push_back(points[0]);
	hull.push_back(points[1]);
	hull.push_back(points[2]);

	for (auto i = 3; i < points.size(); i++) {
		while (hull.size() > 1 && salientAngle(*(hull.end()-2), *(hull.end()-1), points[i])) {
			hull.pop_back();
		}
		hull.push_back(points[i]);
	}
	return hull;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<Point> load_xyz(const std::string &filename) {
	std::vector<Point> points;
	std::ifstream in(filename);
	// TODO
	int n;
	in >> n;
	double x, y;
	int z;
	while (in >> x >> y >> z) {
		Point pt = Point(x, y);
		points.push_back(pt);
	}
	return points;
}

void save_obj(const std::string &filename, Polygon &poly) {
	std::ofstream out(filename);
	if (!out.is_open()) {
		throw std::runtime_error("failed to open file " + filename);
	}
	out << std::fixed;
	for (const auto &v : poly) {
		out << "v " << v.real() << ' ' << v.imag() << " 0\n";
	}
	for (size_t i = 0; i < poly.size(); ++i) {
		out << "l " << i+1 << ' ' << 1+(i+1)%poly.size() << "\n";
	}
	out << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[]) {
	if (argc <= 2) {
		std::cerr << "Usage: " << argv[0] << " points.xyz output.obj" << std::endl;
	}
	std::vector<Point> points = load_xyz(argv[1]);
	Polygon hull = convex_hull(points);
	save_obj(argv[2], hull);
	return 0;
}
