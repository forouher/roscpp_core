/*
 * Copyright (C) 2013, Dariush Forouher
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the names of Stanford University or Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ROSCPP_BOOST_CONTAINER_H
#define ROSCPP_BOOST_CONTAINER_H

#include <boost/container/allocator_traits.hpp>
#include <boost/container/detail/utilities.hpp>
#include <boost/container/detail/type_traits.hpp>
#include <boost/container/detail/transform_iterator.hpp>
#include <boost/container/scoped_allocator.hpp>

#include <boost/interprocess/offset_ptr.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/string.hpp>
#include <vector>
#include <string>
#include <typeinfo>
#include "allocator.h"

namespace ros
{

namespace messages
{

namespace types
{

typedef size_t size_type;


template<
    class charT,
    class Traits = std::char_traits<charT>,
    class Allocator = boost::interprocess::ros_allocator< charT, boost::interprocess::managed_shared_memory::segment_manager>
>
class basic_string: public boost::container::basic_string<charT, Traits, Allocator>
{

public:
    typedef Allocator allocator_type;

    basic_string() {};
    explicit basic_string(const Allocator& alloc) : boost::container::basic_string<charT, Traits, Allocator>(alloc) {};
    basic_string(const boost::container::basic_string<charT, Traits, Allocator>& str, const Allocator& alloc = Allocator()) : boost::container::basic_string<charT, Traits, Allocator>(str, alloc) {};
    basic_string (const basic_string& str, const Allocator& alloc = Allocator()) : boost::container::basic_string<charT, Traits, Allocator>(str, alloc) {};
    basic_string (const basic_string& str, size_type pos, size_type len = std::basic_string<charT, Traits, Allocator>::npos,
              const Allocator& alloc = Allocator())  : boost::container::basic_string<charT, Traits, Allocator>(str,pos,len,alloc) {};
    basic_string (const charT* s, const Allocator& alloc = Allocator()) : boost::container::basic_string<charT, Traits, Allocator>(s,alloc) {};
    basic_string (const charT* s, size_type n,
              const Allocator& alloc = Allocator()) : boost::container::basic_string<charT, Traits, Allocator>(s,n,alloc) {};
    basic_string (size_type n, charT c,
              const Allocator& alloc = Allocator()) : boost::container::basic_string<charT, Traits, Allocator>(n,c,alloc) {};

    basic_string(const std::basic_string<charT, Traits, std::allocator<charT> >& str, const Allocator& alloc = Allocator()) : boost::container::basic_string<charT, Traits, Allocator>(str.c_str(),str.length(), alloc) {};

    // XXX: this str.c_str()+pos looks scary. Test this!
    basic_string (const std::basic_string<charT, Traits, std::allocator<charT> >& str, size_type pos, size_type len = std::basic_string<charT, Traits, Allocator>::npos,
              const Allocator& alloc = Allocator()) : boost::container::basic_string<charT, Traits, Allocator>(str.c_str()+pos,str.length(), alloc) {};

    template <class InputIterator>
    basic_string  (InputIterator first, InputIterator last,
                 const Allocator& alloc = Allocator()) : boost::container::basic_string<charT, Traits, Allocator>(first,last,alloc) {};

    // safe (weil alles const)
    operator const std::basic_string<charT, Traits, std::allocator<charT> >() const { return std::basic_string<charT, Traits, std::allocator<charT> >(this->c_str(),this->length()); };

      basic_string&
      operator+=(const std::basic_string<charT, Traits, std::allocator<charT> >& __str) 
      {  boost::container::basic_string<charT, Traits, Allocator>::operator+=(basic_string(__str)); return *this; }

      basic_string&
      operator+=(const basic_string& __str) 
      {  boost::container::basic_string<charT, Traits, Allocator>::operator+=(__str); return *this; }

      basic_string&
      operator+=(const charT* __str) 
      {  boost::container::basic_string<charT, Traits, Allocator>::operator+=(__str); return *this; }

      /**
       *  @brief  Assign the value of @a str to this string.
       *  @param  str  Source string.
       */
      basic_string&
      operator=(const std::basic_string<charT, Traits, std::allocator<charT> >& __str) 
      { return this->operator=(basic_string(__str.c_str(),__str.length())); }

      basic_string&
      operator=(const basic_string<charT, Traits, Allocator >& __str)
      {
         if (&__str != this)
           this->assign(__str.begin(), __str.end());
         return *this;
      }

      basic_string&
      operator=(const charT* s) 
      { return this->operator=(basic_string(s)); }

    basic_string substr(size_type pos = 0, size_type n = std::basic_string<charT, Traits, Allocator>::npos) const { return basic_string(boost::container::basic_string<charT, Traits, Allocator>::substr(pos,n)); }

};

    template<typename charT, typename Traits, typename Allocator> 
      basic_string< charT, Traits, Allocator >
      operator+(const basic_string< charT, Traits, Allocator > & x, 
                const charT* y) {
	basic_string< charT, Traits, Allocator > temp;
	temp.append(x);
	temp.append(y);
	return temp;
      };

    template<typename charT, typename Traits, typename Allocator> 
      basic_string< charT, Traits, Allocator >
      operator+(const charT* x,
		const basic_string< charT, Traits, Allocator > & y) {
	basic_string< charT, Traits, Allocator > temp;
	temp.append(x);
	temp.append(y);
	return temp;
      };

    template<typename charT, typename Traits, typename Allocator> 
      basic_string< charT, Traits, Allocator >
      operator+(const std::basic_string<charT, Traits, std::allocator<charT> > & x,
		const basic_string< charT, Traits, Allocator > & y) {
	basic_string< charT, Traits, Allocator > temp;
	temp.append(basic_string< charT, Traits, Allocator >(x));
	temp.append(y);
	return temp;
      };

    template<typename charT, typename Traits, typename Allocator> 
      basic_string< charT, Traits, Allocator >
      operator+(const basic_string< charT, Traits, Allocator > & x,
		const std::basic_string<charT, Traits, std::allocator<charT> > & y) {
	basic_string< charT, Traits, Allocator > temp;
	temp.append(x);
	temp.append(basic_string< charT, Traits, Allocator >(y));
	return temp;
      };

    // !!!!!!!!!! TODO !!!!!!!!!!!
    template<typename CharT, typename Traits, typename Allocator> 
      bool operator==(const std::basic_string< CharT, Traits, std::allocator<CharT> > & x, 
                      const basic_string< CharT, Traits, Allocator > & y) { return basic_string< CharT, Traits, Allocator >(x) == y; };

    template<typename CharT, typename Traits, typename Allocator> 
      bool operator==(const basic_string< CharT, Traits, Allocator > & x, 
                      const std::basic_string< CharT, Traits, std::allocator<CharT> > & y) {return y==x;};

    template<typename CharT, typename Traits, typename Allocator> 
      bool operator!=(const std::basic_string< CharT, Traits, std::allocator<CharT> > & x, 
                      const basic_string< CharT, Traits, Allocator > & y) {return !(x==y);};

    template<typename CharT, typename Traits, typename Allocator> 
      bool operator!=(const basic_string< CharT, Traits, Allocator > & x, 
                      const std::basic_string< CharT, Traits, std::allocator<CharT> > & y) {return !(x==y);};


