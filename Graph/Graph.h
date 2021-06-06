#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include <queue>


template<typename T>
class Graph
{
	struct Edge
	{
		size_t first;
		size_t second;
		int weight;
		Edge(const size_t& fst, const size_t& snd, int weight = 1) : first(fst), second(snd), weight(weight) {}
	};
public:
	std::vector<T> m_vertices;
	std::vector<Edge> m_edges;
	bool m_isOriented;

	bool isVertex(const T& elem)const;
	size_t indexOf(const T& elem)const;
	bool hasCycleFrom(const size_t& vertexIndex)const;
	std::vector<T> indexesToT(std::vector<size_t> indexes)const;
	std::vector<T> parentsToPath(std::vector<size_t> parents, size_t startIndex, size_t endIndex)const;
	void dfs1StrongComponentsHelp(size_t vertex, std::vector<bool>& visited, std::stack<size_t>& s)const;
	void dfs2StrongComponentsHelp(size_t vertex, std::vector<bool>& visited, std::vector<size_t>& components, size_t comCount)const;
	std::vector < std::vector<T>> makeComponents(const std::vector<size_t>& components)const;
public:
	Graph(bool isOriented);
	bool add_vertex(const T& v);
	bool add_edge(const T& fst, const T& snd, int weight = 1);
	T& operator[](const size_t& i);
	const T& operator[](const size_t& i)const;
	T& at(const size_t& i);
	const T& at(const size_t& i)const;
	std::vector<T> neighbours(const T& elem)const;
	std::vector<size_t> neighboursIndex(const size_t& i)const;
	bool isOriented()const;
	bool areConnected(const T& fst, const T& snd)const;
	//------------------------------------------------
	//algorithms
	//------------------------------------------------
	std::vector<T> bfs(const T& start, const T& end)const;
	bool isCyclic()const;
	bool isConnected()const;
	bool isBipartite()const;
	void reverseEdges();
	std::vector<std::vector<T>>  strongConnectedComponents()const;
	/*TODOstd::vector<T> topologicalSort()
	FloydWarshall
	Kruskal
	Prim
	bellmanFord
	dijkstra
	*/
};

template<typename T>
bool Graph<T>::isVertex(const T& elem)const
{
	for (size_t i = 0;i < m_vertices.size();i++)
		if (m_vertices[i] == elem) return true;
	return false;
}

template<typename T>
size_t Graph<T>::indexOf(const T& elem)const
{
	for (size_t i = 0;i < m_vertices.size();i++)
		if (m_vertices[i] == elem) return i;
	throw std::logic_error("element is not in the graph");
}

template<typename T>
bool Graph<T>::hasCycleFrom(const size_t& vertexIndex)const
{
	std::stack<std::pair<size_t, size_t>> _stack; // element and his "parent"
	_stack.push(std::pair<size_t, size_t>(vertexIndex, vertexIndex));

	std::vector<size_t> visited;
	for (size_t i = 0;i < m_vertices.size();i++)
		visited.push_back(false);
	visited[vertexIndex] = true;

	while (!_stack.empty())
	{
		std::pair<size_t, size_t> temp = _stack.top();
		_stack.pop();
		std::vector<size_t> adj = neighboursIndex(temp.first);
		for (size_t i = 0;i < adj.size();i++)
		{
			if (!visited[adj[i]])
			{
				_stack.push(std::pair<size_t, size_t>(adj[i], temp.first));
				visited[adj[i]] = true;
			}
			else if (adj[i] != temp.second && adj[i] != temp.first) return true;
		}
	}
	return false;;
}

template<typename T>
std::vector<T> Graph<T>::indexesToT(std::vector<size_t> indexes)const
{
	std::vector<T> forReturn;
	for (size_t i = 0;i < indexes.size();i++)
		forReturn.push_back(m_vertices[indexes[i]]);
	return std::move(forReturn);
}

