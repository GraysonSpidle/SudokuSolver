#pragma once
#include <utility>

template <typename Vertex, typename Weight, typename = std::enable_if<
		std::is_arithmetic_v<Weight>
	>
>
struct Edge : public std::pair<Vertex, Vertex> {
	Weight weight;

	Edge(Vertex& v1, Vertex& v2, Weight& weight) :
		std::pair<Vertex, Vertex>(v1, v2), 
		weight(weight) {}
	~Edge() {}
};