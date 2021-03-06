#if 1

#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;

int main()
{
	float v[3] = {0.1, 0.4, -0.3};
	std::cout << *std::min_element(std::begin(v), std::end(v)) << std::endl;
	std::cout << std::to_string(3.0000000f) << endl;
	{
		stringstream ss;
		ss << 42.7600000f;
		cout << ss.str() << endl;
	}
	{
		stringstream ss;
		ss << 0.12345678f;
		cout << ss.str() << endl;
	}
	{
		stringstream ss;
		ss << 180.0001f;
		cout << ss.str() << endl;
	}
	return 0;
}

#elif 0

#include <iostream>
#include <yaml-cpp/yaml.h>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include "Serialization.hpp"
#include "TextureImporter.hpp"
#include "Archive.hpp"
#include "Serialization/NameValuePair.hpp"
#include "Serialization/helper.hpp"
#include "Serialization/archives/YAMLInputArchive.hpp"

namespace YAML
{
	//template<class T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
	//struct convert
	//{
	//	static bool decode(const Node& node, T & rhs)
	//	{
	//		if (!node.IsScalar())
	//		{
	//			return false;
	//		}
	//		rhs = static_cast<T>(node.as<std::underlying_type<T>>());
	//	}
	//};
}

using namespace FishEngine;
using namespace std;

int main()
{
	const char* path = R"(D:\program\FishEngine\Example\Sponza\crytek-sponza\textures\background.png.meta)";
	auto p = boost::filesystem::path(path);
	auto t = (uint32_t)boost::filesystem::last_write_time(p);
	cout << "last write time: " << t << std::endl;
	FishEngine::YAMLInputArchive archive{ std::ifstream(path) };
	//archive.CurrentNode();
	uint32_t created_time;
	archive >> make_nvp("timeCreated", created_time);
	std::cout << created_time << endl;
	archive.ToNextNode();
	auto importer = archive.DeserializeObject<TextureImporter>();
	std::cout << importer->GetGUID() << std::endl;
	return 0;
}

#elif 0
#include "FishEngine.hpp"

#define CEREAL_NOEXCEPT noexcept

namespace FishEngine
{
    class OutputArchiveBase
    {
    public:
        OutputArchiveBase() = default;
        OutputArchiveBase( OutputArchiveBase && ) CEREAL_NOEXCEPT {}
        OutputArchiveBase & operator=( OutputArchiveBase && ) CEREAL_NOEXCEPT { return *this; }
        virtual ~OutputArchiveBase() CEREAL_NOEXCEPT = default;
        
    private:
        virtual void rtti() {}
    };
    
    template<class ArchiveType>
    class OutputArchive : public OutputArchiveBase
    {
    public:
        //! Construct the output archive
        /*! @param derived A pointer to the derived ArchiveType (pass this from the derived archive) */
        OutputArchive(ArchiveType * const derived) : self(derived), itsCurrentPointerId(1), itsCurrentPolymorphicTypeId(1)
        { }
        
        OutputArchive & operator=( OutputArchive const & ) = delete;
        
        //! Serializes all passed in data
        /*! This is the primary interface for serializing data with an archive */
        template <class ... Types> inline
        ArchiveType & operator()( Types && ... args )
        {
            self->process( std::forward<Types>( args )... );
            return *self;
        }
        
        template <class T> inline
        ArchiveType & operator&( T && arg )
        {
            self->process( std::forward<T>( arg ) );
            return *self;
        }
        
        template <class T> inline
        ArchiveType & operator<<( T && arg )
        {
            self->process( std::forward<T>( arg ) );
            return *self;
        }
        
