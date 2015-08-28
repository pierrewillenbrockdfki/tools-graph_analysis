#ifndef GRAPH_ANALYSIS_INPUTPORTVERTEX_HPP
#define GRAPH_ANALYSIS_INPUTPORTVERTEX_HPP

#include <iostream>
#include <graph_analysis/PortVertex.hpp>
#include <graph_analysis/GraphElement.hpp>

namespace graph_analysis
{

/**
 * \brief An InputPort vertex inherited to allow storing data of input Port type
 * \details For the purpose of storing data with a vertex, this storage type inherits
 * from class PortVertex
 */
class InputPortVertex : public PortVertex
{
public:
    InputPortVertex(const std::string& label = "");
    /** Get class name
     * \return class name
     */
    virtual std::string getClassName() const { return "graph_analysis::InputPortVertex"; }

    /** Get class type
     * \return class type
     */
    static std::string vertexType() { return "graph_analysis::InputPortVertex"; }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual InputPortVertex* getClone() const { return new InputPortVertex(*this); }

};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_INPUTPORTVERTEX_HPP
