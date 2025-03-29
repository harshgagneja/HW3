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
  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( make_details( "Container consistency error" ) );

  ///////////////////////// TO-DO (1) //////////////////////////////
    /// All the containers are the same size, so pick one and return the size of that.  Since the forward_list has to calculate the
    /// size on demand, stay away from using that one.

  /////////////////////// END-TO-DO (1) ////////////////////////////
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Accessors
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// find() const
std::size_t GroceryList::find( const GroceryItem & groceryItem ) const
{
  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( make_details( "Container consistency error" ) );

  ///////////////////////// TO-DO (2) //////////////////////////////
    /// Locate the grocery item in this grocery list and return the zero-based position of that grocery item.  If the grocery item
    /// does not exist, return the size of this grocery list as an indicator the grocery item does not exist.  The grocery item will
    /// be in the same position in all the containers (array, vector, list, and forward_list) so pick just one of those to search.
    /// The STL provides the find() function that is a perfect fit here, but you may also write your own loop.

  /////////////////////// END-TO-DO (2) ////////////////////////////
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
  ///////////////////////// TO-DO (3) //////////////////////////////
    /// Silently discard duplicate items from getting added to the grocery list.  If the to-be-inserted grocery item is already in
    /// the list, simply return.
    ///
    /// Remember, you already have a function that tells you if the to-be-inserted grocery item is already in the list, so use it.
    /// Don't implement it again.

  /////////////////////// END-TO-DO (3) ////////////////////////////


  // Inserting into the grocery list means you insert the grocery item into each of the containers (array, vector, list, and
  // forward_list). Because the data structure concept is different for each container, the way a grocery item gets inserted is a
  // little different for each.  You are to insert the grocery item into each container such that the ordering of all the containers
  // is the same.  A check is made at the end of this function to verify the contents of all four containers are indeed the same.


  { /**********  Part 1 - Insert into array  ***********************/
    ///////////////////////// TO-DO (4) //////////////////////////////
      /// Hint:  "Vector.hpp" in our Sequence Container Implementation Examples shows you how to do this.  (wink wink)
      /// https://github.com/TBettens/Data_Structure_Implementation_Examples/tree/latest/Sequence%20Container%20Implementation%20Examples/Vector
      ///
      /// Unlike the other containers, an std::array has fixed capacity and fixed size (and size is always == capacity) hence has no
      /// insert() function, so you have to write it yourself. Insert into the array by shifting all the items at and after the
      /// insertion point (offsetFromTop) to the right opening a gap in the array that can be populated with the given grocery item.
      /// Remember that arrays have fixed capacity and cannot grow, so make sure there is room in the array for another grocery item
      /// before you start by verifying _gList_array_size (the number of valid elements) is less than _gList_array.size() (the
      /// capacity).  If not, throw CapacityExceeded_ex. Also remember that you must keep track of the number of valid grocery items
      /// in your array, so don't forget to adjust _gList_array_size.
      ///
      /// Open a hole to insert new grocery item by shifting to the right everything at and after the insertion point.
      /// For example:  a[8] = a[7];  a[7] = a[6];  a[6] = a[5];  and so on.
      /// std::shift_* will be helpful, or write your own loop.

    /////////////////////// END-TO-DO (4) ////////////////////////////
  } // Part 1 - Insert into array




  { /**********  Part 2 - Insert into vector  **********************/
    ///////////////////////// TO-DO (5) //////////////////////////////
      /// The vector STL container std::vector has an insert function, which can be directly used here.  But that function takes a
      /// pointer (or more accurately, an iterator) that points to the grocery item to insert before.  You need to convert the
      /// zero-based offset from the top (the index) to an iterator by advancing _gList_vector.begin() offsetFromTop times.  The STL
      /// has a function called std::next() that does that, or you can use simple pointer arithmetic to calculate it.  "Vector.hpp"
      /// in our Sequence Container Implementation Examples also shows you how convert index to iterator, and iterator to index.
      ///
      /// Behind the scenes, std::vector::insert() shifts to the right everything at and after the insertion point, just like you
      /// did for the array above.

    /////////////////////// END-TO-DO (5) ////////////////////////////
  } // Part 2 - Insert into vector




  { /**********  Part 3 - Insert into doubly linked list  **********/
    ///////////////////////// TO-DO (6) //////////////////////////////
      /// The doubly linked list STL container std::list has an insert function, which can be directly used here.  But that function
      /// takes a pointer (or more accurately, an iterator) that points to the grocery item to insert before.  You need to convert
      /// the zero-based offset from the top (the index) to an iterator by advancing _gList_dll.begin() offsetFromTop times.  The
      /// STL has a function called std::next() that does that, or you can write your own loop.

    /////////////////////// END-TO-DO (6) ////////////////////////////
  } // Part 3 - Insert into doubly linked list




  { /**********  Part 4 - Insert into singly linked list  **********/
    ///////////////////////// TO-DO (7) //////////////////////////////
      /// The singly linked list STL container std::forward_list has an insert function, which can be directly used here.  But that
      /// function inserts AFTER the grocery item pointed to, not before like the other containers.  A singly linked list cannot
      /// look backwards, only forward.  You need to convert the zero-based offset from the top (the index) to an iterator by
      /// advancing _gList_sll.before_begin() offsetFromTop times.  The STL has a function called std::next() that does that, or you
      /// can write your own loop.

    /////////////////////// END-TO-DO (7) ////////////////////////////
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
    ///////////////////////// TO-DO (8) //////////////////////////////
      /// Hint:  "Vector.hpp" in our Sequence Container Implementation Examples shows you how to do this.  (wink wink)
      ///
      ///
      /// Close the hole created by shifting to the left everything at and after the remove point.
      /// For example:  a[5] = a[6];  a[6] = a[7];  a[7] = a[8];  and so on
      ///
      /// std::shift_* will be helpful, or write your own loop.  Also remember that you must keep track of the number of valid
      /// grocery items in your array, so don't forget to adjust _gList_array_size.

    /////////////////////// END-TO-DO (8) ////////////////////////////
  } // Part 1 - Remove from array




  { /**********  Part 2 - Remove from vector  **********************/
    ///////////////////////// TO-DO (9) //////////////////////////////
      /// The vector STL container std::vector has an erase function, which can be directly used here.  But that function takes a
      /// pointer (or more accurately, an iterator) that points to the grocery item to be removed.  You need to convert the
      /// zero-based offset from the top (the index) to an iterator by advancing _gList_vector.begin() offsetFromTop times.  The STL
      /// has a function called std::next() that does that, or you can use simple pointer arithmetic to calculate it.  "Vector.hpp"
      /// in our Sequence Container Implementation Examples also shows you how convert index to iterator, and iterator to index.
      ///
      /// Behind the scenes, std::vector::erase() shifts to the left everything after the insertion point, just like you did for the
      /// array above.

    /////////////////////// END-TO-DO (9) ////////////////////////////
  } // Part 2 - Remove from vector




  { /**********  Part 3 - Remove from doubly linked list  **********/
    ///////////////////////// TO-DO (10) //////////////////////////////
      /// The doubly linked list STL container std::list has an erase function, which can be directly used here.  But that function
      /// takes a pointer (or more accurately, an iterator) that points to the grocery item to remove.  You need to convert the
      /// zero-based offset from the top (the index) to an iterator by advancing _gList_dll.begin() offsetFromTop times.  The STL
      /// has a function called std::next() that does that, or you can write your own loop.

    /////////////////////// END-TO-DO (10) ////////////////////////////
  } // Part 3 - Remove from doubly linked list




  {/**********  Part 4 - Remove from singly linked list  **********/
    ///////////////////////// TO-DO (11) //////////////////////////////
      /// The singly linked list STL container std::forward_list has an erase function, which can be directly used here.  But that
      /// function erases AFTER the grocery item pointed to, not the one pointed to like the other containers.  A singly linked list
      /// cannot look backwards, only forward.  You need to convert the zero-based offset from the top (the index) to an iterator by
      /// advancing _gList_sll.before_begin() offsetFromTop times.  The STL has a function called std::next() that does that, or you
      /// can write your own loop.

    /////////////////////// END-TO-DO (11) ////////////////////////////
  } // Part 4 - Remove from singly linked list


  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( make_details( "Container consistency error" ) );
} // remove( std::size_t offsetFromTop )



