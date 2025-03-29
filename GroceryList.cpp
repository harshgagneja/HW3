#include <algorithm>                                                        // find(), shift_left(), shift_right(), equal(), swap(), lexicographical_compare()
#include <cmath>                                                            // min()
#include <compare>                                                          // weak_ordering
#include <cstddef>                                                          // size_t
#include <format>                                                           // format()
#include <initializer_list>                                                 // initializer_list
#include <iomanip>                                                          // setw()
#include <iostream>                                                         // istream, istream
#include <iterator>                                                         // distance(), next()
#include <source_location>                                                  // source_location
#include <stdexcept>                                                        // logic_error
#include <string>                                                           // string
#include <string_view>                                                      // string_view
#include <version>                                                          // defines feature-test macros, __cpp_lib_stacktrace

#if defined( __cpp_lib_stacktrace )                                         // Clang 18 does not yet support std::stacktrace.
  #include <stacktrace>                                                     // stacktrace
#endif


#include "GroceryItem.hpp"
#include "GroceryList.hpp"











namespace  // anonymous, unnamed namespace providing a private working space
{

  std::string make_details( const std::string_view message, const std::source_location location = std::source_location::current() )
  {
    // Note:  GCC 13 does not yet support the quoted string format specifier {:?}, which would allow \"{}\" to be replaced with {:?}
    //        (b.t.w., clang supports this since clang 18.  Rumor has it that gcc 14 will support this)
    //
    //        Clang 18 does not yet support std::stractrace.  Rumors are it may never support stack tracing
    //        GCC 13 supports std::stractrace, but requires and updated library, so it may or may not generate results for you
    //        MSVC++ Version 19.38.33134 (Visual Studio Version 17.8.5) fully supports std::stractrace - Way to go Microsoft!

    return std::format( "{}\n detected in function \"{}\"\n at line {}\n in file \"{}\"\n\n********* Begin Stack Trace *********\n{}\n********* End Stack Trace *********\n",
                        message,
                        location.function_name(),
                        location.line(),
                        location.file_name(),
                        #ifdef  __cpp_lib_stacktrace
                          std::stacktrace::current( 1 ) );                  // Let's not show this function in the trace, so skip 1
                        #else
                          "  Stack trace not available" );
                        #endif
  }
}   // anonymous, unnamed namespace











