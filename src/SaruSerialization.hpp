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

#pragma once

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
    virtual bool Serialize(char * str, size_t str_len, const char * name = nullptr) = 0;
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
    virtual bool Serialize (char * str, size_t str_len, const char * name = nullptr);
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
    virtual bool Serialize (char * str, size_t str_len, const char * name = nullptr);
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
