#include <iostream>

using namespace std;

const int MAX_SIZE = 20;

struct Node {

	int data;
	Node* next_node;
	Node* prev_node;
};

class Error {

public:

	virtual void what() {};
};

class IndexError : public Error {

protected:

	string message;

public:

	IndexError() { message = "Ошибка индекса.\n"; }
	virtual void what() { cout << message; }
};

class SizeError :public Error {

protected:

	string message;

public:

	SizeError() { message = "Ошибка размера.\n"; }
	virtual void what() { cout << message; }
};

class maxSizeError : public SizeError {

protected:

	string newMessagae;

public:

	maxSizeError() { SizeError(); newMessagae = "Размер списка больше, чем максимальный размер.\n"; }
	virtual void what() { cout << message << newMessagae; }
};

class emptySizeError : public SizeError {

protected:

	string newMessage;

public:

	emptySizeError() { SizeError(); newMessage = "Список пуст.\n"; }
	virtual void what() { cout << message << newMessage; }
};

class notSameSize : public SizeError {

protected:

	string newMessage;

public:

	notSameSize() { SizeError(); newMessage = "Списки разной длины.\n"; }
	virtual void what() { cout << message << newMessage; }
};

class negativeIndex : public IndexError {

protected:

	string someMessage;

public:

	negativeIndex() { IndexError(); someMessage = "Индекс меньше нуля.\n"; }
	virtual void what() { cout << message << someMessage; }
};

class ExceedingIndexError : public IndexError {

protected:

	string someMessage;

public:

	ExceedingIndexError() { IndexError(); someMessage = "Индекс превышает размер списка.\n"; }
	virtual void what() { cout << message << someMessage; }
};

class Iterator {

private:

	friend class List;
	Node* elem;

public:

	Iterator() { this->elem = nullptr; }
	Iterator(const Iterator& iterator) { this->elem = iterator.elem; }

	Iterator& operator=(const Iterator& iter) {
		this->elem = iter.elem;
		return *this;
	}

	bool operator==(const Iterator& iter) { return this->elem == iter.elem; }
	bool operator!=(const Iterator& iter) { return this->elem != iter.elem; }

	Iterator& operator++(int) {
		this->elem = this->elem->next_node;
		return *this;
	}

	Iterator& operator--() {
		this->elem = this->elem->prev_node;
		return *this;
	}

	Iterator operator+(const int number) const {
		Iterator temp(*this);
		for (int i = 0; i < number; i++) {
			temp.elem = temp.elem->next_node;
		}
		return temp;
	}

	Iterator operator-(const int number) const {
		Iterator temp(*this);
		for (int i = 0; i < number; i++) {
			temp.elem = temp.elem->prev_node;
		}
		return temp;
	}
	int& operator*() { return this->elem->data; }
};

class List {

private:

	int size;
	Node* head;
	Node* tail;
	Iterator begin;
	Iterator end;

public:

	List(int size, int data);
	List(const List& list);
	List(int size);
	List() { size = 0; head = nullptr; Node* tail = nullptr; }
	~List();

	Iterator first();
	Iterator last();

	void push_back(int data);
	void push_front(int data);
	int pop_back();
	int pop_front();
	bool is_empty();
	int operator () ();

	List& operator = (const List& list);
	int& operator [] (int index);
	List operator * (List& list);

	friend ostream& operator << (ostream& out, const List& list);
	friend istream& operator >> (istream& in, const List& list);
};

List::List(int size, int data) {

	this->size = size;
	if (size > 0) {

		Node* node = new Node;
		node->data = data;
		this->head = node;
		this->tail = node;

		for (int i = 1; i < size; i++) {

			Node* newNode = new Node;
			newNode->data = data;
			tail->next_node = newNode;
			newNode->prev_node = tail;
			tail = newNode;
		}
		tail->next_node = nullptr;
	}
	else {
		this->head = nullptr;
		this->tail = nullptr;
	}
	this->begin.elem = this->head;
	this->end.elem = this->tail;
}

List::List(int size) {

	if (size > MAX_SIZE) throw maxSizeError();

	this->size = size;
	if (size > 0) {

		Node* node = new Node;
		this->head = node;
		this->tail = node;

		for (int i = 1; i < size; i++) {

			Node* newNode = new Node;
			tail->next_node = newNode;
			newNode->prev_node = tail;
			tail = newNode;
		}
		tail->next_node = nullptr;
	}
	else {
		this->head = nullptr;
		this->tail = nullptr;
	}
	this->begin.elem = this->head;
	this->end.elem = this->tail;
}
List::List(const List& list) {

	this->head = nullptr;
	this->tail = nullptr;
	this->size = 0;


	Node* current_node = list.head;
	while (current_node != nullptr) {
		push_back(current_node->data);
		current_node = current_node->next_node;
	}
	this->begin = list.begin;
	this->end = list.end;
}

