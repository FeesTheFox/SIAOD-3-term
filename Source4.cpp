#include <iostream>
#include <stack>
#include <string>
using namespace std;

//Структура данных для хранения узла бинарного дерева
struct Node
{
	char data; //Символ данных в узле
	Node* left, * right; //Указатели на левого и правого потомков

	Node(char data) //Конструктор с одним аргументом data, устанавливающий дату и потомков в nullptr
	{
		this->data = data;
		this->left = this->right = nullptr;
	};

	Node(char data, Node* left, Node* right) //конструктор, который устанавливает значения поля должным образом
	{
		this->data = data;
		this->left = left;
		this->right = right;
	};
};


//Проверка токена на оператор ли он
bool isOperator(char c)
{
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}// Функция принимает символ c и возвращает true, если c является одним из следующих операторов: '+', '-', '*', '/' или '^', иначе false


//Вывод постфиксного древа выражений
void postorder(Node* root) //Ф-ция принимает на вход указатель на корневой узел
{
	if (root == nullptr) //Если корневой узел = nullptr, то ф-ция ничего не делает
	{
		return;
	}

	postorder(root->left); //Рекурсивный вызов для левого потомка
	postorder(root->right); //Рекурсивный вызов для правого потомка
	cout << root->data; //Вывод символа data узла root
}

 //Вывод инфиксного древа выражений
void inorder(Node* root) //Ф-ция принимает на вход указатель на корневой узел
{
	if (root == nullptr) //Если корневой узел равен nullptr, то ничего не происходит
	{
		return;
	}


	//Если токен оператор - вывод скобки
	if (isOperator(root->data)) {
		cout << "(";
	}

	inorder(root->left); //Рекурсивный вызов для левого потомка
	cout << root->data; //Вывод символа data узла root
	inorder(root->right); //Рекурсивный вызов для правого потомка



	//Если токен оператор - вывод скобки
	if (isOperator(root->data)) {
		cout << ")";
	}
}

// Функция для построения дерева выражений из заданного постфиксного выражения, используя стек
Node* construct(string postfix) //Функция принимает строку postfix, содержащую постфиксное выражение
{
	if (postfix.length() == 0) //Если длина строки равна 0, ф-ция возвращает nullptr
	{
		return nullptr;
	}


	// создаем пустой stack для хранения указателей дерева
	stack<Node*> s;

	//Обход постфиксного древа
	for (char c:  postfix)  //Для каждого символа c в строке postfix выполняется 
	{
		//Текущий токен - оператор
		if (isOperator(c))
		{
			//Извлечь из стека два узла x и y
			Node* x = s.top();
			s.pop();

			Node* y = s.top();
			s.pop();



			//Построение нового бинарного древа, корнем которого является оператор, а левый и правый потомки указывают на y и x соответственно
			Node* node = new Node(c, y, x); //Создается новый узел node, у которого data равен c, а left и right указывают на y и x соответственно


			//Помещение текущего узла в стек
			s.push(node);
		}

		//Если токен операнд, создаётся новый узел бинарного древа, чей корень является операндом и помещается в стек
		else {
			s.push(new Node(c));
		}
	}
	// указатель на корень дерева выражений остается в stack
	return s.top(); // возвращается указатель на корень дерева выражений, находящийся в стеке s
}



int main()
{
	setlocale(0, "");
	string postfix = "43+5*82/-";
	Node* root = construct(postfix);

	cout << "Постфиксная запись: ";
	postorder(root);

	cout << "\nПолучившееся выражение: ";
	inorder(root);

	return 0;
}