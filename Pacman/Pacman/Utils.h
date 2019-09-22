#include <cstddef>

namespace Utils
{

    template<class C>
    constexpr inline std::size_t Size(C const& container)
    {
        return container.size();
    }

    template<class C>
    void RandomResize(C& container, std::size_t requestedSize)
    {
        srand(time(NULL));

        int currentSize = int(Size(container));
        while (currentSize > requestedSize)
        {
            auto itr = std::begin(container);
            std::advance(itr, rand() % currentSize);
            container.erase(itr);
            --currentSize;
        }
    }

}

