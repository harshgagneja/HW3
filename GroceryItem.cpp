#include <algorithm>                                                  // max()
#include <cmath>                                                      // abs(), pow()
#include <compare>                                                    // weak_ordering
#include <iomanip>                                                    // quoted(), ios::failbit
#include <iostream>                                                   // istream, ostream, ws()
#include <string>
#include <type_traits>                                                // is_floating_point_v, common_type_t
#include <utility>                                                    // move()

#include "GroceryItem.hpp"



/*******************************************************************************
**  Implementation of non-member private types, objects, and functions
*******************************************************************************/
namespace    // unnamed, anonymous namespace
{
  // Avoid direct equality comparisons on floating point numbers. Two values are equal if they are "close enough", which is
  // represented by Epsilon.  Usually, this is a pretty small number, but since we are dealing with money (only two, maybe three
  // decimal places) we need to be a bit more tolerant.
  //
  // The two values are "close enough" to be considered equal if the distance between lhs and rhs is less than:
  // o)  EPSILON1, otherwise
  // o)  EPSILON2 percentage of the larger value's magnitude

  template< typename T,  typename U >   requires std::is_floating_point_v<std::common_type_t<T, U> >
  constexpr bool floating_point_is_equal( T const lhs,  U const rhs,  long double const EPSILON1 = /*1e-12L*/ 1e-4L,  long double const EPSILON2 = 1e-8L ) noexcept
  {
    return std::abs(lhs - rhs) < EPSILON1 || std::abs(lhs - rhs) < EPSILON2 * std::max(std::abs(lhs), std::abs(rhs));
  }
}    // unnamed, anonymous namespace







/*******************************************************************************
**  Constructors, assignments, and destructor
*******************************************************************************/

// Default and Conversion Constructor
GroceryItem::GroceryItem( std::string productName, std::string brandName, std::string upcCode, double price )
  : _productName(std::move(productName)), _brandName(std::move(brandName)), _upcCode(std::move(upcCode)), _price(price)
{}                                                                    // Avoid setting values in constructor's body (when possible)




// Copy constructor
GroceryItem::GroceryItem( GroceryItem const & other )
  : _productName(other._productName), _brandName(other._brandName), _upcCode(other._upcCode), _price(other._price)
{}                                                                    // Avoid setting values in constructor's body (when possible)




// Move constructor
GroceryItem::GroceryItem( GroceryItem && other ) noexcept
  : _productName(std::move(other._productName)), _brandName(std::move(other._brandName)), _upcCode(std::move(other._upcCode)), _price(other._price)
{}




// Copy Assignment Operator
GroceryItem & GroceryItem::operator=( GroceryItem const & rhs ) &
{
  if (this != &rhs) {
    _productName = rhs._productName;
    _brandName = rhs._brandName;
    _upcCode = rhs._upcCode;
    _price = rhs._price;
  }
  return *this;
}




// Move Assignment Operator
GroceryItem & GroceryItem::operator=( GroceryItem && rhs ) & noexcept
{
  if (this != &rhs) {
    _productName = std::move(rhs._productName);
    _brandName = std::move(rhs._brandName);
    _upcCode = std::move(rhs._upcCode);
    _price = rhs._price;
  }
  return *this;
}



// Destructor
GroceryItem::~GroceryItem() noexcept = default;








/*******************************************************************************
**  Accessors
*******************************************************************************/

// upcCode() const    (L-value objects)
std::string const & GroceryItem::upcCode() const &
{
  return _upcCode;
}




// brandName() const    (L-value objects)
std::string const & GroceryItem::brandName() const &
{
  return _brandName;
}




// productName() const    (L-value objects)
std::string const & GroceryItem::productName() const &
{
  return _productName;
}



// price() const    (L-value and, because there is no R-value overload, R-value objects)
double GroceryItem::price() const &
{
  return _price;
}




// upcCode()    (R-value objects)
std::string GroceryItem::upcCode() &&
{
  return std::move(_upcCode);
}




// brandName()    (R-value objects)
std::string GroceryItem::brandName() &&
{
  return std::move(_brandName);
}




// productName()    (R-value objects)
std::string GroceryItem::productName() &&
{
  return std::move(_productName);
}








/*******************************************************************************
**  Modifiers
*******************************************************************************/

// upcCode(...)
GroceryItem & GroceryItem::upcCode( std::string newUpcCode ) &
{
  _upcCode = std::move(newUpcCode);
  return *this;
}




// brandName(...)
GroceryItem & GroceryItem::brandName( std::string newBrandName ) &
{
  _brandName = std::move(newBrandName);
  return *this;
}




