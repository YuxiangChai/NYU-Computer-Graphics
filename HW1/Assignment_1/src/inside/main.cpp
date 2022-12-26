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

// Return true iff [a,b] intersects [c,d], and store the intersection in ans
bool intersect_segment(const Point &a, const Point &b, const Point &c, const Point &d, Point &ans) {
	// TODO
	Point u = b - a;
	Point v = d - c;

	double t = (c.real()*v.imag() - c.imag()*v.real() + v.real()*a.imag() - v.imag()*a.real()) / (u.real()*v.imag() - u.imag()*v.real());
	double s = (a.real()*u.imag() - a.imag()*u.real() + c.imag()*u.real() - c.real()*u.imag()) / (v.real()*u.imag() - v.imag()*u.real());
	if (t > 0 && t < 1 && s > 0 && s < 1) {
		ans = Point(t*u.real()+a.real(), t*u.imag()+a.imag());
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////

bool comp_x(std::complex<double> x1, std::complex<double> x2) {
	return x1.real() < x2.real();
}

bool comp_y(std::complex<double> x1, std::complex<double> x2) {
	return x1.imag() < x2.imag();
}

bool is_inside(const Polygon &poly, const Point &query) {
	// 1. Compute bounding box and set coordinate of a point outside the polygon
	// TODO
	auto x_min = std::min_element(poly.begin(), poly.end(), comp_x);
	auto y_min = std::min_element(poly.begin(), poly.end(), comp_y);
	Point outside(x_min->real()-100, y_min->imag()-100);
	// 2. Cast a ray from the query point to the 'outside' point, count number of intersections
	// TODO
	int count = 0;
	for (int i = 0; i < poly.size()-1; i++) {
		Point intersect;
		if (intersect_segment(query, outside, poly[i], poly[i+1], intersect)) {
			count += 1;
		}
	}
	Point intersect;
	if (intersect_segment(query, outside, poly[poly.size()-1], poly[0], intersect)) {
		count += 1;
	}
	if (count % 2 == 1)
		return true;
	return false;
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

Polygon load_obj(const std::string &filename) {
	std::ifstream in(filename);
	Polygon poly;
	// TODO
	char c;
	double x, y;
	int z;
	while (in >> c >> x >> y >> z) {
		if (c == 'v') {
			Point pt = Point(x, y);
			poly.push_back(pt);
		}
	}
	return poly;
}

void save_xyz(const std::string &filename, const std::vector<Point> &points) {
	// TODO
	std::ofstream out(filename);
	if (!out.is_open()) {
		throw std::runtime_error("failed to open file " + filename);
	}
	out << std::fixed;
	out << points.size() << "\n";
	for (auto i = points.begin(); i != points.end(); i++) {
		out << i->real() << " " << i->imag() << " 0\n";
	}
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[]) {
	if (argc <= 3) {
		std::cerr << "Usage: " << argv[0] << " points.xyz poly.obj result.xyz" << std::endl;
	}
	std::vector<Point> points = load_xyz(argv[1]);
	Polygon poly = load_obj(argv[2]);
	std::vector<Point> result;
	for (size_t i = 0; i < points.size(); ++i) {
		if (is_inside(poly, points[i])) {
			result.push_back(points[i]);
		}
	}
	save_xyz(argv[3], result);
	return 0;
}
