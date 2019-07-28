#include "pch.h"
#include <iostream>
#include <string>
#include <ctime>
#include <boost/range/numeric.hpp>
#include <boost/filesystem.hpp>
#include <fstream>

using namespace std;
namespace fs = boost::filesystem;

size_t du(fs::path p);
void children_iterator(fs::path p, fstream& out);


int main()
{
	string startPath;
	time_t t;
	fstream out("result.json", ios::out);

	cout << "Enter folder path: ";
	cin >> startPath;
	fs::path fEntry(startPath);
	try {
		if (fs::exists(fEntry)) {
			out.clear();
			t = fs::last_write_time(fEntry);

			out << "{\n\"Name\": " << fEntry.filename() << "\n\"Size\":" << du(fEntry) << " B\n" << "\"Created\": " << ctime(&t) << "\n";
			children_iterator(fEntry, out);
			out << "\n}";

			out.close();
		}
		else
		{
			throw "Wrong path!";
		}
	}
	catch (string e)
	{
		cout << e << endl;
	}
	catch (...)
	{
		cout << "Something went wrong!" << endl;
	}
	return 0;
}

void children_iterator(fs::path p, fstream& out)
{
	std::time_t t;
	out << "\t\"Childrens\": [";
	for (fs::directory_entry& entry : fs::directory_iterator(p))
	{
		out << "\t\n{";
		if (fs::is_directory(entry.path()))
		{
			t = boost::filesystem::last_write_time(entry.path());
			out << "\n\t\"Type\" : \"Folder\"\n\t\"Name\": " << entry.path().filename() << "\n\t\"Size\":" << du(entry.path()) << " B\n" << "\t\"Created\": " << ctime(&t) << "\n";
			children_iterator(entry.path(), out);
		}
		if (fs::is_regular_file(entry.path()))
		{
			out << "\n\t\"Type\" : \"File\"\n\t\"Name\": " << entry.path().filename() << "\n\t\"Size\": " << du(entry.path()) << "B" << "\n\t\"Path\": " << entry.path() << "\n\n";
		}
		out << "\t},";
	}
	out << "\t\n]";
}

size_t du(fs::path p) {
	return fs::is_regular_file(p)
		? file_size(p)
		: boost::accumulate(fs::directory_iterator{ p }, 0ull, [](auto a, auto p) {return a + du(p); });
}