// productName(...)
GroceryItem & GroceryItem::productName( std::string newProductName ) &
{
  _productName = std::move(newProductName);
  return *this;
}




// price(...)
GroceryItem & GroceryItem::price( double newPrice ) &
{
  _price = newPrice;
  return *this;
}








/*******************************************************************************
**  Relational Operators
*******************************************************************************/

// operator<=>(...)
std::weak_ordering GroceryItem::operator<=>( const GroceryItem & rhs ) const noexcept
{
  // Design decision:  A very simple and convenient defaulted 3-way comparison operator
  //                         auto operator<=>( const GroceryItem & ) const = default;
  //                   in the class definition (header file) would get very close to what is needed and would allow both the <=> and
  //                   the == operators defined here to be skipped.  The physical ordering of the attributes in the class definition
  //                   would have to be changed (easy enough in this case) but the default directly compares floating point types
  //                   (price) for equality, and that should be avoided, in general. For example, if x and y are of type double,
  //                   then  x < y  is okay but  x == y  is not.  So these (operator<=> and operator==) explicit definitions are
  //                   provided.
  //
  //                   Also, many ordering (sorting) algorithms, like those used in std::map and std::set, require at least a weak
  //                   ordering of elements. operator<=> provides only a partial ordering when comparing floating point numbers.
  //
  // Weak order:       Objects that compare equal but are not substitutable (identical).  For example, since _price can be within
  //                   EPSILON, GroceryItem("ProductName", "BrandName", "UPC", 9.99999) and GroceryItem("ProductName", "BrandName",
  //                   "UPC", 10.00001) are equal but they are not identical.  If you ignore case when comparing strings, as another
  //                   example, GroceryItem("ProductName") and GroceryItem("productName") are equal but they are not identical.
  //
  // See std::weak_ordering    at https://en.cppreference.com/w/cpp/utility/compare/weak_ordering and
  //     std::partial_ordering at https://en.cppreference.com/w/cpp/utility/compare/partial_ordering
  //     The Three-Way Comparison Operator at  http://modernescpp.com/index.php/c-20-the-three-way-comparison-operator
  //     Spaceship (Three way comparison) Operator Demystified https://youtu.be/S9ShnAFmiWM
  //
  //
  // Grocery items are equal if all attributes are equal (or within Epsilon for floating point numbers, like price). Grocery items are ordered
  // (sorted) by UPC code, product name, brand name, then price.

  auto cmp = _upcCode <=> rhs._upcCode;
  if (cmp != 0) return cmp;
  cmp = _productName <=> rhs._productName;
  if (cmp != 0) return cmp;
  cmp = _brandName <=> rhs._brandName;
  if (cmp != 0) return cmp;
  return floating_point_is_equal(_price, rhs._price) ? std::weak_ordering::equivalent : (_price < rhs._price ? std::weak_ordering::less : std::weak_ordering::greater);
}




// operator==(...)
bool GroceryItem::operator==( const GroceryItem & rhs ) const noexcept
{
  // All attributes must be equal for the two grocery items to be equal to the other.  This can be done in any order, so put the
  // quickest and then the most likely to be different first.

  return _upcCode == rhs._upcCode && _productName == rhs._productName && _brandName == rhs._brandName && floating_point_is_equal(_price, rhs._price);
}








/*******************************************************************************
**  Insertion and Extraction Operators
*******************************************************************************/

// operator>>(...)
std::istream & operator>>( std::istream & stream, GroceryItem & groceryItem )
{
  // A lot can go wrong when reading from streams - no permission, wrong types, end of file, etc. Minimal exception guarantee says
  // there should be no side effects if an error or exception occurs, so let's do our work in a local object and move it into place
  // at the end if all goes well.
  //
  // This function should be symmetrical with operator<< below.  Read what your write, and write what you read

  char delimiter = '\x{0000}';  // C++23 delimited escape sequence for the character whose value is zero, i.e., the null character
  std::string upcCode, brandName, productName;
  double price;
  if (stream >> std::quoted(upcCode) >> delimiter >> std::quoted(brandName) >> delimiter >> std::quoted(productName) >> delimiter >> price) {
    groceryItem = GroceryItem(std::move(productName), std::move(brandName), std::move(upcCode), price);
  } else {
    stream.setstate(std::ios::failbit);
  }
  return stream;
}




// operator<<(...)
std::ostream & operator<<( std::ostream & stream, const GroceryItem & groceryItem )
{
  return stream << std::quoted(groceryItem._upcCode) << ", " << std::quoted(groceryItem._brandName) << ", " << std::quoted(groceryItem._productName) << ", " << groceryItem._price;
}