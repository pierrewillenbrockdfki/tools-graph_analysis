#include "GraphIO.hpp"
#include <sstream>
#include <boost/regex.hpp>
#include <boost/assign/list_of.hpp>

#include "io/GexfWriter.hpp"
#include "io/GexfReader.hpp"
#include "io/YamlWriter.hpp"
#include "io/YamlReader.hpp"
#include "io/GraphvizWriter.hpp"

#include <base/Logging.hpp>

namespace graph_analysis {
namespace representation {

std::map<Type, std::string> TypeTxt = boost::assign::map_list_of
    (UNKNOWN, "UNKNOWN")
    (GEXF, "GEXF")
    (LEMON, "LEMON")
    (GRAPHVIZ, "GRAPHVIZ")
    (YAML, "YAML")
    ;

} // end namespace representation

namespace io {

struct null_deleter
{
        void operator()(void const *) const {}
};

void Writer::write(const std::string& filename, const BaseGraph& graph) const
{
    BaseGraph::Ptr graphPtr(const_cast<BaseGraph*>(&graph), null_deleter());
    write(filename, graphPtr);
}

void Reader::read(const std::string& filename, BaseGraph& graph)
{
    BaseGraph::Ptr graphPtr(&graph, null_deleter());
    read(filename, graphPtr);
}


GraphIO::WriterMap GraphIO::msWriters = boost::assign::map_list_of
    (representation::GEXF, Writer::Ptr( new GexfWriter()))
    (representation::GRAPHVIZ, Writer::Ptr( new GraphvizWriter()))
    (representation::YAML, Writer::Ptr(new YamlWriter()))
    ;

GraphIO::ReaderMap GraphIO::msReaders = boost::assign::map_list_of
    (representation::GEXF, Reader::Ptr( new GexfReader()))
    (representation::YAML, Reader::Ptr( new YamlReader()))
    ;

std::map<representation::Suffix, representation::Type> GraphIO::msSuffixes = boost::assign::map_list_of
    ("yaml", representation::YAML)
    ("yml", representation::YAML)
    ("gexf", representation::GEXF)
    ("xml", representation::GEXF)
    ("lemon", representation::LEMON)
    ("dot", representation::GRAPHVIZ)
    ;


void GraphIO::write(const std::string& filename, const BaseGraph& graph, representation::Type format)
{
    if(format == representation::UNKNOWN)
    {
        format = getTypeFromFilename(filename);
        if(format == representation::UNKNOWN)
        {
            throw std::invalid_argument("graph_analysis::GraphIO::write: "
                " write to file '" + filename + "' failed since filename has no meaningful suffix and no format is given");
        }
    }

    WriterMap::const_iterator cit = msWriters.find(format);
    if(cit != msWriters.end())
    {
        std::string filenameWithSuffix = GraphIO::appendSuffix(filename, format);

        Writer::Ptr writer = cit->second;
        writer->write(filenameWithSuffix, graph);
    } else {
        std::stringstream ss;
        ss << "GraphIO: writing format ";
        ss << "' " << representation::TypeTxt[format] << "'";
        ss << " is not supported";

        throw std::runtime_error(ss.str());
    }
}

void GraphIO::write(const std::string& filename, const BaseGraph::Ptr& graph, representation::Type format)
{
    write(filename, *graph.get(), format);
}

void GraphIO::read(const std::string& filename, BaseGraph& graph, representation::Type format)
{
    if(format == representation::UNKNOWN)
    {
        format = getTypeFromFilename(filename);
        if(format == representation::UNKNOWN)
        {
            throw std::invalid_argument("graph_analysis::GraphIO::read: "
                " reading from file '" + filename + "' failed since filename has no meaningful suffix and no format is given");
        }
    }

    ReaderMap::const_iterator cit = msReaders.find(format);
    if(cit != msReaders.end())
    {
        Reader::Ptr reader = cit->second;
        reader->read(filename, graph);
    } else {
        std::stringstream ss;
        ss << "GraphIO: reading format ";
        ss << "' " << representation::TypeTxt[format] << "'";
        ss << " is not supported";

        throw std::runtime_error(ss.str());
    }
}

void GraphIO::read(const std::string& filename, BaseGraph::Ptr graph, representation::Type format)
{
    read(filename, *graph.get(), format);
}

representation::Type GraphIO::getTypeFromSuffix(representation::Suffix suffix)
{
    SuffixMap::const_iterator cit = msSuffixes.find(suffix);
    if(cit != msSuffixes.end())
    {
        return cit->second;
    }
    throw std::invalid_argument("graph_analysis::GraphIO::getTypeFromSuffix: "
        " unknown suffix '" + suffix + "'");
}

representation::Type GraphIO::getTypeFromFilename(const std::string& filename)
{
    boost::regex expression(".*\\.([a-z]+$)");

    boost::cmatch what;
    if(boost::regex_match(filename.c_str(), what, expression))
    {
        std::string suffix(what[1].first, what[1].second);
        LOG_DEBUG_S << "Found suffix of filename '" << filename << "' : " << suffix;
        return getTypeFromSuffix(suffix);
    } else {
        LOG_DEBUG_S << "No suffix found in filename '" << filename << "'";
    }
    return representation::UNKNOWN;
}

std::string GraphIO::appendSuffix(const std::string& filename, representation::Type format)
{
    representation::Type fileType = getTypeFromFilename(filename);
    if(fileType != representation::UNKNOWN && (format == representation::UNKNOWN || format == fileType))
    {
        // Filename already comes with suffix -- and no other format is
        // requested
        return filename;
    }

    if(fileType == representation::UNKNOWN && format != representation::UNKNOWN)
    {
        // ok append
        std::string filenameWithSuffix = filename + "." + getSuffix(format);
        return filenameWithSuffix;
    }

    throw std::invalid_argument("graph_analysis::GraphIO::appendSuffix: "
                " could not append suffix for format '" + representation::TypeTxt[format] + "' to '" + filename + "'");
}

representation::Suffix GraphIO::getSuffix(representation::Type format)
{
    SuffixMap::const_iterator cit = msSuffixes.begin();
    for(; cit != msSuffixes.end(); ++cit)
    {
        if(cit->second == format)
        {
            return cit->first;
        }
    }
    throw std::runtime_error("graph_analysis::GraphIO::getSuffix: "
            " could not retrieve suffix for format '" + representation::TypeTxt[format] + "'");
}

} // end namespace io
} // end namespace graph_analysis
