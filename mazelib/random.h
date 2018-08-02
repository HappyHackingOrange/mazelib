#include <random>

/*
C++ implementation of Python's randrange function.
*/

namespace random {

	/*
	Generates numbers from 0 to stop value.

	Syntax :
	randrange(stop)

	Parameters :
	stop : Number less than this are generated. This parameter is mandatory.

	Return value :
	A random integer between 0 to stop value.
	*/
	int randrange(int stop);

	/*
	Generates numbers from from start value to stop value,
	skipping with step value.

	Syntax :
	randrange(start, stop)
	randrange(start, stop, step)

	Parameters :
	start : Number consideration for generation starts from this,
			default value is 0.
	stop : Number less than this are generated. This parameter is mandatory.
	step : Opinional step point of range.

	Return value :
	A random integer between start value to stop value,
	which is skipped with opinional step value.

	Exception :
	Throws invalid_argument if stop <= start.
	*/
	//int randrange(int start, int stop, int step = 1);
	int randrange(int start, int stop, int step);

	/*
	Shuffles the elements in the vector.

	Syntax :
	shuffle(vect)

	Parameters :
	vect : a vector which to be shuffled.
	*/
	void shuffle(std::vector<std::pair<int, int>> &vect);

	/*
	Returns a random item from a container.

	Syntax :
	choice(c)

	Parameters :
	c : a mandatory container parameter that can be vector, array, deque, etc.

	Return value :
	A random item.
	*/
	//template <typename Container>
	//auto choice(const Container& c) -> decltype(*begin(c))&;
	std::pair<int, int> choice(const std::vector<std::pair<int, int>> c);

}
