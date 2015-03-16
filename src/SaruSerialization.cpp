/*
Copyright (c) 2012 Christopher Higgins Barrett

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "SaruSerialization.hpp"

namespace CSaruSerial {

//=========================================================================
SerializeToDataMap::SerializeToDataMap(CSaruContainer::DataMapMutator * mutator)
    : m_mutator(mutator)
{}

//=========================================================================
bool SerializeToDataMap::PushGroup(const char * name) {
    if (m_mutator->GetCurrentNode()->GetType() != CSaruContainer::DataNode::Type::Unused)
        m_mutator->Walk(1);
    
    m_mutator->SetToArrayType();
    m_mutator->WriteName(name);
    m_mutator->CreateAndGotoChild();
    
    return true;
}

//=========================================================================
bool SerializeToDataMap::PopGroup(void) {
    if (m_mutator->GetCurrentNode()->GetType() == CSaruContainer::DataNode::Type::Unused) {
        m_mutator->ToParent();
        m_mutator->GetCurrentNode()->DeleteLastChild();
    }
    else {
        m_mutator->ToParent();
    }
    
    return true;
}

//=========================================================================
bool SerializeToDataMap::Serialize(bool * b, const char * name) {
    CommonSerializationPrep(name);
    
    if (name)
        m_mutator->Write(name, *b);
    else
        m_mutator->Write("", *b);
    
    return true;
}

//=========================================================================
bool SerializeToDataMap::Serialize(int * i, const char * name) {
    CommonSerializationPrep(name);
    
    if (name)
        m_mutator->Write(name, *i);
    else
        m_mutator->Write("", *i);
    
    return true;
}

//=========================================================================
bool SerializeToDataMap::Serialize(float * f, const char * name) {
    CommonSerializationPrep(name);
    
    if (name)
        m_mutator->Write(name, *f);
    else
        m_mutator->Write("", *f);
    
    return true;
}

//=========================================================================
bool SerializeToDataMap::Serialize(char * str, size_t str_len, const char * name) {
    CommonSerializationPrep(name);
    
#if defined(_DEBUG)
    if (name)
        m_mutator->WriteSafe(name, strlen(name), str, str_len);
    else
        m_mutator->WriteSafe("", 0, str, str_len);
#else
    if (name)
        m_mutator->Write(name, str);
    else
        m_mutator->Write("", str);
#endif
    
    return true;
}

//=========================================================================
void SerializeToDataMap::CommonSerializationPrep(const char * name) {
    // if there's already data, walk to a new spot before serializing the new
    //   variable.
    if (m_mutator->GetCurrentNode()->GetType() != CSaruContainer::DataNode::Type::Unused)
    {
        m_mutator->Walk(1);
    }
    // otherwise, this is the first piece of data to be written in this container
    else
    {
        // if there's a name, then we need to switch from Array to Object type
        if (name)
            m_mutator->GetParentNode()->SetType(CSaruContainer::DataNode::Type::Object);
    }
    
    // if a name is being written, and this is the second piece of data or
    //   later, we should check if the previous had a name or not.  There
    //   shouldn't be a mix of named and unnamed data in the same container.
#if defined(_DEBUG)
    if (name &&
        m_mutator->GetParentNode()->GetType() == CSaruContainer::DataNode::Type::Array &&
        m_mutator->GetParentNode()->GetChildCount() > 1)
    {
        assert(0 && "Named data given in Array container.  All data in a "
                    "container should be either named or unnamed.");
    }
    else if (name == nullptr &&
             m_mutator->GetParentNode()->GetType() == CSaruContainer::DataNode::Type::Object &&
             m_mutator->GetParentNode()->GetChildCount() > 1)
    {
        assert(0 && "Unnamed data given in Object container.  All data in a "
                    "container should be either named or unnamed.");
    }
#endif
}

//=========================================================================
SerializeFromDataMap::SerializeFromDataMap(CSaruContainer::DataMapReader * reader)
        : m_reader(reader)
{}

//=========================================================================
bool SerializeFromDataMap::PushGroup(const char * name) {
    // mis-matching name and node?
    if (name && strcmp(m_reader->ReadName(), name))
        return false;
    
    m_reader->ToFirstChild();
    if (m_reader->GetCurrentNode() == NULL) {
        m_reader->PopNode();
        return false;
    }
    
    return true;
}

//=========================================================================
bool SerializeFromDataMap::PopGroup(void) {
    // if the current node is nullptr, we should be at (after) the end of the
    //   current container.  In that case, one additional pop is needed (to get
    //   back into the container).
    /*
    if (m_reader->GetCurrentNode() == nullptr)
        m_reader->PopNode();
    //*/
    
    // pop to the container we were in.  Walk to the next sibling in preparation
    //   of the next read.
    //do
    //{
        m_reader->PopNode();
        m_reader->ToNextSibling();
    //} while (m_reader->GetCurrentNode() == nullptr && m_reader->GetCurrentDepth());
    
    return m_reader->GetCurrentNode() != nullptr;
}

//=========================================================================
bool SerializeFromDataMap::Serialize(bool * b, const char * name) {
    // mis-matching name and node?
    if (name && strcmp(m_reader->ReadName(), name))
        return false;
    
    /*
    if (name) {
        if (!m_reader->GetCurrentNode()->IsContainerType())
            m_reader->PopNode();
        
        m_reader->ToChild(name);
        // did the child not exist?
        if (m_reader->GetCurrentNode() == nullptr) {
            m_reader->PopNode();
            return false;
        }
    }
    */
    
    return m_reader->ReadBoolWalkSafe(b);
}

//=========================================================================
bool SerializeFromDataMap::Serialize(int * i, const char * name) {
    // mis-matching name and node?
    if (name && strcmp(m_reader->ReadName(), name))
        return false;
    
    /*
    if (name) {
        if (!m_reader->GetCurrentNode()->IsContainerType())
            m_reader->PopNode();
        
        m_reader->ToChild(name);
        // did the child not exist?
        if (m_reader->GetCurrentNode() == nullptr) {
            m_reader->PopNode();
            return false;
        }
    }
    */
    
    return m_reader->ReadIntWalkSafe(i);
}

//=========================================================================
bool SerializeFromDataMap::Serialize(float * f, const char * name) {
    // mis-matching name and node?
    if (name && strcmp(m_reader->ReadName(), name))
        return false;
    
    /*
    if (name) {
        if (!m_reader->GetCurrentNode()->IsContainerType())
            m_reader->PopNode();
        
        m_reader->ToChild(name);
        // did the child not exist?
        if (m_reader->GetCurrentNode() == nullptr) {
            m_reader->PopNode();
            return false;
        }
    }
    */
    
    return m_reader->ReadFloatWalkSafe(f);
}

//=========================================================================
bool SerializeFromDataMap::Serialize(char * str, size_t str_len, const char * name) {
    // mis-matching name and node?
    if (name && strcmp(m_reader->ReadName(), name))
        return false;
    
    /*
    if (name) {
        if (!m_reader->GetCurrentNode()->IsContainerType())
            m_reader->PopNode();
        
        m_reader->ToChild(name);
        // did the child not exist?
        if (m_reader->GetCurrentNode() == nullptr) {
            m_reader->PopNode();
            return false;
        }
    }
    */
    
    return m_reader->ReadStringWalkSafe(str, str_len);
}

} // namespace CSaruSerial
