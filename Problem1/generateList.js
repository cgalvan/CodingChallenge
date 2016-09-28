/**
 * @file generateList.js
 * @description Node script for generating a JSON file of a list of people with
 *    random names, and random birth/death years between 1900 - 2000 (inclusive).
 *    This script is driven by the following command line parameters:
 *
 */

'use strict';

// For accessing the command line arguments.
const argv = require('yargs').argv;
// For generating random names and numbers.
const Chance = require('chance');
// For writing our JSON file with the people data.
const fs = require('fs');
// For path manipuations.
const path = require('path');

// Retrive the number of people to generate and the desired output file.
const numPeople = argv.n;
const outputFile = argv.o;

// Ensure number of people specified is a valid number within our range.
const minNumPeople = 1;
const maxNumPeople = 9001;
if (typeof numPeople !== 'number' || numPeople < minNumPeople || numPeople > maxNumPeople)
{
	console.error('Specified number of people (' + numPeople + ') is out of valid range [' + minNumPeople + ' - ' + maxNumPeople + ']');
	return;
}

// Specify birth/death year limits.
const minBirthYear = 1900;
const maxDeathYear = 2000;

// Create a chance instance for generating random names and numbers.
let chance = new Chance();

// Generate our list of random people with random birth/death years.
let people = [];
for (let i = 0; i < numPeople; ++i)
{
	// Generate the random birth year and death year.
	let birthYear = chance.integer({
		min: minBirthYear,
		max: maxDeathYear
	});
	let deathYear = chance.integer({
		min: birthYear,
		max: maxDeathYear
	});

	// Add our person object with name and birth/death years to our list of people.
	let person = {
		name: chance.name(),
		birthYear: birthYear,
		deathYear: deathYear
	};
	people.push(person);
}

// Attempt to write out our JSON file of the people data.
try
{
	// Write out our people list in JSON format that is human readable with tab spacing.
	fs.writeFileSync(outputFile, JSON.stringify(people, null, '\t'));
	console.log('Successfully wrote list of random people to: ' + path.resolve(outputFile));
}
catch (error)
{
	// Log the error with stack trace if one occurred.
	console.error(error.stack);
}
