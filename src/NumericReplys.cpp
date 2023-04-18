#include "NumericReplys.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

NumericReplys::NumericReplys()
{
}

NumericReplys::NumericReplys( const NumericReplys & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

NumericReplys::~NumericReplys()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

NumericReplys &				NumericReplys::operator=( NumericReplys const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, NumericReplys const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */