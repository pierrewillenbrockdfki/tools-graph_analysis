#include "VertexTypeManager.hpp"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#include <boost/assign/list_of.hpp>
#include <base/Logging.hpp>

namespace graph_analysis {

VertexTypeManager::VertexTypeManager()
{
    // registering known implemented class-types
    mClassVisualizationMap = boost::assign::map_list_of
        ("base",    Vertex::Ptr ((Vertex *) (new BaseVertex())))
        ("graph_analysis::BaseVertex",    Vertex::Ptr ((Vertex *) (new BaseVertex())))
        ("port",    Vertex::Ptr ((Vertex *) (new PortVertex())))
        ("graph_analysis::PortVertex",    Vertex::Ptr ((Vertex *) (new PortVertex())))
        ("inputport",    Vertex::Ptr ((Vertex *) (new InputPortVertex())))
        ("graph_analysis::InputPortVertex",    Vertex::Ptr ((Vertex *) (new InputPortVertex())))
        ("outputport",    Vertex::Ptr ((Vertex *) (new OutputPortVertex())))
        ("graph_analysis::OutputPortVertex",    Vertex::Ptr ((Vertex *) (new OutputPortVertex())))
        ("cluster",    Vertex::Ptr ((Vertex *) (new ClusterVertex())))
        ("graph_analysis::ClusterVertex",    Vertex::Ptr ((Vertex *) (new ClusterVertex())))
        ("property",    Vertex::Ptr ((Vertex *) (new PropertyVertex())))
        ("graph_analysis::PropertyVertex",    Vertex::Ptr ((Vertex *) (new PropertyVertex())))
        ("operation",    Vertex::Ptr ((Vertex *) (new OperationVertex())))
        ("graph_analysis::OperationVertex",    Vertex::Ptr ((Vertex *) (new OperationVertex())))
        ;
    // initializing the list of registered types (non-repeatingly, non-verbously)
    mRegisteredTypes.clear();
    ClassVisualizationMap::iterator it = mClassVisualizationMap.begin();
    for(; mClassVisualizationMap.end() != it; ++it)
    {
        if("graph_analysis::" != it->first.substr(0, std::string("graph_analysis::").length())) // filters out verbose (&& duplicate!) types
        {
            mRegisteredTypes.insert(it->first);
        }
    }
}

VertexTypeManager::~VertexTypeManager()
{
}

void VertexTypeManager::registerType(const vertex::Type& type, Vertex::Ptr node, bool throwOnAlreadyRegistered)
{
    try {
        vertexByType(type, true);
    } catch(...)
    {
        mClassVisualizationMap[type] = node;
        mRegisteredTypes.insert(type);
    }
    LOG_WARN_S << "graph_analysis::VertexTypeManager::registerType: type '" + type + "' is already registered.";
    if(throwOnAlreadyRegistered)
    {
        throw std::runtime_error("graph_analysis::VertexTypeManager::registerType: type '" + type + "' is already registered");
    }
}

Vertex::Ptr VertexTypeManager::vertexByType(const vertex::Type& type, bool throwOnDefault)
{
    ClassVisualizationMap::iterator it = mClassVisualizationMap.find(type);
    if(it == mClassVisualizationMap.end())
    {
        LOG_DEBUG_S << "graph_analysis::VertexTypeManager::vertexByType: type '" + type + "' is not registered. Using default type 'base'.";
        if(throwOnDefault)
        {
            throw std::runtime_error("graph_analysis::VertexTypeManager::vertexByType: type '" + type + "' is not registered");
        }
        return mClassVisualizationMap["base"];
    }

    LOG_DEBUG_S << "graph_analysis::VertexTypeManager::vertexByType: type '" + type + "' registered.";
    return it->second;
}

Vertex::Ptr VertexTypeManager::createVertex(const vertex::Type& type, const std::string& label)
{
    Vertex::Ptr clonedVertex = vertexByType(type)->clone();
    clonedVertex->setLabel(label);
    return clonedVertex;
}

std::set<std::string> VertexTypeManager::getSupportedTypes()
{
    return mRegisteredTypes;
}

} // end namespace graph_analysis
