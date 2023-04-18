#ifndef NUMERICREPLYS_HPP
# define NUMERICREPLYS_HPP

# include <iostream>
# include <string>

class NumericReplys
{

	public:

		NumericReplys();
		NumericReplys( NumericReplys const & src );
		~NumericReplys();

		NumericReplys &		operator=( NumericReplys const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, NumericReplys const & i );

#endif /* *************************************************** NUMERICREPLYS_H */