#include <iostream>
#include <stack>
#include <string>
using namespace std;

//��������� ������ ��� �������� ���� ��������� ������
struct Node
{
	char data; //������ ������ � ����
	Node* left, * right; //��������� �� ������ � ������� ��������

	Node(char data) //����������� � ����� ���������� data, ��������������� ���� � �������� � nullptr
	{
		this->data = data;
		this->left = this->right = nullptr;
	};

	Node(char data, Node* left, Node* right) //�����������, ������� ������������� �������� ���� ������� �������
	{
		this->data = data;
		this->left = left;
		this->right = right;
	};
};


//�������� ������ �� �������� �� ��
bool isOperator(char c)
{
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}// ������� ��������� ������ c � ���������� true, ���� c �������� ����� �� ��������� ����������: '+', '-', '*', '/' ��� '^', ����� false


//����� ������������ ����� ���������
void postorder(Node* root) //�-��� ��������� �� ���� ��������� �� �������� ����
{
	if (root == nullptr) //���� �������� ���� = nullptr, �� �-��� ������ �� ������
	{
		return;
	}

	postorder(root->left); //����������� ����� ��� ������ �������
	postorder(root->right); //����������� ����� ��� ������� �������
	cout << root->data; //����� ������� data ���� root
}

 //����� ���������� ����� ���������
void inorder(Node* root) //�-��� ��������� �� ���� ��������� �� �������� ����
{
	if (root == nullptr) //���� �������� ���� ����� nullptr, �� ������ �� ����������
	{
		return;
	}


	//���� ����� �������� - ����� ������
	if (isOperator(root->data)) {
		cout << "(";
	}

	inorder(root->left); //����������� ����� ��� ������ �������
	cout << root->data; //����� ������� data ���� root
	inorder(root->right); //����������� ����� ��� ������� �������



	//���� ����� �������� - ����� ������
	if (isOperator(root->data)) {
		cout << ")";
	}
}

// ������� ��� ���������� ������ ��������� �� ��������� ������������ ���������, ��������� ����
Node* construct(string postfix) //������� ��������� ������ postfix, ���������� ����������� ���������
{
	if (postfix.length() == 0) //���� ����� ������ ����� 0, �-��� ���������� nullptr
	{
		return nullptr;
	}


	// ������� ������ stack ��� �������� ���������� ������
	stack<Node*> s;

	//����� ������������ �����
	for (char c:  postfix)  //��� ������� ������� c � ������ postfix ����������� 
	{
		//������� ����� - ��������
		if (isOperator(c))
		{
			//������� �� ����� ��� ���� x � y
			Node* x = s.top();
			s.pop();

			Node* y = s.top();
			s.pop();



			//���������� ������ ��������� �����, ������ �������� �������� ��������, � ����� � ������ ������� ��������� �� y � x ��������������
			Node* node = new Node(c, y, x); //��������� ����� ���� node, � �������� data ����� c, � left � right ��������� �� y � x ��������������


			//��������� �������� ���� � ����
			s.push(node);
		}

		//���� ����� �������, �������� ����� ���� ��������� �����, ��� ������ �������� ��������� � ���������� � ����
		else {
			s.push(new Node(c));
		}
	}
	// ��������� �� ������ ������ ��������� �������� � stack
	return s.top(); // ������������ ��������� �� ������ ������ ���������, ����������� � ����� s
}



int main()
{
	setlocale(0, "");
	string postfix = "43+5*82/-";
	Node* root = construct(postfix);

	cout << "����������� ������: ";
	postorder(root);

	cout << "\n������������ ���������: ";
	inorder(root);

	return 0;
}