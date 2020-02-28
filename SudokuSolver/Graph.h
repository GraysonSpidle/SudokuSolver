#pragma once
#include <vector>
#include <set>
#include <type_traits>

template <typename Vertex, typename Edge, 
	typename = std::enable_if<
		std::is_assignable_v<Edge, std::pair<Vertex, Vertex>> // Edge must have first and second properties like std::pair does
	>
> 
class Graph {
	std::set<Vertex> vertices;
	std::set<Edge> edges;


public:
	using VertexIterator = std::set<Vertex>::iterator;
	using EdgeIterator = std::set<Edge>::iterator;

	Graph() {
		vertices = {};
		edges = {};
	}

	Graph(std::set<Edge>& edges) {
		this->edges = edges;
		vertices = {};
		for (EdgeIterator it = edges.begin(); it != edges.end(); ++it) {
			vertices.insert(it->first);
			vertices.insert(it->second);
			// it shouldn't insert duplicates so I think we're good on that front
		}
	}

	~Graph() {}

	void addVertex(Vertex & v) { vertices.insert(v); }
	void removeVertex(Vertex & v) { vertices.erase(v); }
	void addEdge(Edge & e) { edges.insert(e); }
	void removeEdge(Edge & e) { edges.erase(e); }

	std::vector<Edge> operator[](const Vertex& v) {
		std::vector<Edge> output = {};
		std::function<EdgeIterator(EdgeIterator&)> findNext = [&v](EdgeIterator& begin) {
			return std::find_if(begin, edges.end(), [&v](Edge& e) {
				return e.first == v || e.second == v;
			});
		};

		for (auto it = findNext(edges.begin()); it != edges.end(); it = findNext(++it)) {
			output.push_back(*it);
		}
		return output;
	}

	VertexIterator& verticesBegin() { return vertices.begin(); }
	VertexIterator& verticesEnd() { return vertices.end(); }
	EdgeIterator& edgesBegin() { return edges.begin(); }
	EdgeIterator& edgesEnd() { return edges.end(); }

	std::size_t verticesCount() const noexcept { return vertices.size(); }
	std::size_t edgesCount() const noexcept { return edges.size(); }



};

template <typename Vertex, typename Edge, typename _Ty,
	typename = std::enable_if<
		std::is_assignable_v<Edge, std::pair<Vertex, Vertex>> && // Edge must have first and second properties like std::pair does
		std::is_arithmetic_v<_Ty>
	>
>
typename Graph<Vertex, Edge>::EdgeIterator findNextHSCandidate(typename Graph<Vertex, Edge>::EdgeIterator& begin, typename Graph<Vertex, Edge>::EdgeIterator& end,
	const Graph<Vertex, Edge>& graph,
	const std::set< _Ty >& otherMarkings) {


}

