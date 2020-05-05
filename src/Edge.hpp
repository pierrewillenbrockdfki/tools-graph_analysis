#ifndef GRAPH_ANALYSIS_EDGE_HPP
#define GRAPH_ANALYSIS_EDGE_HPP

#include "Vertex.hpp"
#include <string>

namespace graph_analysis
{

class BaseGraph;
class EdgeTypeManager;

/**
 * \brief An Edge represents the link between two vertices
 * \details For the purpose of storing data in a edge, the storage type needs to be defined by inheriting
 * from this class, some default implementations exists
 */
class Edge : public GraphElement
{
public:
    typedef shared_ptr< Edge > Ptr;
    typedef shared_ptr< const Edge > ConstPtr;
    typedef std::vector< Ptr > PtrList;
    typedef std::set< Ptr > PtrSet;

    Edge(const std::string& label = std::string());

    Edge(const Vertex::Ptr& source, const Vertex::Ptr& target, const std::string& label = std::string());

    virtual ~Edge() {}

    /**
     * Clone this edge -- the returned edge will have no
     * graph association
     */
    Edge::Ptr clone() const;

    // Get class name
    // \return class name
    virtual std::string getClassName() const;

    /**
     * Convert element to string
     * \return string representation of edge
     */
    virtual std::string toString() const;

    /**
     * Convert element to string
     * \return string representation of edge
     */
    virtual std::string toString(uint32_t indent) const;

    /**
     * Get the source vertex
     */
    Vertex::Ptr getSourceVertex() const { return mSourceVertex; }

    /**
     * Get the target vertex
     */
    Vertex::Ptr getTargetVertex() const { return mTargetVertex; }

    /**
     * Set the source vertex
     */
    void setSourceVertex(const Vertex::Ptr& source) { mSourceVertex = source; }

    /**
     * Set the target vertex
     */
    void setTargetVertex(const Vertex::Ptr& target) { mTargetVertex = target; }

    /**
     * Get all involved vertices for two edges
     * \return all distinct vertices of two edges
     */
    static std::vector<Vertex::Ptr> getInvolvedVertices(const Edge::Ptr& e0, const Edge::Ptr& e1);

    /**
     * Check if two edges meet at a vertex
     * \return true if edges meet at least one vertex
     */
    static bool areMeeting(const Edge::Ptr& e0, const Edge::Ptr& e1);

    /**
     * Deprecated function
     * in order for child-classes to iterate the graph, a way to obtain the
     * shared pointer actually respresenting this edge on a particular graph is
     * needed.
     *
     * \param pGraph shared pointer to the graph where this edge is contained
     * \return shared pointer of this edge belonging to the given graph
     */
    Edge::Ptr getSharedPointerFromGraph(const shared_ptr<BaseGraph> &pGraph) const;

    /**
     * Get the shared pointer for this object
     */
    Edge::Ptr getPtr();

    /**
     * Get the shared pointer for this object -- do not use on stack allocated
     * objects
     */
    Edge::ConstPtr getPtr() const;

    /**
     * Hook to register attributes of this vertex, in order to properly
     * perform serialization
     *
     * If serialization is not needed, then registration is optional
     * Otherwise each Edge implementation has to implement this
     * method, and add a static member
     \verbatim
     #include "EdgeRegistration.hpp"

     class DerivedEdge : Edge
     {
        static const EdgeRegistration<DerivedEdge> __attribute__((used)) msRegistration;
     }
     \endverbatim
     * in the source file, e.g. DerivedEdge.cpp add
     \verbatim
     const EdgeRegistration<DerivedEdge> DerivedEdge::msRegistration;
     \endverbatim
     */
    virtual void registerAttributes(EdgeTypeManager*) const {}
protected:
    /**
     * Get instance of an edge
     */
    virtual Edge* getClone() const { return new Edge(*this); }

private:
    Vertex::Ptr mSourceVertex;
    Vertex::Ptr mTargetVertex;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_EDGE_HPP
