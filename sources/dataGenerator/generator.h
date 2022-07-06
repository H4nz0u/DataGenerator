#include <variant>
typedef std::variant<int, float, uint64_t, std::string> value;
class Generator{
    public:
        Generator(){};
        virtual ~Generator(){};
        virtual value getRandom(int min, int max, bool deterministic = true) = 0;
};