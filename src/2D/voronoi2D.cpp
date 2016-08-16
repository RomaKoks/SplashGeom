// Splash (c) - open-source C++ library for geometry and linear algebra.
// Copyright (c) 2016, Ilya Zakharkin, Elena Kirilenko and Nadezhda Kasimova.
// All rights reserved.
/*
	This file is part of Splash.

	Splash is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Splash is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Splash.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "voronoi2D.hpp"

VoronoiDiagram2D::VoronoiDiagram2D()
	: diagram() {}

VoronoiDiagram2D::VoronoiDiagram2D(const vector<Point2D>& points)
{
	MakeVoronoiDiagram2DHalfPlanes(points);
}

Convex2D GetHalfPlaneIntersection(const Point2D& cur_point, const vector<Line2D>& halfplanes, const Rectangle& border_box)
{
	if (halfplanes.size() == 1) {
		return border_box.GetIntersectionalConvex2D(cur_point, halfplanes[0]);
	} else {
		int middle = halfplanes.size() >> 1;
		Convex2D first_convex = GetHalfPlaneIntersection(cur_point, halfplanes/*before middle*/, border_box);
		Convex2D second_convex = GetHalfPlaneIntersection(cur_point, halfplanes/*after middle*/, border_box);
		return first_convex.GetIntersectionalConvex(second_convex);
	}
}

Voronoi2DLocus VoronoiDiagram2D::MakeVoronoi2DLocus(const Point2D& cur_point, const vector<Point2D>& points, const Rectangle& border_box)
{
	Voronoi2DLocus cur_locus;
	vector<Line2D> halfplanes;

	for (int i = 0; i < points.size(); ++i) {
		if (points[i] != cur_point) {
			Segment2D cur_seg(cur_point, points[i]);
			Line2D cur_halfplane(cur_seg.GetCenter(), cur_seg.NormalVec()); // Vector2D(cur_seg.GetCenter(), cur_point) - for orientation
			halfplanes.push_back(cur_halfplane);
		}
	}

	cur_locus.region = GetHalfPlaneIntersection(cur_point, halfplanes, border_box);

	return cur_locus;
}

vector<Voronoi2DLocus> VoronoiDiagram2D::MakeVoronoiDiagram2DHalfPlanes(const vector<Point2D>& points, const Rectangle& border_box)
{
	Voronoi2DLocus cur_locus;

	for (int i = 0; i < points.size(); ++i) {
		cur_locus = MakeVoronoi2DLocus(points[i], points, border_box);
		diagram.push_back(cur_locus);
	}

	return diagram;
}