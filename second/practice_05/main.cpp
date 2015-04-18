#include <type_traits>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <list>
#include <assert.h>
#include <cstdint>

using namespace std;

namespace serialization
{
	typedef char			byte_t;
	typedef vector<byte_t>	bytes_t;

	typedef uint64_t		size_type;

	struct output_stream
	{
		explicit output_stream(bytes_t&& from = bytes_t())
			: buffer_(move(from))
		{
		}

		void write(const void* data, size_t size)
		{
			const byte_t* p = static_cast<const byte_t*>(data);

			size_t old_size = buffer_.size();
			buffer_.resize(old_size + size);
			memcpy(buffer_.data() + old_size, p, size);
		}

		bytes_t detach()
		{
			return move(buffer_);
		}

		bytes_t const& data() const 
		{
			return buffer_;
		}

	private:
		bytes_t	buffer_;
	};

	struct input_stream
	{
		explicit input_stream(bytes_t const& from)
			: begin_(from.data())
			, end_  (from.data() + from.size())
			, cur_  (from.data())
		{
		}

		void read(void* to, size_t size)
		{
			assert(cur_ + size <= end_);
			memcpy(to, cur_, size);
			cur_ += size;
		}

	private:
		byte_t const* begin_;
		byte_t const* end_;
		byte_t const* cur_;

	};

	template <typename T>
	struct is_pod_or_container
	{
		typedef char yes;
		typedef char no;

		template <typename U>
		static auto test(U* u) -> decltype(std::is_pod<U>::value, (*u).begin(), (*u).end(), yes());

		static no test(...);

		enum { value = (sizeof(yes) == sizeof test((T*)0)) };
	};

	template <typename T>
	struct has_begin_end
	{
		typedef char yes;
		typedef char no;

		template <typename U>
		static auto test(U* u) -> decltype((*u).begin(), (*u).end(), yes());

		static no test(...);

		enum { value = (sizeof(yes) == sizeof test((T*)0)) };
	};

	template <typename T>
	void write(serialization::output_stream& stream, T const& t, typename enable_if<!is_pod_or_container<T>::value>::type* = 0)
	{

	}

	template <typename T>
	void write(serialization::output_stream& stream, T const& t, typename enable_if<std::is_pod<T>::value>::type* = 0)
	{
		stream.write(&t, sizeof(t));
	}

	template <typename T>
	void write(serialization::output_stream& stream, T const& t, typename enable_if<has_begin_end<T>::value>::type* = 0)
	{
		size_t size = t.size();
		stream.write((void*) size, sizeof(size_t));
		for (auto it = t.begin(); it != t.end(); ++it)
		{
			stream.write(&it, sizeof(T));
		}
	}

	template <typename T>
	void read(serialization::input_stream& stream, T& t, typename enable_if<!is_pod_or_container<T>::value>::type* = 0)
	{

	}

	template <typename T>
	void read(serialization::input_stream& stream, T& t, typename enable_if<std::is_pod<T>::value>::type* = 0)
	{
		stream.read(&t, sizeof(T));
	}

	template <typename T>
	void read(serialization::input_stream& stream, T const& t, typename enable_if<has_begin_end<T>::value>::type* = 0)
	{
		size_t size;
		stream.read(&size, sizeof(size_t));
		for (size_t i = 0; i < size; ++i)
		{
			T temp;
			stream.read(temp, sizeof(T));
			t.push_back(temp);
		}
	}
} // serialization 


//////////////////////////////////////////////////////////////////////////
// tests


template<class type>
bool eq_container(type const& lhs, type const& rhs)
{
	if (lhs.size() != rhs.size())
		return false;

	for (auto it = lhs.begin(), jt = rhs.begin(); it != lhs.end(); ++it, ++jt)
		if (!(*it == *jt))
			return false;

	return true;
}


struct custom_record
{
	custom_record()
		: number(0)
	{
	}

	custom_record(string const& t, int n)
		: text  (t)
		, number(n)
		, texts (n, t)
	{
	}
	
	string text;
	int    number;
	vector<string> texts;
};

bool operator==(custom_record const& lhs, custom_record const& rhs)
{
	return 
		lhs.text	== rhs.text			&& 
		lhs.number	== rhs.number		&& 
		eq_container(lhs.texts, rhs.texts);
}

template<class stream>
void serialize(stream& s, custom_record& r)
{
	serialize(s, r.text  );
	serialize(s, r.number);
	serialize(s, r.texts );
}

int main()
{
	// custom struct 
	{
		serialization::output_stream os;

		list<custom_record> v;
		v.push_back(custom_record("1", 1));
		v.push_back(custom_record("2", 2));
		v.push_back(custom_record("3", 3));
		v.push_back(custom_record("4", 4));

		write(os, v); 

		list<custom_record> v2;
		
		serialization::input_stream is (os.data());
		read(is, v2);

		assert(eq_container(v, v2));
	}


	// map<string, int>
	{
		serialization::output_stream os;

		std::map<string, int> v;
		v["1"] = 1;
		v["2"] = 2;
		v["3"] = 3;
		v["4"] = 4;

		write(os, v); 

		map<string, int> v2;
		serialization::input_stream is (os.data());
		read(is, v2);

		assert(eq_container(v, v2));
	}

	// vector<int>
	{
		serialization::output_stream os;

		std::vector<int> v;
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);

		write(os, v); 

		vector<int> v2;
		serialization::input_stream is (os.data());
		read(is, v2);

		assert(eq_container(v, v2));
	}

	// vector<sring>
	{
		serialization::output_stream os;

		std::vector<string> v;
		v.push_back("1");
		v.push_back("2");
		v.push_back("3");
		v.push_back("4");

		write(os, v); 

		vector<string> v2;
		serialization::input_stream is (os.data());
		read(is, v2);

		assert(eq_container(v, v2));
	}

	return 0;
}
