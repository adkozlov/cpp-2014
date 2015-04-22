#pragma once

#include <type_traits>
#include <sstream>
#include <iostream>

#include "dict.h"
#include "io_streams.h"

namespace serialization
{
    using std::enable_if;
    using std::is_pod;
    using std::is_arithmetic;
    using std::stringstream;

    template <typename visitor, typename type>
    void reflect(visitor& v, type& obj, const char* key)
    {
        v.visit(obj, key);
    }

    template <typename stream>
    class stream_visitor
    {
    public:
        stream_visitor(stream s)
            : stream_(s)
        {
        }

        template <typename type>
        typename enable_if<is_pod<type>::value, void>::type
        visit(type& obj, ...);

        template <typename type>
        typename enable_if<!is_pod<type>::value, void>::type
        visit(type& obj, ...);

    private:
        stream stream_;
    };

    template <>
    template <typename type>
    typename enable_if<is_pod<type>::value, void>::type
    stream_visitor<output_stream&>::visit(type& obj, ...)
    {
        write(stream_, obj);
    }

    template <>
    template <typename type>
    typename enable_if<!is_pod<type>::value, void>::type
    stream_visitor<output_stream&>::visit(type& obj, ...)
    {
        reflect(*this, obj);
    }

    template<typename type>
    void write(output_stream& os, const type& obj, typename enable_if<is_pod<type>::value>::type* = 0)
    {
        os.write(&obj, sizeof obj);
    }

    template<typename type>
    void write(output_stream& os, const type& obj, typename enable_if<!is_pod<type>::value>::type* = 0)
    {
        stream_visitor<output_stream&> visitor(os);
        reflect(visitor, const_cast<type&>(obj));
    }

    template <>
    template <typename type>
    typename enable_if<is_pod<type>::value, void>::type
    stream_visitor<input_stream&>::visit(type& obj, ...)
    {
        read(stream_, obj);
    }

    template <>
    template <typename type>
    typename enable_if<!is_pod<type>::value, void>::type
    stream_visitor<input_stream&>::visit(type& obj, ...)
    {
        reflect(*this, obj);
    }

    template<typename type>
    void read(input_stream& is, type& obj, typename enable_if<is_pod<type>::value>::type* = 0)
    {
        is.read(&obj, sizeof obj);
    }

    template<typename type>
    void read(input_stream& is, type& obj, typename enable_if<!is_pod<type>::value>::type* = 0)
    {
        stream_visitor<input_stream&> visitor(is);
        reflect(visitor, obj);
    }

    template <typename dictionary>
    class dict_visitor
    {
    public:
        dict_visitor(dictionary d)
            : dict_(d)
        {
        }

        template <typename type>
        void visit(type& obj, const char* key);

    private:
        dictionary dict_;
    };

    template <>
    template <typename type>
    void dict_visitor<dict&>::visit(type &obj, const char* key)
    {
        write(dict_.children[key], obj);
    }

    template <>
    template <typename type>
    void dict_visitor<dict const&>::visit(type &obj, const char* key)
    {
        auto it = dict_.children.find(key);
        if (it != dict_.children.end())
        {
            read(it->second, obj);
        }
    }

    template <typename type>
    void write(dict& d, type const& obj, typename enable_if<is_arithmetic<type>::value>::type* = 0)
    {
        stringstream ss;
        ss << obj;
        d.value = ss.str();
    }

    template <typename type>
    void write(dict& d, type const& obj, typename enable_if<!is_arithmetic<type>::value>::type* = 0)
    {
        dict_visitor<dict&> visitor(d);
        reflect(visitor, const_cast<type&>(obj));
    }

    template <typename type>
    void read(dict const& d, type& obj, typename enable_if<is_arithmetic<type>::value>::type* = 0)
    {
        stringstream ss(d.value);
        ss >> obj;
    }

    template <typename type>
    void read(dict const& d, type& obj, typename enable_if<!is_arithmetic<type>::value>::type* = 0)
    {
        dict_visitor<dict const&> visitor(d);
        reflect(visitor, obj);
    }
} // serializer