26Feb20:
    Removed std::complex from a valid type.
    Added std::array functionality.  Most of the cleanup should be in place.
        - std::array<> implementation was worked out through the refl.hh, lines 2947-2951
        for the template declaration, and lines 2857 - 2870 for write_impl()
    A Skeliton is in place for the C style arrays in reflected_value, however the
    requirements are not quite in place for refl.hh.  Some work to be done.



Reflection library here is used to create a key/value map of a structure, allowing
direct modification of memory through the use of searching a variable's name, and
re-assigning its value through a string value.

This also has the ability to call functions from structures.


Things that are implemented:
- Structured Tree
- int32, int64, uint32, uint64, float, double
- functions from the immediate structure.
- std::string
- bool
- std::array!!!


things still to work on:
- <>std::complex: IN WORK: Complex is currently not supported by refl.hpp.  Working to figure out how to get it supported.  Support is available in reflected_value, but not in the refl header.
- C - Style arrays!
- structure tree's function (being able to call a lower level object's function)


Complex:
- both forms of complex read and write the same string value.
- parsed individually into respective values.
- format: "#.# #.#"
- Currently need to contact people in charge of the refl.hpp to figure out if its a supported type now.


Thoughts on implementation of Arrays:
- Arrays should be loadable item by item (indexing) OR
- arrays should be loadable by shoving a binary file into the array.
    - should take a vector of union{float,uint32_t, int32_t}	
    - read in uint32, use correct type as needed.
    - input of the actual structured data should be the vec, not the file.
    - file opening and parsing should be an external operation.