List::~List() {

	Node* current_node = head;
	while (current_node != nullptr) {
		Node* next_node = current_node->next_node;
		delete current_node;
		current_node = next_node;
	}
	head = nullptr;
}

void List::push_back(int data) {

	Node* new_node = new Node;
	new_node->data = data;
	new_node->next_node = nullptr;

	if (this->head == nullptr) {
		this->head = new_node;
		this->tail = new_node;
		this->begin.elem = this->head;
		this->end.elem = this->tail;
	}
	else {
		tail->next_node = new_node;
		new_node->prev_node = tail;
		tail = new_node;
	}
	this->size++;
}

void List::push_front(int data) {

	Node* new_node = new Node;
	new_node->data = data;

	if (this->head == nullptr) {

		this->head = new_node;
		this->tail = new_node;
		this->size++;
		this->begin.elem = this->head;
		this->end.elem = this->tail;
	}
	else {
		head->prev_node = new_node;
		new_node->next_node = head;
		head = new_node;
		this->size++;
		this->begin.elem = this->head;
	}
}

Iterator List::first() { return this->begin; }
Iterator List::last() { return this->end; }

bool List::is_empty() { return this->size == 0; }

int List::pop_back() {

	if (size == 0) throw emptySizeError();

	int temp;

	if (this->tail != nullptr) {

		Node* current_node = this->tail;
		tail = current_node->prev_node;
		temp = current_node->data;
		tail->next_node = nullptr;
		this->size--;
		this->end.elem = tail;
	}
	return temp;
}

int List::pop_front() {

	int temp;

	if (this->head != nullptr) {
		Node* current_node = this->head;
		head = current_node->next_node;
		temp = current_node->data;
		head->prev_node = nullptr;
		this->size--;
		this->begin.elem = head;
	}
	return temp;
}

List& List::operator = (const List& list) {

	cout << "Оператор присваивания\n";
	if (this == &list) {
		return *this;
	}
	while (head != nullptr) {
		Node* temp = head;
		head = head->next_node;
		delete temp;
	}
	size = 0;

	Node* current_node = list.head;
	while (current_node != nullptr) {
		push_back(current_node->data);
		current_node = current_node->next_node;
	}
	this->begin = list.begin;
	this->end = list.end;
	return *this;
}

int& List::operator [] (int index) {

	if (index < 0) throw negativeIndex();
	if (this->size == 0) throw emptySizeError();
	if (index > this->size) throw ExceedingIndexError();

	if (index < this->size && index >= 0) {
		Node* current_node = this->head;
		for (int i = 0; i != index; i++) {
			current_node = current_node->next_node;
		}
		return current_node->data;
	}
	else {
		cerr << "\nИндекс вышел за границы списка!\n";
		exit(0);
	}
}

int List:: operator () () { return this->size; }

List List::operator* (List& list) {

	if (this->size != list.size) throw notSameSize();

	int temp_size;
	if (this->size > list.size) {
		temp_size = list.size;
	}
	else {
		temp_size = this->size;
	}
	List temp(temp_size, 0);
	for (int i = 0; i < temp_size; i++) {
		temp[i] = (*this)[i] * list[i];
	}
	return temp;
}

ostream& operator << (ostream& out, const List& list) {

	out << "\nВывод элементов списка\n";

	Node* current_node = list.head;
	while (current_node != nullptr)
	{
		out << current_node->data << " ";
		current_node = current_node->next_node;
	}
	return out;
}

istream& operator >> (istream& in, const List& list) {

	cout << "\nВвод элементов списка\n";

	Node* current_node = list.head;
	while (current_node != nullptr) {

		in >> current_node->data;
		current_node = current_node->next_node;
	}
	return in;
}


int main() {

	system("chcp 1251 > Null");

	try {

		int listSize, value, index, amountToDelete;

		List list1;
		Iterator iter;

		cout << "Введите кол-во элементов списка: "; cin >> listSize;
		cout << endl;

		if (listSize < 1) throw "Размер списка должен быть больше, чем ноль.\n";
		if (listSize > MAX_SIZE) throw maxSizeError();

		cout << "Введите элементы списка\n";

		for (int i = 0; i < listSize; i++) {

			cout << i + 1 << " элемент: "; cin >> value;
			list1.push_back(value);
		}
		iter = list1.first();

		cout << "\nВведите индекс элемента для поиска: "; cin >> index;

		if (index < 0) throw negativeIndex();
		if (index > MAX_SIZE) throw ExceedingIndexError();

		cout << "\nЭлемент списка с индексом " << index << ": " << list1[index - 1] << endl;

	}
	catch (Error& e) {
		e.what();
	}

}