//AZFAR NAYYAN 22L-7886.
//ASSIGNMENT NUMBER 05 DATA STRUCTURES.
//BSE-3C

#include<iostream>
#include <cassert>
#include<fstream>
#include <string>

using namespace std;


template <class v>
struct HashItem
{
public:
	int key;
	v value;
	short status;
	HashItem()
	{
		status = 0;
		key = 0;
	}
};

template <class v> 
class HashMap
{
private:
	HashItem<v>* hashArray; 
	int capacity; 
	int currentElements;

	virtual int getNextCandidateIndex(int key, int i)
	{
		return (key + i) % capacity;
	}
	int hashFunction(int key)
	{
		return key % capacity;
	}
	void doubleCapacity()
	{
		int old = capacity;
		HashItem<v>* oldMap = hashArray;

		capacity = capacity * 2;
		currentElements = 0;
		hashArray = new HashItem<v>[capacity]();

		for (int i = 0; i < old; i++) 
		{
			if (oldMap[i].status == 2) 
			{
				insert(oldMap[i].key, oldMap[i].value);
			}
		}

		delete[] oldMap;
	}
	double loadFactor()
	{
		return static_cast<double>(currentElements) / capacity;
	}

public:
	HashMap()
	{
		capacity = 10;
		currentElements = 0;
		hashArray = new HashItem<v>[capacity]();
	}
	HashMap(int const c)
	{
		assert(c > 1);
		capacity = c;
		currentElements = 0;
		hashArray = new HashItem<v>[capacity]();
	}
	int getCapacity()
	{
		return capacity;
	}
	void insert(int const key, v const value)
	{
		if (loadFactor() >= 0.75)
		{
			doubleCapacity();
		}

		if (currentElements == capacity)
		{
			cout << "NAHI JAGA HAI!!" << endl;
			return;
		}

		int index = hashFunction(key);

		if (hashArray[index].status == 2)
		{
			int i = 1;

			while (hashArray[index].status == 2) 
			{
				index = getNextCandidateIndex(key, i);
				i++;
			}
		}

		hashArray[index].key = key;
		hashArray[index].value = value;
		hashArray[index].status = 2;
		currentElements++;
	}
	bool deleteKey(int const key)
	{
		if (currentElements == 0)
		{
			cout << "KHALI HUN, KUCH NAHI HAI DELETE KARNAAY KO !!" << endl;
			return false;
		}

		int index = hashFunction(key);
		if (hashArray[index].status == 2 && hashArray[index].key == key)
		{
			hashArray[index].status = 1;
			return true;
		}

		int i = 1;
		while (hashArray[index].status != 0) 
		{
			if (hashArray[index].status == 2 && hashArray[index].key == key) 
			{
				hashArray[index].status = 1;
				return true;
			}
			index = getNextCandidateIndex(key, i);
			i++;
		}

		return false;
	}
	v* get(int const key)
	{
		if (currentElements == 0)
		{
			cout << "KHALI HUN, KUCH NAHI HAI DENAY KO!!" << endl;
			return nullptr;
		}

		int index = hashFunction(key);
		if (hashArray[index].status == 2 && hashArray[index].key == key)
		{
			return &(hashArray[index].value);
		}

		int i = 1;
		while (hashArray[index].status != 0) 
		{
			if (hashArray[index].status == 2 && hashArray[index].key == key) 
			{
				return &(hashArray[index].value);
			}

			index = getNextCandidateIndex(key, i);
			i++;
		}

		return nullptr;
	}
	void print()
	{
		cout << "THE HASH ARRAY IS: ";
		for (int i = 0; i < capacity; i++)
		{
			if (hashArray[i].status == 2)
			{
				cout << hashArray[i].key << " ";
			}
		}
		cout << endl;
	}
	~HashMap()
	{
		delete[] hashArray;
	}
};



template <class v>
class QuadraticHashMap : public HashMap<v>
{
public:
	QuadraticHashMap()
	{
		HashMap<v>();
	}

private:
	int getNextCandidateIndex(int key, int i) override 
	{
		return (key + (i * i)) % this->getCapacity();
	}
};



template <class v>
class DoubleHashMap : public HashMap<v> 
{
public:
	DoubleHashMap()
	{
		HashMap<v>();
	}

private:
	const int PRIME = 11;
	int getNextCandidateIndex(int key, int i) override
	{
		int firstValue = key % this->getCapacity();
		int secondValue = (PRIME - (key % PRIME));
		return (firstValue + i * secondValue) % this->getCapacity();
	}
};


class Student
{
public:
	string name;
	float cgpa;
	Student() {}
	Student(string n, float c)
	{
		name = n;
		cgpa = c;
	}
	friend ostream& operator<<(ostream& os, const Student& student)
	{
		os << "Name: " << student.name << ", CGPA: " << student.cgpa;
		return os;
	}

};

void populateHash(string filename, HashMap<Student>* hash)
{
	ifstream fr;

	fr.open(filename);

	if (!fr.is_open())
	{
		cout << "TEXT FILE DALO PHELAY FOLDER MEI!!" << endl;
	}

	int id;
	string name;
	float cgpa;

	while (!fr.eof())
	{
		fr >> id;
		fr >> name;
		fr >> cgpa;
		Student s;
		s.name = name;
		s.cgpa = cgpa;
		hash->insert(id, s);

	}

	fr.close();
}


int main()
{
	HashMap<Student>* map; 

	map = new HashMap<Student>;
	populateHash("students.txt", map);
	cout << *map->get(9) << endl;
	map -> deleteKey(9); 
	assert(map -> get(9) == nullptr);
	delete map;

	map = new QuadraticHashMap<Student>;
	populateHash("students.txt", map);
	cout << *map->get(98) << endl;
	map -> deleteKey(98);
	assert(map->get(98) == nullptr);
	delete map;

	map = new DoubleHashMap<Student>;
	populateHash("students.txt", map);
	cout << *map->get(101) << endl;
	map -> deleteKey(101);
	assert(map -> get(101) == nullptr);
	delete map;

	HashMap<int> myHash;

	int choice = 0;
	while (choice != 5)
	{
		cout << "1. Insert." << endl;
		cout << "2. Delete." << endl;
		cout << "3. Get." << endl;
		cout << "4. Print." << endl;
		cout << "5. Exit" << endl;

		cout << "Enter your choice: ";
		cin >> choice;

		if (choice == 1) 
		{
			int key;
			int value;
			cout << "Enter key: ";
			cin >> key;
			cout << "Enter value: ";
			cin >> value;
			myHash.insert(key, value);
		}
		else if (choice == 2) 
		{
			int key;
			cout << "Enter key to delete: ";
			cin >> key;
			if (myHash.deleteKey(key)) 
			{
				cout << "DELETE HO GAYI !" << endl;
			}
			else 
			{
				cout << "nahi mili key" << endl;
			}
		}
		else if (choice == 3) 
		{
			int key;
			cout << "Enter kry to find: ";
			cin >> key;
			int* result = myHash.get(key);
			cout << "Value: " << *result << endl;
		}
		else if (choice == 4) 
		{
			myHash.print();
		}
		else if (choice == 5) 
		{
			cout << "exit" << endl;
		}
		else 
		{
			cout << "sahi option choose kro" << endl;
		}
	}

	system("pause>0");
	return 0;

	//assignment 05 q2
}