// moveToTop()
void GroceryList::moveToTop( const GroceryItem & groceryItem )
{
  ///////////////////////// TO-DO (12) //////////////////////////////
    /// If the grocery item exists, then remove and reinsert it.  Otherwise, do nothing.
    /// Remember, you already have functions to do all this.

  /////////////////////// END-TO-DO (12) ////////////////////////////
}



// operator+=( initializer_list )
GroceryList & GroceryList::operator+=( const std::initializer_list<GroceryItem> & rhs )
{
  ///////////////////////// TO-DO (13) //////////////////////////////
    /// Append (aka concatenate) the right hand side (rhs) to the bottom of this list by repeatedly inserting at the bottom of this
    /// grocery list. The input type is just a container of grocery items accessible with iterators just like all the other
    /// containers.  The constructor above gives an example.  Remember to add that grocery item at the bottom of each container
    /// (array, vector, list, and forward_list) of this grocery list, and that you already have a function that does that.

  /////////////////////// END-TO-DO (13) ////////////////////////////

  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( make_details( "Container consistency error" ) );
  return *this;
}



// operator+=( GroceryList )
GroceryList & GroceryList::operator+=( const GroceryList & rhs )
{
  ///////////////////////// TO-DO (14) //////////////////////////////
    /// Append (aka concatenate) the right hand side (rhs) to the bottom of this list by repeatedly inserting at the bottom of this
    /// grocery list. All the rhs containers (array, vector, list, and forward_list) contain the same information, so pick just one
    /// to traverse. Walk the container you picked inserting its grocery items to the bottom of this grocery list. Remember to add
    /// that grocery item at the bottom of each container (array, vector, list, and forward_list) of this grocery list, and that you
    /// already have a function that does that.

  /////////////////////// END-TO-DO (14) ////////////////////////////

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

  ///////////////////////// TO-DO (15) //////////////////////////////
    /// Find the common extent.  That is, if one list has 20 grocery items, and the other has only 13, then the common extent is 13
    /// grocery items. We can compare only the first 13 grocery items of each list.  Find the first grocery item in the common
    /// extent that is different using three-way comparison (e.g., gItem1 <=> gItem2  !=  0) and return the results.  If no
    /// differences were found in the common extent, then the list with the smaller size is less than the other.  In that case,
    /// return the results of three-way comparing the sizes.
    ///
    ///
    /// The content of all the grocery lists's containers is the same - so pick an easy one to walk.

  /////////////////////// END-TO-DO (15) ////////////////////////////
}



