#ifndef REFLECTED_LAYERED_OBJECT_HH
#define REFLECTED_LAYERED_OBJECT_HH

#include <string>
#include <map>
#include <iostream>


#include "reflected_value.hh"

namespace Terryn {


template<typename ReflectedStructure>
class ReflectedObject
{


private:

    std::map<std::string, ReferencedType*> _ptrMap;
    std::vector<std::string> _keys;

    int32_t calls = 0;
    using MemoryMap = std::map<std::string, ReferencedType*>;


protected:

    template<typename SubStructure>
    MemoryMap HandleReflectedField(SubStructure* field)
    {
        MemoryMap retval;
        uint32_t count = 0;
        for_each(refl::reflect(*field).members, [&](auto member)
        {
            if constexpr (is_field(member))
            {
                auto mem = &member(*field);

                std::string name = get_display_name(member);
//                std::cout << "Name: " << name << std::endl;
                auto nextMap = HandleReflectedField(mem);
//                std::cout << "map size: "<< nextMap.size() << std::endl;
                if(nextMap.size() > 0)
                {
                    for(auto &[mappedName, refType] : nextMap)
                    {
                        std::string newName = name + "." + mappedName;
                        retval[newName] = new ReferencedType(refType);
                    }
                }
                else
                {
                    if constexpr(AllowedType<decltype (mem)>())
                    {
                        retval[name] = new ReferencedType();
                        retval[name]->Set(mem);
                    }
                }
            }
        });

        return retval;

    }


public:
    ReflectedObject(ReflectedStructure *p)
    {
        _ptrMap = HandleReflectedField(p);
        _keys.reserve(_ptrMap.size());

        for(auto & [name, val] : _ptrMap)
        {
            _keys.emplace_back(name);
        }
    }


    size_t Size() const
    {
        return _ptrMap.size();
    }


    std::vector<std::string> Keys() const
    {
        return _keys;
    }

    std::string Get(const std::string &ID) const
    {
        return _ptrMap.at(ID)->Get();
    }

    std::string GetSafely(const std::string &ID) const
    {
        for(auto & [name, v] : _ptrMap)
        {
            if(name == ID)
            {
                return v->Get();
            }
        }
        return "Value: " + ID + " not present";
    }

    bool Set(const std::string &ID, const std::string &val)
    {
        for(auto & [name, v]: _ptrMap)
        {
            if(name == ID)
            {
                v->ExternalAssign(val);
                return true;
            }
        }

        return false;
    }

    std::vector<size_t> Addresses()
    {
        std::vector<size_t> addresses;
        for (auto & [name, val]: _ptrMap)
        {
            addresses.push_back(val->Address());
        }
        return addresses;
    }

    std::map<std::string, size_t> AddressMap()
    {
        std::map<std::string, size_t> addrmap;
        for(auto & [name, val]: _ptrMap)
        {
            addrmap[name] = val->Address();
        }

        return addrmap;
    }

    /// performs opperations safely since this will never be a time critical event
    std::string GetType(const std::string &ID) const
    {
        for(auto & [name, v] : _ptrMap)
        {
            if(name == ID)
            {
                return v->GetType();
            }
        }
        return "Value: " + ID + " not present";
    }


};


}

#endif // REFLECTED_LAYERED_OBJECT_HH
