#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include <queue>


template<typename T>
class Graph
{
	std::vector<T> m_vertices;
	std::vector<std::tuple<size_t, size_t, int>> m_edges;
	bool m_isOriented;

	bool isVertex(const T& elem);
	size_t indexOf(const T& elem);

	bool isVisited(std::vector<T> v, const T& e)const;
	bool hasCycleFrom(const T& vertex);

public:

	Graph(bool isOriented);
	Graph(const Graph<T>& oth);
	Graph(Graph<T>&& oth)noexcept;
	Graph<T>& operator=(const Graph<T>& oth);
	Graph<T>& operator=(Graph<T>&& oth)noexcept;
	~Graph();
	bool add_vertex(const T& v);
	bool add_edge(const T& fst, const T& snd, int weight = 1);
	T& operator[](const size_t& i);
	const T& operator[](const size_t& i)const;
	T& at(const size_t& i);
	const T& at(const size_t& i)const;
	std::vector<T> neighbours(const T& elem)const;
	bool isOriented()const;
	bool areConnected(const T& fst, const T& snd)const;
	//------------------------------------------------
	//algorithms
	//------------------------------------------------

	std::vector<T> bfs(const T& start, const T& end);
	bool isCyclic();
	bool IsConnected();
};

template<typename T>
Graph<T>::Graph(bool isOriented) :m_isOriented(isOriented) {}

template<typename T>
Graph<T>::Graph(const Graph<T>& oth) : m_vertices(oth.m_vertices) {}

template<typename T>
Graph<T>::Graph(Graph<T>&& oth)noexcept :m_vertices(std::move(oth.m_vertices)), m_edges(std::move(oth.m_edges)), m_isOriented(std::move(oth.m_isOriented)) {}

template<typename T>
Graph<T>& Graph<T>::operator=(const Graph<T>& oth)
{
	if (this != &oth)
	{	//String exception guarantee
		std::vector<T> h_vertices(oth.m_vertices);
		std::vector<std::tuple<size_t, size_t, int>> h_edges(oth.m_edges);
		bool h_isOriented(oth.m_isOriented);

		m_vertices = h_vertices;
		m_edges = h_edges;
		m_isOriented = h_isOriented;
	}
	return this;
}

template<typename T>
Graph<T>& Graph<T>::operator=(Graph<T>&& oth)noexcept
{
	m_vertices = std::move(oth.m_vertices);
	m_edges = std::move(oth.m_edges);
	m_isOriented = std::move(oth.m_isOriented);
}

template<typename T>
Graph<T>::~Graph() {}

template<typename T>
bool Graph<T>::isVertex(const T& elem)
{
	for (size_t i = 0;i < m_vertices.size();i++)
		if (m_vertices[i] == elem) return true;
	return false;
}

template<typename T>
size_t Graph<T>::indexOf(const T& elem)
{
	for (size_t i = 0;i < m_vertices.size();i++)
		if (m_vertices[i] == elem) return i;
	throw std::logic_error("element is not in the graph");
}

template<typename T>
bool Graph<T>::add_vertex(const T& v)
{
	if (isVertex(v)) return false;
	m_vertices.push_back(v);
	return true;
}

