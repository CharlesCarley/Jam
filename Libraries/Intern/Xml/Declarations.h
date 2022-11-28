#pragma once
#include "Utils/ScopePtr.h"

namespace Jam::Xml
{
    class Node;
    class Attribute;
    class File;

}  // namespace Jam::Xml

namespace Jam
{
    using XmlNode = Xml::Node;
    using XmlFile = Xml::File;


    using XmlPtr = ScopePtr<Xml::Node*>;

}  // namespace Jam