///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initializer List Constructor
GroceryList::GroceryList( const std::initializer_list<GroceryItem> & initList )
{
  for( auto && groceryItem : initList )   insert( groceryItem, Position::BOTTOM );

  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( make_details( "Container consistency error" ) );
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Queries
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// size() const
std::size_t GroceryList::size() const
{
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( make_details( "Container consistency error" ) );

  return _gList_vector.size(); // Return the size of one of the containers
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Accessors
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// find() const
std::size_t GroceryList::find( const GroceryItem & groceryItem ) const
{
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( make_details( "Container consistency error" ) );

  auto it = std::find(_gList_vector.begin(), _gList_vector.end(), groceryItem);
  return it != _gList_vector.end() ? std::distance(_gList_vector.begin(), it) : size();
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Modifiers
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// insert( position )
void GroceryList::insert( const GroceryItem & groceryItem, Position position )
{
  // Convert the TOP and BOTTOM enumerations to an offset and delegate the work
  if     ( position == Position::TOP    )  insert( groceryItem, 0      );
  else if( position == Position::BOTTOM )  insert( groceryItem, size() );
  else                                     throw std::logic_error( make_details( "Unexpected insertion position" ) );  // Programmer error.  Should never hit this!
}



// insert( offset )
void GroceryList::insert( const GroceryItem & groceryItem, std::size_t offsetFromTop )        // insert provided grocery item at offsetFromTop, which places it before the current grocery item at offsetFromTop
{
  // Validate offset parameter before attempting the insertion.  std::size_t is an unsigned type, so no need to check for negative
  // offsets, and an offset equal to the size of the list says to insert at the end (bottom) of the list.  Anything greater than the
  // current size is an error.
  if( offsetFromTop > size() )   throw InvalidOffset_Ex( make_details( "Insertion position beyond end of current list size" ) );


  /**********  Prevent duplicate entries  ***********************/
  if (find(groceryItem) != size()) return; // Prevent duplicate entries


  // Inserting into the grocery list means you insert the grocery item into each of the containers (array, vector, list, and
  // forward_list). Because the data structure concept is different for each container, the way a grocery item gets inserted is a
  // little different for each.  You are to insert the grocery item into each container such that the ordering of all the containers
  // is the same.  A check is made at the end of this function to verify the contents of all four containers are indeed the same.


  { /**********  Part 1 - Insert into array  ***********************/
    if (_gList_array_size >= _gList_array.size()) throw CapacityExceeded_Ex( make_details( "Capacity exceeded" ) );

    std::shift_right(_gList_array.begin() + offsetFromTop, _gList_array.begin() + _gList_array_size, 1);
    _gList_array[offsetFromTop] = groceryItem;
    ++_gList_array_size;
  } // Part 1 - Insert into array




  { /**********  Part 2 - Insert into vector  **********************/
    _gList_vector.insert(_gList_vector.begin() + offsetFromTop, groceryItem);
  } // Part 2 - Insert into vector




  { /**********  Part 3 - Insert into doubly linked list  **********/
    _gList_dll.insert(std::next(_gList_dll.begin(), offsetFromTop), groceryItem);
  } // Part 3 - Insert into doubly linked list




  { /**********  Part 4 - Insert into singly linked list  **********/
    _gList_sll.insert_after(std::next(_gList_sll.before_begin(), offsetFromTop), groceryItem);
  } // Part 4 - Insert into singly linked list


  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( make_details( "Container consistency error" ) );
} // insert( const GroceryItem & groceryItem, std::size_t offsetFromTop )



// remove( groceryItem )
void GroceryList::remove( const GroceryItem & groceryItem )
{
  // Delegate to the version of remove() that takes an index as a parameter
  remove( find( groceryItem ) );
}



// remove( offset )
void GroceryList::remove( std::size_t offsetFromTop )
{
  // Removing from the grocery list means you remove the grocery item from each of the containers (array, vector, list, and
  // forward_list). Because the data structure concept is different for each container, the way a grocery item gets removed is a
  // little different for each.  You are to remove the grocery item from each container such that the ordering of all the containers
  // is the same.  A check is made at the end of this function to verify the contents of all four containers are indeed the same.

  if( offsetFromTop >= size() )   return;                                           // no change occurs if (zero-based) offsetFromTop >= size()


  { /**********  Part 1 - Remove from array  ***********************/
    std::shift_left(_gList_array.begin() + offsetFromTop, _gList_array.begin() + _gList_array_size, 1);
    --_gList_array_size;
  } // Part 1 - Remove from array




  { /**********  Part 2 - Remove from vector  **********************/
    _gList_vector.erase(_gList_vector.begin() + offsetFromTop);
  } // Part 2 - Remove from vector




  { /**********  Part 3 - Remove from doubly linked list  **********/
    _gList_dll.erase(std::next(_gList_dll.begin(), offsetFromTop));
  } // Part 3 - Remove from doubly linked list




  { /**********  Part 4 - Remove from singly linked list  **********/
    _gList_sll.erase_after(std::next(_gList_sll.before_begin(), offsetFromTop));
  } // Part 4 - Remove from singly linked list


  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( make_details( "Container consistency error" ) );
} // remove( std::size_t offsetFromTop )



// moveToTop()
void GroceryList::moveToTop( const GroceryItem & groceryItem )
{
  auto pos = find(groceryItem);
  if (pos != size()) {
    remove(pos);
    insert(groceryItem, Position::TOP);
  }
}



// operator+=( initializer_list )
GroceryList & GroceryList::operator+=( const std::initializer_list<GroceryItem> & rhs )
{
  for (const auto& item : rhs) {
    insert(item, Position::BOTTOM);
  }

  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( make_details( "Container consistency error" ) );
  return *this;
}



// operator+=( GroceryList )
GroceryList & GroceryList::operator+=( const GroceryList & rhs )
{
  for (const auto& item : rhs._gList_vector) {
    insert(item, Position::BOTTOM);
  }

  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( make_details( "Container consistency error" ) );
  return *this;
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Relational Operators
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// operator<=>
std::weak_ordering GroceryList::operator<=>( GroceryList const & rhs ) const
{
  if( !containersAreConsistant() || !rhs.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( make_details( "Container consistency error" ) );

  auto cmp = std::lexicographical_compare_three_way(_gList_vector.begin(), _gList_vector.end(), rhs._gList_vector.begin(), rhs._gList_vector.end());
  if (cmp != 0) return cmp;
  return size() <=> rhs.size();
}



// operator==
bool GroceryList::operator==( GroceryList const & rhs ) const
{
  if( !containersAreConsistant() || !rhs.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( make_details( "Container consistency error" ) );

  return _gList_vector.size() == rhs._gList_vector.size() && std::equal(_gList_vector.begin(), _gList_vector.end(), rhs._gList_vector.begin());
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private member functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// containersAreConsistant() const
bool GroceryList::containersAreConsistant() const
{
  // Sizes of all containers must be equal to each other
  if(    _gList_array_size != _gList_vector.size()
      || _gList_array_size != _gList_dll.size()
      || _gList_array_size !=  gList_sll_size() ) return false;

  // Element content and order must be equal to each other
  auto current_array_position   = _gList_array .cbegin();
  auto current_vector_position  = _gList_vector.cbegin();
  auto current_dll_position     = _gList_dll   .cbegin();
  auto current_sll_position     = _gList_sll   .cbegin();

  auto end = _gList_vector.cend();
  while( current_vector_position != end )
  {
    if(    *current_array_position != *current_vector_position
        || *current_array_position != *current_dll_position
        || *current_array_position != *current_sll_position ) return false;

    // Advance the iterators to the next element in unison
    ++current_array_position;
    ++current_vector_position;
    ++current_dll_position;
    ++current_sll_position;
  }

  return true;
}



// gList_sll_size() const
std::size_t GroceryList::gList_sll_size() const
{
  return std::distance(_gList_sll.begin(), _gList_sll.end());
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Non-member functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// operator<<
std::ostream & operator<<( std::ostream & stream, const GroceryList & groceryList )
{
  if( !groceryList.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( make_details( "Container consistency error" ) );

  // For each grocery item in the provided grocery list, insert the grocery item into the provided stream.  Each grocery item is
  // inserted on a new line and preceded with its index (aka offset from top)
  unsigned count = 0;
  for( auto && groceryItem : groceryList._gList_sll )   stream << '\n' << std::setw(5) << count++ << ":  " << groceryItem;

  return stream;
}



// operator>>
std::istream & operator>>( std::istream & stream, GroceryList & groceryList )
{
  if( !groceryList.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( make_details( "Container consistency error" ) );

  GroceryItem item;
  while (stream >> item) {
    groceryList.insert(item, GroceryList::Position::BOTTOM);
  }

  return stream;
}
