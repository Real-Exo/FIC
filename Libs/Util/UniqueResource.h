#ifndef UNIQUERESOURCE_H
#define UNIQUERESOURCE_H

#include <functional>
#include <optional>
#include <utility>

namespace Util
{
template <class T>
class UniqueResource
{
public:

    UniqueResource() = default;
    UniqueResource(const UniqueResource&) = delete;
    UniqueResource& operator = (const UniqueResource&) = delete;

    UniqueResource(T resource, std::function<void(T&)> deleter) noexcept :
        mResource{ std::move(resource) },
        mDeleter{ std::move(deleter) }
    {}

    UniqueResource(UniqueResource&& other) noexcept :
        mResource{ std::exchange(other.mResource, std::nullopt) },
        mDeleter{ std::move(other.mDeleter) }
    {}

    UniqueResource&
    operator = (UniqueResource&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        UniqueResource temp{ std::move(other) };
        mResource = std::exchange(temp.mResource, std::move(mResource));
        mDeleter = std::exchange(temp.mDeleter, std::move(mDeleter));

        return *this;
    }

    ~UniqueResource() noexcept
    {
        if (mResource)
        {
            mDeleter(*mResource);
        }
    }

    explicit
    operator bool() const
    {
        return mResource.has_value();
    }

    T&
    operator * ()
    {
        return *mResource;
    }

    const T&
    operator * () const
    {
        return *mResource;
    }

private:

    std::optional<T> mResource;
    std::function<void(T&)> mDeleter;
};
}

#endif //UNIQUERESOURCE_H
