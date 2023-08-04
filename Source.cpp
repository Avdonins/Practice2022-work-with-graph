#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

void create_graph(vector<vector<int>>& graph0, vector<vector<int>>& graph1,
                  vector<vector<int>>& graph2) {
  random_device random_device;
  mt19937 generator(random_device());
  uniform_int_distribution<> distribution(0, 1);

  for (int i = 0; i < (graph0.size() - 1); i++) {
    for (int j = (i + 1); j < graph0.size(); j++) {
      if (distribution(generator)) {
        graph1[i].push_back(j + 1);
        graph2[j][i] = 0;
      } else {
        graph0[i].push_back(j + 1);
        graph2[i][j] = 0;
      }
    }
  }
}

void show_graph(vector<vector<int>>& graph) {
  cout << "{ ";
  for (int i = 0; i < graph.size(); i++) {
    cout << "{ ";
    for (int j = 0; j < graph[i].size(); j++) {
      cout << graph[i][j] << " ";
    }
    cout << "} ";
  }
  cout << " }" << endl;
}

bool dfs(vector<vector<int>>& graph, int start, int finish) {
  if (start == finish) {
    return true;
  } else if (start > finish)
    return false;
  for (int j = 0; j < graph[start].size(); j++) {
    if (dfs(graph, graph[start][j], finish)) {
      return true;
    }
  }
  return false;
}

bool dfs_cycle(vector<vector<int>>& graph, int start, vector<bool>& used) {
  if (used[start]) {
    return true;
  }
  used[start] = true;
  for (int i = 0; i < graph[start].size(); i++) {
    if (graph[start][i] != -1)
      if (dfs_cycle(graph, i, used)) {
        return true;
      }
  }
  used[start] = false;
  return false;
}

bool is_optimal(vector<vector<int>>& graph1, vector<vector<int>>& graph2) {
  for (int i = 0; i < graph1.size(); i++) {
    for (int j = (i + 1); j < graph1.size(); j++) {
      if (dfs(graph1, i, j) && dfs(graph2, i, j)) {
        return false;
      }
    }
  }
  return true;
}

bool is_optimal_for_graph_with_all_roads(vector<vector<int>>& graph) {
  vector<bool> used(graph.size());
  for (int i = 0; i < graph.size(); i++) {
    if (dfs_cycle(graph, i, used)) {
      return false;
    }
    used.assign(used.size(), false);
  }
  return true;
}

int main() {
  setlocale(LC_ALL, "");
  system("color F0");
  int count_v = 0;
  cout << "Кол-во вершин: ";
  cin >> count_v;

  cout << "|===============||===========||=====================||============"
          "=="
          "=========|"
       << endl
       << "| Кол-во вершин ||   Метод   ||      Результат      || Время "
          "выполнения (ms) |"
       << endl
       << "|===============||===========||=====================||============"
          "=="
          "=========|"
       << endl;

  vector<vector<int>> graph_with_types_0(count_v);
  vector<vector<int>> graph_with_types_1(count_v);
  vector<vector<int>> graph_with_all_roads(count_v, vector<int>(count_v, -1));

  create_graph(graph_with_types_0, graph_with_types_1, graph_with_all_roads);

  cout << "| " << setw(13) << count_v << " || "
       << "Два графа"
       << " || ";

  auto t0 = chrono::high_resolution_clock::now();
  if (is_optimal(graph_with_types_0, graph_with_types_1))
    cout << "Граф оптимальный";
  else
    cout << "Граф не оптимальный";
  auto t1 = chrono::high_resolution_clock::now();
  std::chrono::duration<double, milli> fp_ms = t1 - t0;

  cout << " || " << setw(21) << fixed << setprecision(5) << fp_ms.count()
       << " | " << endl
       << "| " << setw(13) << count_v << " || "
       << "Один граф"
       << " || ";

  t0 = chrono::high_resolution_clock::now();
  if (is_optimal_for_graph_with_all_roads(graph_with_all_roads))
    cout << "Граф оптимальный";
  else
    cout << "Граф не оптимальный";
  t1 = chrono::high_resolution_clock::now();
  fp_ms = t1 - t0;

  cout << " || " << setw(21) << fp_ms.count() << " | " << endl
       << "|===============||===========||=====================||============"
          "=="
          "=========|"
       << endl;

  graph_with_all_roads.clear();
  graph_with_types_0.clear();
  graph_with_types_1.clear();

  cout << endl << "runtime = " << clock() / 1000.0 << endl;
  return 0;
}