template<typename T>
std::vector<T> Graph<T>::parentsToPath(std::vector<size_t> parents, size_t startIndex, size_t endIndex)const
{
	std::vector<size_t> path;
	path.push_back(endIndex);
	size_t iterator = parents[endIndex];

	while (iterator != startIndex)
	{
		path.push_back(iterator);
		iterator = parents[iterator];
	}
	if (startIndex != endIndex) path.push_back(startIndex);

	for (size_t i = 0; i < path.size() / 2;i++)
		std::swap(path[i], path[path.size() - i - 1]);
	return std::move(indexesToT(path));
}

template<typename T>
void Graph<T>::dfs1StrongComponentsHelp(size_t vertex, std::vector<bool>& visited, std::stack<size_t>& s)const
{
	if (visited[vertex]) return;
	visited[vertex] = true;
	std::vector<size_t> adj = neighboursIndex(vertex);
	for (size_t i = 0;i < adj.size();i++)
		if (!visited[adj[i]]) dfs1StrongComponentsHelp(adj[i], visited, s);
	s.push(vertex);
}

template<typename T>
void Graph<T>::dfs2StrongComponentsHelp(size_t vertex, std::vector<bool>& visited, std::vector<size_t>& components, size_t comCount)const
{
	if (visited[vertex]) return;
	visited[vertex] = true;
	components[vertex] = comCount;
	std::vector<size_t> adj = neighboursIndex(vertex);
	for (size_t i = 0;i < adj.size();i++)
		if (!visited[adj[i]]) dfs2StrongComponentsHelp(adj[i], visited, components, comCount);
}

template<typename T>
std::vector < std::vector<T>> Graph<T>::makeComponents(const std::vector<size_t>& components)const
{
	size_t max = components[0];
	for (size_t i = 1;i < components.size();i++)
		if (max < components[i]) max = components[i];

	std::vector<std::vector<T>> forReturn;
	for (size_t i = 0;i <= max;i++)
		forReturn.push_back(std::vector<T>());

	for (size_t i = 1;i < components.size();i++)
		forReturn[components[i]].push_back(m_vertices[i]);

	return std::move(forReturn);
}

template<typename T>
Graph<T>::Graph(bool isOriented) :m_isOriented(isOriented) {}

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
		m_edges.push_back(Edge(fst_index, snd_index, weight));
		if (!m_isOriented && !(fst == snd)) m_edges.push_back(Edge(snd_index, fst_index, weight));
		return true;
	}
	catch (...)
	{
		return false;
	}
}

template<typename T>
T& Graph<T>::operator[](const size_t& i)
{
	return m_vertices[i];
}

template<typename T>
const T& Graph<T>::operator[](const size_t& i)const
{
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
		if (m_vertices[m_edges[i].fst] == elem)
			_neighbours[m_edges[i].snd] = true;

	std::vector<T> forReturn;
	for (size_t i = 0;i < _neighbours.size();i++)
		if (_neighbours[i]) forReturn.push_back(m_vertices[i]);
	return std::move(forReturn);
}

template<typename T>
std::vector<size_t> Graph<T>::neighboursIndex(const size_t& index)const
{
	std::vector<bool> adj;
	for (size_t i = 0;i < m_vertices.size();i++)
		adj.push_back(false);

	for (size_t i = 0;i < m_edges.size();i++)
		if (m_edges[i].first == index)
			adj[m_edges[i].second] = true;

	std::vector<size_t> indexes;
	for (size_t i = 0;i < adj.size();i++)
		if (adj[i])
			indexes.push_back(i);
	return std::move(indexes);
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
		if (m_vertices[m_edges[i].first] == fst && m_vertices[m_edges[i].second] == snd)
			return true;
	return false;
}