template<typename value_type, typename Allocator = boost::container::scoped_allocator_adaptor<boost::interprocess::ros_allocator< value_type, boost::interprocess::managed_shared_memory::segment_manager> > >
class vector: public boost::container::vector<value_type, Allocator>
{
public:
    typedef Allocator allocator_type;

    explicit vector (const Allocator& alloc = Allocator()) : boost::container::vector<value_type, Allocator>(alloc) {};
    vector (size_type n, const value_type& val = value_type(),
            const Allocator& alloc = Allocator()) : boost::container::vector<value_type, Allocator>(n,val,alloc) {};
    template <class InputIterator>
         vector (InputIterator first, InputIterator last,
                 const Allocator& alloc = Allocator()) : boost::container::vector<value_type, Allocator>(first,last,alloc) {};
    vector (const vector& x, const Allocator& alloc = Allocator()) : boost::container::vector<value_type, Allocator>(x,alloc) {};

    // safe
    vector& operator= (const std::vector<value_type, std::allocator<value_type> >& v) {
	this->assign(v.begin(), v.end());// TODO: veeery slow!
	return *this;
    };
    vector& operator= (const vector<value_type, Allocator>& v) {
	this->assign(v.begin(), v.end());// TODO: veeery slow!
	return *this;
    };

    void push_back(const value_type& x) { boost::container::vector<value_type, Allocator>::push_back(x);};

    // TODO: REMOVE???
    void swap(std::vector<value_type, std::allocator<value_type> >& v) {
	std::vector<value_type, std::allocator<value_type> > v_temp = v;
	v.resize(this->size());
	memcpy(v.data(),this->data(),this->size()*sizeof(value_type));
	this->resize(v_temp.size());
	memcpy(this->data(),v_temp.data(),v_temp.size()*sizeof(value_type)); // DANGEROUS
    };

    // safe, but ugly
    operator const std::vector<value_type, std::allocator<value_type> >() const {
	std::vector<value_type, std::allocator<value_type> > temp;
	temp.assign(this->begin(), this->end());
	return temp;
    };

};

typedef basic_string<char, std::char_traits<char>, ros::alloc<char>::type > string;
 
}

}

}

#endif // ROSCPP_BOOST_CONTAINER_H