template<typename T>
bool  Graph<T>::add_edge(const T& fst, const T& snd, int weight)
{
	try
	{
		size_t fst_index = indexOf(fst);
		size_t snd_index = indexOf(snd);

		std::tuple<size_t, size_t, int> temp(fst_index, snd_index, weight);
		m_edges.push_back(temp);
		if (!m_isOriented)
		{
			std::tuple<size_t, size_t, int> temp2(snd_index, fst_index, weight);
			m_edges.push_back(temp2);
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

template<typename T>
T& Graph<T>::operator[](const size_t& i)
{   //assume that i will be correct index
	return m_vertices[i];
}

template<typename T>
const T& Graph<T>::operator[](const size_t& i)const
{  //assume that i will be correct index
	return m_vertices[i];
}

template<typename T>
T& Graph<T>::at(const size_t& i)
{
	if (i >= m_vertices.size()) throw std::out_of_range("Wrong index");
	return m_vertices[i];
}

template<typename T>
const T& Graph<T>::at(const size_t& i)const
{
	if (i >= m_vertices.size()) throw std::out_of_range("Wrong index");
	return m_vertices[i];
}

template<typename T>
std::vector<T> Graph<T>::neighbours(const T& elem)const
{  
	std::vector<bool> _neighbours;
	for (size_t i = 0;i < m_vertices.size();i++)
		_neighbours.push_back(false);

	for (size_t i = 0;i < m_edges.size();i++)
		if (m_vertices[std::get<0>(m_edges[i])] == elem)
			_neighbours[std::get<1>(m_edges[i])] = true;

	std::vector<T> forReturn;
	for (size_t i = 0;i < _neighbours.size();i++)
		if (_neighbours[i]) forReturn.push_back(m_vertices[i]);

	//O(n) complexity
	return forReturn;
}

template<typename T>
bool  Graph<T>::isOriented()const
{
	return m_isOriented;
}

template<typename T>
bool Graph<T>::areConnected(const T& fst, const T& snd)const
{
	for (size_t i = 0;i < m_edges.size();i++)
		if (m_vertices[std::get<0>(m_edges[i])] == fst && m_vertices[std::get<1>(m_edges[i])] == snd)
			return true;
	return false;
}


//------------------------------------------------
//algorithms
//------------------------------------------------

//help function
template<typename T>
bool Graph<T>::isVisited(std::vector<T> v, const T& e)const
{
	for (size_t i = 0;i < v.size();i++)
		if (v[i] == e)
			return true;
	return false;
}

template<typename T>
std::vector<T> Graph<T>::bfs(const T& start, const T& end)
{
	if (start == end) {
		std::vector<T> forReturn;
		forReturn.push_back(start);
		return std::move(forReturn);
	}
	if (m_vertices.size() == 0) throw std::logic_error("Empty graph");
	if (!isVertex(start) || !isVertex(end)) throw std::logic_error("Vertex not in graph");

	std::queue<std::pair<T, std::vector<T> >> q; //queue < pair < Vertex V, road from start to V > >
	q.push(std::pair<T, std::vector<T>>(start, std::vector<T>())); //start condition -> push (START,empty vector)
	std::vector<T> visited;
	visited.push_back(start);

	while (!q.empty())
	{
		std::vector<T> adj = neighbours(q.front().first);

		for (size_t i = 0;i < adj.size();i++)
			if (!isVisited(visited, adj[i]))
			{   //updating road
				q.front().second.push_back(q.front().first);
				if (adj[i] == end)
				{   //make the road full
					q.front().second.push_back(adj[i]);
					return std::move(q.front().second);
				}
				q.push(std::pair<T, std::vector<T>>(adj[i], q.front().second));
				visited.push_back(adj[i]);
			}
		q.pop();
	}
	throw std::logic_error("cant find path from start to end");
}


template<typename T>
bool Graph<T>::hasCycleFrom(const T& vertex)
{
	std::stack<std::pair<T, T>> _stack; // element and his "parent"
	std::vector<T> visited;
	_stack.push(std::pair<T,T>(vertex, vertex));
	visited.push_back(vertex);

	while (!_stack.empty())
	{
		std::pair<T, T> temp = _stack.top();
		_stack.pop();
		std::vector<T> adj = neighbours(temp.first);
		for (size_t i = 0;i < adj.size();i++)
		{								  
			if (!isVisited(visited, adj[i]))
			{
				_stack.push(std::pair<T,T>(adj[i],temp.first));
				visited.push_back(adj[i]);
			}
			else if (adj[i] != temp.second) return true;
		}
	}
	return false;;
}

template<typename T>
bool Graph<T>::isCyclic()
{
	if (m_vertices.size() < 3) return false;
	for (size_t i = 0;i<m_vertices.size();i++)
		if (hasCycleFrom(m_vertices[i])) return true;
	return false;
}

template<typename T>
bool Graph<T>::IsConnected()
{
	if (m_vertices.size() == 0) return false;
	std::stack<T> s;
	s.push(m_vertices[0]);
	std::vector<T> visited;
	visited.push_back(s.top());

	while (!s.empty())
	{
		T temp = s.top();
		s.pop();
		std::vector<T> adj = neighbours(temp);
		for (size_t i = 0;i < adj.size();i++)
			if (!isVisited(visited, adj[i]))
			{
				s.push(adj[i]);
				visited.push_back(adj[i]);
			}
	}
	return visited.size() == m_vertices.size();
}