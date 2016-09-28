/**
 * @file FindLiveliestYear.cpp
 * @description Application that takes in a JSON input file as a command line
 *    argument containing a list of people with random names and birth/death
 *    years between 1900 - 2000 and outputs the year(s) with the most number
 *    of people alive (including their names).
 */

// Standard libraries.
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>

// External JSON parsing library.
#include <json/json.h>

using namespace std;

// Type for our years.
typedef unsigned int Year;

// Set our birth/death year constraints.
static const Year MIN_BIRTH_YEAR = 1900;
static const Year MAX_DEATH_YEAR = 2000;

// Type for a yearwhich
// Map type where the key is the year and the value is a list of the people
// alive during that year.
typedef map<Year, list<string>> PeopleByYear;

/**
 * @brief Given a JSON list of people objects, return a map of the the people
 *    alive by year.  If there is an error with any of the JSON data, an empty
 *    map will be returned.
 * @param people - A JSON list of people objects.
 * @return A map of the people alive by year
 */
PeopleByYear GetPeopleAlivePerYear(const Json::Value& people)
{
	PeopleByYear emptyMap;
	PeopleByYear peopleMap;
	// Iterate through the list of people to add their name to the appropriate
	// lists during the years they were alive.
	for (auto const& person : people)
	{
		// Make sure there is a name value for this person.
		auto nameValue = person["name"];
		if (nameValue.isNull())
		{
			cout << "Person missing name." << endl;
			return emptyMap;
		}

		// Extract the name and make sure it isn't empty.
		string name = nameValue.asString();
		if (name.empty())
		{
			cout << "Person with empty name." << endl;
			return emptyMap;
		}

		// Make sure there are birth and death years.
		auto birthYearValue = person["birthYear"];
		auto deathYearValue = person["deathYear"];
		if (birthYearValue.isNull() || deathYearValue.isNull())
		{
			cout << "Person (" << name << ") is missing birth or death year." << endl;
			return emptyMap;
		}

		// Make sure the birth/death years are valid (inside our range, and the
		// death year must be >= the birth year).
		Year birthYear = birthYearValue.asUInt();
		Year deathYear = deathYearValue.asUInt();
		if (birthYear < MIN_BIRTH_YEAR || birthYear > MAX_DEATH_YEAR)
		{
			cout << "Person (" << name << ") has birth year (" << birthYear << ") out of valid range [" << MIN_BIRTH_YEAR << " - " << MAX_DEATH_YEAR << "]." << endl;
			return emptyMap;
		}
		if (deathYear < MIN_BIRTH_YEAR || deathYear > MAX_DEATH_YEAR)
		{
			cout << "Person (" << name << ") has death year (" << deathYear << ") out of valid range [" << MIN_BIRTH_YEAR << " - " << MAX_DEATH_YEAR << "]." << endl;
			return emptyMap;
		}
		if (deathYear < birthYear)
		{
			cout << "Person (" << name << ") somehow died before they were born " << birthYear << " - " << deathYear << "." << endl;
			return emptyMap;
		}

		// Now that we've ensured the name and birth/death years are valid, we
		// can add this persons name to the appropriate lists in our map of
		// people alive during specified years.
		for (Year i = birthYear; i <= deathYear; ++i)
		{
			peopleMap[i].push_back(name);
		}
	}

	return peopleMap;
}

/**
 * @brief Main entry point function for our Application
 * @param argc - The number of command line arguments for our application.
 * @param argv - A list of command line arguments for our application.
 */
int main(int argc, char* argv[])
{
	// Ensure there is a single argument that will be the path to our input JSON
	// file that we will parse.
	if (argc != 2)
	{
		cout << "Wrong number of arguments, should only provide path to the input file." << endl;
		return -1;
	}

	// Retrieve the input file from the command line argument.
	char* inputFilePath = argv[1];

	// Attempt to open a read stream on our input file.
	ifstream inputFile(inputFilePath);
	if (!inputFile.good())
	{
		// Log error if we failed to open a read stream on the input file.
		cout << "Input file " << inputFilePath << " does not exist, or cannot be accessed at this time." << endl;
		return -1;
	}

	// Parse the JSON input file into our people list.
	Json::Value people;
	Json::Reader reader;
	bool success = reader.parse(inputFile, people);
	if (success)
	{
		// First check if the JSON was empty.
		if (people.empty())
		{
			cout << "The input JSON was empty." << endl;
			return -1;
		}

		// Retrieve our map of people alive per year. If it is empty then something
		// went wrong, so stop processing.
		PeopleByYear peopleAlivePerYear = GetPeopleAlivePerYear(people);
		if (peopleAlivePerYear.empty())
		{
			return -1;
		}

		// Iterate through our map of people alive per year to find the year(s)
		// that have the most people alive.
		Year mostPeopleAlive = 0;
		list<Year> liveliestYears;
		for (auto const& itr : peopleAlivePerYear)
		{
			Year year = itr.first;
			list<string> persons = itr.second;
			unsigned int peopleAlive = persons.size();
			if (peopleAlive >= mostPeopleAlive)
			{
				// If this year had more people alive then our current liveliest
				// years, then we need to clear our current list of liveliest years
				// and set a new high.
				if (peopleAlive > mostPeopleAlive)
				{
					liveliestYears.clear();
					mostPeopleAlive = peopleAlive;
				}

				// If this was a new high, it will be the only year in the list.
				// If it matched our previous high, this year will be added to the list.
				liveliestYears.push_back(year);
			}
		}

		// Print out the highest number of people alive and the corresponding
		// year(s), including the names of those people.
		cout << "Most number of people alive: " << mostPeopleAlive << endl;
		cout << "Year(s) with most people alive:" << endl;
		for (auto const& year : liveliestYears)
		{
			// Print out the year indented.
			cout << "\t" << year << " - ";

			// Print out the names of the people alive during that year.
			auto names = peopleAlivePerYear[year];
			unsigned int numNames = names.size();
			unsigned int count = 1;
			for (auto const& name : names)
			{
				cout << name;

				// If this isn't the last name in the list, then append a comma
				// to separate from the next name.
				if (count != numNames)
				{
					cout << ", ";
				}
				++count;
			}
			cout << endl;
		}
	}
	else
	{
		cout << "Error parsing JSON input file " << inputFilePath << endl;
		return -1;
	}

	return 0;
}
