#ifndef REFLECTED_FUNCTIONS_HH
#define REFLECTED_FUNCTIONS_HH


#include "refl.hh"
#include <map>
#include <vector>
#include <string>
#include <iostream>




template<typename StructType>
using funcPtr = int(StructType::*)(const std::string&);


template<typename StructType>
class FunctionPointer
{
private:

    std::map<std::string, funcPtr<StructType>> _mappedFunctions;
    std::vector<std::string> _keys;

    StructType *caller;


    void insert(const std::string &key, funcPtr<StructType> func)
    {
        _mappedFunctions[key] = func;
        _keys.push_back(key);
    }

public:

    FunctionPointer(StructType * st) : caller(st)
    {
        std::cout << "Running constructor: " << std::endl;
        for_each(refl::reflect(*st).members, [&](auto member)
        {
            if constexpr(refl::descriptor::is_function(member))
            {
                std::string name = get_display_name(member);
//                insert(name, member.pointer);
                _mappedFunctions[name] = member.pointer;
                _keys.push_back(name);
            }
            if constexpr(is_field(member))
            {
                std::cout << "Member!: " << get_display_name(member) << std::endl;
            }
        });
    }

    size_t Size() const
    {
        return _mappedFunctions.size();
    }

    std::vector<std::string> Keys() const
    {
        return _keys;
    }


    int Call(const std::string & key, const std::string & args)
    {
        /// need to verify function exists in map, or else return -1
        funcPtr<StructType> func = _mappedFunctions[key];
        return (caller->*func)(args);
    }

};





#endif // REFLECTED_FUNCTIONS_HH
