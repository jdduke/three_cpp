#ifndef THREE_ATTRIBUTE_HPP
#define THREE_ATTRIBUTE_HPP

#include <three/common.hpp>
#include <three/constants.hpp>

#include <three/utils/any.hpp>
#include <three/utils/index.hpp>
#include <three/utils/noncopyable.hpp>

#include <unordered_map>

namespace three {

class Attribute /*: public NonCopyable*/
{
public:
    explicit Attribute(THREE::AttributeType type = THREE::v3, int arraySize = 0)
        : type(type), buffer(0), createUniqueBuffers(false), needsUpdate(false), size(0), itemSize(1), __glInitialized(false), __original(nullptr)
    {

        if (arraySize > 0)
            array.resize(arraySize);
    }

    /////////////////////////////////////////////////////////////////////////

    THREE::AttributeType type;

    // TODO: Use the union for indices support (rather than casting from float)
    /*union Value {
    int i;
    float f;
  };*/
    std::vector<float> array;
    any value; // This will be a vector, of float/vec2/vec3/color/vec4
    std::string belongsToAttribute;
    std::string boundTo;
    Buffer buffer;
    bool createUniqueBuffers;
    bool needsUpdate;
    int size;
    int itemSize;

    bool __glInitialized;
    Attribute* __original;
};

//////////////////////////////////////////////////////////////////////////

//typedef std::unordered_map<std::string, Attribute> Attributes;
typedef Properties<std::string, Attribute> Attributes;
typedef std::unordered_map<std::string, Index> AttributeLocations;

//////////////////////////////////////////////////////////////////////////

namespace detail {

    template <typename T, bool TypeIsScalar>
    struct FillerImpl
    {
        enum { Stride = sizeof(T) / sizeof(float) };

        inline void fill(const std::vector<T>& src, std::vector<float>& dst)
        {
            /*const auto srcData = (const float*)src.data();
    const auto srcDataSize = src.size() * Stride;
    std::copy( srcData, srcData + srcDataSize, dst.data() );*/
            const auto count = src.size();
            for (size_t i = 0, offset = 0; i < count; ++i, offset += Stride)
            {
                for (int j = 0; j < Stride; ++j)
                {
                    dst[offset + j] = src[i][j];
                }
            }
        }

        template <typename SortArray>
        inline void fill(const std::vector<T>& src, const SortArray& sortArray, std::vector<float>& dst)
        {
            const auto count = src.size();
            for (size_t i = 0, offset = 0; i < count; ++i, offset += Stride)
            {
                const auto& index = sortArray[i].second;
                for (int j = 0; j < Stride; ++j)
                {
                    dst[offset + j] = src[index][j];
                }
            }
        }
    };

    template <typename T>
    struct FillerImpl<T, true>
    {
        inline void fill(const std::vector<T>& src, std::vector<float>& dst)
        {
            static_assert(std::is_same<T, float>::value, "Only float targets are supported");
            std::copy(src.begin(), src.end(), dst.begin());
        }

        template <typename SortArray>
        inline void fill(const std::vector<T>& src, const SortArray& sortArray, std::vector<float>& dst)
        {
            static_assert(std::is_same<T, float>::value, "Only float targets are supported");
            const auto count = src.size();
            for (size_t i = 0; i < count; i++)
            {
                const auto& index = sortArray[i].second;
                dst[i] = src[index];
            }
        }
    };

    template <typename T>
    struct Filler : public FillerImpl<T, sizeof(T) <= sizeof(float)>
    {
    };

} // namespace detail

//////////////////////////////////////////////////////////////////////////

template <typename T>
void fillFromAny(const any& src, std::vector<float>& dst)
{
    detail::Filler<T> filler;
    filler.fill(src.cast<std::vector<T>>(), dst);
}

template <typename T, typename SortArray>
void fillFromAny(const any& src, const SortArray& sortArray, std::vector<float>& dst)
{
    detail::Filler<T> filler;
    filler.fill(src.cast<std::vector<T>>(), sortArray, dst);
}

//////////////////////////////////////////////////////////////////////////

namespace AttributeKey {

#if 0
#    define DECLARE_ATTRIBUTE_KEY(a)                     \
        inline const std::string& a()                    \
        {                                                \
            static const std::string attributeKey((#a)); \
            return attributeKey;                         \
        }
#else
#    define DECLARE_ATTRIBUTE_KEY(a) \
        inline const char* a() { return #a; }
#endif

    DECLARE_ATTRIBUTE_KEY(position)
    DECLARE_ATTRIBUTE_KEY(normal)
    DECLARE_ATTRIBUTE_KEY(index)
    DECLARE_ATTRIBUTE_KEY(color)
    DECLARE_ATTRIBUTE_KEY(uv)
    DECLARE_ATTRIBUTE_KEY(uv2)
    DECLARE_ATTRIBUTE_KEY(tangent)
    DECLARE_ATTRIBUTE_KEY(skinVertexA)
    DECLARE_ATTRIBUTE_KEY(skinVertexB)
    DECLARE_ATTRIBUTE_KEY(skinWeight)
    DECLARE_ATTRIBUTE_KEY(skinIndex)
    DECLARE_ATTRIBUTE_KEY(morphTarget)

#undef DECLARE_ATTRIBUTE_KEY

} // namespace AttributeKey


} // namespace three

#endif // THREE_ATTRIBUTE_H