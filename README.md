# CodingChallenge
Coding challenges for Scientific Games.  There are two problems that have been
given that will be outlined below.

## Problem 1
Given a list of people with their birth and end years (all between 1900 and 2000),
find the year with the most number of people alive.  Solve using a language of your
choice and dataset of your own creation.  Please upload your code, dataset, and
example of the program’s output to Bit Bucket or Github.

### Requirements
* Node.js 4.6.0+
* C++ compiler (Microsoft Visual Studio or MinGW)

### Solution Steps
- Change your working directory to the Problem1 sub-directory.
- Install the necessary node dependencies by running `npm install`
- Create a random person dataset by running `node generateList.js -o <outputFile> -n <numberOfPeople>`,
OR you can use an existing dataset from the testData directory
- Compile the C++ code for the application that will compute the year(s) with the
most number of people alive.  There are two source files (FindLiveliestYear.cpp and jsoncpp/jsoncpp.cpp) and
you will need to add the include directory `jsoncpp`.  Compiling with MinGW would look like
```
> g++ -o FindLiveliestYear FindLiveliestYear.cpp jsoncpp\jsoncpp.cpp -I jsoncpp -std=gnu++11
```
- Execute the application by passing in the path to a JSON list of people e.g.
```
> FindLiveliestYear.exe testData\data1Person.json
Most number of people alive: 1
Year(s) with most people alive:
        1980 - Randall Burton
        1981 - Randall Burton
        1982 - Randall Burton
        1983 - Randall Burton
        1984 - Randall Burton
        1985 - Randall Burton
        1986 - Randall Burton
        1987 - Randall Burton
        1988 - Randall Burton
        1989 - Randall Burton
```

## Problem 2
TODO