        inline std::uint32_t registerSharedPointer( void const * addr )
        {
            // Handle null pointers by just returning 0
            if(addr == 0) return 0;
            
            auto id = itsSharedPointerMap.find( addr );
            if( id == itsSharedPointerMap.end() )
            {
                auto ptrId = itsCurrentPointerId++;
                itsSharedPointerMap.insert( {addr, ptrId} );
                return ptrId | detail::msb_32bit; // mask MSB to be 1
            }
            else
                return id->second;
        }


}


#elif

#include <iostream>
#include <sstream>
#include <cassert>

#include <Archive.hpp>
#include <Serialization.hpp>

//template<class T>
//class NameValuePair
//{
//private:
//	// If we get passed an array, keep the type as is, otherwise store
//	// a reference if we were passed an l value reference, else copy the value
//	using Type = typename std::conditional<std::is_array<typename std::remove_reference<T>::type>::value,
//		typename std::remove_cv<T>::type,
//		typename std::conditional<std::is_lvalue_reference<T>::value,
//		T,
//		typename std::decay<T>::type>::type>::type;
//	static_assert(!std::is_base_of<NameValuePair, T>::value,
//		"Cannot pair a name to a NameValuePair");
//
//	NameValuePair & operator=(NameValuePair const &) = delete;
//
//public:
//	//! Constructs a new NameValuePair
//	/*! @param n The name of the pair
//	@param v The value to pair.  Ideally this should be an l-value reference so that
//	the value can be both loaded and saved to.  If you pass an r-value reference,
//	the NameValuePair will store a copy of it instead of a reference.  Thus you should
//	only pass r-values in cases where this makes sense, such as the result of some
//	size() call.
//	@internal */
//	NameValuePair(char const * n, T && v) : name(n), value(std::forward<T>(v)) {}
//
//	char const * name;
//	Type value;
//};


// ######################################################################
// Common BinaryArchive serialization functions

//template<class T> inline
//void Serialize(OutputArchive & archive, T const & t);
//
//template<class T> inline
//void Deserialize(InputArchive & archive, T & t);


////! Serializing NVP types to binary
//template<class T> inline
//void Serialize(OutputArchive & archive, NameValuePair<T> const & t)
//{
//	archive(t.value);
//}

using namespace std;

template<typename T>
void test_binary(const T& t)
{
	cout << "type: " << typeid(T).name() << endl;
	ostringstream ss;
	FishEngine::BinaryOutputArchive bar(ss);
	bar << t;
	istringstream s_in(ss.str());
	FishEngine::BinaryInputArchive bin(s_in);
	T x;
	bin >> x;
	//assert(x == t);
}

template<typename T>
void test_text(const T& t)
{
	cout << "type: " << typeid(T).name() << endl;
	ostringstream ss;
	FishEngine::TextOutputArchive bar(ss);
	bar << t;
	istringstream s_in(ss.str());
	FishEngine::TextInputArchive bin(s_in);
	T x;
	bin >> x;
	{
		FishEngine::TextOutputArchive bar(std::cout);
		bar << x;
	}
	//assert(x == t);
}

template<typename T>
void test(const T& t)
{
	test_binary(t);
	test_text(t);
}


int main()
{
	test(3110103671);
	test(true);
	test(false);
	test(3.1415826);
	test(3.1415926f);
	test(-1);
	test(0);
	test(std::string("Hello World!"));
	//test("Hello World!");
	test(FishEngine::Vector3(1, 2, 3));
	test(FishEngine::LightType::Directional);
	test(FishEngine::Transform());
	return 0;
}

#elif 0

#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <cereal/archives/json.hpp>

class Data
{
public:
    Data(int x, float y, double z) : x(x), y(y), z(z) {}
private:
    int x;
    float y;
    double z;
    
    template<typename Archive>
    friend void serialize(Archive & archive, Data & m);
};

template<class Archive>
void serialize(Archive & archive, Data & m)
{
    archive( m.x, m.y, m.z );
}


int main()
{
    Data d(1, 2.0f, 3.0);
    cereal::JSONOutputArchive ar( std::cout );
    ar( d );
    return 0;
}

#else

int main()
{
}

#endif
