class Generator{
    public:
        Generator(){};
        virtual ~Generator(){};
        /**
         * @brief pure virtual function to work as a abstract class
         * 
         * @return std::string 
         */
        virtual std::string getRandom() = 0;
};