template<typename T>
std::vector<T> Graph<T>::bfs(const T& start, const T& end)const
{
	size_t startIndex = indexOf(start), endIndex = indexOf(end);
	std::queue<size_t> _queue;
	_queue.push(startIndex);

	std::vector<bool> visited;
	std::vector<size_t> parent;
	for (size_t i = 0;i < m_vertices.size();i++)
	{
		visited.push_back(false);
		parent.push_back(0);
	}
	visited[startIndex] = true;

	while (!_queue.empty())
	{
		size_t curr = _queue.front();
		_queue.pop();
		if (curr == endIndex) return std::move(parentsToPath(parent, startIndex, endIndex));

		std::vector<size_t> adj = neighboursIndex(curr);
		for (size_t i = 0;i < adj.size();i++)
		{
			if (!visited[adj[i]])
			{
				_queue.push(adj[i]);
				visited[adj[i]] = true;
				parent[adj[i]] = curr;
			}
		}
	} throw std::logic_error("cant find path from start to end");
}

template<typename T>
bool Graph<T>::isCyclic()const
{
	if (m_vertices.size() < 3) return false;
	for (size_t i = 0;i < m_vertices.size();i++)
		if (hasCycleFrom(i)) return true;
	return false;
}

template<typename T>
bool Graph<T>::isConnected()const
{
	if (m_vertices.size() == 0) return false;
	std::stack<size_t> s;
	s.push(0);//random vertex

	std::vector<bool> visited;
	for (size_t i = 0;i < m_vertices.size();i++)
		visited.push_back(false);
	visited[0] = true;

	while (!s.empty())
	{
		size_t curr = s.top();
		s.pop();
		std::vector<size_t> adj = neighboursIndex(curr);
		for (size_t i = 0;i < adj.size();i++)
			if (!visited[adj[i]])
			{
				s.push(adj[i]);
				visited[adj[i]] = true;
			}
	}
	for (size_t i = 0;i < visited.size();i++)
		if (!visited[i]) return false;
	return true;
}

template<typename T>
bool Graph<T>::isBipartite()const
{
	short constexpr white = 0, blue = 1, red = 2;

	if (!isConnected()) throw std::logic_error("Only Connected graphs can be bipartite");
	std::queue<size_t> q;
	q.push(0); //random vertex

	std::vector<int> color;
	for (size_t i = 0;i < m_vertices.size();i++)
		color.push_back(white);
	color[0] = blue;

	while (!q.empty())
	{
		size_t curr = q.front();
		q.pop();

		std::vector<size_t> adj = neighboursIndex(curr);
		for (size_t i = 0;i < adj.size();i++)
		{
			if (color[adj[i]] == color[curr]) return false;
			if (color[adj[i]] == white)
			{
				q.push(adj[i]);
				if (color[curr] == blue)color[adj[i]] = red;
				else color[adj[i]] = blue;
			}
		}
	}
	return true;
}

template<typename T>
void Graph<T>::reverseEdges()
{
	for (size_t i = 0;i < m_edges.size();i++)
	{
		Edge temp(m_edges[i].second, m_edges[i].first, m_edges[i].weight);
		std::swap(m_edges[i], temp);
	}
}

template<typename T>
std::vector<std::vector<T>> Graph<T>::strongConnectedComponents()const
{
	std::stack<size_t> _stack;
	size_t componentNum = 0;
	std::vector<bool> visited;
	std::vector<size_t> components;
	for (size_t i = 0;i < m_vertices.size();i++)
	{
		components.push_back(0);
		visited.push_back(false);
	}

	for (size_t i = 0;i < m_vertices.size();i++)
		dfs1StrongComponentsHelp(i, visited, _stack);

	Graph<T> gReverse = *this;
	gReverse.reverseEdges();
	for (size_t i = 0;i < visited.size();i++)
		visited[i] = false;

	while (!_stack.empty())
	{
		size_t curr = _stack.top();
		_stack.pop();
		if (!visited[curr])
		{
			gReverse.dfs2StrongComponentsHelp(curr, visited, components, componentNum);
			componentNum++;
		}
	}
	return std::move(makeComponents(components));
}