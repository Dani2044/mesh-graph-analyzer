// Self-contained test harness for the Mesh Graph Analyzer core.
//
// Verifies the KD-Tree nearest-neighbour search against a brute-force
// O(n) scan over a synthetic mesh, and runs a Dijkstra sanity check on
// a unit cube. Plain asserts only: no external test framework.
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

#include "../Grafo.h"
#include "../KDTree.h"
#include "../NodoKDTree.h"
#include "../objeto.h"
#include "../vertice.h"

namespace {

int checks = 0;
int failures = 0;

#define CHECK(cond)                                                        \
  do {                                                                     \
    ++checks;                                                              \
    if (!(cond)) {                                                         \
      ++failures;                                                          \
      std::cerr << "FAIL " << __FILE__ << ":" << __LINE__ << ": " #cond     \
                << "\n";                                                   \
    }                                                                      \
  } while (0)

double bruteForceNearest(const std::vector<Vertice>& points, const Vertice& q) {
  double best = std::numeric_limits<double>::max();
  for (const Vertice& p : points) best = std::min(best, p.distancia(q));
  return best;
}

// 300 random points in [-10,10]^3; 50 random queries. The point returned by
// the KD-Tree must be exactly as close as the brute-force minimum.
void testKDTreeNearestNeighbor() {
  std::mt19937 rng(42);
  std::uniform_real_distribution<double> dist(-10.0, 10.0);

  std::vector<Vertice> points;
  KDTree tree;
  for (int i = 0; i < 300; ++i) {
    Vertice v(dist(rng), dist(rng), dist(rng));
    points.push_back(v);
    tree.insertar(v, "synthetic");
  }
  CHECK(points.size() == 300);

  for (int i = 0; i < 50; ++i) {
    Vertice query(dist(rng), dist(rng), dist(rng));
    NodoKDTree* node = tree.encontrarMasCercano(query);
    CHECK(node != nullptr);
    if (!node) continue;
    double kdDistance = node->getPunto().distancia(query);
    double bruteDistance = bruteForceNearest(points, query);
    CHECK(std::fabs(kdDistance - bruteDistance) < 1e-9);
  }
}

// Unit cube, 8 corners and 6 quad faces. The shortest edge-to-edge path from
// vertex 0 (0,0,0) to vertex 6 (0,1,1) runs 0 -> 2 -> 6 with total length 2.
Grafo buildUnitCubeGraph() {
  std::vector<Vertice> verts;
  for (int z = 0; z <= 1; ++z)
    for (int y = 0; y <= 1; ++y)
      for (int x = 0; x <= 1; ++x)
        verts.push_back(Vertice(x == 1 ? 1.0 : 0.0,
                                y == 1 ? 1.0 : 0.0,
                                z == 1 ? 1.0 : 0.0));

  std::vector<std::vector<int>> faceIdx = {
      {0, 1, 3, 2}, {4, 5, 7, 6}, {0, 2, 6, 4},
      {1, 3, 7, 5}, {0, 1, 5, 4}, {2, 3, 7, 6},
  };

  Objeto cubo("cubo", verts, {});
  for (const std::vector<int>& f : faceIdx) {
    Cara cara;
    for (int idx : f) cara.agregarVertice(verts[idx]);
    cubo.agregarCara(cara);
  }
  return Grafo(cubo);
}

void testDijkstraOnCube() {
  Grafo grafo = buildUnitCubeGraph();
  CHECK(grafo.getNumVertices() == 8);

  std::vector<double> distancias = grafo.Dijsktra(0);
  CHECK(std::fabs(distancias[6] - 2.0) < 1e-9);

  std::vector<int> ruta = grafo.obtenerRuta(0, 6);
  CHECK(ruta.size() >= 2);
  if (!ruta.empty()) CHECK(ruta.front() == 0);
  CHECK(ruta.back() == 6);

  double longitud = 0.0;
  for (size_t i = 0; i + 1 < ruta.size(); ++i) {
    longitud += grafo.getVertices()[ruta[i]].distancia(grafo.getVertices()[ruta[i + 1]]);
  }
  CHECK(std::fabs(longitud - 2.0) < 1e-9);
}

}  // namespace

int main() {
  testKDTreeNearestNeighbor();
  testDijkstraOnCube();

  std::cout << checks << " checks, " << failures << " failures\n";
  return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}