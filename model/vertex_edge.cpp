#include "vertex_edge.h"

using namespace vp;

void Vertex::transform(const TransformMatrix& m) { m.transformPoint(*this); }

std::pair<Vertex*, Vertex*> Edge::ordered() const {
  return (_begin <= _end) ? std::make_pair(_begin, _end)
                          : std::make_pair(_end, _begin);
}

Edge::Edge(Vertex* begin, Vertex* end) : _begin(begin), _end(end) {}

bool Edge::operator<(const Edge& other) const {
  auto [a1, a2] = this->ordered();
  auto [b1, b2] = other.ordered();

  return (a1 < b1) || (a1 == b1 && a2 < b2);
}

bool Edge::operator==(const Edge& other) const {
  auto [a1, a2] = this->ordered();
  auto [b1, b2] = other.ordered();
  return a1 == b1 && a2 == b2;
}

const Vertex* Edge::getBeginPosition() const { return _begin; }

const Vertex* Edge::getEndPosition() const { return _end; }

size_t Edge::HashEdge::operator()(const Edge& e) const {
  auto [p1, p2] = e.ordered();
  return std::hash<Vertex*>{}(p1) ^ (std::hash<Vertex*>{}(p2) << 1);
}