// operator==
bool GroceryList::operator==( GroceryList const & rhs ) const
{
  if( !containersAreConsistant() || !rhs.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( make_details( "Container consistency error" ) );

  ///////////////////////// TO-DO (16) //////////////////////////////
    /// Two lists are different if their sizes are different, or one of their grocery items is different.  Otherwise the lists are
    /// equal.
    ///
    /// Comparing the sizes is quick (constant time), so check that first.
    ///
    /// Walk the list looking for grocery items that don't match.  The content of all the grocery lists's containers is the same -
    /// so pick an easy one to walk.

  /////////////////////// END-TO-DO (16) ////////////////////////////
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
  ///////////////////////// TO-DO (17) //////////////////////////////
    /// Some implementations of a singly linked list maintain the size (number of elements in the list).  std::forward_list does
    /// not. The size of singly linked list must be calculated on demand by walking the list from beginning to end counting the
    /// number of elements visited.  The STL's std::distance() function does that, or you can write your own loop.

  /////////////////////// END-TO-DO (17) ////////////////////////////
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

  ///////////////////////// TO-DO (18) //////////////////////////////
    /// Extract until end of file grocery items from the provided stream and insert them at the bottom of the provided grocery list.
    /// Be sure to extract grocery items and not individual fields such as product name or UPC.

  /////////////////////// END-TO-DO (18) ////////////////////////////

  return stream;
}
