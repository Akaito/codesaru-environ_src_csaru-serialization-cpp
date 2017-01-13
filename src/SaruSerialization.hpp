/*
Copyright (c) 2016 Christopher Higgins Barrett

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include <cstdlib>

#include <csaru-datamap-cpp/DataMapMutator.hpp>
#include <csaru-datamap-cpp/DataMapReader.hpp>

namespace CSaruSerial {

class ISerializer {
public:
    // Methods
    virtual ~ISerializer() {}

    // RETURNS: false on inability to comply
    virtual bool PushGroup(const char * name = nullptr) = 0;

    // RETURNS: false on inability to comply
    virtual bool PopGroup() = 0;

    // RETURNS: false on inability to comply
    virtual bool Serialize(bool * b, const char * name = nullptr) = 0;

    // RETURNS: false on inability to comply
    virtual bool Serialize(int * i, const char * name = nullptr) = 0;

    // RETURNS: false on inability to comply
    virtual bool Serialize(float * f, const char * name = nullptr) = 0;

    // str_len [in]: Length of str in elements (not necessarily bytes).
    // RETURNS: false on inability to comply
    virtual bool Serialize(char * str, std::size_t str_len, const char * name = nullptr) = 0;
};


class SerializeToDataMap : public ISerializer {
private:
    // Data
    CSaruContainer::DataMapMutator * m_mutator;

    // Helpers
    // checks for mixed named/unnamed data, and prepares the Mutator to write
    //   into the proper location
    void CommonSerializationPrep (const char * name);

public:
    // NOTE: mutator is not deleted by this class.
    SerializeToDataMap (CSaruContainer::DataMapMutator * mutator);

    // Commands
    virtual bool PushGroup (const char * name = nullptr);
    virtual bool PopGroup ();
    virtual bool Serialize (bool * b, const char * name = nullptr);
    virtual bool Serialize (int * i, const char * name = nullptr);
    virtual bool Serialize (float * f, const char * name = nullptr);
    virtual bool Serialize (char * str, std::size_t str_len, const char * name = nullptr);
};


class SerializeFromDataMap : public ISerializer {
private:
    // Data
    CSaruContainer::DataMapReader * m_reader;

public:
    // NOTE: mutator is not deleted by this class.
    SerializeFromDataMap (CSaruContainer::DataMapReader * reader);

    // Commands
    // if name is NULL, will go to first child
    virtual bool PushGroup (const char * name = nullptr);

    virtual bool PopGroup ();
    virtual bool Serialize (bool * b, const char * name = nullptr);
    virtual bool Serialize (int * i, const char * name = nullptr);
    virtual bool Serialize (float * f, const char * name = nullptr);
    virtual bool Serialize (char * str, std::size_t str_len, const char * name = nullptr);
};


/*
//
// Write
//

void PushGroup(ISerializer * dest, const char * name = nullptr);

void PopGroup(ISerializer * dest);

void Serialize(ISerializer * dest, bool * b, const char * name = nullptr);

void Serialize(ISerializer * dest, int * i, const char * name = nullptr);

void Serialize(ISerializer * dest, float * f, const char * name = nullptr);

void Serialize(ISerializer * dest, char * str, size_t str_len, const char * name = nullptr);

template <typename T>
void Serialize(ISerializer * dest, T * usertype, const char * name = nullptr)
{
usertype->SaruSerialize(dest, name);
}

//
// Read
//

// index [in]: The zero-based indexed child is stepped into.
void PushGroup(Core::DataMapReader * src, int index);

// name [in]: The named child is found and stepped into.
void PushGroup(Core::DataMapReader * src, const char * name);

// name [in]: The named child (NOT sibling!) is found and read from.
void Serialize(Core::DataMapReader * src, const char * name);

// the current node at the 'cursor' is read into out-param b.
void Serialize(Core::DataMapReader * src, bool * b);

// the named child is found and read from, into out-param b.
void Serialize(Core::DataMapReader * src, bool * b, const char * name);

// the current node at the 'cursor' is read into out-param i.
void Serialize(Core::DataMapReader * src, int * i);

// the named child is found and read from, into out-param i.
void Serialize(Core::DataMapReader * src, int * i, const char * name);

// the current node at the 'cursor' is read into out-param f.
void Serialize(Core::DataMapReader * src, float * f);

// the named child is found and read from, into out-param f.
void Serialize(Core::DataMapReader * src, float * f, const char * name);

// the current node at the 'cursor' is read into out-param str.
void Serialize(Core::DataMapReader * src, char * str, size_t str_len);

// the named child is found and read from, into out-param str.
void Serialize(Core::DataMapReader * src, char * str, size_t str_len, const char * name);
//*/

} // namespace CSaruSerial
